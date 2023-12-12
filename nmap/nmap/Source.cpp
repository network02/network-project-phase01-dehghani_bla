#include<winsock.h>
#include <stdio.h>
#include <limits.h>

int tcp_connection_init/* creates a socket and place it's file descriptor in sourceSocket and connects to destination !*/
(SOCKET &sourceSocket, char destinationIpAddr[16], unsigned short destinationPortNumber) ; 
void mainMenu();
int bounded_port_scan(unsigned short lowerBound, unsigned short upperBound, char *ipAddr); // scan a host with ip=ipAddr and ports from lowerBound to upperBound
char *get_ip_by_hostname(char *hostName); // return (malloc-ed string) ip addres in standard format with . (NULL if host was not available);

// client program
int main()
{
	mainMenu();
	return (0);
}

int tcp_connection_init(SOCKET &sourceSocket , char destinationIpAddr[16], unsigned short destinationPortNumber) 
{
	//Start up Winsock�
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
	return connectionStatus;
}

void mainMenu()
{
	int select;
	char serverIpAddr[16], serverHostName[50];
	unsigned short serverPortNumber , lowPN , highPN;
	int onlineStatus , status;
	char *ip;
	char getPost[32];
	char receiveItem[1024];
	SOCKET sourceSocket;
	do
	{
		system("CLS");
		puts("\t\t\t in the name of GOD");
		puts("1_ online test with IP address .");
		puts("2_ online test with host name .");
		puts("3_ port scanning .");
		puts("4_ get and post http simulator with server.py .");
		puts("other : exit");
		puts("please select one choices above and enter its number :");
		scanf_s("%d", &select);
		switch (select)
		{
		case 1:
			puts("server ip addres ra vared konid(be forme <255.255.255.255> ) :");
			scanf_s("%s", serverIpAddr, 16);
			onlineStatus = bounded_port_scan(0, USHRT_MAX, serverIpAddr);
			if (onlineStatus == 1)
				puts("server is online !");
			else
				puts("server is offline !");
			system("pause");
			break;
		case 2:
			puts("server_host_name ra vared konid(e.g. <google.com>) :");
			scanf_s("%s", serverHostName,50);
			ip=get_ip_by_hostname(serverHostName);
			if (ip == NULL){
				printf("%s is unavailable\n", serverHostName);
				exit(EXIT_FAILURE);
			}
			onlineStatus = bounded_port_scan(0, USHRT_MAX, ip);
			if (onlineStatus ==1)
				puts("server is online !");
			else
				puts("server is offline !");
			free(ip);
			system("pause");
			break;
		case 3:
			puts("please enter destination ip address :");
			scanf_s("%s", serverIpAddr, 16);
			puts("please enter port number low and high for scan :");
			scanf_s("%hu", &lowPN);
			scanf_s("%hu", &highPN);
			onlineStatus = bounded_port_scan(lowPN, highPN, serverIpAddr);
			if (onlineStatus == 1)
				puts("server is online !");
			else
				puts("server is offline !");
			system("pause");
			break;
		case 4:
			puts("Enter 'GET user_id' or 'POST user_name user_age' to simualte a request : ");
			fflush(stdin);
			gets_s(getPost,32);
			tcp_connection_init(sourceSocket, "127.0.0.1", 8080);
			if (sourceSocket == SOCKET_ERROR){
				puts("can not connect to server!");
				system("pause");
				exit(EXIT_FAILURE);
			}
			status=send(sourceSocket, getPost, 32 , 0);
			if (status == SOCKET_ERROR){
				puts("can not send http request!");
				system("pause");
				exit(EXIT_FAILURE);
			}
			status=recv(sourceSocket, receiveItem, 1024, 0);
			if (status == SOCKET_ERROR){
				puts("can not receive http response!");
				system("pause");
				exit(EXIT_FAILURE);
			}
			puts("response from the server:");
			puts(receiveItem);
			closesocket(sourceSocket);
			WSACleanup();
			system("pause");
			break;
		default:
			exit(EXIT_SUCCESS);
			break;
		}

	}while (true);
}


int bounded_port_scan(unsigned short lowerBound, unsigned short upperBound, char *ipAddr)
{
	int i=0;
	int onlineStatus = 0; // was host online? 
	int connectionStatus; // was connection successful ?
	//Start up Winsock�
	WSADATA wsadata;

	int error = WSAStartup(0x0202, &wsadata);

	//Did something happen?
	if (error)
		return false;

	//Did we get the right Winsock version?
	if (wsadata.wVersion != 0x0202)
	{
		WSACleanup(); //Clean up Winsock
		return false;
	}
	SOCKET sourceSocket;
	sourceSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
	if (sourceSocket == INVALID_SOCKET){
		puts("can not creat socket!");
		system("pause");
		exit(EXIT_FAILURE);
	}
	struct sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.S_un.S_addr = inet_addr(ipAddr);
	for (i = lowerBound; i <= upperBound; i++)
	{	
		serverAddr.sin_port = htons((unsigned short)i);
		connectionStatus = connect(sourceSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
		if (connectionStatus != SOCKET_ERROR) {
			onlineStatus = 1;
			break;
		}
		printf("<%s:%d> is closed!\n", ipAddr, i);
	}
	closesocket(sourceSocket);
	WSACleanup();
	return onlineStatus;
}



char *get_ip_by_hostname(char *hostName)
{
	char *ipAddr = (char *) malloc(sizeof(char) * 16);
	struct hostent *x; // a pointer to DNS response data
	in_addr *address;

	//Start up Winsock�
	WSADATA wsadata;

	int error = WSAStartup(0x0202, &wsadata);

	//Did something happen?
	if (error)
		exit(EXIT_FAILURE);

	//Did we get the right Winsock version?
	if (wsadata.wVersion != 0x0202)
	{
		WSACleanup(); //Clean up Winsock
		exit(EXIT_FAILURE);
	}

	x = gethostbyname(hostName); // DNS request for ip address
	if (x == NULL)
	{
		free(ipAddr);
		return NULL;
	}
	//printf("%d", h_errno)	;					//for debugging
	address = (in_addr *)(x->h_addr_list); // extracting ip address from dns request
	strcpy_s(ipAddr ,16, inet_ntoa(*address)); // convert ip address from network format to address (human readable) format
	// free(x);          // dns hostent data is static please read <How does `gethostbyname` return `struct hostent *` without requiring the caller to release the resource?>
	WSACleanup();
	return ipAddr;
}