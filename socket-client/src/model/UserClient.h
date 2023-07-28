// Copyright [2023] <BRAINSTORMERS>
#pragma once

#include<string>
using std::string;

class UserClient {
 private:
        int id;
        string name;
        string password;
        string gender;
        string birthday;
        string address;
 public:
        UserClient();
        UserClient(string name, string password);
        UserClient(int id, string name);
        UserClient(int id, string name, string password);
        int getId();
        string getName();
        string getPassword();
        string getGender();
        string getBirthday();
        string getAddress();
        void setId(int id);
        void setName(string name);
        void setPassword(string password);
        void setGender(string gender);
        void setBirthday(string birthday);
        void setAddress(string address);
};