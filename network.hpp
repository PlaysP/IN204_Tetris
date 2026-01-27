#include <iostream>
#include "constant.hpp"
#include "grid.hpp"
#include <map>
namespace enet {
    #include <enet/enet.h>
}

class Client {
    enet::ENetAddress address;
    enet::ENetHost* client;
    enet::ENetPeer* server;
    enet::ENetEvent event;

    std::map<int, std::string> peerNames;
    std::string serverName;

    std::map<int, Grid&> peerGrids;
    Grid& serverGrid;

    std::map<int, Grid&> peerTetrominos;
    Grid& serverTetromino;

public:
    Client(char* aServerHost) {
        if (enet::enet_initialize() != 0) {
        throw std::runtime_error("Failed to initialize ENet");
    }
    atexit(enet::enet_deinitialize);

    client = enet::enet_host_create(nullptr, 2, 1, 0, 0);

    if (!client) {
        throw std::runtime_error("Failed to create ENet client host");
    }

    enet::enet_address_set_host(&address, aServerHost);
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
        // enet::enet_deinitialize();
    }
    
    // Mon idée est de faire comme dans mon exemple avec client.cpp un thread constamment
    // activé traitant les receptions et mettant a jour les grilles et tetrominos
    // des autres joueurs (peers et serveur)

    // A chaque iteration (dans la boucle du jeu, on affiche les grilles et tetrominos
    // des autres joueurs en fonction de l'état actuel stocké dans les maps peerGrids et peerTetrominos)
    
};