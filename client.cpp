#include <iostream>
#include <cstring>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma warning(disable:4996) 
#pragma comment(lib, "Ws2_32.lib")

using namespace std;

const int PORT = 12345;
const char* IP_ADDRESS = "127.0.0.1";

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cerr << "Failed to initialize Winsock." << endl;
        return -1;
    }

    int clientSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (clientSocket == -1) {
        cerr << "Error creating client socket." << endl;
        WSACleanup();
        return -1;
    }

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = inet_addr(IP_ADDRESS);
    serverAddress.sin_port = htons(PORT);

   while (true){
      cout << "Enter credentials (username:password): ";
        cin.getline(buffer, sizeof(buffer));
    
    closesocket(clientSocket);
    WSACleanup();

    return 0;
}
