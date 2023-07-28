// Copyright [2023] <BRAINSTORMERS>
#include<string>
#include"UserClient.h"
using std::string;

UserClient::UserClient() {
}

UserClient::UserClient(string name, string password) : name(name), password(password) {
}

UserClient::UserClient(int id, string name) : id(id), name(name) {
}

UserClient::UserClient(int id, string name, string password)
: id(id), name(name), password(password) {
}

int UserClient::getId() {
    return id;
}

string UserClient::getName() {
    return name;
}

string UserClient::getPassword() {
    return password;
}

string UserClient::getGender() {
    return gender;
}

string UserClient::getBirthday() {
    return birthday;
}

string UserClient::getAddress() {
    return address;
}

void UserClient::setId(int id) {
    this->id = id;
}

void UserClient::setName(string name) {
    this->name = name;
}

void UserClient::setPassword(string password) {
    this->password = password;
}

void UserClient::setGender(string gender) {
    this->gender = gender;
}

void UserClient::setBirthday(string birthday) {
    this->birthday = birthday;
}

void UserClient::setAddress(string address) {
    this->address = address;
}