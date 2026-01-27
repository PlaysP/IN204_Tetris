// =========================
// server.cpp (ENet)
// =========================
namespace enet {
#include <enet/enet.h>}
#include "constant.hpp"
#include "grid.hpp"
#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <mutex>
#include <algorithm>


class Server {
    enet::ENetAddress address;
    std::vector<enet::ENetPeer> client;
    std::mutex clients_mutex;
    enet::ENetHost* server;

    std::string clientName;

    Grid& peerGrid;
    Grid& clientGrid;

    tetromino& peerTetrominos;
    tetromino& clientTetromino;

    bool peerGameOver;
    bool clientGameOver;

public:
    Server() {
        if (enet::enet_initialize() != 0) {
            throw std::runtime_error("Failed to initialize ENet");
        }
        atexit(enet::enet_deinitialize);

        address.host = enet::ENET_HOST_ANY;
        address.port = ADDRESS_PORT;

        server = enet::enet_host_create(
            &address,
            1, /* max clients */
            1,  /* canaux */
            0,
            0
        );

        if (!server) {
            throw std::runtime_error("Failed to create ENet server host");
        }

        std::cout << "ENet server started on port " << ADDRESS_PORT << std::endl;

        std::thread receiver([server, &clients, &clients_mutex]() {
        enet::ENetEvent event;
            if (enet_host_service(server, &event, 100) > 0) {
                switch (event.type) {
                case enet::ENET_EVENT_TYPE_CONNECT: {
                    std::cout << "Client connecté: " << event.peer << std::endl;

                    // Timeout agressif pour détecter les clients morts
                    enet_peer_timeout(event.peer, 5000, 10000, 30000);

                    std::lock_guard<std::mutex> lock(clients_mutex);
                    clients.push_back(event.peer);
                    break;
                }

                case enet::ENET_EVENT_TYPE_RECEIVE: {
                    deserializeGrid(event.packet->data, event.packet->dataLength, clientGrid, clientGameOver);
                    break;
                }

                case ENET_EVENT_TYPE_DISCONNECT:
                    std::cout << "Client déconnecté: " << event.peer << std::endl;
                    {
                        std::lock_guard<std::mutex> lock(clients_mutex);
                        auto it = std::find(clients.begin(), clients.end(), event.peer);
                        if (it != clients.end()) {
                            clients.erase(it);
                        }
                    }
                    break;

                default:
                    break;
                }
            }
    });
    receiver.detach();

    // Thread principal pour envoyer des messages du serveur

        std::cout << "Serveur> ";
        std::getline(std::cin, msg);
            std::lock_guard<std::mutex> lock(clients_mutex);
            if (clients.empty()) {
                std::cout << "Aucun client connecté" << std::endl;
            } else {
                for (ENetPeer* peer : clients) {
                    auto data = serializeGrid(peerGrid, peerGameOver);

                    enet::ENetPacket* packet = enet_packet_create(
                        data.data(),
                        data.size(),
                        enet::ENET_PACKET_FLAG_RELIABLE );
                        enet_peer_send(peer, 0, packet);
                }
                enet_host_flush(server);
            }
        enet_host_destroy(server);
    }
}
