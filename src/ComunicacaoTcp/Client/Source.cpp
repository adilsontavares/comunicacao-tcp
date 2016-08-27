#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <thread>
#include <cstdlib>
#include <locale>
#include "Errors.h"

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
	if (sock < 0)
	{
		cerr << "Não foi possível inicializar o socket." << endl;
		return 1;
	}

	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(9999);
	inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);

	int csock = connect(sock, (sockaddr*)&addr, sizeof(addr));
	if (csock < 0)
	{
		cerr << "Não foi possível se conectar ao servidor." << endl;
		return 1;
	}

	cout << "Conversa com servidor iniciada!" << endl;

	Server * server = new Server();
	server->ssock = sock;

	new thread(handleServer, server);

	char buffer[1024];

	while (true)
	{
		cin >> buffer;

		if (send(sock, buffer, strlen(buffer), 0) == SOCKET_ERROR)
		{
			cerr << "Não foi possível enviar dados para o servidor." << endl;
			break;
		}

		cout << "Enviado para o servidor: " << buffer << endl;
	}

	WSACleanup();

	return 0;
}

void handleServer(Server *server)
{
	cout << "Iniciando escuta ao servidor..." << endl;

	char buffer[1024];
	int bytes;

	while (true)
	{
		bytes = recv(server->ssock, buffer, sizeof(buffer), 0);

		if (bytes == SOCKET_ERROR)
		{
			cerr << "Ocorreu um erro ao receber dados do servidor." << endl;
			break;
		}

		if (bytes < 0)
		{
			cerr << "A conexão com o servidor foi encerrada." << endl;
			return;
		}

		buffer[bytes] = '\0';
		handleServerMessage(server, buffer, bytes);
	}

	cout << "Desconectando do servidor... ";
	closesocket(server->ssock);
	cout << "OK" << endl;
}

void handleServerMessage(Server *server, const char * message, size_t size)
{
	cout << "Servidor enviou: " << message << endl;
}
