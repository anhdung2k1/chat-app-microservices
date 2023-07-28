// Copyright [2023] <BRAINSTORMERS>
#include"ChatRepository.h"

using std::string;
using std::vector;
using std::stringstream;
using std::stoi;

struct ChatRepository::connectionDetails {
    const char*server, *user, *password, *database;
};

// string ChatRepository::ConvertIntToString(int id) {
//     stringstream ss;
//     ss << id;
//     string s;
//     ss >> s;
//     return s;
// }

MYSQL* ChatRepository::mysqlConnectionSetup(
    struct connectionDetails mysql_details) {
    MYSQL *connection = mysql_init(NULL);
    if (!mysql_real_connect(connection,
    mysql_details.server, mysql_details.user,
    mysql_details.password, mysql_details.database, 0, NULL, 0)) {
        perror("Connection Error");
        exit(1);
    }
    return connection;
}

MYSQL_RES* ChatRepository::mysqlExecuteQuery(
    MYSQL *connection, const char *sql_query) {
    if (mysql_query(connection, sql_query)) {
        perror("MySQL Query Error");
        exit(1);
    }
    return mysql_use_result(connection);
}

MYSQL* ChatRepository::connectToDatabase(string address) {
    struct connectionDetails mySql;
    mySql.server = address.c_str();
    mySql.user = "messager";
    mySql.password = "messager";
    mySql.database = "messager";
    return  mysqlConnectionSetup(mySql);
}

bool ChatRepository::CheckUserName(string username, string address) {
    bool returnValue;
    string query = "select * from users where BINARY user_name = '" + username + "';";
    MYSQL *con = connectToDatabase(address);
    MYSQL_RES *res = mysqlExecuteQuery(con, query.c_str());
    if (mysql_fetch_row(res) != NULL) {
        returnValue = true;
    } else {
        returnValue = false;
    }
    mysql_free_result(res);
    mysql_close(con);
    return returnValue;
}

// For testing only
string ChatRepository::GetLatestRoomId(string mysql_address) {
    string roomId;
    MYSQL_ROW row;
    MYSQL_RES *res;
    MYSQL *con = connectToDatabase(mysql_address);
    string query = "select * from room order by room_id desc limit 1;";
    // std::cout<< query << std::endl;
    res = mysqlExecuteQuery(con, query.c_str());
    if ((row = mysql_fetch_row(res)) != NULL) {
        roomId = row[0];
    }
    mysql_free_result(res);
    mysql_close(con);
    return roomId;
}

// Clear the room test
bool ChatRepository::CleanUp(string mysql_address) {
    string roomId = GetLatestRoomId(mysql_address);
    MYSQL_ROW row;
    MYSQL_RES *res;
    MYSQL *con = connectToDatabase(mysql_address);
    string query = "delete from message where room_id = " + roomId + ";";
    // std::cout<< query << std::endl;
    res = mysqlExecuteQuery(con, query.c_str());
    mysql_free_result(res);

    query = "delete from room_user where room_id = " + roomId + ";";
    // std::cout<< query << std::endl;
    res = mysqlExecuteQuery(con, query.c_str());
    mysql_free_result(res);

    query = "delete from room where room_id = " + roomId + ";";
    // std::cout<< query << std::endl;
    res = mysqlExecuteQuery(con, query.c_str());
    mysql_free_result(res);

    query = "alter table room AUTO_INCREMENT = " + roomId + ";";
    // std::cout<< query << std::endl;
    res = mysqlExecuteQuery(con, query.c_str());
    mysql_free_result(res);
    mysql_close(con);

    return true;
}

// void ChatRepository::CreateRoom(Chat room) {
//     MYSQL_ROW row;
//     MYSQL_RES *res;
//     MYSQL *con = connectToDatabase();
//     string query = "insert into room (room_name) values ('"+ 
//     room.getRoomName() + "');";
//     res = mysqlExecuteQuery(con, query.c_str());
//     mysql_free_result(res);
//     mysql_close(con);
// }

// void ChatRepository::GetListRoom(vector<Chat> &listChatRooms) {
//     int id;
//     MYSQL_ROW row;
//     MYSQL_RES *res;
//     MYSQL *con = connectToDatabase();
//     Chat room;
//     res = mysqlExecuteQuery(con, "select * from room;");
//     while ((row = mysql_fetch_row(res)) != NULL) {
//         room.setChatId(stoi(row[0]));
//         room.setRoomName(row[2]);
//         listChatRooms.push_back(room);
//     }
//     mysql_free_result(res);
//     mysql_close(con);
// }

// bool ChatRepository::CheckRoom(string id) {
//     bool isRoomVailable = true;
//     MYSQL_ROW row;
//     MYSQL_RES *res;
//     MYSQL *con = connectToDatabase();
//     string query = "select * from room where room_id = " + id + ";";
//     res = mysqlExecuteQuery(con, query.c_str());
//     if (mysql_fetch_row(res) == NULL) {
//         isRoomVailable = false;
//     } else {
//         isRoomVailable = true;
//     }
//     mysql_free_result(res);
//     mysql_close(con);
//     return isRoomVailable;
// }

// void ChatRepository::GetRoom(string id, Chat &chat) {
//     MYSQL_ROW row;
//     MYSQL_RES *res;
//     MYSQL *con = connectToDatabase();
//     string query = "select * from room where room_id = " + id + ";";
//     res = mysqlExecuteQuery(con, query.c_str());
//     while ((row = mysql_fetch_row(res)) != NULL) {
//         chat.setChatId(stoi(row[0]));
//         chat.setRoomName(row[2]);
//     }
//     mysql_free_result(res);
//     mysql_close(con);
// }

// /*void ChatRepository::SaveUserToRoom(int userId, int roomId) {
//     MYSQL_ROW row;
//     MYSQL_RES *res;
//     MYSQL *con = connectToDatabase();
//     string query = "insert into UserChatRoom (user_id, room_id) values (" +
//         ConvertIntToString(userId) + ", " +
//         ConvertIntToString(roomId) + ");";
//     res = mysqlExecuteQuery(con, query.c_str());
//     mysql_free_result(res);
//     mysql_close(con);
// }*/

// void ChatRepository::CreateMessage(Message message, int roomId, int userId) {
//     MYSQL_ROW row;
//     MYSQL_RES *res;
//     MYSQL *con = connectToDatabase();

//     string query =
//         "insert into message (content, room_id, user_id) values ('" +
//         message.getText() + "', " +
//         ConvertIntToString(roomId) + ", " +
//         ConvertIntToString(userId) + ");";
//     // std::cout<<query<<std::endl;
//     res = mysqlExecuteQuery(con, query.c_str());
//     mysql_free_result(res);
//     mysql_close(con);
// }

// User ChatRepository::GetUserById(string id) {
//     MYSQL_ROW row;
//     MYSQL_RES *res;
//     User user;
//     MYSQL *con = connectToDatabase();
//     string query = "select * from users where user_id = " + id + ";";
//     res = mysqlExecuteQuery(con, query.c_str());
//     while ((row = mysql_fetch_row(res)) != NULL) {
//         user.setId(stoi(row[0]));
//         user.setName(row[6]);
//     }
//     mysql_free_result(res);
//     mysql_close(con);
//     return user;
// }

// int ChatRepository::GetUserIdByName(string name) {
//     MYSQL_ROW row;
//     MYSQL_RES *res;
//     User user;
//     MYSQL *con = connectToDatabase();
//     string query = "select * from users where user_name = '" + name + "';";
//     res = mysqlExecuteQuery(con, query.c_str());
//     int id;
//     while ((row = mysql_fetch_row(res)) != NULL) {
//         id = stoi(row[0]);
//     }
//     mysql_free_result(res);
//     mysql_close(con);
//     return id;
// }

// void ChatRepository::GetListMessage(Chat& room) {
//     MYSQL_ROW row;
//     MYSQL_RES *res;
//     MYSQL *con = connectToDatabase();
//     string query = "select * from message where room_id = "
//      + ConvertIntToString(room.getChatId()) + ";";
//     res = mysqlExecuteQuery(con, query.c_str());
//     while ( (row = mysql_fetch_row(res) ) != NULL ) {
//         User sender = GetUserById(row[5]);
//         Message message(sender, row[2]);
//         room.addMessage(message);
//     }
//     mysql_free_result(res);
//     mysql_close(con);
// }
