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

    std::string serverName;

    std::vector<std::vector<char>> serverGrid;

    // tetromino& serverTetromino;
    
    bool serverGameOver = false;

public:
    Client(char* aServerAdress) {
    if (enet::enet_initialize() != 0) {
        throw std::runtime_error("Failed to initialize ENet");
    }
    atexit(enet::enet_deinitialize);

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

    int serviceResult = enet_host_service(client, &event, 10000);
    if (serviceResult > 0 && event.type == enet::ENET_EVENT_TYPE_CONNECT) {
        std::cout << "Connecté au serveur" << std::endl;
    } else {
        throw std::runtime_error("Timeout connexion");
    }
    };

    ~Client() {
        enet::enet_peer_disconnect(server, 0);
        enet::enet_host_destroy(client);
    }

    void startReceiving() {
        std::thread receiver([this]() {
        while (true) {
            if (enet_host_service(client, &event, 100) > 0) {
                if (event.type == enet::ENET_EVENT_TYPE_RECEIVE) {
                    enet::ENetPeer* sender = event.peer;
                    deserializeGrid(event.packet->data, event.packet->dataLength, serverGrid, serverGameOver);
                    enet::enet_packet_destroy(event.packet);
                }
            }
        }
    });
    receiver.detach();
    }
    
    void send(Grid& localGrid, bool localGameOver) {
        auto data = serializeGrid(localGrid.getGrid(), localGameOver);
        enet::ENetPacket* packet = enet::enet_packet_create(
            data.data(),
            data.size(),
            enet::ENET_PACKET_FLAG_RELIABLE
        );
        enet::enet_peer_send(server, 0, packet);
    }

    void disconnect() {
        enet::enet_peer_disconnect(server, 0);
        enet::enet_host_destroy(client);
    }

    std::vector<std::vector<char>>& getServerGrid() {
        return serverGrid;
    }

    bool isServerGameOver() {
        return serverGameOver;
    }
    
};