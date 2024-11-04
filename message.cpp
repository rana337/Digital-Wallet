#include "message.h"

Message::Message()
{
    this->from = "/////";
    this->message = "*****";
    this->status = "-----";
    this->amount = 0;
}

void Message::serializeString(std::ofstream& ofs, const std::string& str) const {
    size_t size = str.size();
    ofs.write(reinterpret_cast<const char*>(&size), sizeof(size));
    ofs.write(str.c_str(), size);
}

// Helper function to deserialize strings
std::string Message:: deserializeString(std::ifstream& ifs) const {
    size_t size;
    ifs.read(reinterpret_cast<char*>(&size), sizeof(size));
    std::string str(size, '\0');
    ifs.read(&str[0], size);
    return str;
}


// Serialization method for Message
void Message::serialize(std::ofstream& ofs) const {
    serializeString(ofs, from);
    serializeString(ofs, status);
    serializeString(ofs, message);
    ofs.write(reinterpret_cast<const char*>(&amount), sizeof(amount));
}

// Deserialization method for Message
void Message::deserialize(std::ifstream& ifs) {
    from = deserializeString(ifs);
    status = deserializeString(ifs);
    message = deserializeString(ifs);
    ifs.read(reinterpret_cast<char*>(&amount), sizeof(amount));
}
