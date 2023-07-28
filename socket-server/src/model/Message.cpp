// Copyright [2023] <BRAINSTORMERS>
#include<string>

#include"Message.h"

Message::Message() {
}

Message::Message(User sender, string text) : sender(sender), text(text) {
}

Message::Message(int id, User sender, string text)
: id(id), sender(sender), text(text) {
}

int Message::getId() {
    return id;
}

User Message::getSender() {
    return sender;
}

string Message::getText() {
    return text;
}

void Message::setId(int id) {
    this->id = id;
}

void Message::setSender(User sender) {
    this->sender = sender;
}

void Message::setText(string text) {
    this->text = text;
}
