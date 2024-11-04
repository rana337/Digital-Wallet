#include "card.h"

Card::Card()
{
    this->card_number = "****************";
    this->card_holder_name = "-----";
    this->card_ATM_pin = 0000;
    this->card_balance = 0;
}

// Helper function to serialize strings
void Card::serializeString(std::ofstream& ofs, const std::string& str) const {
    size_t size = str.size();
    ofs.write(reinterpret_cast<const char*>(&size), sizeof(size));
    ofs.write(str.c_str(), size);
}

// Helper function to deserialize strings
std::string Card::deserializeString(std::ifstream& ifs) const {
    size_t size;
    ifs.read(reinterpret_cast<char*>(&size), sizeof(size));
    std::string str(size, '\0');
    ifs.read(&str[0], size);
    return str;
}



// Serialization method for Card
void Card::serialize(std::ofstream& ofs) const {
    serializeString(ofs, card_number);
    serializeString(ofs, card_holder_name);
    ofs.write(reinterpret_cast<const char*>(&card_ATM_pin), sizeof(card_ATM_pin));
    ofs.write(reinterpret_cast<const char*>(&card_balance), sizeof(card_balance));
}

// Deserialization method for Card
void Card::deserialize(std::ifstream& ifs) {
    card_number = deserializeString(ifs);
    card_holder_name = deserializeString(ifs);
    ifs.read(reinterpret_cast<char*>(&card_ATM_pin), sizeof(card_ATM_pin));
    ifs.read(reinterpret_cast<char*>(&card_balance), sizeof(card_balance));
}
