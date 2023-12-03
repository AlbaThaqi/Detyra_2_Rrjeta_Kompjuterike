#include <iostream>
#include <cstring>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <thread>
#include <vector>
#include <fstream>
#include <sstream>
#include <queue>

#pragma warning(disable:4996)
#pragma comment(lib, "Ws2_32.lib")

using namespace std;

const int PORT = 1205;
const char* IP_ADDRESS = "0.0.0.0";

int clientcount = 0;

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

void handleClient(int serverSocket, sockaddr_in clientAddress) {
    char buffer[1024];
    int bytesRead;

    while (true) {

        memset(buffer, 0, sizeof(buffer));

        int clientAddressLength = sizeof(clientAddress);

        bytesRead = recvfrom(serverSocket, buffer, sizeof(buffer), 0, (struct sockaddr*)&clientAddress, &clientAddressLength);

        bool readOnly;
        if (authenticate(buffer, readOnly)) {
            const char* authSuccessMessage = "Authentication successful. Access granted.";
            sendto(serverSocket, authSuccessMessage, strlen(authSuccessMessage), 0, (struct sockaddr*)&clientAddress, sizeof(clientAddress));
            
            clientcount++;

            if (clientcount > 4) {
            
                const char* noSpaceMessage = "Limit of clients reached!";
                sendto(serverSocket, noSpaceMessage, strlen(noSpaceMessage), 0, (struct sockaddr*)&clientAddress, sizeof(clientAddress));
            };

            while (true) {

                memset(buffer, 0, sizeof(buffer));

                bytesRead = recvfrom(serverSocket, buffer, sizeof(buffer), 0, (struct sockaddr*)&clientAddress, &clientAddressLength);

                string command(buffer);
                istringstream iss(command);
                string action, content, fileName;
                iss >> action;

                if (action == "read") {
                    iss >> fileName;

                    ifstream file(fileName, ios::binary);

                    if (file) {
                        string fileContent((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
                        file.close();

                        sendto(serverSocket, fileContent.c_str(), fileContent.length(), 0, (struct sockaddr*)&clientAddress, sizeof(clientAddress));
                    }
                    else {
                        const char* errorMessage = "Error: File not found.";
                        sendto(serverSocket, errorMessage, strlen(errorMessage), 0, (struct sockaddr*)&clientAddress, sizeof(clientAddress));
                    }
                }
                else if (action == "write" && !readOnly) {
                    iss >> content >> fileName;

                    ofstream file(fileName, ios::binary);

                    if (file) {
                        file << content;
                        file.close();

                        const char* successMessage = "File was written successfully.";
                        sendto(serverSocket, successMessage, strlen(successMessage), 0, (struct sockaddr*)&clientAddress, sizeof(clientAddress));
                    }
                    else {
                        const char* errorMessage = "Error: Unable to write to file.";
                        sendto(serverSocket, errorMessage, strlen(errorMessage), 0, (struct sockaddr*)&clientAddress, sizeof(clientAddress));
                    }
                }

                else if (action == "execute" && !readOnly) {

                    getline(iss, content);
                    string result = executeCommand(content);
                    sendto(serverSocket, result.c_str(), result.length(), 0, (struct sockaddr*)&clientAddress, sizeof(clientAddress));
                }
                else {

                    const char* errorMessage = "Error: Unauthorized or unsupported command.";
                    sendto(serverSocket, errorMessage, strlen(errorMessage), 0, (struct sockaddr*)&clientAddress, sizeof(clientAddress));
                }
            }
        }
        else {
            const char* authFailureMessage = "Authentication failed. Access denied.";
            sendto(serverSocket, authFailureMessage, strlen(authFailureMessage), 0, (struct sockaddr*)&clientAddress, sizeof(clientAddress));
        }
    }
}

int main() {
    WSADATA wsaData;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cerr << "Failed to initialize Winsock." << endl;
        return -1;
    }

    int serverSocket = socket(AF_INET, SOCK_DGRAM, 0);

    if (serverSocket == -1) {
        cerr << "Error creating server socket." << endl;
        WSACleanup();
        return -1;
    }

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = inet_addr(IP_ADDRESS);
    serverAddress.sin_port = htons(PORT);

    if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
        cerr << "Error binding server socket to address and port." << endl;
        closesocket(serverSocket);
        WSACleanup();
        return -1;
    }

    cout << "Success binding the server socket..." << endl;

    queue<thread> clientThreadsQueue;

    while (true) {

        sockaddr_in clientAddress;

        if (clientThreadsQueue.size() < 2) {
            int clientAddressLength = sizeof(clientAddress);
            
            thread newClientThread(handleClient, serverSocket, clientAddress);
            clientThreadsQueue.push(move(newClientThread));

        }

        while (!clientThreadsQueue.empty() && !clientThreadsQueue.front().joinable()) {
            clientThreadsQueue.pop();
        }
    }

    closesocket(serverSocket);
    WSACleanup();

    return 0;
}
