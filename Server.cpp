/*
 * Server.cpp
 *
 *  Created on: 12 01 2020
 *      Author: Liron Vaitzman
 */
#include "Server.h"

Server::Server(int port)throw (const char*) {
	//Opening a communication channel - socket
	fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd < 0)
	{
		throw "socket failed.";
	}

	//update the server struct
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(port);

	//bind thr ip address and the port to the socket that we get
	int this_bind = bind(fd, (struct sockaddr *)&server, sizeof(server));
	if (this_bind < 0)
	{
		throw "bind failed";
	}

	//the server requests permission to listen
	int  this_listen = listen(fd, 1);
	if ( this_listen < 0)
	{
		throw "listen failed";
	}
	
	//bool variable - to check if we need to stop the connect
	need_stop = false;

}

void Server::start(ClientHandler& ch)throw(const char*){	
	t = new thread([&ch, this]() { //lambda expression , instead of pointer to function
		while (!need_stop)
		{
			socklen_t client_size = sizeof(client);
			alarm(5);
			int a_client = accept(fd, (struct sockaddr *)&client, &client_size);
			if (a_client < 0)
			{
				throw "accept failure";
			}
			ch.handle(a_client);
			close(a_client);
		}
		close(fd);
	});
}

void Server::stop(){
	need_stop = true;
	t->join(); 
}

Server::~Server() {
}

