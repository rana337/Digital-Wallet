#include "transaction.h"
#include "QDate"
#include "QTime"

Transaction::Transaction()
{
    this->from = "-----";
    this->to = "-----";
    this->status = SUCCESSED;
    this->transaction_kind = SENDER;
    this->amount= 0;
}
Transaction::Transaction(string sender_user_name,string receiver_user_name,float amount,kind k)
{
    this->from = sender_user_name;
    this->to = receiver_user_name;
    this->status = SUCCESSED;
    this->transaction_kind=k;
    this->amount=amount;
    this->date.day= QDate::currentDate().day();
    this->date.month=QDate::currentDate().month();
    this->date.year= QDate::currentDate().year();
    this->date.hour= QTime:: currentTime().hour();
    this->date.minutes=QTime:: currentTime().minute();
    this->date.seconds=QTime:: currentTime().hour();
}




enum status Transaction:: convertStatusToEnum(string transaction_status)
{
    if(transaction_status=="SUCCESSED")
        return SUCCESSED;
    else
        return FAILED;
}



enum kind Transaction:: convertKindToEnum(string transaction_kind)
{
    if(transaction_kind=="SENDER")
        return SENDER;
    else if(transaction_kind=="RECIVER")
        return RECIVER;
}



template<typename Enum>
string Transaction::enumToString(Enum value) const
{
    static_assert(std::is_enum<Enum>::value, "EnumToString requires an enum type");
    switch(value) {
    case Enum::SUCCESSED:
        return "SUCCESSED";
    case Enum::FAILED:
        return "FAILED";
    default:
        return "Unknown";
    }
}

template<typename Enum>
string Transaction::enumToString2(Enum value) const
{
    static_assert(std::is_enum<Enum>::value, "EnumToString requires an enum type");
    switch(value) {
    case Enum::SENDER:
        return "SENDER";
    case Enum::RECIVER:
        return "RECIVER";
    default:
        return "Unknown";
    }
}



// Helper function to serialize strings
void Transaction:: serializeString(std::ofstream& ofs, const std::string& str) const {
    size_t size = str.size();
    ofs.write(reinterpret_cast<const char*>(&size), sizeof(size));
    ofs.write(str.c_str(), size);
}

// Helper function to deserialize strings
std::string Transaction:: deserializeString(std::ifstream& ifs) const {
    size_t size;
    ifs.read(reinterpret_cast<char*>(&size), sizeof(size));
    std::string str(size, '\0');
    ifs.read(&str[0], size);
    return str;
}


// Serialization method for Transaction
void Transaction:: serialize(std::ofstream& ofs) const {
    serializeString(ofs, from);
    serializeString(ofs, to);
    ofs.write(reinterpret_cast<const char*>(&date), sizeof(date));
    serializeString(ofs, enumToString(status));
    serializeString(ofs,enumToString2(transaction_kind));
    ofs.write(reinterpret_cast<const char*>(&amount), sizeof(amount));
}

// Deserialization method for Transaction
void Transaction::deserialize(std::ifstream& ifs) {
    from = deserializeString(ifs);
    to = deserializeString(ifs);
    ifs.read(reinterpret_cast<char*>(&date), sizeof(date));
    status = convertStatusToEnum(deserializeString(ifs));
    transaction_kind = convertKindToEnum(deserializeString(ifs));
    ifs.read(reinterpret_cast<char*>(&amount), sizeof(amount));
}
