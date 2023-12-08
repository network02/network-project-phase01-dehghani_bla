#include<winsock.h>
#include <stdio.h>


int init_tcp_connection(SOCKET &sourceSocket, char destinationIpAddr[16], unsigned short destinationPortNumber) ;
void mainMenu();

// client program
int main()
{
	mainMenu();
	return (0);
}

int init_tcp_connection(SOCKET &sourceSocket , char destinationIpAddr[16], unsigned short destinationPortNumber) /* connection status*/
{
	//Start up Winsock…
	WSADATA wsadata;

	int error = WSAStartup(0x0202, &wsadata);

	//Did something happen?
	if (error)
		return false;

	//Did we get the right Winsock version?
	if(wsadata.wVersion != 0x0202)
	{
		WSACleanup(); //Clean up Winsock
		return false;
	}
	sourceSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
	if (sourceSocket == INVALID_SOCKET){
		puts("can't create socket!");
		exit(EXIT_FAILURE);
	}
	struct sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(destinationPortNumber);
	serverAddr.sin_addr.S_un.S_addr = inet_addr(destinationIpAddr);
	int connectionStatus = connect(sourceSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
	WSACleanup();
	return connectionStatus;
}

void mainMenu()
{
	int select;
	char serverIpAddr[16], serverHostName[50];
	unsigned short serverPortNumber;
	SOCKET clientSocket;
	int onlineStatus; // was connection successful? 
	struct hostent *x; // a pointer to DNS response data
	do
	{
		system("CLS");
		puts("\t\t\t in the name of GOD");
		puts("1_ online test with IP address .");
		//puts("2_ online test with host name .");
		puts("2_ port scanning .");
		puts("3_ get and post http request .");
		puts("other : exit");
		puts("please select one choices above and enter its number :");
		scanf_s("%d", &select);
		switch (select)
		{
		case 1:
			puts("server ip addres ra vared konid(be forme <255.255.255.255> ) :");
			scanf_s("%s", serverIpAddr,16);
			puts("server socket port_Number ra vared konid :");
			scanf_s("%hu", &serverPortNumber);
			onlineStatus = init_tcp_connection(clientSocket, serverIpAddr, serverPortNumber);
			if (onlineStatus == SOCKET_ERROR){
				puts("server is offline !");
			}
			else
				puts("server is online !");
			closesocket(clientSocket);
			system("pause");
			break;
		/*case 2:
			puts("server_host_name ra vared konid(e.g. <google.com>) :");
			scanf_s("%s", serverHostName,50);
			DnsQuery_A(serverHostName, DNS_TYPE_A, DNS_QUERY_STANDARD , NULL , NULL , NULL );

			puts("server socket port_Number ra vared konid :");
			scanf_s("%hu", &serverPortNumber);
			onlineStatus = init_tcp_connection(clientSocket, (x->h_addr_list)[0], serverPortNumber);
			if (onlineStatus == SOCKET_ERROR){
				puts("server is offline !");
			}
			else
				puts("server is online !");
			closesocket(clientSocket);
			delete x;
			system("pause");
			break;*/
		case 2:

			system("pause");
			break;
		case 3:

			system("pause");
			break;
		default:
			exit(EXIT_SUCCESS);
			break;
		}

	}while (true);
}