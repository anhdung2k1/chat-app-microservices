// Copyright [2023] <BRAINSTORMERS>
#pragma once

#include<string>
#include<vector>

#include"UserClient.h"

using std::string;

class MessageClient {
 private:
        int id;
        UserClient sender;
        string text;
 public:
        MessageClient();
        MessageClient(UserClient sender, string text);
        MessageClient(int id, UserClient sender, string text);
        int getId();
        UserClient getSender();
        string getText();
        void setId(int id);
        void setSender(UserClient sender);
        void setText(string text);
};
