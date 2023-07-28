// Copyright [2023] <BRAINSTORMERS>
#pragma once
#include<vector>
#include<string>

#include "Message.h"

using std::string;
using std::vector;

class Chat {
 private:
    int id;
    string roomName;
    vector<Message> messages;
 public:
    Chat();
    Chat(int id, string roomName);
    explicit Chat(string roomName);
    void addMessage(Message message);
    const vector<Message>& getMessage();
    string getRoomName();
    void setRoomName(string roomName);
    int getChatId();
    void setChatId(int id);
};
