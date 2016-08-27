#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <thread>
#include <cstdlib>
#include <locale>

#include "Error.h"
#include "Console.h"

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
	Error::assert(sock != SOCKET_ERROR, "Não foi possível inicializar o socket.");

	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(9999);
	inet_pton(AF_INET, "0.0.0.0", &addr.sin_addr.s_addr);

	Error::assert(::bind(sock, (sockaddr*)&addr, sizeof(addr)) >= 0, "Não foi possível realizar o bind.");
	Error::assert(listen(sock, SOMAXCONN) != SOCKET_ERROR, "Não foi possível iniciar a escuta por clientes.");

	Console::log("Aguardando conexões de clientes...");

	while (true)
	{
		sockaddr_in caddr;
		socklen_t caddrlen = sizeof(caddr);

		int csock = accept(sock, (sockaddr*)&caddr, &caddrlen);
		if (csock < 0) 
		{
			Console::error("Erro ao aceitar a conexão de um cliente.");
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
	Console::log("Cliente conectado: %d", client->csock);

	char buffer[1024];
	int bytes;

	while (true)
	{
		bytes = recv(client->csock, buffer, sizeof(buffer), 0);

		if (bytes == SOCKET_ERROR)
		{
			Console::error("Ocorreu um erro ao receber dados do cliente %d.", client->csock);
			break;
		}

		if (bytes < 0)
		{
			Console::error("A conexão com o cliente %d foi encerrada.", client->csock);
			return;
		}

		buffer[bytes] = '\0';
		handleClientMessage(client, buffer, bytes);
	}

	Console::log("Desconectando cliente: %d... ", client->csock);
	closesocket(client->csock);
	Console::log("Cliente %d desconectado.", client->csock);
}

void handleClientMessage(Client * client, const char * message, size_t size)
{
	Console::writeLine("O cliente %d enviou: %s", client->csock, message);

	char *buffer = new char[size + 1];
	buffer[size] = '\0';

	for (int i = 0; i < size; ++i)
		buffer[i] = toupper(message[i]);

	if (send(client->csock, buffer, strlen(buffer), 0) == SOCKET_ERROR)
		Console::error("Erro ao responder echo ao cliente %d.", client->csock);
}
