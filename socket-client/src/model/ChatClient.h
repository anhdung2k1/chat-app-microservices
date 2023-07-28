// Copyright [2023] <BRAINSTORMERS>
#pragma once
#include <vector>
#include <string>

#include "MessageClient.h"

using std::string;
using std::vector;

class ChatClient
{
private:
   int id;
   string roomName;
   int countUnreadMessage;
   vector<MessageClient> messages;

public:
   ChatClient();
   ChatClient(int id, string roomName);
   ChatClient(int id, string roomName, int countUnreadMessage);
   explicit ChatClient(string roomName);
   void addMessage(MessageClient message);
   const vector<MessageClient> &getMessage();
   void clearMessage();
   string getRoomName();
   void setRoomName(string roomName);
   int getId();
   void setId(int id);
   int getCountUnreadMessage();
   void setCountUnreadMessage(int countUnreadMessage);
};