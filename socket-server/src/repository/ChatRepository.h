// // Copyright [2023] <BRAINSTORMERS>
#pragma once
#include <mysql/mysql.h>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>

#include"../model/Chat.h"
#include"../model/User.h"

using std::string;
using std::vector;

class ChatRepository {
 private:
        // hatRepository chatRepository;
 public:
    static string ConvertIntToString(int id);
    struct connectionDetails;
    static MYSQL* mysqlConnectionSetup(
        struct connectionDetails mysql_details);
    static MYSQL_RES* mysqlExecuteQuery(
        MYSQL *connection, const char *sql_query);
    static MYSQL* connectToDatabase(string address);
    static bool CheckUserName(string username, string address);
    static string GetLatestRoomId(string mysql_address);
    static bool CleanUp(string mysql_address);
    // static void CreateRoom(Chat room);
    // static void GetListRoom(vector<Chat>& listChatRooms);
    // static bool CheckRoom(string id);
    // static void GetRoom(string id, Chat& chat);
    // static void SaveUserToRoom(int userId, int roomId);
    // static void CreateMessage(Message message, int roomId, int userId);
    // static User GetUserById(string id);
    // static void GetListMessage(Chat& room);
    // static int GetUserIdByName(string name);
};
