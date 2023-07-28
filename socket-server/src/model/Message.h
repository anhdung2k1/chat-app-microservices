// Copyright [2023] <BRAINSTORMERS>
#pragma once

#include<string>
#include<vector>

#include"User.h"

using std::string;

class Message {
 private:
        int id;
        User sender;
        string text;
 public:
        Message();
        Message(User sender, string text);
        Message(int id, User sender, string text);
        int getId();
        User getSender();
        string getText();
        void setId(int id);
        void setSender(User sender);
        void setText(string text);
};
