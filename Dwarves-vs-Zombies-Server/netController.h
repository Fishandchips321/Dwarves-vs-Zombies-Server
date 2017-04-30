#pragma once
#include <string>
#include <queue>
#include <boost/thread.hpp>
#include <boost/asio.hpp>
#include <boost/date_time.hpp>
#include <string>
#include "eventController.h"
#include "counter.h"

using boost::asio::ip::tcp;
using boost::asio::ip::udp;

//setup server networking stuff
//see kindle book
//tcp acceptor

/*
net message structure (client=>server)
timestamp - string 8 long <- total universal miliseconds
client ID (-1 if there isn't one) - int 4 long <- change to unsigned char
network control message - int 4 long <- change to unsigned char
paramaters - string ?? long

net message structure (server=>client)
timestamp - string 8 long <- total universal miliseconds
network control message - int 4 long <- change to unsigned char
paramaters - string ?? long

For the changes to unsigned char. they will need to be converted into signed ints somehow. static_cast?

*/

class netController
{
public:
	struct netMessage;
	netController();
	static bool init(); //initalises the net controller
	static void update();
	static void updateLobby(); //called when the server is in the lobby state
	static void updateGame(); //called when the server is in the play state
	static void eventUpdate(); //called when an SDL2 event is sent. Is currently unused
	static void sendGameTo(std::string data, int client); //sends a game message to a certain client
	static void broadcastGame(std::string data); //sends a game message to every client

	static void sendControlTo(std::string data, int client);//sends a control message to a certain client
	static void sendControlTo(int ncm, int client, std::string params = "");
	static void sendControlTo(netMessage message, int client);

	static void broadcastControl(std::string data);//sends a control message to every client
	static void broadcastControl(int ncm, std::string params = "");
	static void broadcastControl(netMessage message);
	~netController();

	enum class clientState //a copy of nState from the client program
	{
		DISCONNECTED,
		CONNECTING,
		CONNECTED,
		AUTHENTICATING, //unused
		AUTHREPLY,
		SEND_USER_DATA,
		LOBBY,
		LOAD_GAME,
		PLAY_GAME,
		GAME_END,
		DISCONNECTING,
		QUICK_DISCONNECT,
		//server side only
		AUTH_TOKEN,
		ERROR_CONNECT,
		ERROR_CONNECT_UDP
	};

	enum NCM
	{
		//control messages
		KEEPALIVE_PING,
		KEEPALIVE_PONG,
		UDP_CONNECT, //both TCP (server to client) and UDP (client to server)
		AUTHENTICATE, //client to server: client username, client login token // server to client: no paramaters
		USER_ID, //server to client: player ID
		LOBBY_PLAYER_UPDATE,
		LOAD_MAP, //server to client: map ID // client to server: no paramaters
		MAP_LOADED, //client to server: no paramaters
		MAP_LOAD_ERROR, //client to server: no paramaters
		PLAYER_SPAWN, //client to server: class ID
		PLAYER_MOB_SELECT,// client to server: class ID //for lobby
		DISCONNECT,
		GET_STATE,
		RESYNC_STATE,
		QUICK_DISCONNECT, //client to server: no paramaters
		//game messages //client to server: updates of client player. server to clients: updates of other players and resyncing of client player
		PLAYER_UPDATE, // client to server: x, y, x velocity, y velocity, flying (optional; bool)
		PLAYER_ATTACK, //client to server: equipped weapon (player hit will be determined by the server) // server to client: clientID (person attacking), equipped weapon (for animation), clientID (person being attacked; optional, only if a player was hit)
		PLAYER_PLACE, //client to server: blockID, x, y
		PLAYER_BREAK,
		PLAYER_DIE,
		PLAYER_DAMAGED,
		SPAWN_PROJECTILE //server to client: projectile ID, x, y, x velocity, y velocity, team (dwarves or zombies)
	};

	struct netMessage
	{
		int playerID; //unique ID of the client that sent a message. unused when sent from a server
		int NCM = -1; //the control message
		std::vector<std::string> paramaters; //holds any paramaters that were sent along with the message
		boost::posix_time::ptime timestamp; //12 chars for microsecond timestamp
		std::string completeMessage; //holds the complete message in string form
	};

	struct clientData
	{
		clientData() :socket(netController::io_service), receiving(false) {}
		void nextGameMessage() //gets the next game message and puts it into currentGameMessage
		{
			if (!gameRecieved.empty())
			{
				currentGameMessage = gameRecieved.front();
				gameRecieved.pop();
			}
			else
			{
				netMessage m;
				currentGameMessage = m;
			}
		}
		void nextControlMessage() //gets the next control message and puts it into currentControlMessage
		{
			if (!gameRecieved.empty())
			{
				currentControlMessage = gameRecieved.front();
				gameRecieved.pop();
			}
			else
			{
				netMessage m;
				currentControlMessage = m;
			}
		}

		tcp::socket socket;
		bool receiving;
		boost::array<char, 1024> buffer;
		tcp::endpoint TEndpoint;
		udp::endpoint UEndpoint;
		counter lastMessage;
		clientState state;
		std::string username;
		std::string token;

		std::queue<netMessage> gameRecieved;
		std::queue<std::string> gameToSend;
		std::queue<netMessage> controlRecieved;
		std::queue<std::string> controlToSend;
		netMessage currentGameMessage;
		netMessage currentControlMessage;
	};

	static bool hostsConnected; //whether there is anyone connected to the server
	static const int maxClients = 20;
	static clientData* clients[maxClients];

private:

	static void netControlSend();
	static void netControlRecieve();
	static void netGameSend();
	static void netGameRecieve();
	static void connectHandler();
	static void getNextMessage();
	static netMessage unpackMessage(std::string data);
	static std::vector<std::string> splitString(std::string to_split);
	static void cleanupClient(int ID);
	static void TCPRecieveHandler(int client);

	static boost::asio::io_service io_service;
	static boost::thread gameSendThread, gameRecieveThread, controlSendThread, controlRecieveThread, tcpAcceptorThread;

	static udp::socket USocket;
	static boost::mutex USLock;

	static tcp::endpoint TEndpoint;
	static tcp::acceptor TAcceptor;

	static const std::string port;
	static bool halt;
	static int players;
};

