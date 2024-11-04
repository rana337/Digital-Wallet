#ifndef User_H
#define User_H


#include <iostream>
#include<stack>
#include<vector>
#include<list>
#include<fstream>
#include<string>
#include"Transaction.h"
#include"Card.h"
#include"Message.h"

using namespace std;
struct Date
{
    int day=0;
    int month=0;
    int year=0;
};
class User
{
public:
    //user information
    int Pin;
    string user_name;
    string password;
    string email;
    Date birth_date;
    string phone_number;
    string address_account;
    string status;
    float balance;

    //datastructure that related to the user
    stack<Transaction>transactions;
    vector<Card>cards;
    list<Message>messages;


    User();
    User(string user_name, string password, Date birth_date, string email, string phone_number, int pin);
    void AddMoney(string card_num,float amount);
    void DeductMoney(string card_num,float amount);
    void StorTransaction(string sender_user_name,string receiver_user_name,float amount, kind k);

    bool requestMoneyUsingPhoneNumber(string phoneNumber,float ammount);
    bool requestMoneyUsingPaymentAddress(string userName,float ammount);
    string passwordEnc(string pass);
    string passwordDec(string pass);

    //string HasingPass(string password);


    //files functions

    void serializeString(std::ofstream& ofs, const std::string& str)const;
    std::string deserializeString(std::ifstream& ifs) const;


    void serializeStack(std::ofstream& ofs,const std::stack<Transaction>& stack) const;
    void deserializeStack(std::ifstream& ifs, std::stack<Transaction>& stack)const;
    void serializeList(std::ofstream& ofs, const std::list<Message>& myList)const;
    void deserializeList(std::ifstream& ifs, std::list<Message>& myList)const;
    void serializeVector(std::ofstream& ofs,const  std::vector<Card>& vec)const;
    void deserializeVector(std::ifstream& ifs, std::vector<Card>& vec)const;

    void serialize(std::ofstream& ofs)const;
    void deserialize(std::ifstream& ifs);

};



#endif // USER_H
