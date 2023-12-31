#include <iostream>
#include <cstring>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma warning(disable:4996)
#pragma comment(lib, "Ws2_32.lib")

using namespace std;

const int PORT = 1205;
const char* IP_ADDRESS = "192.168.1.17"; // IP adresa e serverit
// const char* IP_ADDRESS = "127.0.0.1";

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

    char buffer[1024];

    while (true) {
        cout << "Enter credentials (username:password): ";
        cin.getline(buffer, sizeof(buffer));

        sendto(clientSocket, buffer, strlen(buffer), 0, (struct sockaddr*)&serverAddress, sizeof(serverAddress));

        memset(buffer, 0, sizeof(buffer));
        int bytesRead = recvfrom(clientSocket, buffer, sizeof(buffer), 0, NULL, NULL);

        cout << "Server response: " << buffer << endl;

        if (strcmp(buffer, "Authentication successful. Access granted.") == 0) {

            while (true) {
                cout << "Enter a command ('read <file_name>', 'write <string> <file_name>, execute <command> or type 'exit' to end): ";
                cin.getline(buffer, sizeof(buffer));

                if (strcmp(buffer, "exit") == 0) {
                    const char* exitMessage = "Client disconnected.";
                    sendto(clientSocket, exitMessage, strlen(exitMessage), 0, (struct sockaddr*)&serverAddress, sizeof(serverAddress));
                    break; 
                }

                sendto(clientSocket, buffer, strlen(buffer), 0, (struct sockaddr*)&serverAddress, sizeof(serverAddress));

                memset(buffer, 0, sizeof(buffer));
                int bytesRead = recvfrom(clientSocket, buffer, sizeof(buffer), 0, NULL, NULL);

                cout << "Server Response: " << buffer << endl;

                if (strcmp(buffer, "Limit of clients reached!") == 0) {
                    cout << "Disconnecting the client...";
                    break;
                }
            }
            break; 
        }
    }

    closesocket(clientSocket);
    WSACleanup();

    return 0;
}
