#pragma once

#include <iostream>
#include "constant.hpp"
#include "grid.hpp"
#include "tetrominos.hpp"
#include <map>
#include <thread>
#include "network_utils.hpp"

class Client {
    enet::ENetAddress address;
    enet::ENetHost* client;
    enet::ENetPeer* server;
    enet::ENetEvent event;
    std::thread receiverThread;

    std::string serverName;

    Grid serverGridObj;
    std::vector<std::vector<char>>& serverGrid;

    // tetromino& serverTetromino;
    
    bool serverGameOver = false;
    bool running = true;
    bool dataReceived = false;
    bool serverDisconnected = false;

public:
    Client(std::string aServerName, const char* aServerAdress): serverName(aServerName), serverGrid(serverGridObj.getGrid()) {
    client = enet::enet_host_create(nullptr, 1, 1, 0, 0);

    if (!client) {
        throw std::runtime_error("Failed to create ENet client host");
    }

    enet::enet_address_set_host(&address, aServerAdress);
    address.port = ADDRESS_PORT;

    server = enet_host_connect(client, &address, 2, 0);
    if (!server) {
        std::cerr << "Connection failed" << std::endl;
    }

    int serviceResult = enet::enet_host_service(client, &event, 10000);
    if (serviceResult > 0 && event.type == enet::ENET_EVENT_TYPE_CONNECT) {
        std::cout << "Connecté au serveur" << std::endl;
    } else {
        throw std::runtime_error("Timeout connexion");
    }
    };

    ~Client() {
        running = false;
        if (receiverThread.joinable()) {
            receiverThread.join();
        }
        if (server) {
            enet::enet_peer_disconnect(server, 0);
        }
        if (client) {
            enet::enet_host_destroy(client);
        }
    }

    void startReceiving() {
        receiverThread = std::thread([this]() {
        enet::ENetEvent localEvent;
        while (running) {
            int result = enet::enet_host_service(client, &localEvent, 100);
            if (result > 0) {
                switch (localEvent.type) {
                case enet::ENET_EVENT_TYPE_RECEIVE:
                    deserializeGrid(localEvent.packet->data, localEvent.packet->dataLength, serverGrid, serverGameOver);
                    dataReceived = true;
                    enet::enet_packet_destroy(localEvent.packet);
                    break;
                    
                case enet::ENET_EVENT_TYPE_DISCONNECT:
                    std::cout << "Serveur déconnecté !" << std::endl;
                    serverDisconnected = true;
                    break;
                    
                default:
                    break;
                }
            }
            // Flush les événements réseau pour éviter le blocage
            enet_host_flush(client);
        }
    });
    }
    
    int send(Grid& localGrid, bool localGameOver) {
        auto data = serializeGrid(localGrid.getGrid(), localGameOver);
        enet::ENetPacket* packet = enet::enet_packet_create(
            data.data(),
            data.size(),
            enet::ENET_PACKET_FLAG_RELIABLE
        );
        enet::enet_peer_send(server, 0, packet);
        enet_host_flush(client);
        return 0;
    }

    void disconnect() {
        running = false;
        enet::enet_peer_disconnect(server, 0);
    }

    std::vector<std::vector<char>>& getServerGrid() {
        return serverGrid;
    }

    bool isServerGameOver() {
        return serverGameOver;
    }

    bool hasReceivedData() {
        return dataReceived;
    }

    void resetDataReceived() {
        dataReceived = false;
    }
    
    bool isServerDisconnected() {
        return serverDisconnected;
    }
};