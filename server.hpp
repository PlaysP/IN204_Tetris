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


class Server {
    enet::ENetAddress address;
    std::vector<enet::ENetPeer*> clients;
    // Utilisation d'un mutex pour protéger l'accès à la liste des clients entre les threads
    std::mutex clients_mutex;
    enet::ENetHost* server;
    std::thread receiverThread;

    std::string clientName;

    Grid clientGridObj;
    std::vector<std::vector<char>>& clientGrid;


    bool clientGameOver;
    bool running = true;
    bool dataReceived = false;

public:
    Server(std::string aClientName): clientName(aClientName), clientGrid(clientGridObj.getGrid()) {
        address.host = enet::ENET_HOST_ANY;
        address.port = ADDRESS_PORT;

        server = enet::enet_host_create(
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
            enet::enet_host_destroy(server);
        }
    };

    void startReceiving() {
        receiverThread = std::thread([this]() {
        enet::ENetEvent event;
        while (running) {
            int result = enet::enet_host_service(server, &event, 100);
            if (result > 0) {
                switch (event.type) {
                case enet::ENET_EVENT_TYPE_CONNECT: {
                    std::cout << "Client connecté: " << event.peer << std::endl;

                    // Timeout agressif pour détecter les clients morts
                    enet::enet_peer_timeout(event.peer, 5000, 10000, 30000);

                    std::lock_guard<std::mutex> lock(clients_mutex);
                    clients.push_back(event.peer);
                    break;
                }

                case enet::ENET_EVENT_TYPE_RECEIVE: {
                    deserializeGrid(event.packet->data, event.packet->dataLength, clientGrid, clientGameOver);
                    dataReceived = true;
                    enet::enet_packet_destroy(event.packet);
                    break;
                }

                case enet::ENET_EVENT_TYPE_DISCONNECT:
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
            enet::enet_host_flush(server);
        }
    });
    }

    int send(Grid& localGrid, bool localGameOver) {
        std::lock_guard<std::mutex> lock(clients_mutex);
        if (clients.empty()) {
            return 1;
        } else {
            for (enet::ENetPeer* peer : clients) {
                auto data = serializeGrid(localGrid.getGrid(), localGameOver);

                enet::ENetPacket* packet = enet::enet_packet_create(
                    data.data(),
                    data.size(),
                    enet::ENET_PACKET_FLAG_RELIABLE);
                    enet::enet_peer_send(peer, 0, packet);
            }
            enet::enet_host_flush(server);
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
        std::lock_guard<std::mutex> lock(clients_mutex);
        return clients.size();
    }

    bool isClientDisconnected() {
        std::lock_guard<std::mutex> lock(clients_mutex);
        return clients.empty();
    }

};
