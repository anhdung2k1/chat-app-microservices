// Copyright [2023] <BRAINSTORMERS>
#include<vector>
#include<string>

#include"ChatClient.h"

using std::string;
using std::vector;

ChatClient::ChatClient() {
}

ChatClient::ChatClient(int id, string roomName) : id(id), roomName(roomName) {
}
ChatClient::ChatClient(int id, string roomName, int countUnreadMessage) : id(id), roomName(roomName), countUnreadMessage(countUnreadMessage){
}
ChatClient::ChatClient(string roomName) : roomName(roomName) {
}

void ChatClient::addMessage(MessageClient message) {
    messages.push_back(message);
}

void ChatClient::clearMessage() {
    this->messages.clear();
}

const vector<MessageClient>& ChatClient::getMessage() {
    return messages;
}

string ChatClient::getRoomName() {
    return roomName;
}

void ChatClient::setRoomName(string roomName) {
    this->roomName = roomName;
}

int ChatClient::getId() {
    return id;
}

void ChatClient::setId(int id) {
    this->id = id;
}

int ChatClient::getCountUnreadMessage(){
    return countUnreadMessage;
}

void ChatClient::setCountUnreadMessage(int countUnreadMessage){
    this->countUnreadMessage = countUnreadMessage;
}
