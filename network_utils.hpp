#pragma once

#include <vector>
#include <iostream>
#include "grid.hpp"
#include <stdint.h>

namespace enet {
    #include <enet/enet.h>
}

std::vector<uint8_t> serializeGrid(const std::vector<std::vector<char>>& grid, bool gameOver) {
    std::vector<uint8_t> buffer;

    int rows = grid.size();
    int cols = grid.empty() ? 0 : grid[0].size();

    std::cout << "Sérialisation: " << rows << "x" << cols << " octets, gameOver=" << gameOver << std::endl;

    // Ajouter dimensions
    buffer.insert(buffer.end(), (uint8_t*)&rows, (uint8_t*)&rows + sizeof(int));
    buffer.insert(buffer.end(), (uint8_t*)&cols, (uint8_t*)&cols + sizeof(int));

    // Ajouter GameOver
    uint8_t go = gameOver ? 1 : 0;
    buffer.push_back(go);

    // Ajouter la grille
    for (const auto& row : grid) {
        buffer.insert(buffer.end(), row.begin(), row.end());
    }

    std::cout << "Taille du paquet: " << buffer.size() << " octets" << std::endl;
    return buffer;
}


// code pour envoyer 
// auto data = serializeGrid(m_grid, GameOver);

// ENetPacket* packet = enet_packet_create(
//     data.data(),
//     data.size(),
//     ENET_PACKET_FLAG_RELIABLE
// );

// enet_peer_send(peer, 0, packet);


void deserializeGrid(const uint8_t* data, size_t size,
                     std::vector<std::vector<char>>& grid,
                     bool& gameOver)
{
    // Vérifier qu'il y a assez de données
    if (size < 2 * sizeof(int) + 1) {
        std::cerr << "Erreur: paquet trop petit (" << size << " octets)" << std::endl;
        return;
    }

    const uint8_t* ptr = data;

    int rows = *(int*)ptr; ptr += sizeof(int);
    int cols = *(int*)ptr; ptr += sizeof(int);

    // Valider les dimensions (limites raisonnables)
    if (rows < 0 || rows > 1000 || cols < 0 || cols > 1000) {
        std::cerr << "Erreur: dimensions invalides (rows=" << rows << ", cols=" << cols << ")" << std::endl;
        return;
    }

    // Vérifier que le paquet contient assez de données
    size_t expectedSize = 2 * sizeof(int) + 1 + (rows * cols);
    if (size < expectedSize) {
        std::cerr << "Erreur: paquet incomplet (attendu " << expectedSize << ", reçu " << size << ")" << std::endl;
        return;
    }

    gameOver = (*ptr != 0);
    ptr++;

    grid.assign(rows, std::vector<char>(cols));

    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            grid[r][c] = *ptr;
            ptr++;
        }
    }
    
    std::cout << "Grille désérialisée: " << rows << "x" << cols << " octets, gameOver=" << gameOver << std::endl;
}


// reception via enet:
// if (event.type == ENET_EVENT_TYPE_RECEIVE) {
//     std::vector<std::vector<char>> grid;
//     bool gameOver;

//     deserializeGrid(event.packet->data, event.packet->dataLength, grid, gameOver);

//     // Maintenant tu as la grille et le GameOver
// }
