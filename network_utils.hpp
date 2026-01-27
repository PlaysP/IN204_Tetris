#include <vector>
#include "grid.hpp"
#include <stdint.h>

namespace enet {
    #include <enet/enet.h>
}

std::vector<uint8_t> serializeGrid(const std::vector<std::vector<char>>& grid, bool gameOver) {
    std::vector<uint8_t> buffer;

    int rows = grid.size();
    int cols = grid[0].size();

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
    const uint8_t* ptr = data;

    int rows = *(int*)ptr; ptr += sizeof(int);
    int cols = *(int*)ptr; ptr += sizeof(int);

    gameOver = (*ptr != 0);
    ptr++;

    grid.assign(rows, std::vector<char>(cols));

    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            grid[r][c] = *ptr;
            ptr++;
        }
    }
}


// reception via enet:
// if (event.type == ENET_EVENT_TYPE_RECEIVE) {
//     std::vector<std::vector<char>> grid;
//     bool gameOver;

//     deserializeGrid(event.packet->data, event.packet->dataLength, grid, gameOver);

//     // Maintenant tu as la grille et le GameOver
// }
