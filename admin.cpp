#include "Admin.h"
#include "User.h"

unordered_map<string, User> Admin::users;
unordered_map<string, string> Admin::phones;
unordered_map<string, string> Admin::cards;

void Admin::addUser(User new_user)
{
    users[new_user.user_name] = new_user;
    phones[new_user.phone_number]=new_user.user_name;
}

User Admin::getUserData(string user_name, string password)
{
    User returned_user;
    unordered_map<string, User>::iterator it;
    it = users.find(user_name);
    if (it != users.end())
    {
        if(it->second.password==returned_user.passwordEnc( password))
            returned_user = it->second;
    }

    return returned_user;
}

int Admin::check_signin_data(string user,string phone)
{
    if(users.find(user)!=users.end())
    {
        return 1;
    }
    else if(phones.find(phone)!=phones.end())
    {
        return 2;
    }
    else
    {
        return 3;
    }
}

void Admin::dis()
{
    unordered_map<string, User>::iterator it;
    unordered_map<string, string>::iterator it1;
    for(it =users.begin();it!=users.end();it++)
    {
        //cout<<it->second.address_account<<" "<<it->second.phone_number<<" "<<it->second.user_name<<" "<<it->second.birth_date.day<<endl;
        //if(it->second==nullptr)
        if(it->second.user_name=="")
        cout<<it->first<<" "<<it->second.user_name<<"*"<<it->second.balance<<"\n";
        else
            cout<<it->first<<" "<<it->second.user_name<<"\n";
    }
    cout<<"======================================\n";
    // for(it1 =phones.begin();it1!=phones.end();it1++)
    // {
    //     cout<<it1->first<<" "<<it1->second<<endl;
    // }
}

Admin::Admin()
{
    userName = "AdminAhmed2003";
    password = "123456789ahmed2003";
    Date date;
    date.day = 5;
    date.month = 5;
    date.year = 2004;
    //	string userName, string password, Date birthDate, string email, string phoneNumber, int pin
    // User* user1 = new User("Ahmed1", "123456789", date, "ah@gmail.com", "01112343263", 147852);
    // User* user2 = new User("Lamiaa2","987654321", date, "lm@gmail.com", "01121178539", 147852);
    // User* user3 = new User("Ahmed3", "123456789", date, "ah@gmail.com", "01112343263", 147852);
    // User* user4 = new User("Lamiaa4","987654321", date, "lm@gmail.com", "01121178539", 147852);
    // User* user5 = new User("Ahmed5", "123456789", date, "ah@gmail.com", "01112343263", 147852);
    // User* user6 = new User("Lamiaa6","987654321", date, "lm@gmail.com", "01121178539", 147852);
    // User* user7 = new User("Ahmed7", "123456789", date, "ah@gmail.com", "01112343263", 147852);
    // User* user8 = new User("Lamiaa8","987654321", date, "lm@gmail.com", "01121178539", 147852);
    // User* user9 = new User("Ahmed9", "123456789", date, "ah@gmail.com", "01112343263", 147852);
    // User* user10 = new User("Lamiaa10","987654321", date, "lm@gmail.com", "01121178539", 147852);
    // User* user11 = new User("Ahmed11", "123456789", date, "ah@gmail.com", "01112343263", 147852);
    // User* user12 = new User("Lamiaa12","987654321", date, "lm@gmail.com", "01121178539", 147852);
    // User* user13 = new User("Ahmed13", "123456789", date, "ah@gmail.com", "01112343263", 147852);
    // User* user14 = new User("Lamiaa14","987654321", date, "lm@gmail.com", "01121178539", 147852);
    // User* user15 = new User("Ahmed15", "123456789", date, "ah@gmail.com", "01112343263", 147852);
    // User* user16 = new User("Lamiaa16","987654321", date, "lm@gmail.com", "01121178539", 147852);
    // User* user17 = new User("Ahmed17", "123456789", date, "ah@gmail.com", "01112343263", 147852);
    // User* user18 = new User("Lamiaa18","987654321", date, "lm@gmail.com", "01121178539", 147852);
    // User* user19 = new User("Ahmed19", "123456789", date, "ah@gmail.com", "01112343263", 147852);
    // User* user20 = new User("Lamiaa20","987654321", date, "lm@gmail.com", "01121178539", 147852);
    // User* user21 = new User("Ahmed21", "123456789", date, "ah@gmail.com", "01112343263", 147852);
    // User* user22 = new User("Lamiaa22","987654321", date, "lm@gmail.com", "01121178539", 147852);
    // User* user23 = new User("Ahmed23", "123456789", date, "ah@gmail.com", "01112343263", 147852);
    // User* user24 = new User("Lamiaa24","987654321", date, "lm@gmail.com", "01121178539", 147852);
    // user2->status="Suspended";
    // users[user1->user_name] = *user1;
    // users[user2->user_name] = *user2;
    // users[user3->user_name] = *user3;
    // users[user4->user_name] = *user4;
    // users[user5->user_name] = *user5;
    // users[user6->user_name] = *user6;
    // users[user7->user_name] = *user7;
    // users[user8->user_name] = *user8;
    // users[user9->user_name] = *user9;
    // users[user10->user_name] = *user10;
    // users[user11->user_name] = *user11;
    // users[user12->user_name] = *user12;
    // users[user13->user_name] = *user13;
    // users[user14->user_name] = *user14;
    // users[user15->user_name] = *user15;
    // users[user16->user_name] = *user16;
    // users[user17->user_name] = *user17;
    // users[user18->user_name] = *user18;
    // users[user19->user_name] = *user19;
    // users[user20->user_name] = *user20;
    // users[user21->user_name] = *user21;
    // users[user22->user_name] = *user22;
    // users[user23->user_name] = *user23;
    // users[user24->user_name] = *user24;


    // phones["01121178539"]="Lamiaa2004";
    // phones["01112343263"]="Ahmed2003";


    // Card  card1 ;
    // card1.card_ATM_pin = 0000;
    // card1.card_balance = 100;
    // card1.card_holder_name = "Maysoon";
    // card1.card_number = "7894 4561 1230 7410";
    // Card card2 ;
    // card2.card_ATM_pin = 0000;
    // card2.card_balance = 100;
    // card2.card_holder_name = "Rana";
    // card2.card_number = "7894 4561 1230 7410";
    // Card card3 ;
    // card3.card_ATM_pin = 0000;
    // card3.card_balance = 100;
    // card3.card_holder_name = "Lamiaa";
    // card3.card_number = "7894 4561 1230 7410";
    // Card card4 ;
    // card4.card_ATM_pin = 0000;
    // card4.card_balance = 100;
    // card4.card_holder_name = "Ahmed";
    // card4.card_number = "7894 4561 1230 7410";
    // users[user1->user_name].cards.push_back(card1);
    // users[user1->user_name].cards.push_back(card2);
    // users[user1->user_name].cards.push_back(card3);
    // users[user1->user_name].cards.push_back(card4);

    // Transaction newTransaction1;
    // newTransaction1.from = "Maysoon Name";
    // newTransaction1.to = "cairo Name";
    // newTransaction1.date.day = 15;
    // newTransaction1.date.month = 1;
    // newTransaction1.date.year = 2005;
    // newTransaction1.status = SUCCESSED;
    // newTransaction1.transaction_kind = SENDER;
    // newTransaction1.amount = 1000000;
    // users[user1->user_name].transactions.push(newTransaction1);

    // Transaction newTransaction2;
    // newTransaction2.from = "Lamiaa Name";
    // newTransaction2.to = "cairo Name";
    // newTransaction2.date.day = 5;
    // newTransaction2.date.month = 5;
    // newTransaction2.date.year = 2004;
    // newTransaction2.status = SUCCESSED;
    // newTransaction2.transaction_kind = SENDER;
    // newTransaction2.amount = 1000000;
    // users[user1->user_name].transactions.push(newTransaction2);

    // Transaction newTransaction3;
    // newTransaction3.from = "Rana Name";
    // newTransaction3.to = "cairo Name";
    // newTransaction3.date.day = 12;
    // newTransaction3.date.month = 6;
    // newTransaction3.date.year = 2003;
    // newTransaction3.status = SUCCESSED;
    // newTransaction3.transaction_kind = SENDER;
    // newTransaction3.amount = 1000000;
    // users[user1->user_name].transactions.push(newTransaction3);

    // Transaction newTransaction4;
    // newTransaction4.from = "Ahmed Name";
    // newTransaction4.to = "cairo Name";
    // newTransaction4.date.day = 21;
    // newTransaction4.date.month = 5;
    // newTransaction4.date.year = 2003;
    // newTransaction4.status = SUCCESSED;
    // newTransaction4.transaction_kind = RECIVER;
    // newTransaction4.amount = 1;
    // users[user1->user_name].transactions.push(newTransaction4);

    // Transaction newTransaction5;
    // newTransaction5.from = "Mostafa Name";
    // newTransaction5.to = "cairo Name";
    // newTransaction5.date.day = 24;
    // newTransaction5.date.month = 12;
    // newTransaction5.date.year = 2004;
    // newTransaction5.status = SUCCESSED;
    // newTransaction5.transaction_kind = RECIVER;
    // newTransaction5.amount = 1000;
    // users[user1->user_name].transactions.push(newTransaction5);

    // Transaction newTransaction6;
    // newTransaction6.from = "Hossam Name";
    // newTransaction6.to = "cairo Name";
    // newTransaction6.date.day = 5;
    // newTransaction6.date.month = 8;
    // newTransaction6.date.year = 2004;
    // newTransaction6.status = SUCCESSED;
    // newTransaction6.transaction_kind = RECIVER;
    // newTransaction6.amount = 1000;
    // users[user1->user_name].transactions.push(newTransaction6);

    // Transaction newTransaction7;
    // newTransaction7.from = "Maysoon Name";
    // newTransaction7.to = "cairo Name";
    // newTransaction7.date.day = 5;
    // newTransaction7.date.month = 8;
    // newTransaction7.date.year = 2004;
    // newTransaction7.status = SUCCESSED;
    // newTransaction7.transaction_kind = SENDER;
    // newTransaction7.amount = 1000;
    // users[user1->user_name].transactions.push(newTransaction7);


    // Message message1;
    // message1.from = "Maysoon";
    // message1.message = "Has requested";
    // message1.amount = 100;
    // message1.status = "Waiting";
    // users[user1->user_name].messages.push_back(message1);

    // Message message2;
    // message2.from = "Lamiaa";
    // message2.message = "Has requested";
    // message2.amount = 100;
    // message2.status = "Waiting";
    // users[user1->user_name].messages.push_back(message2);


    // Message message3;
    // message3.from = "Rana";
    // message3.message = "Has requested";
    // message3.amount = 100;
    // message3.status = "Waiting";
    // users[user1->user_name].messages.push_back(message3);
}

void Admin::SendMoney(float amount,float fee, string sender_card_num, string receiver_card_num,string sender_user_name, string receiver_user_name){

    users[sender_user_name].DeductMoney(sender_card_num,amount+fee);
    users[sender_user_name].StorTransaction(sender_user_name,receiver_user_name,amount,SENDER);


    users[receiver_user_name].AddMoney(receiver_card_num,amount);
    users[receiver_user_name].StorTransaction(sender_user_name,receiver_user_name,amount,RECIVER);

}






