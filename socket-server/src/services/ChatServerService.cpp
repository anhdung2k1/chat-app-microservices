// Copyright [2023] <BRAINSTORMERS>
#include "ChatServerService.h"

using std::cout;
using std::endl;
using std::string;
using std::stringstream;
using std::thread;
using std::to_string;
using std::vector;

string ChatServerService::ConvertIntToString(int id)
{
    stringstream ss;
    ss << id;
    string s;
    ss >> s;
    return s;
}

void ChatServerService::CheckConnected(int new_socket, bool flag)
{
    if (!flag)
    {
        close(new_socket);
        return;
    }
}

void ChatServerService::SendResponse(int new_socket, string response)
{
    if (!send(new_socket, response.c_str(), 1024, 0))
    {
        cout << "Failed!\n";
    }
}

struct ChatServerService::Client
{
    int sock;
    User user;
    Chat joinedRoom;
    CURL *curl;
    CURLcode res;
    struct curl_slist *slist;
};

bool ChatServerService::ReadResponse(Client client, char *buffer, map<int, Client> clientMap)
{
    int bytes_received = recv(client.sock, buffer, 1024, 0);
    if (bytes_received <= 0)
    {
        cout << "User " << client.user.getName() << " has disconnected" << endl;
        string inform = client.user.getName() + " has left the room.";
        HandleInform(clientMap, client, inform);
        return false;
    }
    return true;
}

void ChatServerService::HandleMessage(map<int, Client> clientMap, Client client, Message message, bool &flag)
{
    map<int, Client>::iterator it;
    for (it = clientMap.begin(); it != clientMap.end(); it++)
    {
        if (it->second.joinedRoom.getChatId() == client.joinedRoom.getChatId() && it->first != client.sock)
        {
            flag = true;
            string content = message.getSender().getName() + ": " + message.getText();
            // cout << content;
            SendResponse(it->first, "USER_CHATTING|" + content);
            cout << "\n";
            // cout << std::setw(100) << "Sent!" << endl;
        }
        // else if (it->second.joinedRoom.getChatId() == client.joinedRoom.getChatId() && it->first == client.sock){
        //     // string content = "me : " + message.getText();
        //     // // cout << content;
        //     // SendResponse(it->first, "USER_CHATTING|" + content);
        // }
    }
}

void ChatServerService::RequestReload(map<int, Client> clientMap, Client client, string response)
{
    map<int, Client>::iterator it;
    for (it = clientMap.begin(); it != clientMap.end(); it++)
    {
        if (it->second.joinedRoom.getChatId() == client.joinedRoom.getChatId() && it->first != client.sock)
        {
            SendResponse(it->first, response);
        }
    }
}

void ChatServerService::HandleInform(map<int, Client> clientMap, Client client, string inform)
{
    map<int, Client>::iterator it;
    for (it = clientMap.begin(); it != clientMap.end(); it++)
    {
        if (it->second.joinedRoom.getChatId() == client.joinedRoom.getChatId() && it->first != client.sock)
        {
            SendResponse(it->first, "INFORM|" + inform);
        }
    }
}

void ChatServerService::RemoveClient(map<int, Client> &clientMap, Client client, pollfd fds[], int i)
{
    clientMap.erase(client.sock);
    fds[i].fd = -1;
}

void ChatServerService::processPattern(char *buffer, string &pattern, string &value)
{
    string convertedBuffer = (string)buffer;
    int pos = convertedBuffer.find("|");
    pattern = convertedBuffer.substr(0, pos);
    value = convertedBuffer.substr(pos + 1);
}
bool ChatServerService::handleClient(map<int, Client> &clientMap, Client &client, pollfd fds[], int i, string apiIp, string mysqlIp)
{
    char buffer[1024];
    CurlUtils curlUtils;
    stringstream formData;
    string pattern, value;
    bool flag = true;
    bzero(buffer, 1024);
    // Receive data from client
    if (!ReadResponse(client, buffer, clientMap))
    {
        close(client.sock);
        RemoveClient(clientMap, client, fds, i);
        return false;
    }
    cout << buffer << endl;
    processPattern(buffer, pattern, value);

    if (pattern == "LOGIN_USER")
    {
        flag = ChatRepository::CheckUserName(value, mysqlIp);
        if (flag)
        {
            client.user.setName(value);
        }
        SendResponse(client.sock, "LOGIN_USER|" + to_string(flag));
    }
    else if (pattern == "LOGIN_PASSWORD")
    {
        client.user.setPassword(value);
        // Call API
        formData << "{\"userName\": "
                 << "\"" + client.user.getName() << "\","
                 << "\"password\": "
                 << "\"" << client.user.getPassword() << "\"}";
        // cout << formData.str() << endl;
        curlUtils.authUtil(apiIp + "/accounts/signin", formData.str(), flag, client.curl, client.res, client.slist);
        if (flag)
        {
            string response = curlUtils.getUtil(client.curl, client.res, apiIp + "/users/find?user_name=" + client.user.getName(), flag);
            nlohmann::json j = nlohmann::json::parse(response);
            client.user.setId(j.at("user_id"));
        }
        SendResponse(client.sock, "LOGIN_PASSWORD|" + to_string(flag));
    }
    else if (pattern == "REGISTER_USER")
    {
        flag = !ChatRepository::CheckUserName(value, mysqlIp);
        if (flag)
        {
            client.user.setName(value);
        }
        SendResponse(client.sock, "REGISTER_USER|" + to_string(flag));
    }
    else if (pattern == "REGISTER_PASSWORD")
    {
        client.user.setPassword(value);
        // Call API
        formData << "{\"userName\": "
                 << "\"" + client.user.getName() << "\","
                 << "\"password\": "
                 << "\"" << client.user.getPassword() << "\"}";
        curlUtils.authUtil(apiIp + "/accounts/signup", formData.str(), flag, client.curl, client.res, client.slist);
        if (flag)
        {
            string response = curlUtils.getUtil(client.curl, client.res, apiIp + "/users/find?user_name=" + client.user.getName(), flag);
            nlohmann::json j = nlohmann::json::parse(response);
            client.user.setId(j.at("user_id"));
        }
        SendResponse(client.sock, "REGISTER_PASSWORD|" + to_string(flag));
    }
    else if (pattern == "CREATE_ROOM")
    {
        formData << "{\"roomName\": "
                 << "\"" + value + "\"}";
        string response = curlUtils.postUtil(client.curl, client.res, apiIp + "/user/" + ConvertIntToString(client.user.getId()) + "/room", formData.str(), flag);
        if (response.length() != 0)
        {
            RequestReload(clientMap, client, "RELOAD_ROOM|" + response);
        }
        SendResponse(client.sock, "CREATE_ROOM|" + response);
    }
    else if (pattern == "GET_LIST_ROOM")
    {
        string response = curlUtils.getUtil(client.curl, client.res, apiIp + "/room?isPrivate=0", flag);
        SendResponse(client.sock, "GET_LIST_ROOM|" + response);
    }
    else if (pattern == "GET_LIST_PRIVATE_ROOM")
    {
        string response = curlUtils.getUtil(client.curl, client.res, apiIp + "/user/" + ConvertIntToString(client.user.getId()) + "/room", flag);
        SendResponse(client.sock, "GET_LIST_PRIVATE_ROOM|" + response);
    }
    else if (pattern == "JOIN_ROOM")
    {
        string response = curlUtils.getUtil(client.curl, client.res, apiIp + "/room/" + value, flag);
        SendResponse(client.sock, "JOIN_ROOM|" + response);
        if (response.length() != 0)
        {
            nlohmann::json j = nlohmann::json::parse(response);
            client.joinedRoom.setChatId(j.at("roomId"));
            // client.joinedRoom.setRoomName(j.at("roomName"));
            formData << "";
            curlUtils.putUtil(client.curl, client.res, apiIp + "/user/" + ConvertIntToString(client.user.getId()) + "/room/" + value, formData.str(), flag);
            string inform = client.user.getName() + " has joined the room.";
            HandleInform(clientMap, client, inform);
        }
    }
    else if (pattern == "JOIN_ROOM_PRIVATE")
    {
        formData << "{\"roomName\": "
                 << "\"" + client.user.getName() + "_" + value + "\"}";
        string url = apiIp + "/useradmin/" + ConvertIntToString(client.user.getId()) + "/user/" + value + "/room";
        // Value buffer --> ID Target User
        // Create a private room between two clients
        string response = curlUtils.getUtil(client.curl, client.res, url + "?isPrivate=1", flag);
        cout << "res GET: " << response << endl;
        if (response.length() != 0)
        { // If room is exists --> Don't need to create --> Send back to client
            SendResponse(client.sock, "JOIN_ROOM_PRIVATE|" + response);
            nlohmann::json j = nlohmann::json::parse(response);
            client.joinedRoom.setChatId(j.at("roomId"));
            client.joinedRoom.setRoomName(j.at("roomName"));
            string inform = client.user.getName() + " has joined the room.";
            HandleInform(clientMap, client, inform);
            // Updated all the message inside when joined room successfully --> isSeen = 1
            response = curlUtils.putUtil(client.curl, client.res, apiIp + "/user/" + value + "/room/" + ConvertIntToString(client.joinedRoom.getChatId()) + "/message", "", flag);
            cout << "DEBUG is seen: " << response << endl;
        }
        else
        {                                                                                      // If the room didn't have --> Create a new room --> Get that room --> Send back to client
            response = curlUtils.postUtil(client.curl, client.res, url, formData.str(), flag); // Create new room
            cout << "res POST: " << response << endl;
            if (response.length() != 0)
            {
                SendResponse(client.sock, "CREATE_ROOM|");
                RequestReload(clientMap, client, "RELOAD_ROOM|");
                string response_get = curlUtils.getUtil(client.curl, client.res, url + "?isPrivate=1", flag);
                cout << "res GET_2: " << response_get << endl;
                if (response_get.length() != 0)
                { // If room is exists --> Don't need to create
                    SendResponse(client.sock, "JOIN_ROOM_PRIVATE|" + response_get);
                    nlohmann::json j = nlohmann::json::parse(response_get);
                    client.joinedRoom.setChatId(j.at("roomId"));
                    client.joinedRoom.setRoomName(j.at("roomName"));
                    string inform = "\033[0;32m" + client.user.getName() + " has joined the room." + "\033[0m";
                    HandleInform(clientMap, client, inform);
                }
            }
            else
            {
                SendResponse(client.sock, "CREATE_ROOM|Create Private Room Failed\n");
            }
        }
    }
    else if (pattern == "GET_LIST_CHAT")
    {
        string url = apiIp + "/room/" + value + "/message";
        string response = curlUtils.getUtil(client.curl, client.res, url, flag);
        // if (response.length() == 2) {
        //     SendResponse(client.sock, "GET_LIST_CHAT|");
        // } else
        if (response.length() > 1010)
        {
            nlohmann::json mainResponse = nlohmann::json::parse(response);
            nlohmann::json tempResponse = nlohmann::json::array();
            nlohmann::json smallResponse = nlohmann::json::array();
            for (auto i : boost::adaptors::reverse(mainResponse))
            {
                tempResponse.insert(tempResponse.begin(), i);
                if ((tempResponse.dump()).length() <= 1010)
                {
                    smallResponse.insert(smallResponse.begin(), i);
                }
                else
                {
                    // cout << smallResponse.dump() << endl;
                    cout << "GET_LIST_CHAT|" + smallResponse.dump() << endl;
                    SendResponse(client.sock, "GET_LIST_CHAT|" + smallResponse.dump());
                    tempResponse.clear();
                    smallResponse.clear();
                    break;
                    // tempResponse.insert(tempResponse.begin(), i);
                    // smallResponse.insert(smallResponse.begin(), i);
                }
            }
            // cout << smallResponse.dump() << endl;
            // cout << smallResponse.dump().length() << endl;
            // SendResponse(client.sock, "GET_LIST_CHAT|" + smallResponse.dump());
        }
        else
        {
            cout << "GET_LIST_CHAT|" + response << endl;
            SendResponse(client.sock, "GET_LIST_CHAT|" + response);
        }
    }
    else if (pattern == "USER_CHATTING")
    {
        formData << "{\"text\": "
                 << "\"" + value + "\"}";
        // cout << "User "<< client.user.getName() << " sent a message to room id "<< client.joinedRoom.getChatId()<<" : " << value << endl;
        Message newMessage(client.user, value);
        curlUtils.postUtil(client.curl, client.res, apiIp + "/user/" + ConvertIntToString(client.user.getId()) + "/room/" + ConvertIntToString(client.joinedRoom.getChatId()) + "/message", formData.str(), flag);
        if (flag)
        {
            bool sendSuccess = false;
            HandleMessage(clientMap, client, newMessage, sendSuccess);
        }
    }
    else if (pattern == "LOGOUT_USER")
    {
        curlUtils.handleDestruct(client.curl, client.slist);
        // cout << "User "<< client.user.getName() << " has logged out!\n";
        client.user.setId(-1);
        client.user.setName("");
        client.user.setPassword("");
        flag = true;
    }
    else if (pattern == "USER_EXIT")
    {
        string inform = client.user.getName() + " has left the room.";
        HandleInform(clientMap, client, inform);
        client.joinedRoom.setChatId(-1);
        // client.joinedRoom.setRoomName("");
        flag = true;
    }
    else if (pattern == "END_READING")
    {
        SendResponse(client.sock, "EndReading!");
        // client.joinedRoom.setChatId(-1);
        // client.joinedRoom.setRoomName("");
        flag = true;
    }
    else if (pattern == "CONTINUE_READING")
    {
        client.joinedRoom.setChatId(stoi(value));
        // string response = curlUtils.getUtil(client.curl, client.res, apiIp + "/room/" + value, flag);
        // client.joinedRoom.setRoomName(response);
    }
    else if (pattern == "FIND_USER")
    {
        string response = curlUtils.getUtil(client.curl, client.res, apiIp + "/users/search?userName=" + value, flag);
        SendResponse(client.sock, "FIND_USER|" + response);
    }
    else if (pattern == "UPDATE_PROFILE")
    {
        string response = curlUtils.getUtil(client.curl, client.res, apiIp + "/users/" + ConvertIntToString(client.user.getId()), flag);
        // cout << ConvertIntToString(client.user.getId()) << endl;
        // cout << response << endl;
        nlohmann::json j = nlohmann::json::parse(response);
        if (j.at("address").is_null())
        {
            j.at("address") = to_string(j.at("address"));
        }
        if (j.at("gender").is_null())
        {
            j.at("gender") = to_string(j.at("gender"));
        }
        client.user.setAddress(j.at("address"));
        client.user.setGender(j.at("gender"));
        SendResponse(client.sock, "UPDATE_PROFILE|" + response);
    }
    else if (pattern == "UPDATE_ADDRESS")
    {
        formData << "{\"address\": \"" + value + "\",\"gender\": \"" + client.user.getGender() + "\"}";
        // cout << formData.str() << endl;
        string response = curlUtils.patchUtil(client.curl, client.res, apiIp + "/users/" + ConvertIntToString(client.user.getId()), formData.str(), flag);
    }
    else if (pattern == "UPDATE_GENDER")
    {
        formData << "{\"address\": \"" + client.user.getAddress() + "\",\"gender\": \"" + value + "\"}";
        // cout << formData.str() << endl;
        string response = curlUtils.patchUtil(client.curl, client.res, apiIp + "/users/" + ConvertIntToString(client.user.getId()), formData.str(), flag);
    }
    else if (pattern == "CHANGE_PASSWORD")
    {
        formData << "{\"password\": \"" + value + "\"}";
        // cout << formData.str() << endl;
        // cout << client.user.getName() << " changed password!\n";
        string accountId = curlUtils.getUtil(client.curl, client.res, apiIp + "/accounts/find?userName=" + client.user.getName(), flag);
        string url = apiIp + "/accounts/" + accountId;
        string response = curlUtils.putUtil(client.curl, client.res, url, formData.str(), flag);
    }
    else if (pattern == "LIST_USER")
    {
        string url = apiIp + "/room/" + ConvertIntToString(client.joinedRoom.getChatId()) + "/members";
        cout << url << endl;
        string response = curlUtils.getUtil(client.curl, client.res, url, flag);
        // cout << response << endl;
        SendResponse(client.sock, "LIST_USER|" + response);
        cout << response << endl;
    }
    else if (pattern == "USER_EXIT_APP")
    {
        close(client.sock);
        RemoveClient(clientMap, client, fds, i);
        flag = true;
        SendResponse(client.sock, "USER_EXIT_APP|True");
    }
    else if (pattern == "USER_PROFILE")
    {
        string response = curlUtils.getUtil(client.curl, client.res, apiIp + "/users/" + value, flag);
        cout << response << endl;
        nlohmann::json j = nlohmann::json::parse(response);
        if (j.at("userName").is_null())
        {
            j.at("userName") = to_string(j.at("userName"));
        }
        if (j.at("address").is_null())
        {
            j.at("address") = to_string(j.at("address"));
        }
        if (j.at("gender").is_null())
        {
            j.at("gender") = to_string(j.at("gender"));
        }
        client.user.setName(j.at("userName"));
        client.user.setAddress(j.at("address"));
        client.user.setGender(j.at("gender"));
        SendResponse(client.sock, "USER_PROFILE|" + response);
    }
    else if (pattern == "ACTIVATE_THREAD")
    {
        SendResponse(client.sock, "|");
    }
    else if (pattern == "RECONNECT")
    {
        nlohmann::json j = nlohmann::json::parse(value);
        client.user.setName(j.at("userName"));
        client.user.setPassword(j.at("userPassword"));
        formData << "{\"userName\": "
                 << "\"" + client.user.getName() << "\","
                 << "\"password\": "
                 << "\"" << client.user.getPassword() << "\"}";
        curlUtils.authUtil(apiIp + "/accounts/signin", formData.str(), flag, client.curl, client.res, client.slist);
        if (flag)
        {
            string response = curlUtils.getUtil(client.curl, client.res, apiIp + "/users/find?user_name=" + client.user.getName(), flag);
            nlohmann::json j = nlohmann::json::parse(response);
            client.user.setId(j.at("user_id"));
        }
        if (j.at("roomId") != 0)
        {
            client.joinedRoom.setChatId(j.at("roomId"));
        }
        SendResponse(client.sock, "RECONNECT|");
    }
    else if (pattern == "RESEND")
    {
        nlohmann::json j = nlohmann::json::parse(value);
        for (string i : j)
        {
            formData << "{\"text\": "
                     << "\"" + i + "\"}";
            Message newMessage(client.user, i);
            curlUtils.postUtil(client.curl, client.res, apiIp + "/user/" + ConvertIntToString(client.user.getId()) + "/room/" + ConvertIntToString(client.joinedRoom.getChatId()) + "/message", formData.str(), flag);
            if (flag)
            {
                bool sendSuccess = false;
                HandleMessage(clientMap, client, newMessage, sendSuccess);
            }
        }
    }
    else if (pattern == "LOAD_HISTORY")
    {
        string url = apiIp + "/room/" + std::to_string(client.joinedRoom.getChatId()) + "/message";
        string response = curlUtils.getUtil(client.curl, client.res, url, flag);
        nlohmann::json mainResponse = nlohmann::json::parse(response);
        int endOfRes = mainResponse[0].at("id");
        nlohmann::json tempResponse = nlohmann::json::array();
        nlohmann::json smallResponse = nlohmann::json::array();
        if (stoi(value) == endOfRes)
        {
            cout << "DONE!";
            SendResponse(client.sock, "LOAD_HISTORY|");
        }
        else
        {
            bool isEnd = true;
            for (auto i : boost::adaptors::reverse(mainResponse))
            {
                if (i.at("id") < stoi(value))
                {
                    tempResponse.insert(tempResponse.begin(), i);
                    if ((tempResponse.dump()).length() <= 1000)
                    {
                        smallResponse.insert(smallResponse.begin(), i);
                        SendResponse(client.sock, "LOAD_HISTORY|" + smallResponse.dump());
                    }
                    else
                    {
                        isEnd = false;
                        cout << "LOAD_HISTORY|" + smallResponse.dump() << endl;
                        SendResponse(client.sock, "LOAD_HISTORY|" + smallResponse.dump());
                        break;
                        // tempResponse.insert(tempResponse.begin(), i);
                        // smallResponse.insert(smallResponse.begin(), i);
                    }
                }
            }
            if (isEnd)
            {
                // client.currentLoad = endOfRes;
                cout << "LOAD_HISTORY|" + smallResponse.dump() << endl;
                SendResponse(client.sock, "LOAD_HISTORY|" + smallResponse.dump());
            }
        }
        tempResponse.clear();
        smallResponse.clear();
    }
    return flag;
}

bool ChatServerService::handleConnect(int &serverFd, int maxClient, int &addrlen, struct sockaddr_in &address, bool isLocal, int port)
{
    int opt = 1;
    addrlen = sizeof(address);
    // Creating socket file descriptor
    if ((serverFd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("socket failed");
        return false;
    }
    // Forcefully attaching socket to the port 8080
    if (setsockopt(serverFd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        perror("setsockopt");
        return false;
    }
    address.sin_family = AF_INET;
    if (isLocal)
    {
        address.sin_addr.s_addr = inet_addr("127.0.0.1");
    }
    else
    {
        address.sin_addr.s_addr = INADDR_ANY;
    }
    address.sin_port = htons(port);
    // Forcefully attaching socket to the port 8080
    if (bind(serverFd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("bind failed");
        return false;
    }
    return true;
}

void ChatServerService::ProcessPollValue(map<int, Client> &clientMap, int maxClient, pollfd fds[], int serverFd, int &addrlen, struct sockaddr_in &address, int timeout, string apiIp, string mysqlIp)
{
    int nready = poll(fds, maxClient, timeout);
    if (nready < 0)
    {
        perror("failed to poll file descriptors");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < maxClient; i++)
    {
        if (fds[i].revents & POLLIN)
        {
            if (i == 0)
            {
                // Accept new connection
                int clientSocket = accept(serverFd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
                if (clientSocket < 0)
                {
                    perror("Failed to accept connection");
                    exit(EXIT_FAILURE);
                }
                Client client;
                client.sock = clientSocket;
                for (int j = 1; j < 10; j++)
                {
                    if (fds[j].fd == -1)
                    {
                        fds[j].fd = clientSocket;
                        clientMap[clientSocket] = client;
                        break;
                    }
                    if (j >= maxClient)
                    {
                        cout << "Coredump" << endl;
                    }
                }
            }
            else
            {
                handleClient(clientMap, clientMap[fds[i].fd], fds, i, apiIp, mysqlIp);
            }
        }

        else if (fds[i].revents & POLLHUP)
        {
            cout << fds[i].fd << " closed!\n";
        }
    }
}

void ChatServerService::ListenToClient(pollfd fds[], int serverFd, int maxClient)
{
    if (listen(serverFd, maxClient) < 0)
    {
        perror("listen");
        exit(1);
    }

    for (int i = 0; i < maxClient; i++)
    {
        fds[i].fd = -1;
        fds[i].events = POLLIN;
    }

    fds[0].fd = serverFd;
    fds[0].events = POLLIN;
}

void ChatServerService::HandleMultiClient(map<int, Client> &clientMap, int maxClient, int serverFd,
                                          struct sockaddr_in &address, int &addrlen, pollfd fds[], string apiIp, string mysqlIp)
{
    ListenToClient(fds, serverFd, maxClient);
    while (1)
    {
        ProcessPollValue(clientMap, maxClient, fds, serverFd, addrlen, address, -1, apiIp, mysqlIp);
    }
}

void ChatServerService::ShutDownServer(int serverFd)
{
    // closing the listening socket
    shutdown(serverFd, SHUT_RDWR);
}
