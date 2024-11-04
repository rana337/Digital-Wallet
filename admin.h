#ifndef Admin_H
#define Admin_H
#include <iostream>
#include <string>
#include "User.h"
#include <unordered_map>
using namespace std;
class Admin
{
public:
    string userName,password;

    //datastructure related to the system
    static unordered_map<string, User>users;
    static unordered_map<string, string>phones;
    static unordered_map<string, string>cards;

    void addUser(User new_user);
    User getUserData(string user_name, string password);

    int check_signin_data(string user,string phone);
    void dis();

    Admin();
    void SendMoney(float amount,float fee,string sender_card_num, string receiver_card_num,string sender_user_name,string receiver_user_name);



};

#endif // Admin_H
