#ifndef MESSAGE_H
#define MESSAGE_H

#include<iostream>
#include<fstream>
using namespace std;
class Message
{
public:
    string from;
    string status;
    string message;
    int amount;

    Message();

    //files functions

    void serializeString(std::ofstream& ofs, const std::string& str)const;
    std::string deserializeString(std::ifstream& ifs) const;

    void serialize(std::ofstream& ofs) const;
    void deserialize(std::ifstream& ifs);

};
#endif // MESSAGE_H
