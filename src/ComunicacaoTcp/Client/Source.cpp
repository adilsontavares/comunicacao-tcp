#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <thread>
#include <cstdlib>
#include <locale>
#include "Error.h"
#include "Console.h"

using namespace std;

struct Server
{
	int ssock;
};

void handleServer(Server *server);
void handleServerMessage(Server *server, const char *message, size_t size);

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
	inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);

	int csock = connect(sock, (sockaddr*)&addr, sizeof(addr));
	Error::assert(csock != SOCKET_ERROR, "Não foi possível conectar ao servidor.");

	Console::log("Conversa com o servidor iniciada!");

	Server * server = new Server();
	server->ssock = sock;

	new thread(handleServer, server);

	char buffer[1024];

	while (true)
	{
		cin >> buffer;

		if (send(sock, buffer, strlen(buffer), 0) == SOCKET_ERROR)
		{
			Console::error("Não foi possível enviar dados para o servidor.");
			break;
		}

		Console::log("Enviado para o servidor: %s", buffer);
	}

	WSACleanup();

	return 0;
}

void handleServer(Server *server)
{
	Console::log("Iniciando escuta ao servidor...");

	char buffer[1024];
	int bytes;

	while (true)
	{
		bytes = recv(server->ssock, buffer, sizeof(buffer), 0);

		if (bytes == SOCKET_ERROR)
		{
			Console::error("Ocorreu um erro ao receber dados do servidor.");
			break;
		}

		if (bytes < 0)
		{
			Console::error("A conexão com o servidor foi encerrada.");
			return;
		}

		buffer[bytes] = '\0';
		handleServerMessage(server, buffer, bytes);
	}

	Console::log("Desconectando do servidor...");
	closesocket(server->ssock);
	Console::log("Desconectado.");
}

void handleServerMessage(Server *server, const char * message, size_t size)
{
	Console::writeLine("Servidor enviou: %s", message);
}
