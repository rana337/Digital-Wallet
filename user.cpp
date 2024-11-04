#include "user.h"
#include "transaction.h"
#include "admin.h"
#include<QDateTime>

User::User(string user_name, string password, Date birth_date, string email, string phone_number, int pin)
{
    this->user_name = user_name;
    this->password = password;
    this->birth_date = birth_date;
    this->email = email;
    this->phone_number = phone_number;
    Admin::phones[phone_number]=user_name;
    this->Pin = pin;
    this->address_account=user_name+"@easypay.com";
    this->status="Actived";
    this->balance=000;
}

User::User()
{
    this->balance=000;
    this->user_name ="";
    this->password = "";
    this->birth_date.day = 0;
    this->birth_date.month = 4;
    this->birth_date.year = 0;
    this->email = "";
    this->phone_number = "";
    this->Pin = 000000;
}

void User::AddMoney(string card_num,float amount){
    if(card_num=="Easy Pay Wallet")
    {
        balance+=amount;
    }
    else
    {
        if(card_num!="-1"){
            for (int i = 0; i < cards.size(); ++i) {
                if (cards[i].card_number==card_num){
                    cards[i].card_balance+=amount;
                    break;
                }
            }
        }
    }
}

void User::DeductMoney(string sender_card_num,float amount){
    if(sender_card_num=="Easy Pay Wallet")
    {
        balance-=amount;
    }
    else
    {
        for (int i = 0; i < cards.size(); ++i) {
            if (cards[i].card_number==sender_card_num){
                cards[i].card_balance-=amount;
                break;
            }
        }
    }
}

void User::StorTransaction(string sender_user_name,string receiver_user_name,float amount, kind k){
    Transaction tmp = Transaction(sender_user_name,receiver_user_name,amount,k);
    //QDate currentDate = QDate::currentDate();
    tmp.date.day   = QDate::currentDate().day();
    tmp.date.month = QDate::currentDate().month();
    tmp.date.year  = QDate::currentDate().year();

    QTime current_time=QTime::currentTime();
    tmp.date.hour = current_time.hour();
    tmp.date.month =current_time.minute();
    tmp.date.year = current_time.second();
    transactions.push(tmp);
}

bool User::requestMoneyUsingPhoneNumber(string phoneNumber,float ammount)
{
    if(Admin::phones.find(phoneNumber)==Admin::phones.end())
    {
        return false;
    }
    else
    {
        string reciverUserName=Admin::phones[phoneNumber];

        Message tempMessage;
        tempMessage.from=this->user_name;
        tempMessage.amount=ammount;

        tempMessage.message=this->user_name+" has requested "+to_string(ammount);
        tempMessage.status="Waiting";
        Admin::users[reciverUserName].messages.push_back(tempMessage);

        return true;
    }

}

bool User::requestMoneyUsingPaymentAddress(string userName,float ammount)
{
    if(Admin::users.find(userName)==Admin::users.end())
    {
        return false;
    }
    else
    {
        Message tempMessage;
        tempMessage.from=this->user_name;
        tempMessage.amount=ammount;
        tempMessage.message=this->user_name+" has requested "+to_string(ammount);
        tempMessage.status="Waiting";
        Admin::users[userName].messages.push_back(tempMessage);

        return true;
    }
}

string User::passwordEnc(string pass)
{
    //setUnorderedMap();
    int keyMatrix[4] = { 1,4,0,2 };
    if (pass.size() % 2 != 0)
    {
        pass.push_back('?');
    }

    string encryptedPass = "";

    for (int i = 0,j=1; j< pass.size(); i += 2,j+=2)
    {
        int fChar=pass[i]-32;
        int secChar=pass[j]-32;
        //cout << fChar << "    " << secChar << ">>>>>>>>";


        //cout << fChar << "    " << secChar << ">>>>>>>>";
        //cout <<(char) fChar << "    " <<(char) secChar << endl;

        encryptedPass.push_back(((keyMatrix[0] * fChar + keyMatrix[1] * secChar) % 95)+32);
        encryptedPass.push_back(((keyMatrix[2] * fChar + keyMatrix[3] * secChar) % 95 )+32);
    }

    return encryptedPass;
}

string User::passwordDec(string pass)
{
    int keyMatrix[4] = { 1,93,0,48 };


    string main_password = "";

    for (int i = 0, j = 1;  j < pass.size(); i += 2, j += 2)
    {
        main_password.push_back(((keyMatrix[0] * (pass[i]-32) + keyMatrix[1] * (pass[j]-32)) % 95)+32);
        main_password.push_back(((keyMatrix[2] * (pass[i]-32) + keyMatrix[3] * (pass[j]-32)) % 95)+32);
    }

    if (main_password.back() == '?')
        main_password.pop_back();


    return main_password;
}

//--------------------------------------------------------------------------------------------------------------
// Helper function to serialize strings
void User::serializeString(std::ofstream& ofs, const std::string& str) const {
    size_t size = str.size();
    ofs.write(reinterpret_cast<const char*>(&size), sizeof(size));
    ofs.write(str.c_str(), size);
}

// Helper function to deserialize strings
std::string User:: deserializeString(std::ifstream& ifs) const {
    size_t size;
    ifs.read(reinterpret_cast<char*>(&size), sizeof(size));
    std::string str(size, '\0');
    ifs.read(&str[0], size);
    return str;
}



// Serialize a stack
void User:: serializeStack(std::ofstream& ofs,const std::stack<Transaction>& stack) const {
    size_t size = stack.size();
    ofs.write(reinterpret_cast<const char*>(&size), sizeof(size));
    std::stack<Transaction> tempStack = stack; // Create a copy of the stack
    while (!tempStack.empty()) {
        Transaction element = tempStack.top();
        element.serialize(ofs);
        tempStack.pop();
    }
}

// Deserialize a stack
void User::deserializeStack(std::ifstream& ifs, std::stack<Transaction>& stack)const {
    size_t size;
    ifs.read(reinterpret_cast<char*>(&size), sizeof(size));
    for (size_t i = 0; i < size; ++i) {
        Transaction element;
        element.deserialize(ifs);
        stack.push(element);
    }
}



// Serialize a list
void User::serializeList(std::ofstream& ofs,const std::list<Message>& myList) const {
    size_t size = myList.size();
    ofs.write(reinterpret_cast<const char*>(&size), sizeof(size));
    for (const auto& element : myList) {
        element.serialize(ofs);
    }
}

// Deserialize a list
void User::deserializeList(std::ifstream& ifs, std::list<Message>& myList)const {
    size_t size;
    ifs.read(reinterpret_cast<char*>(&size), sizeof(size));
    for (size_t i = 0; i < size; ++i) {
        Message element;
        element.deserialize(ifs);
        myList.push_back(element);
    }
}


// Serialize a vector
void User:: serializeVector(std::ofstream& ofs,const std::vector<Card>& vec) const{
    size_t size = vec.size();
    ofs.write(reinterpret_cast<const char*>(&size), sizeof(size));
    for (const auto& element : vec) {
        element.serialize(ofs);
    }
}

// Deserialize a vector
void User:: deserializeVector(std::ifstream& ifs, std::vector<Card>& vec)const {
    size_t size;
    ifs.read(reinterpret_cast<char*>(&size), sizeof(size));
    vec.resize(size);
    for (size_t i = 0; i < size; ++i) {
        vec[i].deserialize(ifs);
    }
}



void User:: serialize(std::ofstream& ofs) const {
    ofs.write(reinterpret_cast<const char*>(&Pin), sizeof(Pin));
    ofs.write(reinterpret_cast<const char*>(&balance), sizeof(balance));
    serializeString(ofs, user_name);
    serializeString(ofs, password);
    serializeString(ofs, email);
    ofs.write(reinterpret_cast<const char*>(&birth_date), sizeof(birth_date));
    serializeString(ofs, phone_number);
    serializeString(ofs, address_account);
    serializeString(ofs, status);

    // Serialize transactions
    serializeStack(ofs,transactions);

    // Serialize cards
    serializeVector(ofs,cards);

    // Serialize messages
    serializeList(ofs,messages);
}

// Deserialization method for User
void User::deserialize(std::ifstream& ifs) {
    ifs.read(reinterpret_cast<char*>(&Pin), sizeof(Pin));
    ifs.read(reinterpret_cast<char*>(&balance), sizeof(balance));
    user_name = deserializeString(ifs);
    password = deserializeString(ifs);
    email = deserializeString(ifs);
    ifs.read(reinterpret_cast<char*>(&birth_date), sizeof(birth_date));
    phone_number = deserializeString(ifs);
    address_account = deserializeString(ifs);
    status = deserializeString(ifs);

    // Deserialize transactions
    deserializeStack(ifs, transactions);

    //// deserialize cards
    deserializeVector(ifs, cards);

    //// deserialize messages
    deserializeList(ifs, messages);
}
