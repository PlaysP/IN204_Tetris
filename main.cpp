#include <raylib.h>
#include "server.hpp"
#include "client.hpp"
#include <iostream>
#include <random>
#include <thread>
#include <chrono>
#include "grid.hpp"
#include "tetrominos.hpp"
#include "timer.hpp"
#include "constant.hpp"
#include "draw.hpp"
#include <queue>


int updateScore(int rowsRemoved, int score, int level);
int updateLevel(int level, int* rowsRemovedCounter);
char randomTetromino();
void tetrominoIsGrounded();

float speed[21] = {
    53.0/60.0f, 49.0/60.0f, 45.0/60.0f, 41.0/60.0f, 37.0/60.0f,
    33.0/60.0f, 28.0/60.0f, 22.0/60.0f, 17.0/60.0f, 11.0/60.0f,
    10.0/60.0f, 9.0/60.0f, 8.0/60.0f, 7.0/60.0f, 6.0/60.0f,
    6.0/60.0f, 5.0/60.0f, 5.0/60.0f, 4.0/60.0f, 4.0/60.0f, 3.0/60.0f
};

std::queue<char> futureTetrominos;
Grid grid;
tetromino t = tetromino('T', grid);
int score = 0;
int level = 0;
bool EnableBotPos = true;
int rowsRemovedOnce;
int rowsRemovedCounter = 0;
RepeatingTimer fallTimer(speed[level]);

// Timer pour limiter la fréquence d'envoi réseau (10 fois par seconde max)
float networkSendTimer = 0.0f;
const float NETWORK_SEND_INTERVAL = 0.1f; // 100ms

int main()
{
    // Initialiser ENet une seule fois au démarrage
    if (enet::enet_initialize() != 0) {
        std::cerr << "Failed to initialize ENet" << std::endl;
        return 1;
    }
    atexit(enet::enet_deinitialize);

    grid.print();

    float backgroundTimer = 0.0f;
    RepeatingTimer SendTimer(NETWORK_SEND_INTERVAL);

    bool fastfall = false;
    bool grounded = false;

    futureTetrominos.push(randomTetromino());
    futureTetrominos.push(randomTetromino());

    KeyPressTimer keyRight(0.15f, KEY_RIGHT);
    KeyPressTimer keyLeft(0.15f, KEY_LEFT);
    KeyPressTimer keyDown(0.15f, KEY_DOWN);
    KeyPressTimer keyUp(0.20f, KEY_UP);
    KeyPressTimer keySpace(0.20f, KEY_SPACE);
    KeyPressTimer keyEnter(0.20f, KEY_ENTER);

    KeyPressTimer key1(0.20f, KEY_ONE);
    KeyPressTimer key2(0.20f, KEY_TWO);
    KeyPressTimer keyC(0.20f, KEY_C);
    KeyPressTimer keyJ(0.20f, KEY_J);

    t = tetromino(futureTetrominos.front(), grid);
    futureTetrominos.pop();
    futureTetrominos.push(randomTetromino());

    Grid advGrid;

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Tetris");

    if (!IsWindowReady()) {
        std::cerr << "Failed to initialize window (display unavailable). Exiting." << std::endl;
        return 1;
    }
    printf("Window initialized successfully.\n");

    SetTargetFPS(60);              

    bool gameOver = false;
    bool HomeScreen = true;
    bool Multi = false;
    bool MultiScreen = false;
    bool Playing = false;
    bool WaitingForClient = false;
    bool IsServer;
    bool AskingForIP = false;

    // Variables pour la saisie de l'IP
    const int MaxIPLength = 15;
    char ipAddress[16] = "";
    int charCount = 0;

    // Créer le serveur ou client UNE SEULE FOIS avant la boucle
    Server* server = nullptr;
    Client* client = nullptr;

    while (!WindowShouldClose())
    {
    if (HomeScreen) {
        BeginDrawing();
        ClearBackground(BLACK);
        drawMiddle("Press 1 for Singleplayer\nPress 2 for Multiplayer");
        EndDrawing();

        if (key1.IsPressedAndReady()) {
            HomeScreen = false;
            Playing = true;
            MultiScreen = false;
        }
        if (key2.IsPressedAndReady()) {
            HomeScreen = false;
            Multi = true;
            MultiScreen = true;
        }
    }
    if (MultiScreen) {
        BeginDrawing();
        ClearBackground(BLACK);
        drawMiddle("Press C to Create game\nPress J to Join game");
        EndDrawing();

        if (keyC.IsPressedAndReady()) {
            IsServer = true;
            server = new Server("ServeurRayan");
            server->startReceiving();
            MultiScreen = false;
            WaitingForClient = true;
        }
        if (keyJ.IsPressedAndReady()) {
            IsServer = false;
            MultiScreen = false;
            AskingForIP = true;
        }
    }
    if (AskingForIP) {
        int key = GetCharPressed();
        while (key > 0) {
            // NOTE: Only allow keys in range [32..125]
            if ((key >= 32) && (key <= 125) && (charCount < MaxIPLength))
            {
                ipAddress[charCount] = (char)key;
                ipAddress[charCount+1] = '\0'; // Add null terminator at the end of the string
                charCount++;
            }
            key = GetCharPressed();  // Check next character in the queue
        }

        if (IsKeyPressed(KEY_BACKSPACE)) {
            charCount--;
            if (charCount < 0) charCount = 0;
            ipAddress[charCount] = '\0';
        }

        if (keyEnter.IsPressedAndReady() && charCount > 0) {
            client = new Client("Client", ipAddress);
            client->startReceiving();
            AskingForIP = false;
            Playing = true;
            // Réinitialiser pour la prochaine fois
            charCount = 0;
            ipAddress[0] = '\0';
        }

        BeginDrawing();
        ClearBackground(BLACK);
        drawAskIP(ipAddress);
        EndDrawing();
        
        keyEnter.Update();
    }

    if (WaitingForClient) {
        BeginDrawing();
        ClearBackground(BLACK);
        drawMiddle("Waiting for client to connect...");
        EndDrawing();
        
        if (server->numberOfClients() > 0) {
            std::cout << "Client connected!\n";
            WaitingForClient = false;
            Playing = true;
        }
    } 
    if (Playing) {
        // Vérifier si le joueur s'est déconnecté
        if (Multi && ((!IsServer && client->isServerDisconnected()) || (IsServer && server->isClientDisconnected()))) {
            BeginDrawing();
            ClearBackground(BLACK);
            drawMiddle("Opponent disconnected !\nPress SPACE to return to the menu");
            EndDrawing();
            
            if (keySpace.IsPressedAndReady()) {
                Playing = false;
                HomeScreen = true;
                Multi = false;
                delete client;
                client = nullptr;
            }
            keySpace.Update();
            continue;
        }
        
        if (Multi) {
            // SEND
            SendTimer.Update();
            if (SendTimer.Trigger()) {
                if (IsServer) server->send(grid, gameOver);
                else client->send(grid, gameOver);
            }
            // RECEIVE 
            if (IsServer) {
            // Check for received data from client
                if (server->hasReceivedData()) {
                    advGrid = server->getClientGrid();
                    server->resetDataReceived();
                }
            } else {
            // Check for received data from server
                if (client->hasReceivedData()) {
                    advGrid = client->getServerGrid();
                    client->resetDataReceived();
                }
            }
        }

        if (gameOver || ( Multi && IsServer && server->isClientGameOver())
                    || (Multi && !IsServer && client->isServerGameOver())) {

        BeginDrawing();

        ClearBackground(BLACK);
        drawBackground(score, level, futureTetrominos, backgroundTimer, grid);
    

        if (Multi) {
            grid.draw(false);
            if (gameOver) drawGameOver("Game\nOver", "Press SPACE to\nreturn to the menu",false);
            else drawGameOver("Game\nWin", "Press SPACE to\nreturn to the menu", false);
            advGrid.draw(true);
            if ((IsServer && server->isClientGameOver()) || (!IsServer && client->isServerGameOver())) drawGameOver("Game\nOver", "Press SPACE to\nreturn to the menu", true);
            else drawGameOver("Game\nWin", "Press SPACE to\nreturn to the menu", true);
        } else {
            grid.draw(false);
            if (gameOver) drawGameOver("Game\nOver", "Press SPACE to play",false);
            else drawGameOver("Game\nWin", "Press SPACE to play", false);
        }

        EndDrawing();

        if (keySpace.IsPressedAndReady()) {
            // Reset game
            grid.reset();

            t = tetromino(futureTetrominos.front(), grid);
            futureTetrominos.pop();
            futureTetrominos.push(randomTetromino());

            grounded = false;
            gameOver = false;
            score = 0;
            level = 0;
            rowsRemovedCounter = 0;
            fallTimer.SetInterval(speed[level]);

            if (Multi) {
                if (IsServer) delete server;
                else delete client;
                Playing = false;
                HomeScreen = true;
                Multi = false;
            }
        }
        keySpace.Update();

    } else {
        // Game is still playing

        fallTimer.Update();
        if (fallTimer.Trigger() || fastfall) {
            if (fastfall) {
                fastfall = false;
                fallTimer.Reset();
            }
            grounded = t.fall(grid);
            if (grounded) {
                tetrominoIsGrounded();
                if (grid.GameOver()){
                    gameOver = true;
                }
            }
        } // Make the tetromino fall every second
        if(keySpace.IsPressedAndReady()){t.fastFall(); fastfall = true;}
        if(keyRight.IsPressedAndReady()){t.moveRight(grid);}
        if(keyLeft.IsPressedAndReady()){t.moveLeft(grid);}
        if(keyDown.IsPressedAndReady()){fallTimer.SkipToNextTrigger();}
        if(keyUp.IsPressedAndReady()){t.rotateClockwise(grid);}
        if(keyEnter.IsPressedAndReady()){EnableBotPos = !EnableBotPos;}

        keyDown.Update();
        keyLeft.Update();
        keyRight.Update();
        keyUp.Update();
        keySpace.Update();


        BeginDrawing();

            ClearBackground(BLACK);
            drawBackground(score, level, futureTetrominos, backgroundTimer, grid);
            grid.draw(false);
            if (Multi) advGrid.draw(true);
            t.draw(EnableBotPos);

        EndDrawing();
    }
    }
    backgroundTimer += GetFrameTime();
    }
    
    // Nettoyer les ressources réseau
    if (server != nullptr) delete server;
    if (client != nullptr) delete client;
    
    CloseWindow();
    return 0;
} 

void tetrominoIsGrounded(){
    grid.placeTetromino(t);
    rowsRemovedOnce = grid.removeFilledRows();
    rowsRemovedCounter += rowsRemovedOnce;

    level = updateLevel(level,&rowsRemovedCounter);
    score = updateScore(rowsRemovedOnce, score, level);
    if(level <= 20) fallTimer.SetInterval(speed[level]);

    // New tetromino
    t = tetromino(futureTetrominos.front(), grid);
    futureTetrominos.pop();
    futureTetrominos.push(randomTetromino());
}

char shapes[] = {'T', 'O', 'I', 'J', 'L', 'S', 'Z'};

char randomTetromino(){

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, 6);

    return shapes[dist(gen)];
}



int updateLevel(int level, int* rowsRemovedCounter){
    if(*rowsRemovedCounter >=10){
        *rowsRemovedCounter -=10;
        level++;
    }
    return level;
}


int updateScore(int rowsRemoved,int score, int level){
    int points;
    switch(rowsRemoved) {
        case 0:
            points = 0;
            break;
        case 1:
            points = 40 * (level + 1);
            break;
        case 2:
            points= 100 * (level + 1);
            break;
        case 3:
            points = 300 * (level + 1);
            break;
        default:
            points = 1200 * (level + 1);
            break;
    }

    score += points;

    return score;
}