#ifndef CARD_H
#define CARD_H

#include<iostream>
#include<string>
#include<fstream>


using namespace std;


class Card
{
public:
    string card_number;
    string card_holder_name;
    int card_ATM_pin;
    float card_balance;

    Card();

    //files functions

    void serializeString(std::ofstream& ofs, const std::string& str)const;
    std::string deserializeString(std::ifstream& ifs) const;

    void deserialize(std::ifstream& ifs);
    void serialize(std::ofstream& ofs) const;


};



#endif // CARD_H
