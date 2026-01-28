#pragma once

#include "network_utils.hpp"
#include "constant.hpp"
#include "grid.hpp"
#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <mutex>
#include <algorithm>
#include <cstring>


class Server {
    ENetAddress address;
    std::vector<ENetPeer*> clients;
    std::mutex clients_mutex;
    ENetHost* server;
    std::thread receiverThread;

    std::string clientName;

    Grid clientGridObj;
    std::vector<std::vector<char>>& clientGrid;

    // tetromino& clientTetromino;

    bool clientGameOver;
    bool running = true;
    bool dataReceived = false;

public:
    Server(std::string aClientName): clientName(aClientName), clientGrid(clientGridObj.getGrid()) {
        memset(&address, 0, sizeof(address));
        address.host = ENET_HOST_ANY;
        address.port = ADDRESS_PORT;
        
        std::cout << "DEBUG: Creating ENet host..." << std::endl;

        server = enet_host_create(
            &address,
            32, /* max clients */
            2,  /* canaux */
            0,
            0
        );

        if (!server) {
            running = false;
            throw std::runtime_error("Failed to create ENet server host");
        }
    };

    ~Server() {
        running = false;
        if (receiverThread.joinable()) {
            receiverThread.join();
        }
        if (server) {
            enet_host_destroy(server);
        }
    };

    void startReceiving() {
        receiverThread = std::thread([this]() {
        ENetEvent event;
        while (running) {
            int result = enet_host_service(server, &event, 100);
            if (result > 0) {
                switch (event.type) {
                case ENET_EVENT_TYPE_CONNECT: {
                    std::cout << "Client connecté: " << event.peer << std::endl;

                    // Timeout agressif pour détecter les clients morts
                    enet_peer_timeout(event.peer, 5000, 10000, 30000);

                    std::lock_guard<std::mutex> lock(clients_mutex);
                    clients.push_back(event.peer);
                    break;
                }

                case ENET_EVENT_TYPE_RECEIVE: {
                    deserializeGrid(event.packet->data, event.packet->dataLength, clientGrid, clientGameOver);
                    dataReceived = true;
                    enet_packet_destroy(event.packet);
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
            // Flush les événements réseau pour éviter le blocage
            enet_host_flush(server);
        }
    });
    }

    int send(Grid& localGrid, bool localGameOver) {
        std::lock_guard<std::mutex> lock(clients_mutex);
        if (clients.empty()) {
            return 1;
        } else {
            for (ENetPeer* peer : clients) {
                auto data = serializeGrid(localGrid.getGrid(), localGameOver);

                ENetPacket* packet = enet_packet_create(
                    data.data(),
                    data.size(),
                    ENET_PACKET_FLAG_RELIABLE );
                    enet_peer_send(peer, 0, packet);
            }
            enet_host_flush(server);
            return 0;
        }
    }
    
    std::vector<std::vector<char>>& getClientGrid() {
        return clientGrid;
    }

    bool isClientGameOver() {
        return clientGameOver;
    }

    bool hasReceivedData() {
        return dataReceived;
    }

    void resetDataReceived() {
        dataReceived = false;
    }

    int numberOfClients() {
        return clients.size();
    }

    bool isClientDisconnected() {
        return clients.empty();
    }

};
