// Copyright [2023] <BRAINSTORMERS>
#include<string>

#include"MessageClient.h"

MessageClient::MessageClient() {
}

MessageClient::MessageClient(UserClient sender, string text) : sender(sender), text(text) {
}

MessageClient::MessageClient(int id, UserClient sender, string text)
: id(id), sender(sender), text(text) {
}

int MessageClient::getId() {
    return id;
}

UserClient MessageClient::getSender() {
    return sender;
}

string MessageClient::getText() {
    return text;
}

void MessageClient::setId(int id) {
    this->id = id;
}

void MessageClient::setSender(UserClient sender) {
    this->sender = sender;
}

void MessageClient::setText(string text) {
    this->text = text;
}
