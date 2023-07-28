// Copyright [2023] <BRAINSTORMERS>
#include<vector>
#include<string>
#include"Chat.h"

using std::string;
using std::vector;

Chat::Chat() {
}

Chat::Chat(int id, string roomName) : id(id), roomName(roomName) {
}

Chat::Chat(string roomName) : roomName(roomName) {
}

void Chat::addMessage(Message message) {
    messages.push_back(message);
}

const vector<Message>& Chat::getMessage() {
    return messages;
}

string Chat::getRoomName() {
    return roomName;
}

void Chat::setRoomName(string roomName) {
    this->roomName = roomName;
}

int Chat::getChatId() {
    return id;
}

void Chat::setChatId(int id) {
    this->id = id;
}
