// Copyright [2023] <BRAINSTORMERS>
#include"ChatServerController.h"
#include"../services/ChatServerService.h"

#define PORT 8000
struct ChatServerService::Client {
    int sock;
    User user;
    Chat joinedRoom;
    CURL *curl;
    CURLcode res;
    struct curl_slist *slist;
};

void ChatServerController::connectToClient() {
    int serverFd, addrlen;
    struct sockaddr_in address;
    int maxClient = 10;
    // string apiIp = "http://10.104.197.253:8080/api"; // // k8s
    string apiIp = "http://172.18.0.2:8080/api"; //local
    // string mysqlIp = "10.110.47.64"; // //k8s
    string mysqlIp = "172.18.0.3"; // local
    map<int, ChatServerService::Client> clientMap;
    bool establishConnection = ChatServerService::handleConnect(serverFd, maxClient, addrlen, address, false, PORT);
    if (establishConnection) {
        pollfd fds[maxClient];
        ChatServerService::HandleMultiClient(clientMap, maxClient, serverFd, address, addrlen, fds, apiIp, mysqlIp);
    } else {
        exit(1);
    }
}