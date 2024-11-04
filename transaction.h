#ifndef TRANSACTION_H
#define TRANSACTION_H

#include<iostream>
#include<fstream>
#include<string>

using namespace std;
enum status{
    SUCCESSED,FAILED
};
enum kind{
    SENDER,RECIVER
};

struct TrasactionDate
{
    int day=0;
    int month=0;
    int year=0;
    int hour=0;
    int minutes=0;
    int seconds=0;
};

class Transaction
{
public:
    string from;
    string to;
    TrasactionDate date;
    status status;
    kind transaction_kind;
    float amount;


    Transaction();
    Transaction(string sender_user_name,string receiver_user_name,float amount,kind k);
    enum status convertStatusToEnum(string transaction_status);
    enum kind convertKindToEnum(string transaction_kind);


    template<typename Enum>
    string enumToString(Enum value)const;
    template<typename Enum>
    string enumToString2(Enum value)const;


    //files functions
    void serializeString(std::ofstream& ofs, const std::string& str)const;
    std::string deserializeString(std::ifstream& ifs) const;

    void serialize(std::ofstream& ofs) const;
    void deserialize(std::ifstream& ifs);

};

#endif // TRANSACTION_H
