/*
 * Server.h
 *
 *  Created on: 12 01 2020
 *      Author: Liron Vaitzman
 */

#ifndef SERVER_H_
#define SERVER_H_


#include <iostream>
#include <fstream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <pthread.h>
#include <thread>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "CLI.h"

using namespace std;

//ClientHandler interface :
class ClientHandler
{
public:
	virtual void handle(int clientID) = 0;
};

// socket input-outpot class
class SocketIO : public DefaultIO
{
	int client_id;

public:
	SocketIO(int clientID)
	{
		this->client_id = clientID;
	}
	
	string read()
	{
		string line_from_server = "";
		char c = 0;
		::read(this->client_id, &c, sizeof(char));
		while (c != '\n')
		{
			line_from_server += c;
			::read(this->client_id, &c, sizeof(char));
		}
		return line_from_server;
	}

	void write(string text)
	{
		const char *c = text.c_str();
		send(this->client_id, c, strlen(c), 0);
	}
	void write(float f)
	{
		string s = to_string(f);
		s.erase(s.find_last_not_of('0') + 1, std::string::npos); //delete '0' at the end
		const char* c = s.c_str();
		send(this->client_id, c, strlen(c), 0);
		/*
		stringstream num;
		num << f;
		string s_num = num.str();
		const char *c_num = s_num.c_str();
		send(this->client_id, c_num, s_num.size(), 0);*/
	}

	void read(float *f)
	{
		::read(this->client_id, f, sizeof(float));
	}
};

// AnomalyDetectionHandler class:
class AnomalyDetectionHandler : public ClientHandler
{
public:
	virtual void handle(int clientID)
	{
		SocketIO sio(clientID);
		CLI cli(&sio);
		cli.start();
	}
};

// implement on Server.cpp
class Server
{
	thread *t; 			// the thread to run the start() method in
	int fd; 			// the file descriptor
	sockaddr_in server; 
	sockaddr_in client;
	bool need_stop = false;

public:
	Server(int port) throw(const char *);
	virtual ~Server();
	void start(ClientHandler &ch) throw(const char *);
	void stop();
};

#endif /* SERVER_H_ */
