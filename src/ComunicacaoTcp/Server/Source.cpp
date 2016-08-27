#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <thread>
#include <cstdlib>
#include <locale>
#include "Errors.h"

using namespace std;

struct Client
{
	int ssock;
	int csock;

};

void handleClient(Client *client);
void handleClientMessage(Client *client, const char *message, size_t size);

int main(int argc, char *argv[])
{
	setlocale(LC_ALL, "pt-BR");

	WSAData data;
	WSAStartup(MAKEWORD(2, 2), &data);

	int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock < 0)
	{
		cerr << "Não foi possível inicializar o socket." << endl;
		return 1;
	}

	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(9999);
	inet_pton(AF_INET, "0.0.0.0", &addr.sin_addr.s_addr);

	if (::bind(sock, (sockaddr*)&addr, sizeof(addr)) < 0)
	{
		cerr << "Não foi possível realizar o bind." << endl;
		return 1;
	}

	if (listen(sock, SOMAXCONN) < 0)
	{
		cerr << "Não foi possível iniciar a escuta por clientes." << endl;
		return 1;
	}

	cout << "Aguardando conexões de clientes..." << endl;

	while (true)
	{
		sockaddr_in caddr;
		socklen_t caddrlen = sizeof(caddr);

		int csock = accept(sock, (sockaddr*)&caddr, &caddrlen);
		if (csock < 0) 
		{
			cerr << "Erro ao aceitar a conexão de um cliente." << endl;
			continue;
		}

		Client *client = new Client();
		client->ssock = sock;
		client->csock = csock;

		new thread(handleClient, client);
	}

	WSACleanup();

	return 0;
}

void handleClient(Client * client)
{
	cout << "Cliente conectado: " << client->csock << endl;

	char buffer[1024];
	int bytes;

	while (true)
	{
		bytes = recv(client->csock, buffer, sizeof(buffer), 0);

		if (bytes == SOCKET_ERROR)
		{
			cerr << "Ocorreu um erro ao receber dados do cliente " << client->csock << endl;
			break;
		}

		if (bytes < 0)
		{
			cerr << "A conexão com o cliente " << client->csock << " foi encerrada." << endl;
			return;
		}

		buffer[bytes] = '\0';
		handleClientMessage(client, buffer, bytes);
	}

	cout << "Desconectando cliente: " << client->csock << "... ";
	closesocket(client->csock);
	cout << "OK" << endl;
}

void handleClientMessage(Client * client, const char * message, size_t size)
{
	cout << "Cliente " << client->csock << " enviou: " << message << endl;

	char buffer[] = "Recebi sua mensagem!!!";

	if (send(client->csock, buffer, strlen(buffer), 0) == SOCKET_ERROR)
		cerr << "Erro ao responder echo ao cliente " << client->csock << endl;
}
