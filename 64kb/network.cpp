
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <iostream>
#include <winsock2.h>
#include "main.h"
#include "terrain.h"
#include "input.h"
#include "hud.h"
#include <thread>
#include "network.h"
#include "player.h"
#include "bots.h"



    std::string Networking::_itos(int n) {
        return std::to_string(n);
    }

    int Networking::createSocket() {
        // Create a socket
        serverSocket = INVALID_SOCKET;
        serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

        // Check for socket creation success
        if (serverSocket == INVALID_SOCKET) {
            hud_->PrintConsole(std::string("Error at socket(): ").append(_itos(WSAGetLastError())));
            WSACleanup();
            return 0;
        }
        else {
            hud_->PrintConsole("Socket is OK!");
            serverIsUp = true;
        }
    }

    int Networking::bindSocket() {
        // Bind the socket to an IP address and port number
        sockaddr_in service;
        service.sin_family = AF_INET;
        service.sin_addr.s_addr = INADDR_ANY;// inet_addr("127.0.0.1");  // Replace with your desired IP address
        service.sin_port = htons(13337);  // Choose a port number

        // Use the bind function
        if (bind(serverSocket, reinterpret_cast<SOCKADDR*>(&service), sizeof(service)) == SOCKET_ERROR) {
            hud_->PrintConsole(std::string("bind() failed: ").append(_itos(WSAGetLastError())));
            closesocket(serverSocket);
            WSACleanup();
            return 0;
        }
        else {
            hud_->PrintConsole("bind() is OK!");
            // Listen for incoming connections
            if (listen(serverSocket, 1) == SOCKET_ERROR) {
                hud_->PrintConsole(std::string("listen(): Error listening on socket: ").append(_itos(WSAGetLastError())));
            }
            else {
                //hud_->PrintConsole("listen() is OK! I'm waiting for new connections...");
                hud_->PrintConsole("Listening...");
            }
        }
    }

    void Networking::mainLoop()
    {
        if (serverIsUp && connectionEstablished) {
            sendToClient(eMsg::playerDataFromServer, player_->serialize(), sizeof(player_->data));
        }
        else if (clientIsUp && connectionEstablished) {
            sendToServer(eMsg::playerDataFromClient, player_->serialize(), sizeof(player_->data));
        }
    }

    void Networking::bindReferences(HUD* hud, Terrain* terrain, Player* player, BotManager* bots)
    {
        hud_ = hud;
        terrain_ = terrain;
        player_ = player;
        bots_ = bots;
    }

    int Networking::sendToClientRaw(const void* buf, size_t len) {
        //unsigned char* buffer = new unsigned char[len] {0};
        //memcpy(buffer, buf, len);

        int sbyteCount = send(acceptSocket, (char*)buf, len, 0);
        if (sbyteCount == SOCKET_ERROR) {
            std::cout << "Server send error: " << WSAGetLastError() << std::endl;
            hud_->PrintConsole(std::string("Server send error: ").append(_itos(WSAGetLastError())));
            return -1;
        }
        else {
            std::cout << "Server: Sent " << sbyteCount << " bytes" << std::endl;
            //hud_->PrintConsole(std::string("Server: Sent ").append(_itos(sbyteCount)));
        }
    }


    Networking::Message::Message(eMsg eMsg, size_t size, void* serializedData)
        : eMsg_(eMsg), size_(size), serializedData_((unsigned char*)serializedData)
    {
        fullSize_ = size_ + sizeof(size_t) + sizeof(size_t) + sizeof(eMsg);
    }
    Networking::Message::Message(void* msgHeader, void* serializedData)
    {
        memcpy(&eMsg_, (unsigned char*)(msgHeader), sizeof(eMsg));
        memcpy(&size_, (unsigned char*)(msgHeader)+sizeof(eMsg), sizeof(size_));
        serializedData_ = new unsigned char[size_] {};
        memcpy(serializedData_, serializedData, size_);
        fullSize_ = size_ + sizeof(size_t) + sizeof(size_t) + sizeof(eMsg);
        
        std::string str("");
        //if(eMsg_ == eMsg::text)
        //    str.append(" text: ").append((const char*)serializedData_);
        //str.append(std::to_string(*((unsigned char*)serializedData_)));
    }
    Networking::Message::~Message() 
    {
        delete serializedData_;
        delete dataBlock_;
    }
    void* Networking::Message::Serialize()
    {
        dataBlock_ = new unsigned char[fullSize_] {};
        //memcpy((void*)data, (void*) & eMsg_, sizeof(eMsg) + sizeof(size_t) + sizeof(size_t));
        
        memcpy(dataBlock_, &eMsg_, sizeof(eMsg));
        memcpy(dataBlock_ + sizeof(eMsg), &size_, sizeof(size_));
        memcpy(dataBlock_ + sizeof(eMsg) + sizeof(size_), &fullSize_, sizeof(fullSize_));
        memcpy(dataBlock_ + sizeof(eMsg) + sizeof(size_) + sizeof(fullSize_), serializedData_, size_);
        
        return (void*)dataBlock_;
    }

    void Networking::sendText(const char* text) {
        if (clientIsUp) {
            std::string str = std::string("Client: ").append(text);
            hud_->PrintConsole(str, eClrs::blue);
            Message* msg = new Message(eMsg::textFromClient, strlen(text)+1, (void*)text);
            sendToServerRaw(msg->Serialize(), msg->fullSize_);
        }
        if (serverIsUp) {
            std::string str = std::string("Server: ").append(text);
            hud_->PrintConsole(str, eClrs::green);
            Message* msg = new Message(eMsg::textFromServer, strlen(text) + 1, (void*)text);
            sendToClientRaw(msg->Serialize(), msg->fullSize_);
        }
    }

    void Networking::receiveMessage(Message* msg) {

        eClrs clr;
        std::string str = "";
        switch (msg->eMsg_) {
        case eMsg::levelDataFromServer:
            terrain_->BuildScene(true, true, *((unsigned long int*)msg->serializedData_));
            str.append("Seed: ").append(std::to_string(*((unsigned long int*)msg->serializedData_)));
            hud_->PrintConsole(str);
            break;
        case eMsg::textFromClient:
            clr = eClrs::blue;
            str.append("Client: ").append((const char*)msg->serializedData_);
            hud_->PrintConsole(str, clr);
            break;
        case eMsg::playerDataFromServer:
        {
            Player::sData data;
            memcpy(&data, msg->serializedData_, sizeof(Player::sData));
            otherPlayer& ply = bots_->otherPlayers[0];
            ply.online = true;
            ply.x = data.xpos;
            ply.y = data.ypos - 2.0f;
            ply.z = data.zpos;
            ply.yr = data.yrot;
        }
            break;
        case eMsg::playerDataFromClient:
        {
            Player::sData data;
            memcpy(&data, msg->serializedData_, sizeof(Player::sData));
            otherPlayer& ply = bots_->otherPlayers[0];
            ply.online = true;
            ply.x = data.xpos;
            ply.y = data.ypos - 2.0f;
            ply.z = data.zpos;
            ply.yr = data.yrot;
        }
            break;
        case eMsg::textFromServer:
            clr = eClrs::green;
            str.append("Server: ").append((const char*)msg->serializedData_);
            hud_->PrintConsole(str, clr);
            break;
        default:
            clr = eClrs::default;
        }
    }

    void Networking::sendToClient(eMsg eMsg, void* data, size_t size) {
        Message* msg = new Message(eMsg, size, data);
        sendToClientRaw(msg->Serialize(), msg->fullSize_);
    }

    void Networking::sendToServer(eMsg eMsg, void* data, size_t size) {
        Message* msg = new Message(eMsg, size, data);
        sendToServerRaw(msg->Serialize(), msg->fullSize_);
    }

    //Message* msg = new Message(eMsg::connection, sizeof(terrain_->seed), &(terrain_->seed));
    void Networking::onConnection() {
        sendToClient(eMsg::levelDataFromServer, &(terrain_->seed_), sizeof(terrain_->seed_));
        std::string str = "";
        str.append("Seed: ").append(std::to_string(terrain_->seed_));
        hud_->PrintConsole(str);
    }

    int Networking::acceptConnections() {
        //while (true) 
        {

            // Accept incoming connections
            acceptSocket = accept(serverSocket, nullptr, nullptr);

            // Check for successful connection
            if (acceptSocket == INVALID_SOCKET) {
                hud_->PrintConsole(std::string("accept failed: ").append(_itos(WSAGetLastError())));
                WSACleanup();
                return -1;
            }
            else {
                hud_->PrintConsole("accept() is OK!");
    
                //sendMsgToClient();

                if (true) {

                    //std::string str("OutputValue: ");
                    //unsigned long long int tick = GetTickCount();
                    //str.append(std::to_string(tick));
                    //hud_->PrintConsole(str);
                    //unsigned char byte = 111;
                    //Message* msg = new Message(eMsg::connection, sizeof(byte), &(byte));

                    onConnection();
                    //delete(msg);

                    //const char* text = "hello there 13123123";
                    //msg = new Message(eMsg::textFromServer, strlen(text)+1, (void*)text);
                    //sendToClientRaw(msg->Serialize(), msg->fullSize_);
                }
            }


            while (true) {
                serverReceiveLoop(); // Receiving data from the client
            }

        }

    }

    void Networking::serverReceiveLoop() {

        char receiveBuffer[sizeof(eMsg) + sizeof(size_t) + sizeof(size_t)];

        int rbyteCount = recv(acceptSocket, receiveBuffer, sizeof(eMsg) + sizeof(size_t) + sizeof(size_t), 0);

        size_t size_ = 0;
        memcpy(&size_, (char*)(&receiveBuffer[0]) + sizeof(eMsg), sizeof(size_));

        char* serializedData = new char[size_];
        rbyteCount = recv(acceptSocket, &serializedData[0], size_, 0);

        Message* msg = new Message((void*)(&receiveBuffer), (void*)(&serializedData[0]));
        receiveMessage(msg);

        // Receive data from the client
        if (rbyteCount < 0) {
            std::cout << "Server recv error: " << WSAGetLastError() << std::endl;
            hud_->PrintConsole(std::string("Server recv error: ").append(_itos(WSAGetLastError())));
            return;
        }
        else {
            connectionEstablished = true;
            std::cout << "Received data: " << receiveBuffer << std::endl;
            //hud_->PrintConsole(std::string("Received data: ").append(receiveBuffer));
        }
    }

    int Networking::networkMain() {
        // Initialize WSA variables
        WSADATA wsaData;
        int wsaerr;
        WORD wVersionRequested = MAKEWORD(2, 2);
        wsaerr = WSAStartup(wVersionRequested, &wsaData);

        // Check for initialization success
        if (wsaerr != 0) {
            hud_->PrintConsole("The Winsock dll not found!");
            return 0;
        }
        else {
            hud_->PrintConsole("The Winsock dll found");
            hud_->PrintConsole(std::string("The status: ").append(wsaData.szSystemStatus));
        }

        // Continue with the server setup...
        // (See the full code here: https://github.com/Tharun8951/cpp-tcp-server/")
        createSocket();
        bindSocket();
        //acceptConnections();


        //std::thread thread(acceptConnections);
        std::thread thread([this] { acceptConnections(); });
        thread.detach();

        return 0;
    }


    int Networking::connectToServer(std::string ip) {
        // Connect to the server
        sockaddr_in clientService;
        clientService.sin_family = AF_INET;
        clientService.sin_addr.s_addr = inet_addr(ip.c_str());//"127.0.0.1");  // Replace with the server's IP address
        clientService.sin_port = htons(13337);  // Use the same port as the server

        // Use the connect function
        if (connect(clientSocket, reinterpret_cast<SOCKADDR*>(&clientService), sizeof(clientService)) == SOCKET_ERROR) {
            std::cout << "Client: connect() - Failed to connect: " << WSAGetLastError() << std::endl;
            //hud_->PrintConsole(std::string("Client: connect() - Failed to connect: ").append(_itos(WSAGetLastError())));
            hud_->PrintConsole(std::string("Client: connect() - Failed to connect: ").append(_itos(WSAGetLastError())));
            WSACleanup();
            return 0;
        }
        else {
            std::cout << "Client: Connect() is OK!" << std::endl;
            std::cout << "Client: Can start sending and receiving data..." << std::endl;
            hud_->PrintConsole(std::string("Client: Connect() is OK!"));
            hud_->PrintConsole(std::string("Client: Can start sending and receiving data..."));

            clientIsUp = true;
        }
    }

    void Networking::clientReceiveLoop() {
            //return 0;

            char receiveBuffer[sizeof(eMsg) + sizeof(size_t) + sizeof(size_t)];

            int rbyteCount = recv(clientSocket, receiveBuffer, sizeof(eMsg) + sizeof(size_t) + sizeof(size_t), 0);
            
            //hud_->PrintConsole(std::string("received: ").append(std::to_string(receiveBuffer[0])));
            
            
            //hud_->PrintConsole(receiveBuffer[0]);
            //size_t size = *(size_t*)(&(receiveBuffer[0]) + sizeof(eMsg));


            //memcpy(&eMsg_, (char*)(msgData), sizeof(eMsg));
            size_t size_ = 0;
            memcpy(&size_, (char*)(&receiveBuffer[0])+sizeof(eMsg), sizeof(size_));


            //hud_->PrintConsole(std::string("size: ").append(std::to_string(size_)));
            //break;
            char* serializedData = new char[size_];
            rbyteCount = recv(clientSocket, &serializedData[0], size_, 0);

            Message* msg = new Message((void*)(&receiveBuffer), (void*)(&serializedData[0]));
            receiveMessage(msg);
            //char* serializedData = new char[size_t(&receiveBuffer[sizeof(eMsg)])];
            //int rbyteCount = recv(clientSocket, receiveBuffer, 200, 0);
            


            if (rbyteCount < 0) {
                //std::cout << "Client recv error: " << WSAGetLastError() << std::endl;
                hud_->PrintConsole(std::string("Client recv error: ").append(_itos(WSAGetLastError())));
                return;
            }
            else {
                std::cout << "Client: Received data: " << receiveBuffer << std::endl;
                //hud_->PrintConsole(std::string("Client: Received data: ").append(receiveBuffer));
            }
    }

    int Networking::acceptClient() {
        while (true) 
        {
            clientReceiveLoop(); // Receiving data from the server
        }
    }


    int Networking::sendToServerRaw(const void* buf, size_t len) {

        int sbyteCount = send(clientSocket, (char*)buf, len, 0);
        if (sbyteCount == SOCKET_ERROR) {
            std::cout << "Server send error: " << WSAGetLastError() << std::endl;
            hud_->PrintConsole(std::string("Server send error: ").append(_itos(WSAGetLastError())));
            return -1;
        }
        else {
            std::cout << "Client: Sent " << sbyteCount << " bytes" << std::endl;
            //hud_->PrintConsole(std::string("Client: Sent ").append(_itos(sbyteCount)));
        }
    }

    int Networking::networkMainClient(std::string ip) {

        // Initialize WSA variables
        WSADATA wsaData;
        int wserr;
        WORD wVersionRequested = MAKEWORD(2, 2);
        wserr = WSAStartup(wVersionRequested, &wsaData);

        // Check for initialization success
        if (wserr != 0) {
            std::cout << "The winsock dll not found" << std::endl;
            hud_->PrintConsole(std::string("The winsock dll not found"));

            return 0;
        }
        else {
            std::cout << "The Winsock dll found" << std::endl;
            hud_->PrintConsole(std::string("The winsock dll found"));

            std::cout << "The status: " << wsaData.szSystemStatus << std::endl;
            hud_->PrintConsole(std::string("The status: ").append(wsaData.szSystemStatus));

        }

        // Create a socket
        clientSocket = INVALID_SOCKET;
        clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

        // Check for socket creation success
        if (clientSocket == INVALID_SOCKET) {
            std::cout << "Error at socket(): " << WSAGetLastError() << std::endl;
            hud_->PrintConsole(std::string("Error at socket(): ").append(_itos(WSAGetLastError())));
            WSACleanup();
            return 0;
        }
        else {
            std::cout << "Socket is OK!" << std::endl;
            hud_->PrintConsole(std::string("Socket is OK!"));
        }

        // Continue with the client setup...
        // (See the full code here: https://github.com/Tharun8951/cpp-tcp-server/")

        connectToServer(ip);

        // Continue with the client setup...

        //std::thread thread(acceptClient);
        connectionEstablished = true;

        std::thread thread([this] { acceptClient(); });
        thread.detach();

        return 0;
    }

Networking* network = new Networking;