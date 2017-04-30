#include "netController.h"

boost::asio::io_service netController::io_service; //the IO service
udp::socket netController::USocket(io_service); //the socket used to communicate with the server
bool netController::hostsConnected = false; //whether a server session is active
const std::string netController::port = "37568"; //the port number of the server
boost::mutex netController::USLock; //mutex for the UDP socket
boost::thread netController::gameSendThread, netController::gameRecieveThread, netController::controlSendThread, netController::controlRecieveThread, netController::tcpAcceptorThread; //the threads for the control and game net
tcp::endpoint netController::TEndpoint(tcp::v4(), atoi(port.c_str())); //TCP Endpoint
tcp::acceptor netController::TAcceptor(io_service, TEndpoint); //TCP Acceptor
netController::clientData* netController::clients[20]; //holds the clients' net data
int netController::players = 0; //the amount of players currently connected
bool netController::halt = false; //set to true when the server is shutting down
const int netController::maxClients;


netController::netController()
{
}

bool netController::init()
{
	try
	{
		//initalize the client containers
		for (int i = 0; i < 20; i++)
		{
			clients[i] = new clientData;
		}

		//start the threads
		gameSendThread = boost::thread(netGameSend);
		gameRecieveThread = boost::thread(netGameRecieve);
		controlSendThread = boost::thread(netControlSend);
		controlRecieveThread = boost::thread(netControlRecieve);
		tcpAcceptorThread = boost::thread(connectHandler);
		TAcceptor.listen(); //start the tcp acceptor
		std::cout << "[INFO]: netController ready to accept clients" << std::endl;
		return true;
	}
	catch (std::exception &e)
	{
		std::cout << "[ERROR]: Something went wrong while initialising the net controller. error: \"" << e.what() << "\"" << std::endl;
		return false;
	}

}

void netController::update() //FINISH ME :(
{
	getNextMessage(); //get the next UDP and TCP message recieved for each client

	for (int i = 0; i < players; i++)
	{
		switch (clients[i]->state)
		{
		case clientState::CONNECTING:
			if (clients[i]->TEndpoint.address() == clients[i]->UEndpoint.address()) //if we have recieved a UDP packet, the connection has succeeded
				clients[i]->state = clientState::CONNECTED; //the client is connected
			break;
		case clientState::CONNECTED:
			sendControlTo(AUTHENTICATE, i); //request the client's login and login token
			clients[i]->state = clientState::AUTHREPLY; //we are waiting for an authentication reply
			break;
		case clientState::AUTHREPLY:
			if (clients[i]->currentControlMessage.NCM == AUTHENTICATE) //if the client is replying to the authentication request
			{
				clients[i]->username = clients[i]->currentControlMessage.paramaters[0]; //get the client's username
				clients[i]->token = clients[i]->currentControlMessage.paramaters[1]; //get the client's login token
				clients[i]->state = clientState::AUTH_TOKEN;
			}
			break;
		case clientState::AUTH_TOKEN:
			break;
		case clientState::SEND_USER_DATA:
			sendControlTo(USER_ID, i, std::to_string(i));
			break;
		case clientState::LOBBY:
			break;
		case clientState::LOAD_GAME:
			break;
		case clientState::PLAY_GAME:

			if (clients[i]->currentControlMessage.NCM == PLAYER_SPAWN)
			{

			}

			break;
		case clientState::GAME_END:
			break;
		case clientState::DISCONNECTING:
			break;
		case clientState::QUICK_DISCONNECT: //a client needs to quickly disconnect. don't send any more messages and assume they will disconnect properly. clean up the clientData container
			cleanupClient(i);
			break;
		case clientState::ERROR_CONNECT:
			break;
		case clientState::ERROR_CONNECT_UDP:
			break;
		}

		if (clients[i]->currentControlMessage.NCM == KEEPALIVE_PING)
		{
			sendControlTo(KEEPALIVE_PONG, i);
		}

		if (clients[i]->lastMessage.getTicks() > 5000)
		{
			sendControlTo(KEEPALIVE_PING, i);
		}
		//if nothing has been heard from the client for 10 seconds, assume they have disconnected abruptly
		if (clients[i]->lastMessage.getTicks() > 10000)
		{
			clients[i]->state = clientState::QUICK_DISCONNECT;
		}
	}
}

void netController::updateLobby()
{
}

void netController::updateGame()
{
}

void netController::eventUpdate()
{
}

void netController::sendGameTo(std::string data, int client)
{
	boost::posix_time::ptime time = boost::posix_time::microsec_clock::universal_time(); //initalise the ptime container used to initalise the time_duration container. uses universal time (doesn't depend on timezones)
	boost::posix_time::time_duration duration(time.time_of_day()); //initalise the time_duration container used to get the current microsecond time

	clients[client]->gameToSend.push(std::to_string(duration.total_microseconds()) + " " + data);
}

void netController::broadcastGame(std::string data)
{
	boost::posix_time::ptime time = boost::posix_time::microsec_clock::universal_time(); //initalise the ptime container used to initalise the time_duration container. uses universal time (doesn't depend on timezones)
	boost::posix_time::time_duration duration(time.time_of_day()); //initalise the time_duration container used to get the current microsecond time

	for (size_t x = 0; x < maxClients; x++)
	{
		clients[x]->gameToSend.push(std::to_string(duration.total_microseconds()) + " " + data);
	}
}

void netController::sendControlTo(std::string data, int client)
{
	//setup the time container
	boost::posix_time::ptime time = boost::posix_time::microsec_clock::universal_time(); //initalise the ptime container used to initalise the time_duration container. uses universal time (doesn't depend on timezones)
	boost::posix_time::time_duration duration(time.time_of_day()); //initalise the time_duration container used to get the current microsecond time


	clients[client]->controlToSend.push(std::to_string(duration.total_microseconds()) + " " + data); //push the total microseconds and the data onto the to send queue
}

void netController::sendControlTo(int ncm, int client, std::string params)
{
	boost::posix_time::ptime time = boost::posix_time::microsec_clock::universal_time();
	boost::posix_time::time_duration duration(time.time_of_day());

	clients[client]->controlToSend.push(std::to_string(duration.total_microseconds()) + " " + std::to_string(ncm) + " " + params);
}

void netController::sendControlTo(netMessage message, int client)
{
	boost::posix_time::ptime time = boost::posix_time::microsec_clock::universal_time();
	boost::posix_time::time_duration duration(time.time_of_day());

	std::string paramaters;

	for (size_t x = 0; x < message.paramaters.size(); x++)
	{
		paramaters += message.paramaters[x];
		if (x != message.paramaters.size() - 1)
		{
			paramaters += " ";
		}
	}

	clients[client]->controlToSend.push(std::to_string(duration.total_microseconds()) + " " + std::to_string(message.NCM) + " " + paramaters);
}

void netController::broadcastControl(std::string data)
{
	boost::posix_time::ptime time = boost::posix_time::microsec_clock::universal_time();
	boost::posix_time::time_duration duration(time.time_of_day());

	for (size_t x = 0; x < maxClients; x++)
	{
		clients[x]->controlToSend.push(std::to_string(duration.total_microseconds()) + " " + data);
	}
	
}

void netController::broadcastControl(int ncm, std::string params)
{
	boost::posix_time::ptime time = boost::posix_time::microsec_clock::universal_time();
	boost::posix_time::time_duration duration(time.time_of_day());

	for (size_t x = 0; x < maxClients; x++)
	{
		clients[x]->controlToSend.push(std::to_string(duration.total_microseconds()) + " " + std::to_string(ncm) + " " + params);
	}
}

void netController::broadcastControl(netMessage message)
{
	boost::posix_time::ptime time = boost::posix_time::microsec_clock::universal_time();
	boost::posix_time::time_duration duration(time.time_of_day());

	std::string paramaters;

	for (size_t y = 0; y < maxClients; y++)
	{
		for (size_t x = 0; x < message.paramaters.size(); x++)
		{
			paramaters += message.paramaters[x];
			if (x != message.paramaters.size() - 1)
			{
				paramaters += " ";
			}
		}
		clients[y]->controlToSend.push(std::to_string(duration.total_microseconds()) + " " + std::to_string(message.NCM) + " " + paramaters);
	}
}

netController::~netController()
{
}

void netController::netControlSend()
{
	while (!halt)
	{
		for (size_t x = 0; x < maxClients; x++)
		{
			if (clients[x]->controlToSend.size() > 0)
			{
				clients[x]->socket.send(boost::asio::buffer(clients[x]->controlToSend.front()));
			}
		}
	}
}

void netController::TCPRecieveHandler(int client)
{
	netMessage message = unpackMessage(clients[client]->buffer.data()); //convert the string message into the netMessage format

	clients[client]->controlRecieved.push(message); //push the message to the queue

	clients[client]->buffer.assign(0); //clear the buffer

	clients[client]->receiving = false;
}

void netController::netControlRecieve()
{
	while (!halt) //while the server isn't going down
	{
		
		for (size_t x = 0; x < maxClients; x++) 
		{
			if (!clients[x]->receiving) //if the client socket isn't recieving, call the async_recieve function
			{
				clients[x]->socket.async_receive(boost::asio::buffer(clients[x]->buffer, 1024), boost::bind(&TCPRecieveHandler, x)); //works in the background
				clients[x]->receiving = true; //don't call async_recieve on this client again
			}
		}
	}
}

void netController::netGameSend()
{
	while (!halt)
	{
		for (size_t x = 0; x < maxClients; x++) //loop through all of the clients
		{
			if (clients[x]->gameToSend.size() > 0) //if there is something to send to the client, send it
			{
				USLock.lock();
				USocket.send_to(boost::asio::buffer(clients[x]->gameToSend.front()), clients[x]->UEndpoint); //send the next message in the queue
				USLock.unlock();
				clients[x]->gameToSend.pop(); //remove the sent message from the queue
			}
		}
	}
}

void netController::netGameRecieve()
{
	while (!halt)
	{
		boost::array<char, 128> buf; //the buffer to store the message
		udp::endpoint endpoint; //where the message came from
		boost::system::error_code error; //container for errors
		
		USLock.lock();
		USocket.receive_from(boost::asio::buffer(buf), endpoint, 0, error); //wait for a message to be recieved
		USLock.unlock();

		if (endpoint.port() == atoi(port.c_str())) //if the message was sent to the correct port
		{
			for (int i = 0; i < 20; i++) //find the client that sent the message
			{
				if (clients[i]->TEndpoint.address() == endpoint.address()) //if the addresses match
				{
					clients[i]->gameRecieved.push(unpackMessage(buf.data())); //push the message to the client container
					clients[i]->lastMessage.reset();
					break; //exit the for loop
				}
			}
		}
	}
}

void netController::connectHandler()
{
	while (!halt) //while the server isn't shutting down
	{
		if (players < 20) //don't check for players if the server is full
		{
			//find the first open player slot and wait for a connecting player
			for (int i = 0; i < 20; i++)
			{
				if (clients[i]->state == clientState::DISCONNECTED)
				{
					TAcceptor.accept(clients[i]->socket, clients[i]->TEndpoint);
					clients[i]->state = clientState::CONNECTING;
					clients[i]->socket.send(boost::asio::buffer(std::to_string(UDP_CONNECT)));
				}
			}
		}
	}
}

void netController::getNextMessage()
{
	for (int i = 0; i < players; i++)
	{
		clients[i]->nextControlMessage();
	}
}

netController::netMessage netController::unpackMessage(std::string data)
{
	std::vector<std::string> splitData = splitString(data);
	netMessage message;
	message.completeMessage = data;
	message.timestamp = boost::posix_time::time_from_string(splitData[0]);
	message.playerID = std::atoi(splitData[1].c_str());
	message.NCM = std::atoi(splitData[2].c_str());
	
	for (size_t i = 3; i < splitData.size(); i++)
	{
		message.paramaters.push_back(splitData[i]);
	}

	return message;
}

std::vector<std::string> netController::splitString(std::string to_split)
{
	//source: https://www.quora.com/How-do-I-split-a-string-by-space-into-an-array-in-c++/answer/Grzegorz-Go%C5%82da?srid=tzNb&share=73558a3a
	std::vector<std::string> array;
	std::size_t pos = 0, found;
	while ((found = to_split.find_first_of(' ', pos)) != std::string::npos) {
		array.push_back(to_split.substr(pos, found - pos));
		pos = found + 1;
	}
	array.push_back(to_split.substr(pos));

	return array;
}

void netController::cleanupClient(int ID)
{
	clients[ID]->socket.shutdown(tcp::socket::shutdown_both);
	delete clients[ID];
	clients[ID] = nullptr;
	clients[ID] = new clientData;
}
