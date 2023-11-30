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
 

    if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
        cerr << "Error binding server socket to address and port." << endl;
        closesocket(serverSocket);
        WSACleanup();
        return -1;
    }

    cout << "Success binding the server socket...";

    closesocket(serverSocket);
    WSACleanup();

    return 0;
}
