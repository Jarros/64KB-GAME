#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS

class HUD;
class Terrain;
class Player;

class Networking {
private:
	HUD* hud_ = nullptr;
	Terrain* terrain_ = nullptr;
	Player* player_ = nullptr;
	int tick = 0;
public:
	void bindReferences(HUD* hud, Terrain* terrain, Player* player);
	void serverReceiveLoop();//Async, detached thread
	void clientReceiveLoop();//Async, detached thread

	enum class eMsg : unsigned char {
		null,
		levelDataFromServer,
		playerDataFromServer,
		playerDataFromClient,
		entitiesData,
		test,
		connection,
		textFromServer,
		textFromClient,
	};

	struct Message {
		Message(eMsg eMsg, size_t size, void* serializedData);
		Message(void* msgHeader, void* msgData);
		~Message();
		void* Serialize();
		unsigned char* serializedData_ = nullptr;
		eMsg eMsg_ = eMsg::null;
		size_t size_ = 0;
		size_t fullSize_ = 0;
		unsigned char* dataBlock_ = nullptr;
	};

	SOCKET serverSocket;
	SOCKET acceptSocket;
	SOCKET clientSocket;

	bool serverIsUp = false;
	bool clientIsUp = false;

	void mainLoop();

	int sendToClientRaw(const void* buffer, size_t len);
	void sendToClient(eMsg eMsg, void* data, size_t size);
	void sendTerrainDataToClient();

	std::string _itos(int n);

	int createSocket();

	int bindSocket();

	void sendText(const char* text);

	void receiveMessage(Message* msg);

	void onConnection();

	void receiveText(const char* text);

	int acceptConnections();

	int networkMain();

	int sendToServerRaw(const void* buf, size_t len);
	void sendToServer(eMsg eMsg, void* data, size_t size);

	int networkMainClient(std::string ip);

	int connectToServer(std::string ip);

	int acceptClient();

	int acceptServerData();
};

extern Networking* network;