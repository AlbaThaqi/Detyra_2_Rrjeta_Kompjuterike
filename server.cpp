#include <iostream>
#include <cstring>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma warning(disable:4996) 
#pragma comment(lib, "Ws2_32.lib")

using namespace std;

const int PORT = 12345;
const char* IP_ADDRESS = "127.0.0.1";
bool authenticate(const char* credentials, bool& readOnly) {

    if (strcmp(credentials, "admin:admin123") == 0) {
        readOnly = false;
        return true;
    }
    else if (strcmp(credentials, "readonly:readonly123") == 0) {
        readOnly = true;
        return true;
    }
    else {
        return false;
    }
}
string executeCommand(const string& command) {
    string result;

    FILE* pipe = _popen(command.c_str(), "r");

    if (!pipe) {
        return "Error executing command.";
    }

    char buffer[128];
    while (!feof(pipe)) {
        if (fgets(buffer, 128, pipe) != nullptr) {
            result += buffer;
        }
    }

    _pclose(pipe);

    return result.empty() ? "Command executed successfully!" : result;
}
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

WSADATA WSADATA wsaData;
if (WSAStartup (MAKEWORD (2, 2), &wsaData) != 0) {
    cerr << "Failed to initialize Winsock." << endl;
    return -1;
}

int serverSocket = socket(AF_INET, SOCK_DGRAM, 0);
if (serverSocket == -1) {
    cerr << "Error creating server socket." << endl;
    
}
