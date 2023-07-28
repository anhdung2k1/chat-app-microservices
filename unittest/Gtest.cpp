#include<gtest/gtest.h>
#include"../socket-server/src/services/ChatServerService.h"
#include"../socket-client/src/services/ChatService.h"
#include<iostream>

using std::thread;
using std::ref;

struct ChatServerService::Client {
    int sock;
    User user;
    Chat joinedRoom;
    CURL *curl;
    CURLcode res;
    struct curl_slist *slist;
};
ChatServerService::Client client;
int serverFd, addrlen;
struct sockaddr_in address;
int maxClient = 10;
map<int, ChatServerService::Client> clientMap;
pollfd fds[10];
char buffer[1024];
string pattern;
string value;
string roomId;
string apiIp = "http://" + (string)getenv("authenIp") + ":8080/api";
string mysqlIp = getenv("mysqlIp");
const string MYSQL_ADDRESS = mysqlIp;
// Check establishing connection in the server
TEST(TestConnection, ServerSideConnection) {
    EXPECT_TRUE(ChatServerService::handleConnect(serverFd, maxClient, addrlen, address, true, 8100));
}

// Test handle multiclient
TEST(TestHandleMultiClient, ServerSideConnection) {
    // The server listen to client
    ChatServerService::ListenToClient(fds, serverFd, maxClient);
    std::cout << "Test IP" << std::endl;
    std::cout << "API IP: " << apiIp << std::endl;
    std::cout << "mysql IP: " << mysqlIp << std::endl;
    // Start polling in the server
    thread connectThread = thread(ChatServerService::ProcessPollValue, ref(clientMap), maxClient, ref(fds), serverFd, ref(addrlen), ref(address), -1, apiIp, mysqlIp);

    // The FIRST client connect to the server
    EXPECT_EQ(ChatService::handleConnect("127.0.0.1", 8100), 4);
    connectThread.join();

    // Assign the socket client for the struct Client
    client.sock = 5;
    EXPECT_EQ(serverFd, fds[0].fd);
    EXPECT_EQ(client.sock, fds[1].fd);
    EXPECT_EQ(client.sock, clientMap[client.sock].sock); 
}

// Test registering with testing account 
// You should create the account first
// Account's name: TestAccount
// Account's password: abc123!@#
TEST(FeatureTesting, RegisterUserName) {
    // The client send REGISTER PATTERN
    EXPECT_TRUE(ChatService::RequestSend("REGISTER_USER|TestAccount", 4));
    // Shoulbe be FALSE because the account is already available
    EXPECT_FALSE(ChatServerService::handleClient(clientMap, client, fds, 1,apiIp,mysqlIp));
    bzero(buffer, 1024);
    // The Client read response from the server
    EXPECT_TRUE(ChatService::ResponseReceive(4, buffer));
    ChatService::processPattern(buffer, pattern, value);
    EXPECT_EQ("REGISTER_USER", pattern);
    EXPECT_FALSE(ChatService::ConvertToBool(value));
}

TEST(FeatureTesting, LoginUserName) {
    // The client send LOGIN USERNAME PATTERN
    EXPECT_TRUE(ChatService::RequestSend("LOGIN_USER|TestAccount", 4));
    // Shoulbe be TRUE because the account is already available
    EXPECT_TRUE(ChatServerService::handleClient(clientMap, client, fds, 1,apiIp,mysqlIp));
    bzero(buffer, 1024);
    EXPECT_TRUE(ChatService::ResponseReceive(4, buffer));
    ChatService::processPattern(buffer, pattern, value);
    EXPECT_EQ("LOGIN_USER", pattern);
    EXPECT_TRUE(ChatService::ConvertToBool(value));
}

TEST(FeatureTesting, LoginIncorrectPassword) {
    // The client send LOGIN PASSWORD PATTERN
    EXPECT_TRUE(ChatService::RequestSend("LOGIN_PASSWORD|abc123", 4));
    // Shoulbe be FALSE because the password is incorrect
    EXPECT_FALSE(ChatServerService::handleClient(clientMap, client, fds, 1,apiIp,mysqlIp));
    bzero(buffer, 1024);
    EXPECT_TRUE(ChatService::ResponseReceive(4, buffer));
    ChatService::processPattern(buffer, pattern, value);
    EXPECT_EQ("LOGIN_PASSWORD", pattern);
    EXPECT_FALSE(ChatService::ConvertToBool(value));
}

TEST(FeatureTesting, LoginCorrectPassword) {
    // The client send LOGIN PASSWORD PATTERN
    EXPECT_TRUE(ChatService::RequestSend("LOGIN_PASSWORD|abc123!@#", 4));
    // Shoulbe be TRUE because the password is correct
    EXPECT_TRUE(ChatServerService::handleClient(clientMap, client, fds, 1,apiIp,mysqlIp));
    bzero(buffer, 1024);
    EXPECT_TRUE(ChatService::ResponseReceive(4, buffer));
    ChatService::processPattern(buffer, pattern, value);
    EXPECT_EQ("LOGIN_PASSWORD", pattern);
    EXPECT_TRUE(ChatService::ConvertToBool(value));
}

TEST(FeatureTesting, CreateRoom) {
    // The client send CREATE ROOM PATTERN
    EXPECT_TRUE(ChatService::RequestSend("CREATE_ROOM|TestRoom", 4));
    // Shoulbe be TRUE if there is no bug!
    EXPECT_TRUE(ChatServerService::handleClient(clientMap, client, fds, 1,apiIp,mysqlIp));
    bzero(buffer, 1024);
    EXPECT_TRUE(ChatService::ResponseReceive(4, buffer));
    ChatService::processPattern(buffer, pattern, value);
    EXPECT_EQ("CREATE_ROOM", pattern);
    EXPECT_NE("[]", value);
    // Get testing room Id
    roomId = ChatRepository::GetLatestRoomId(mysqlIp);
}

TEST(FeatureTesting, GetListRoom) {
    // The client send GET LIST ROOM PATTERN
    EXPECT_TRUE(ChatService::RequestSend("GET_LIST_ROOM", 4));
    // Shoulbe be TRUE if there is no bug!
    EXPECT_TRUE(ChatServerService::handleClient(clientMap, client, fds, 1,apiIp,mysqlIp));
    bzero(buffer, 1024);
    EXPECT_TRUE(ChatService::ResponseReceive(4, buffer));
    ChatService::processPattern(buffer, pattern, value);
    EXPECT_EQ("GET_LIST_ROOM", pattern);
    EXPECT_GE(value.length(), 0);
}

TEST(FeatureTesting, JoinRoom) {
    // The client send JOIN ROOM PATTERN
    EXPECT_TRUE(ChatService::RequestSend("JOIN_ROOM|" + roomId, 4));
    // Shoulbe be TRUE if there is no bug!
    EXPECT_TRUE(ChatServerService::handleClient(clientMap, client, fds, 1,apiIp,mysqlIp));
    bzero(buffer, 1024);
    EXPECT_TRUE(ChatService::ResponseReceive(4, buffer));
    ChatService::processPattern(buffer, pattern, value);
    EXPECT_EQ("JOIN_ROOM", pattern);
    EXPECT_GE(value.length(), 0);
}

TEST(FeatureTesting, GetListChat) {
    // The client send GET LIST CHAT PATTERN
    EXPECT_TRUE(ChatService::RequestSend("GET_LIST_CHAT|" + roomId, 4));
    // Shoulbe be true if there is no bug!
    EXPECT_TRUE(ChatServerService::handleClient(clientMap, client, fds, 1,apiIp,mysqlIp));
    bzero(buffer, 1024);
    EXPECT_TRUE(ChatService::ResponseReceive(4, buffer));
    ChatService::processPattern(buffer, pattern, value);
    EXPECT_EQ("GET_LIST_CHAT", pattern);
    EXPECT_EQ( "[]", value);
}

TEST(FeatureTesting, ListUserInRoom) {
    // The client send GET LIST USER IN ROOM PATTERN
    EXPECT_TRUE(ChatService::RequestSend("LIST_USER|" + roomId, 4));
    // Shoulbe be true if there is no bug!
    EXPECT_TRUE(ChatServerService::handleClient(clientMap, client, fds, 1,apiIp,mysqlIp));
    bzero(buffer, 1024);
    EXPECT_TRUE(ChatService::ResponseReceive(4, buffer));
    ChatService::processPattern(buffer, pattern, value);
    EXPECT_EQ("LIST_USER", pattern);
    EXPECT_NE( "[]", value);
}

TEST(FeatureTesting, UserChat) {
    // The client send CHATTING PATTERN
    EXPECT_TRUE(ChatService::RequestSend("USER_CHATTING|Hello, testing message!", 4));
    // Shoulbe be true if there is no bug!
    EXPECT_TRUE(ChatServerService::handleClient(clientMap, client, fds, 1,apiIp,mysqlIp));
}

TEST(FeatureTesting, FindUser) {
    // The client send FIND USER PATTERN
    EXPECT_TRUE(ChatService::RequestSend("FIND_USER|TestAccount", 4));
    // Shoulbe be true if there is no bug!
    EXPECT_TRUE(ChatServerService::handleClient(clientMap, client, fds, 1,apiIp,mysqlIp));
    bzero(buffer, 1024);
    EXPECT_TRUE(ChatService::ResponseReceive(4, buffer));
    ChatService::processPattern(buffer, pattern, value);
    EXPECT_EQ("FIND_USER", pattern);
    EXPECT_NE( "[]", value);
}

TEST(FeatureTesting, UpdateProfile) {
    // The client send UPDATE PROFILE PATTERN
    EXPECT_TRUE(ChatService::RequestSend("UPDATE_PROFILE|TestAccount", 4));
    // Shoulbe be true if there is no bug!
    EXPECT_TRUE(ChatServerService::handleClient(clientMap, client, fds, 1,apiIp,mysqlIp));
    bzero(buffer, 1024);
    EXPECT_TRUE(ChatService::ResponseReceive(4, buffer));
    ChatService::processPattern(buffer, pattern, value);
    EXPECT_EQ("UPDATE_PROFILE", pattern);
    EXPECT_NE( "[]", value);
}

TEST(FeatureTesting, UpdateAddress) {
    // The client send UPDATE ADDRESS PATTERN
    EXPECT_TRUE(ChatService::RequestSend("UPDATE_GENDER|TestAddress", 4));
    // Shoulbe be true if there is no bug!
    EXPECT_TRUE(ChatServerService::handleClient(clientMap, client, fds, 1,apiIp,mysqlIp));
}

TEST(FeatureTesting, UpdateGender) {
    // The client send UPDATE GENDER PATTERN
    EXPECT_TRUE(ChatService::RequestSend("UPDATE_ADDRESS|TestGender", 4));
    // Shoulbe be true if there is no bug!
    EXPECT_TRUE(ChatServerService::handleClient(clientMap, client, fds, 1,apiIp,mysqlIp));
}

TEST(FeatureTesting, ChangePassword) {
    // The client send UPDATE PROFILE PATTERN
    EXPECT_TRUE(ChatService::RequestSend("CHANGE_PASSWORD|newpassword", 4));
    // Shoulbe be true if there is no bug!
    EXPECT_TRUE(ChatServerService::handleClient(clientMap, client, fds, 1,apiIp,mysqlIp));
}

TEST(FeatureTesting, Logout) {
    // The client send UPDATE PROFILE PATTERN
    EXPECT_TRUE(ChatService::RequestSend("LOGOUT_USER|", 4));
    // Shoulbe be true if there is no bug!
    EXPECT_TRUE(ChatServerService::handleClient(clientMap, client, fds, 1,apiIp,mysqlIp));
}

TEST(NewPasswordTesting, Relogin) {
    // The client send LOGIN USERNAME PATTERN
    EXPECT_TRUE(ChatService::RequestSend("LOGIN_USER|TestAccount", 4));
    // Shoulbe be TRUE because the account is already available
    EXPECT_TRUE(ChatServerService::handleClient(clientMap, client, fds, 1,apiIp,mysqlIp));
    EXPECT_TRUE(ChatService::ResponseReceive(4, buffer));
    ChatService::processPattern(buffer, pattern, value);
    EXPECT_EQ("LOGIN_USER", pattern);
    EXPECT_TRUE(ChatService::ConvertToBool(value));
    // The client send LOGIN PASSWORD PATTERN
    EXPECT_TRUE(ChatService::RequestSend("LOGIN_PASSWORD|newpassword", 4));
    // Shoulbe be TRUE because the password is correct
    EXPECT_TRUE(ChatServerService::handleClient(clientMap, client, fds, 1,apiIp,mysqlIp));
    EXPECT_TRUE(ChatService::ResponseReceive(4, buffer));
    ChatService::processPattern(buffer, pattern, value);
    EXPECT_EQ("LOGIN_PASSWORD", pattern);
    EXPECT_TRUE(ChatService::ConvertToBool(value));
}


TEST(CleanUpStage, CleanUpTesting) {
    EXPECT_TRUE(ChatService::RequestSend("CHANGE_PASSWORD|abc123!@#", 4));
    // Shoulbe be true if there is no bug!
    EXPECT_TRUE(ChatServerService::handleClient(clientMap, client, fds, 1,apiIp,mysqlIp));
    EXPECT_TRUE(ChatService::RequestSend("USER_EXIT_APP|", 4));
    
    EXPECT_TRUE(ChatServerService::handleClient(clientMap, client, fds, 1,apiIp,mysqlIp));
    EXPECT_TRUE(ChatRepository::CleanUp(mysqlIp));
    ChatServerService::ShutDownServer(serverFd);
}

int main(int argc,char **argv){
	testing::InitGoogleTest(&argc,argv);
	return RUN_ALL_TESTS();
}
