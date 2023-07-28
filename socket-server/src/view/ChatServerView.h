#pragma once
#include<iostream>
#include"../model/Message.h"
#include"../model/Chat.h"

class ChatServerView {
 public:
    static void displayMessage(Message message);
    static void displayChat(Chat chat);
};
