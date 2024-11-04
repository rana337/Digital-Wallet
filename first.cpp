#include "first.h"
#include "ui_first.h"
#include "admin.h"
#include <QMessageBox>
#include <QScrollArea>
#include <stack>
#include <cstdlib> // For srand() & rand()
#include <ctime>
#include <QVBoxLayout>
#include <QInputDialog>
#include <iterator>
#include<QTimer>
#include <cstdio>


First::First(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::First)
{

    ui->setupUi(this);
    // displayBalance();
    // updateUIWithAccounts();
    // updateUIWithMessages();
    // updateUIWithTransactions();
    // displaytransactioninPageData();

    //Deserialization(read data from files)
    std::ifstream inputFile("info.bin", std::ios::binary);
    if (!inputFile.is_open()) {
        std::cerr << "Error opening file for reading." << std::endl;
    }

    std::unordered_map<std::string, User> loadedUsers;
    while (!inputFile.eof()) {
        std::string key;
        std::getline(inputFile, key, '\0'); // Read key
        if (key.empty()) break;
        User user;
        user.deserialize(inputFile); // Read value
        ad1.users[key] = user;

        ad1.phones[user.phone_number]=key;
        for(int i=0;i<ad1.users[key].cards.size();i++)
        {
            ad1.cards[ad1.users[key].cards[i].card_number]=key;
        }
    }
    inputFile.close();

    if (std::remove("info.bin") != 0) {
        std::cerr << "Error deleting file" << std::endl;
    }
    else {
        std::cout << "File successfully deleted" << std::endl;
    }
    cout<<ad1.cards.size()<<endl;
    cout<<ad1.phones.size()<<endl;
}

First::~First()
{
    //sending data to files
    std::ofstream outputFile("info.bin", std::ios::binary);
    if (!outputFile.is_open()) {
        std::cerr << "Error opening file for writing." << std::endl;
    }

    for (const auto& pair :ad1.users) {
        if(pair.second.user_name=="")
            continue;
        outputFile.write(pair.first.c_str(), pair.first.size() + 1); // Write key
        pair.second.serialize(outputFile); // Write value
    }
    outputFile.close();
    delete ui;
}

bool First::askForPin() {
    QInputDialog inputDialog;
    inputDialog.setStyleSheet("QInputDialog { border: 2px solid #7A69E9; background-color: black; color: white; font-size: 20px; }"
                              "QLineEdit { color: white; font-size: 20px; }"
                              "QPushButton { background-color: #7A69E9; color: white; }");

    inputDialog.resize(500, 200); // Adjusted size for better visibility
    bool ok;

    QString pinText = inputDialog.getText(this, "Enter PIN", "PIN:", QLineEdit::Password, "", &ok);

    if (ok && !pinText.isEmpty()) {
        int enteredPin = pinText.toInt();
        int storedPin = ad1.users[registered_user].Pin;

        if (enteredPin == storedPin) {
            // PINs match
            return true;
        } else {
            QMessageBox messageBox;
            messageBox.setStyleSheet("QMessageBox { border: 2px solid #7A69E9; background-color: black; color: white; font-size: 20px; }"
                                     "QPushButton { background-color: #7A69E9; color: white; }"
                                     "QLabel { color: red; }");
            messageBox.setIcon(QMessageBox::Warning);
            messageBox.setWindowTitle("Warning");
            messageBox.setText("PINs don't match.");
            messageBox.exec();
        }
    }
   return false;
}

bool First :: logOut (){

    QMessageBox msgBox;
    msgBox.setWindowTitle("Logout");
    msgBox.setText("Are you sure you want to log out?");
    msgBox.setStandardButtons(QMessageBox::Yes      |        QMessageBox::No);
    msgBox.setGeometry(1000, 500, 800, 1000);

    msgBox.setStyleSheet("QMessageBox { border: 2px solid #7A69E9; background-color: black; font-size: 20px; }"
                         "QMessageBox QLabel { color: white; }" // Set text color to white
                         "QPushButton { background-color: #7A69E9; color: white; font-size: 20px; align: ceneter;}");

    int reply = msgBox.exec();

    if (reply == QMessageBox::Yes) {
        ui->lineEdit_user_name8->setText("");
        ui->lineEdit_password8->setText("");
        return true;
    }

    return false;
}

//************************SEND MONEY PAGES && BUTTONS****************************

//Transaction by Card Number
void First::on_pushButton_8_clicked()
{
    ui->label_142->setText("");
    ui->label_146->setText("");
    ui->label_141->setText("");

    string receiver_card_num =ui->lineEdit_7->text().toStdString();
    float amount =ui->lineEdit_12->text().toInt();
    string sender_card_num =ui->comboBox->currentText().toStdString();
    string sender_user_name = registered_user;
    string receiver_user_name;
    if(ad1.cards.find(receiver_card_num)!=ad1.cards.end())
        receiver_user_name = ad1.cards[receiver_card_num];
    else {
        receiver_user_name ="unknown";
        receiver_card_num="-1";
    }
    if(receiver_card_num =="")
        ui->label_141->setText("please enteer the card Number frist");
    else if(ui->lineEdit_7->text().toStdString().length()!=16)
        ui->label_141->setText("please check the card Number again");
    else if(ui->lineEdit_12->text()=="")
        ui->label_146->setText("please enteer the amount frist");
    else{

        float fee =amount*0.025;
        if(fee<2)
            fee=2;
        else if(fee>100)
            fee=100;

        if(sender_card_num=="Easy Pay Wallet")
        {
            if(ad1.users[registered_user].balance<amount+fee)
                ui->label_142->setText("the balance is not enough");
            else
            {
                ad1.SendMoney(amount,fee,sender_card_num,receiver_card_num,sender_user_name,receiver_user_name);
                updateUIWithTransactions();
                displayBalance();
                deleteWidget(delete_transactions_history);
                displaytransactioninPageData();
                //displayDatatransactions();
                QMessageBox::information(this,"Transaction Done","The Transaction has been made succesfully");

                QTimer::singleShot(1500, this, [=]() {

                    ui->stackedWidget->setCurrentIndex(2);
                });

            }
        }
        else
        {
            int i,size;
            size=ad1.users[registered_user].cards.capacity();
            for ( i = 0; i < size; ++i) {
                if(sender_card_num == ad1.users[registered_user].cards[i].card_number)
                    break;
            }
            if(ad1.users[sender_user_name].cards[i].card_balance<amount+fee)
                ui->label_142->setText("the balance of this card is not enough");
            else
            {
                ad1.SendMoney(amount,fee,sender_card_num,receiver_card_num,sender_user_name,receiver_user_name);
                updateUIWithTransactions();
                displayBalance();
                deleteWidget(delete_transactions_history);
                displaytransactioninPageData();
                //displayDatatransactions();
                QMessageBox::information(this,"Transaction Done","The Transaction has been made succesfully");

                QTimer::singleShot(1500, this, [=]() {

                    ui->stackedWidget->setCurrentIndex(2);
                });
            }
        }
    }
}
void First::on_lineEdit_12_textChanged(const QString &arg1)
{
    float amount =ui->lineEdit_12->text().toInt();
    if(ui->lineEdit_12->text()==""){
        ui->label_149->setText("Transaction fee : " );
        ui->label_150->setText("Total : " );
    }
    else{
        float fee =amount*0.025;
        if(fee<2)
            fee=2;
        else if(fee>100)
            fee=100;
        ui->label_149->setText("Transaction fee : " + QString::number(fee));
        ui->label_150->setText("Total : " + QString::number((fee+amount)));
    }
}

//Transaction by Mobile Number
void First::on_pushButton_101_clicked()
{
    ui->label_138->setText("");
    ui->label_87->setText("");
    ui->label_148->setText("");

    string receiver_Mobile_Num =ui->lineEdit_13->text().toStdString();
    float amount =ui->lineEdit_17->text().toInt();
    string sender_card_num =ui->comboBox_2->currentText().toStdString();
    string sender_user_name = registered_user;
    string receiver_user_name ;
    User receiver ;
    if(ad1.phones.find(receiver_Mobile_Num)==ad1.phones.end()){
        ui->label_87->setText("this Mobile Number was not found ");
    }
    else{
        receiver_user_name = ad1.phones[receiver_Mobile_Num];
        if(receiver_Mobile_Num=="")
            ui->label_87->setText("please enteer the Mobile Number frist");
        else if(receiver_Mobile_Num.length()!=11)
            ui->label_87->setText("please check the Mobile Number again");
        else if(ui->lineEdit_17->text()=="")
            ui->label_148->setText("please enteer the amount frist");
        else{
            float fee =amount*0.025;
            if(fee<2)
                fee=2;
            else if(fee>100)
                fee=100;

            if(sender_card_num=="Easy Pay Wallet")
            {
                if(ad1.users[registered_user].balance<amount+fee)
                    ui->label_138->setText("the balance is not enough");
                else
                {
                    ad1.SendMoney(amount,fee,sender_card_num,"Easy Pay Wallet",sender_user_name,receiver_user_name);
                    updateUIWithTransactions();
                    displayBalance();
                    deleteWidget(delete_transactions_history);
                    displaytransactioninPageData();
                    //displayDatatransactions();
                    //ad1.users[registered_user].display();
                    QMessageBox::information(this,"Transaction Done","The Transaction has been made succesfully");

                    QTimer::singleShot(1500, this, [=]() {

                        ui->stackedWidget->setCurrentIndex(2);
                    });
                }
            }
            else
            {
                int i,size;
                size=ad1.users[registered_user].cards.capacity();
                for ( i = 0; i < size; ++i) {
                    if(sender_card_num == ad1.users[registered_user].cards[i].card_number)
                        break;
                }

                if(ad1.users[sender_user_name].cards[i].card_balance<amount+fee)
                    ui->label_138->setText("the balance of this card is not enough");
                else
                {
                    ad1.SendMoney(amount,fee,sender_card_num,"Easy Pay Wallet",sender_user_name,receiver_user_name);
                    updateUIWithTransactions();
                    displayBalance();
                    deleteWidget(delete_transactions_history);
                    displaytransactioninPageData();
                    //displayDatatransactions();

                    QMessageBox::information(this,"Transaction Done","The Transaction has been made succesfully");

                    QTimer::singleShot(1500, this, [=]() {

                        ui->stackedWidget->setCurrentIndex(2);
                    });
                }
            }
        }
    }
}
void First::on_lineEdit_17_textChanged(const QString &arg1)
{
    float amount =ui->lineEdit_17->text().toInt();
    if(ui->lineEdit_17->text()==""){
        ui->label_154->setText("Transaction fee : " );
        ui->label_153->setText("Total : " );
    }else{
        float fee =amount*0.025;
        if(fee<2)
            fee=2;
        else if(fee>100)
            fee=100;
        ui->label_154->setText("Transaction fee : " + QString::number(fee));
        ui->label_153->setText("Total : " + QString::number((fee+amount)));
    }
}

//Transaction by address
void First::on_pushButton_100_clicked()
{
    ui->label_140->setText("");
    ui->label_139->setText("");
    ui->label_147->setText("");

    string receiver_user_name =ui->lineEdit_15->text().toStdString();
    float amount =ui->lineEdit_16->text().toInt();
    string sender_card_num =ui->comboBox_3->currentText().toStdString();
    string sender_user_name = registered_user;


    if(receiver_user_name=="")
        ui->label_139->setText("please enteer the address frist");
    else if(ui->lineEdit_16->text()=="")
        ui->label_147->setText("please enteer the amount frist");
    else if(ad1.users.find(receiver_user_name)==ad1.users.end()){
        ui->label_139->setText("this address was not found ");
    }
    else
    {
        float fee =amount*0.025;
        if(fee<2)
            fee=2;
        else if(fee>100)
            fee=100;

        if(sender_card_num=="Easy Pay Wallet")
        {
            if(ad1.users[registered_user].balance<amount+fee)
                ui->label_140->setText("the balance is not enough");
            else
            {
                ad1.SendMoney(amount,fee,sender_card_num,"Easy Pay Wallet",sender_user_name,receiver_user_name);

                deleteWidget(delete_transactions_history);
                displaytransactioninPageData();
                updateUIWithTransactions();
                displayBalance();
                //ad1.users[registered_user].display();
                QMessageBox::information(this,"Transaction Done","The Transaction has been made succesfully");

                QTimer::singleShot(1500, this, [=]() {

                    ui->stackedWidget->setCurrentIndex(2);
                });
            }
        }

        else
        {
            int i,size;
            size=ad1.users[registered_user].cards.capacity();
            for ( i = 0; i < size; ++i) {
                if(sender_card_num == ad1.users[registered_user].cards[i].card_number)
                    break;
            }

            if(ad1.users[sender_user_name].cards[i].card_balance<amount+fee)
                ui->label_140->setText("the balance of this card is not enough");
            else
            {
                ad1.SendMoney(amount,fee,sender_card_num,"Easy Pay Wallet",sender_user_name,receiver_user_name);

                deleteWidget(delete_transactions_history);
                displaytransactioninPageData();
                updateUIWithTransactions();
                displayBalance();
                //ad1.users[registered_user].display();
                QMessageBox::information(this,"Transaction Done","The Transaction has been made succesfully");

                QTimer::singleShot(1500, this, [=]() {

                    ui->stackedWidget->setCurrentIndex(2);
                });
            }
        }

    }
}
void First::on_lineEdit_16_textChanged(const QString &arg1)
{
    float amount =ui->lineEdit_16->text().toInt();
    if(ui->lineEdit_16->text()==""){
        ui->label_152->setText("Transaction fee : " );
        ui->label_151->setText("Total : " );
    }
    else{
        float fee =amount*0.025;
        if(fee<2)
            fee=2;
        else if(fee>100)
            fee=100;
        ui->label_152->setText("Transaction fee : " + QString::number(fee));
        ui->label_151->setText("Total : " + QString::number((fee+amount)));
    }

}

//go to send money buttons
void First::on_pushButton_119_clicked()
{
    goToSendMoneyPage();
}
void First::on_pushButton_37_clicked()
{
    goToSendMoneyPage();
}
void First::on_pushButton_53_clicked()
{
    goToSendMoneyPage();
}
void First::on_pushButton_21_clicked()
{
    goToSendMoneyPage();
}
void First::on_pushButton_69_clicked()
{
    goToSendMoneyPage();
}
void First::on_pushButton_86_clicked()
{
    goToSendMoneyPage();
}
void First::on_pushButton_136_clicked()
{
    goToSendMoneyPage();
}

void First::goToSendMoneyPage(){
    if(askForPin()){
        ui->pushButton_102->setStyleSheet("color: rgb(119, 70, 234);background-color: rgb(31, 31, 31);border-radius:15px;");
        ui->pushButton_104->setStyleSheet("background-color: rgb(31, 31, 31);color:white; border-radius:15px;");
        ui->pushButton_103->setStyleSheet("background-color: rgb(31, 31, 31);color:white; border-radius:15px;");

        ui->lineEdit_13->setText("");
        ui->lineEdit_17->setText("");
        int size =Admin::users[registered_user].cards.size();

        ui->comboBox->clear();
        ui->comboBox_2->clear();
        ui->comboBox_3->clear();

        ui->comboBox->addItem("Easy Pay Wallet");
        ui->comboBox_2->addItem("Easy Pay Wallet");
        ui->comboBox_3->addItem("Easy Pay Wallet");
        for (int i = 0; i < size; ++i) {
            QString tmp =QString::fromStdString(Admin::users[registered_user].cards[i].card_number);
            ui->comboBox->addItem(tmp);
            ui->comboBox_2->addItem(tmp);
            ui->comboBox_3->addItem(tmp);
        }
        cout<<ui->comboBox->count()<<"Alaraab";
        ui->label_100->setText(QString::fromStdString(registered_user));
        ui->pushButton_119->setStyleSheet("color: rgb(119, 70, 234);background-color: rgb(31, 31, 31);border-radius:15px;");
        ui->pushButton_123->setStyleSheet("background-color: rgb(31, 31, 31);color:white; border-radius:15px;");
        ui->pushButton_102->setStyleSheet("color: rgb(119, 70, 234);background-color: rgb(31, 31, 31);border-radius:15px;");
        ui->stackedWidget->setCurrentIndex(10);
        ui->stackedWidget_2->setCurrentIndex(0);
        ui->stackedWidget_3->setCurrentIndex(2);

        ui->label_100->setText(QString::fromStdString(registered_user));
        ui->label_100->setAlignment(Qt::AlignRight);
        QFont font = ui->label_100->font();
        font.setPointSize(18);
        ui->label_100->setFont(font);
    }
}
//go to send money page buttons

//send money by Mobile Number page button
void First::on_pushButton_102_clicked()
{
    ui->stackedWidget_3->setCurrentIndex(2);
    ui->lineEdit_13->setText("");
    ui->lineEdit_17->setText("");
    ui->label_87->setText("");
    ui->label_138->setText("");
    ui->label_148->setText("");
    ui->pushButton_102->setStyleSheet("color: rgb(119, 70, 234);background-color: rgb(31, 31, 31);border-radius:15px;");
    ui->pushButton_104->setStyleSheet("background-color: rgb(31, 31, 31);color:white; border-radius:15px;");
    ui->pushButton_103->setStyleSheet("background-color: rgb(31, 31, 31);color:white; border-radius:15px;");
}

//send money by Address page button
void First::on_pushButton_103_clicked()
{
    ui->stackedWidget_3->setCurrentIndex(1);
    ui->lineEdit_15->setText("");
    ui->lineEdit_16->setText("");
    ui->label_139->setText("");
    ui->label_140->setText("");
    ui->label_147->setText("");
    ui->pushButton_103->setStyleSheet("color: rgb(119, 70, 234);background-color: rgb(31, 31, 31);border-radius:15px;");
    ui->pushButton_102->setStyleSheet("background-color: rgb(31, 31, 31);color:white; border-radius:15px;");
    ui->pushButton_104->setStyleSheet("background-color: rgb(31, 31, 31);color:white; border-radius:15px;");
}

//send money by Card Number page button
void First::on_pushButton_104_clicked()
{
    ui->stackedWidget_3->setCurrentIndex(0);
    ui->lineEdit_12->setText("");
    ui->lineEdit_7->setText("");
    ui->label_141->setText("");
    ui->label_142->setText("");
    ui->label_146->setText("");
    ui->pushButton_104->setStyleSheet("color: rgb(119, 70, 234);background-color: rgb(31, 31, 31);border-radius:15px;");
    ui->pushButton_102->setStyleSheet("background-color: rgb(31, 31, 31);color:white; border-radius:15px;");
    ui->pushButton_103->setStyleSheet("background-color: rgb(31, 31, 31);color:white; border-radius:15px;");
}

// REQUEST MONEY PAGES && BUTTONS

//*******************************button to go to request money****************************

//Go to payment address in request money
void First::on_pushButton_108_clicked()
{
    ui->stackedWidget_4->setCurrentIndex(1);
    ui->lineEdit_18->setText("");
    ui->lineEdit_19->setText("");
    ui->label_155->setText("");

    ui->pushButton_108->setStyleSheet("color: rgb(119, 70, 234);background-color: rgb(31, 31, 31);border-radius:15px;");
    ui->pushButton_107->setStyleSheet("background-color: rgb(31, 31, 31);color:white; border-radius:15px;");
}

//Go to phone number in request money
void First::on_pushButton_107_clicked()
{
    ui->stackedWidget_4->setCurrentIndex(2);
    ui->lineEdit_21->setText("");
    ui->lineEdit_20->setText("");
    ui->label_156->setText("");
    ui->label_157->setText("");
    ui->pushButton_107->setStyleSheet("color: rgb(119, 70, 234);background-color: rgb(31, 31, 31);border-radius:15px;");
    ui->pushButton_108->setStyleSheet("background-color: rgb(31, 31, 31);color:white; border-radius:15px;");
}

//Request money using phone number
void First::on_pushButton_106_clicked()
{
    string phoneNumber;
    float ammount;

    QRegularExpression phoneNumberRe("^0[01][0125][0-9]{8}$");
    QString number = ui->lineEdit_20->text();

    QRegularExpression ammountRe("^\\d*\\.?\\d+$");
    QString ammountString =ui->lineEdit_21->text();

    if (!phoneNumberRe.match(number).hasMatch()||!ammountRe.match(ammountString).hasMatch())
    {
        if(!phoneNumberRe.match(number).hasMatch())
        {
            ui->label_157->setText("");
            ui->label_156->setText("Invalid Phone Number!");
        }
        else if(!ammountRe.match(ammountString).hasMatch())
        {
            ui->label_156->setText("");
            ui->label_157->setText("Invalid Ammount");
        }
    }

    else
    {
        ammount = ammountString.toFloat();
        phoneNumber = number.toStdString();

        bool succeed=(Admin::users[registered_user].requestMoneyUsingPhoneNumber(phoneNumber,ammount));

        if(!succeed)
        {

            QMessageBox x;

            x.warning(this,"Request Failed","User not found!");

        }
        else
        {
            QMessageBox::information(this,"Request Done","The request has been sent succesfully");

            QTimer::singleShot(1500, this, [=]() {

                ui->stackedWidget->setCurrentIndex(2);
            });
        }
    }
}

//Request money using payment address
void First::on_pushButton_105_clicked()
{
    QString userNameString = ui->lineEdit_18->text();
    string userName;
    float ammount;

    QRegularExpression ammountRe("^\\d*\\.?\\d+$");
    QString ammountString =ui->lineEdit_19->text();

    if(!ammountRe.match(ammountString).hasMatch())
    {

        ui->label_155->setText("Invalid Ammount");
    }
    else
    {
        ammount = ammountString.toFloat();
        userName = userNameString.toStdString();
        ui->label_155->setText("");

        if(!Admin::users[registered_user].requestMoneyUsingPaymentAddress(userName,ammount))
        {
            QMessageBox::warning(this,"Request Failed","User not found!");
        }
        else
        {
            QMessageBox::information(this,"Request Done","The request has been sent succesfully");

            QTimer::singleShot(1500, this, [=]() {

                ui->stackedWidget->setCurrentIndex(2);
            });
        }
    }

}

void First::goToRequestMoneyPage(){
    if(askForPin()){
        ui->pushButton_123->setStyleSheet("color: rgb(119, 70, 234);background-color: rgb(31, 31, 31);border-radius:15px;");
        ui->pushButton_119->setStyleSheet("background-color: rgb(31, 31, 31);color:white; border-radius:15px;");

        ui->stackedWidget->setCurrentIndex(10);
        ui->stackedWidget_4->setCurrentIndex(2);
        ui->stackedWidget_2->setCurrentIndex(1);
        ui->pushButton_107->setStyleSheet("color: rgb(119, 70, 234);background-color: rgb(31, 31, 31);border-radius:15px;");
        ui->pushButton_108->setStyleSheet("background-color: rgb(31, 31, 31);color:white; border-radius:15px;");

        ui->lineEdit_18->setText("");
        ui->lineEdit_19->setText("");
        ui->lineEdit_20->setText("");
        ui->lineEdit_21->setText("");
        ui->label_157->setText("");
        ui->label_156->setText("");
        ui->label_155->setText("");

        ui->label_111->setText(QString::fromStdString(registered_user));
        ui->label_111->setAlignment(Qt::AlignRight);
        QFont font = ui->label_111->font();
        font.setPointSize(18);
        ui->label_111->setFont(font);
    }
}

//button to go to request money
void First::on_pushButton_140_clicked()
{
    goToRequestMoneyPage();
}
void First::on_pushButton_123_clicked()
{
   goToRequestMoneyPage();
}
void First::on_pushButton_41_clicked()
{
    goToRequestMoneyPage();
}
void First::on_pushButton_25_clicked()
{
    goToRequestMoneyPage();
}
void First::on_pushButton_57_clicked()
{
   goToRequestMoneyPage();
}
void First::on_pushButton_73_clicked()
{
    goToRequestMoneyPage();
}
void First::on_pushButton_90_clicked()
{
    goToRequestMoneyPage();
}

// ************************SIDE VIEW OF SEND MONEY AND REQUEST MONEY***********************

// Go to Dashboard in Send Money & Request Money
void First::on_pushButton_120_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}

// Go to View Balance in Send Money & Request Money
void First::on_pushButton_121_clicked()
{
    if(askForPin()){
        ui->stackedWidget->setCurrentIndex(3);
        ui->label_24->setText(QString::fromStdString(registered_user));
        ui->label_24->setAlignment(Qt::AlignRight);
        QFont font = ui->label_24->font();
        font.setPointSize(18);
        ui->label_24->setFont(font);
    }
}

// Go to View History in Send Money & Request Money
void First::on_pushButton_117_clicked()
{
    ui->stackedWidget->setCurrentIndex(13);

    ui->label_124->setText(QString::fromStdString(registered_user));
    ui->label_124->setAlignment(Qt::AlignRight);
    QFont font = ui->label_124->font();
    font.setPointSize(18);
    ui->label_124->setFont(font);
}

// Go to My Profile in Send Money & Request Money
void First::on_pushButton_122_clicked()
{
    goToProfile();
}

// Go to Settings in Send Money & Request Money
void First::on_pushButton_124_clicked()
{
    ui->stackedWidget->setCurrentIndex(4);
    ui->label_42->setText(QString::fromStdString(registered_user));
    ui->label_42->setAlignment(Qt::AlignRight);
    QFont font = ui->label_42->font();
    font.setPointSize(18);
    ui->label_42->setFont(font);
}

// Log out Button in Send Money & Request Money
void First::on_pushButton_118_clicked()
{
    if(logOut()){
        deleteWidget(delete_transactions_history);
        ui->stackedWidget->setCurrentIndex(7);
    }
}

// ADD NEW CARD SCREEN (Page 16)

// Go to Dashboard Screen
void First::on_pushButton_87_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}

// Go to View Balance Screen
void First::on_pushButton_88_clicked()
{
    if(askForPin()){
        ui->stackedWidget->setCurrentIndex(3);
        ui->label_24->setText(QString::fromStdString(registered_user));
        ui->label_24->setAlignment(Qt::AlignRight);
        QFont font = ui->label_24->font();
        font.setPointSize(18);
        ui->label_24->setFont(font);
    }
}

// Go to View History Screen
void First::on_pushButton_84_clicked()
{
    ui->stackedWidget->setCurrentIndex(13);

    ui->label_124->setText(QString::fromStdString(registered_user));
    ui->label_124->setAlignment(Qt::AlignRight);
    QFont font = ui->label_124->font();
    font.setPointSize(18);
    ui->label_124->setFont(font);
}

// Go to My Profile Screen
void First::on_pushButton_89_clicked()
{
    goToProfile();
}

// Go to Settings Screen
void First::on_pushButton_91_clicked()
{
    ui->stackedWidget->setCurrentIndex(4);
    ui->label_42->setText(QString::fromStdString(registered_user));
    ui->label_42->setAlignment(Qt::AlignRight);
    QFont font = ui->label_42->font();
    font.setPointSize(18);
    ui->label_42->setFont(font);
}

// Log out Button
void First::on_pushButton_85_clicked()
{
    if(logOut()){
        deleteWidget(delete_transactions_history);
        ui->stackedWidget->setCurrentIndex(7);
    }
}

//Add a new card
void First::on_pushButton_92_clicked()
{
    QString PINString = ui->lineEdit_3->text();
    QString ammountString = ui->lineEdit_14->text();
    QString cardNumberString = ui->lineEdit_2->text();


    QRegularExpression ammountRe("^\\d*\\.?\\d+$");
    QRegularExpression cardNumberRe("^\\d{16}$");
    QRegularExpression PINRe("^\\d{4}$");

    if(!ammountRe.match(ammountString).hasMatch()||!cardNumberRe.match(cardNumberString).hasMatch()||!PINRe.match(PINString).hasMatch())
    {
        if(!cardNumberRe.match(cardNumberString).hasMatch())
        {
            ui->label_159->setText("Invalid Card Number");
            ui->label_160->setText("");
            ui->label_161->setText("");
        }
        else if(!PINRe.match(PINString).hasMatch())
        {
            ui->label_159->setText("");
            ui->label_160->setText("Invalid PIN");
            ui->label_161->setText("");
        }
        else if(!ammountRe.match(ammountString).hasMatch())
        {
            ui->label_159->setText("");
            ui->label_160->setText("");
            ui->label_161->setText("Invalid Balance");
        }
    }
    else
    {

        int PIN = PINString.toInt();
        float ammount = ammountString.toFloat();
        string cardNumber = cardNumberString.toStdString();
        if(ad1.cards.find(cardNumber)!=ad1.cards.end())
        {
            ui->label_159->setText("This card already exists");
        }
        else
        {
            ui->label_159->setText("");
            ui->label_160->setText("");
            ui->label_161->setText("");

            Card temp;
            temp.card_number=cardNumber;
            temp.card_holder_name=ad1.users[registered_user].user_name;
            temp.card_ATM_pin=PIN;
            temp.card_balance=ammount;

            ad1.users[registered_user].cards.push_back(temp);
            ad1.cards[cardNumber]=registered_user;

            updateUIWithAccounts();
            displayBalance();

            QMessageBox::information(this,"Card Added","The card has been added succesfully");

            QTimer::singleShot(1500, this, [=]() {

                ui->stackedWidget->setCurrentIndex(2);
            });
        }
    }


}

// DASHBOARD SCREEN (Page 3)

// Go to Add new card screen (Dashboard Screen)
void First::on_pushButton_9_clicked()
{
    ui->stackedWidget->setCurrentIndex(6);
    ui->lineEdit_2->setText("");
    ui->lineEdit_3->setText("");
    ui->lineEdit_14->setText("");
    ui->label_159->setText("");
    ui->label_160->setText("");
    ui->label_161->setText("");

    ui->label_54->setText(QString::fromStdString(registered_user));
    ui->label_54->setAlignment(Qt::AlignRight);
    QFont font = ui->label_54->font();
    font.setPointSize(18);
    ui->label_54->setFont(font);

    ui->lineEdit_3->setEchoMode(QLineEdit::Password);
    ui->pushButton_160->setIcon(QIcon(":/new/prefix1/eye-password-show-svgrepo-com (1).png"));
}

// Go to Cash in screen (Dashboard Screen)
void First::on_pushButton_10_clicked()
{
    ui->stackedWidget->setCurrentIndex(5);
    ui->label_46->setText(QString::fromStdString(registered_user));
    ui->label_46->setAlignment(Qt::AlignRight);
    QFont font = ui->label_46->font();
    font.setPointSize(18);
    ui->label_46->setFont(font);
}

// Go to View Balance After Pop Message for Pin
void First::on_pushButton_39_clicked() {

    if(askForPin()){
        ui->stackedWidget->setCurrentIndex(3);
        ui->label_24->setText(QString::fromStdString(registered_user));
        ui->label_24->setAlignment(Qt::AlignRight);
        QFont font = ui->label_24->font();
        font.setPointSize(18);
        ui->label_24->setFont(font);
    }
}

// Go to View_History (Dashboard Screen)
void First::on_pushButton_35_clicked()
{
    ui->stackedWidget->setCurrentIndex(13);

    ui->label_124->setText(QString::fromStdString(registered_user));
    ui->label_124->setAlignment(Qt::AlignRight);
    QFont font = ui->label_124->font();
    font.setPointSize(18);
    ui->label_124->setFont(font);
}

// Go to My_Profile (Dashboard Screen)
void First::on_pushButton_40_clicked()
{
    goToProfile();
}

// Go to Settings (Dashboard Screen)
void First::on_pushButton_42_clicked()
{
    cout<<registered_user<<endl;
    ui->stackedWidget->setCurrentIndex(4);
    ui->label_42->setText(QString::fromStdString(registered_user));
    ui->label_42->setAlignment(Qt::AlignRight);
    QFont font = ui->label_42->font();
    font.setPointSize(18);
    ui->label_42->setFont(font);
}

// Log-out Button
void First::on_pushButton_36_clicked()
{
    if(logOut()){
        deleteWidget(delete_transactions_history);
        ui->stackedWidget->setCurrentIndex(7);
    }
}

// Display the Accounts
void First::updateUIWithAccounts() {
    // Clear the existing layout and remove all widgets
    QLayout *scrollAreaLayout1 = ui->scrollAreaWidgetContents_2->layout();

    if (scrollAreaLayout1) {
        QLayoutItem *child;
        while ((child = scrollAreaLayout1->takeAt(0)) != nullptr) {
            delete child->widget(); // Delete the widget
            delete child; // Delete the layout item
        }
    } else {
        scrollAreaLayout1 = new QVBoxLayout(ui->scrollAreaWidgetContents_2); // Create a new layout
    }

    QString username = QString::fromStdString(ad1.users[registered_user].user_name);
    ui->label_17->setText(username);
    ui->label_36->setText(username);

    ui->label_36->setText(QString::fromStdString(registered_user));
    ui->label_36->setAlignment(Qt::AlignRight);
    QFont font = ui->label_36->font();
    font.setPointSize(18);
    ui->label_36->setFont(font);


    // QLabel for "Easypay wallet" (always present)
    QLabel *walletLabel = new QLabel("Easypay wallet");
    walletLabel->setStyleSheet("color: white; font-size: 20px; border: 1px solid #3B3B3B;");
    //edited
    scrollAreaLayout1->addWidget(walletLabel);

    // Iterate through the vector of cards
    for (const auto& card : ad1.users[registered_user].cards) {
        QLabel *cardLabel = new QLabel();
        cardLabel->setStyleSheet("color: white; font-size: 20px; border: 1px solid #3B3B3B;");
        cardLabel->setText(QString("Card Number: %1\nHolder Name: %2")
                               .arg(QString::fromStdString(card.card_number),
                                    QString::fromStdString(card.card_holder_name)));
        //edited
        scrollAreaLayout1->addWidget(cardLabel);
    }
}

// Display the Requests
void First::updateUIWithMessages() {
    // Clear the existing layout and remove all widgets
    QLayout *scrollAreaLayout = ui->scrollAreaWidgetContents_4->layout();
    if (scrollAreaLayout) {
        QLayoutItem *child;
        while ((child = scrollAreaLayout->takeAt(0)) != nullptr) {
            delete child->widget(); // Delete the widget and its children
            delete child; // Delete the layout item
        }
    } else {
        scrollAreaLayout = new QVBoxLayout(ui->scrollAreaWidgetContents_4);
    }

    // Iterate through the list of messages
    for (auto it = ad1.users[registered_user].messages.begin(); it != ad1.users[registered_user].messages.end(); ++it) {
        const auto& msg = *it; // Get the message from the iterator

        // Create a horizontal layout to contain the message label and buttons
        QHBoxLayout *messageLayout = new QHBoxLayout();

        // Create QLabel for the message
        QLabel *label = new QLabel();
        label->setStyleSheet("color: white; font-size: 20px; border: 1px solid #3B3B3B;");
        label->setText(QString("From: %1\nStatus: %2\nMessage: %3\nAmount: %4")
                           .arg(QString::fromStdString(msg.from),
                                QString::fromStdString(msg.status),
                                QString::fromStdString(msg.message),
                                QString::number(msg.amount)));
        messageLayout->addWidget(label);

        // Create QPushButton for accepting the message
        QPushButton *acceptButton = new QPushButton("Accept");
        acceptButton->setStyleSheet("background-color: #7A69E9; color: white;");
        acceptButton->setFixedSize(acceptButton->sizeHint());
        connect(acceptButton, &QPushButton::clicked, this, [=]() {
            int index = std::distance(ad1.users[registered_user].messages.begin(), it);
            auto msgIt = ad1.users[registered_user].messages.begin();
            std::advance(msgIt, index);

            // Deduct balance and add it to the requested user if accept button is clicked and the amount is greater than the balance and change status to accepted and add it to the transaction
            if (msgIt->amount <= ad1.users[registered_user].balance) {
                ad1.users[registered_user].balance -= msgIt->amount;
                string requested_user = msgIt->from;
                ad1.users[requested_user].balance +=  msgIt->amount;

                // Create transactions if the Accept button is clicked
                Transaction transaction(ad1.users[registered_user].user_name,msgIt->from,msgIt->amount,SENDER);
                Transaction transaction1(ad1.users[registered_user].user_name,msgIt->from,msgIt->amount,RECIVER);

                // Push the transaction into the stack
                ad1.users[registered_user].transactions.push(transaction);
                ad1.users[requested_user].transactions.push(transaction1);

                ad1.users[registered_user].messages.erase(it);

                //ad1.users[registered_user].display();

                deleteWidget(delete_transactions_history);
                displaytransactioninPageData();
                updateUIWithTransactions();
                displayBalance();
                // Remove the container widget
                QWidget *containerWidget = acceptButton->parentWidget();
                delete containerWidget;
            } else {
                QMessageBox messageBox;
                messageBox.setStyleSheet("QMessageBox { border: 2px solid #7A69E9; background-color: black; color: white;}"
                                         "QPushButton { background-color: #7A69E9; }""QMessageBox QLabel { color: red; font-size: 20px; }");
                messageBox.setIcon(QMessageBox::Warning);
                messageBox.setWindowTitle("Warning");
                messageBox.setText("Amount in the Balance is not enough");
                messageBox.exec();
            }

            // Remove the container widget

        });
        messageLayout->addWidget(acceptButton);

        // Create QPushButton for rejecting the message
        QPushButton *rejectButton = new QPushButton("Reject");
        rejectButton->setStyleSheet("background-color: #7A69E9; color: white;");
        rejectButton->setFixedSize(rejectButton->sizeHint());
        connect(rejectButton, &QPushButton::clicked, this, [=]() {
            int index = std::distance(ad1.users[registered_user].messages.begin(), it);
            auto msgIt = ad1.users[registered_user].messages.begin();
            std::advance(msgIt, index);

            string requested_user = msgIt->from;
            Transaction transaction1(ad1.users[registered_user].user_name,msgIt->from,msgIt->amount,RECIVER);
            transaction1.status=FAILED;
            ad1.users[requested_user].transactions.push(transaction1);

            ad1.users[registered_user].messages.erase(it);



            // Remove the container widget
            QWidget *containerWidget = rejectButton->parentWidget();
            delete containerWidget;
        });
        messageLayout->addWidget(rejectButton);

        // Create a container widget to hold the message layout
        QWidget *messageWidget = new QWidget;
        messageWidget->setLayout(messageLayout);

        // Add the container widget to the main layout
        scrollAreaLayout->addWidget(messageWidget);

    }
    // Set the layout of scrollArea
    ui->scrollArea_3->setWidget(ui->scrollAreaWidgetContents_4);

}

// Display the Transactions
void First::updateUIWithTransactions() {

    // Clear the existing layout and remove all widgets
    QLayout *scrollAreaLayout = ui->scrollAreaWidgetContents->layout();
    //scrollAreaLayout = new QVBoxLayout(ui->scrollAreaWidgetContents);

    if (scrollAreaLayout) {
        QLayoutItem *child;
        while ((child = scrollAreaLayout->takeAt(0)) != nullptr) {
            delete child->widget(); // Delete the widget
            delete child; // Delete the layout item
        }
    } else {
        scrollAreaLayout = new QVBoxLayout(ui->scrollAreaWidgetContents);
    }

    // Populate the layout with transaction data
    int transactionsToShow = 6;
    stack<Transaction> temp;
    int count = 0;
    while (!ad1.users[registered_user].transactions.empty() && count < transactionsToShow) {
        const Transaction& transaction = ad1.users[registered_user].transactions.top();

        QLabel *transactionLabel = new QLabel();
        transactionLabel->setStyleSheet("color: white; font-size: 20px; border: 1px solid #3B3B3B;");

        // Set text for QLabel
        transactionLabel->setText(QString("From: %1\nTo: %2\nDate: %3/%4/%5 %6:%7:%8\nStatus: %9\nKind: %10\nAmount: %11")
                                      .arg(QString::fromStdString(transaction.from),
                                           QString::fromStdString(transaction.to),
                                           QString::number(transaction.date.day),
                                           QString::number(transaction.date.month),
                                           QString::number(transaction.date.year),
                                           QString::number(transaction.date.hour),
                                           QString::number(transaction.date.minutes),
                                           QString::number(transaction.date.seconds),
                                           QString::fromStdString(transaction.enumToString(transaction.status)),
                                           QString::fromStdString(transaction.enumToString2(transaction.transaction_kind)),
                                           QString::number(transaction.amount)));

        // Add QLabel to the layout
        scrollAreaLayout->addWidget(transactionLabel);

        // Pop the transaction from the stack
        temp.push(ad1.users[registered_user].transactions.top());
        ad1.users[registered_user].transactions.pop();

        // Increment count
        count++;
    }

    // Push back transactions from temp stack to original stack
    while (!temp.empty()) {
        ad1.users[registered_user].transactions.push(temp.top());
        temp.pop();
    }

    // Set the layout of scrollArea
    ui->scrollArea->setWidget(ui->scrollAreaWidgetContents);
}

// CASH IN SCREEN (Page 14)

//Go to Dashboard
void First::on_pushButton_70_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}

// Go to View Balance After Pop Message for Pin
void First::on_pushButton_71_clicked() {

    if(askForPin()){
        ui->stackedWidget->setCurrentIndex(3);
        ui->label_24->setText(QString::fromStdString(registered_user));
        ui->label_24->setAlignment(Qt::AlignRight);
        QFont font = ui->label_24->font();
        font.setPointSize(18);
        ui->label_24->setFont(font);
    }
}

// Go to View History
void First::on_pushButton_67_clicked()
{
    ui->stackedWidget->setCurrentIndex(13);

    ui->label_124->setText(QString::fromStdString(registered_user));
    ui->label_124->setAlignment(Qt::AlignRight);
    QFont font = ui->label_124->font();
    font.setPointSize(18);
    ui->label_124->setFont(font);
}

// Go to My_Profile
void First::on_pushButton_72_clicked()
{
    goToProfile();
}

// Go to Settings
void First::on_pushButton_74_clicked()
{
    ui->stackedWidget->setCurrentIndex(4);
    ui->label_42->setText(QString::fromStdString(registered_user));
    ui->label_42->setAlignment(Qt::AlignRight);
    QFont font = ui->label_42->font();
    font.setPointSize(18);
    ui->label_42->setFont(font);
}

// Logout Button
void First::on_pushButton_68_clicked()
{
    if(logOut()){
        deleteWidget(delete_transactions_history);
        ui->stackedWidget->setCurrentIndex(7);
    }
}

// Request Button
void First::on_pushButton_75_clicked()
{
    // Check if line edit has data
    if (!ui->lineEdit->text().isEmpty()) {
        srand(time(nullptr));

        // Generate a random 6-digit code
        int randomCode = rand() % 900000 + 100000;

        // Convert integer to QString
        QString codeString = QString::number(randomCode);

        // Create a custom message box
        QMessageBox msgBox;
        msgBox.setWindowTitle("Generated Code");
        msgBox.setText(codeString);
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setGeometry(1000, 500, 800, 1000);

        // Set the stylesheet for the message box
        msgBox.setStyleSheet("QMessageBox { background-color: black; border: 2px solid #7A69E9; color: white; font-size: 20px; }"
                             "QPushButton { background-color: #7A69E9; color: white; font-family: 'Segoe I Emoji'; font-size: 18px; }"
                             "QMessageBox QLabel { color: white; font-size: 50px; }");

        // Connect the "OK" button clicked signal to a slot
        connect(&msgBox, &QMessageBox::buttonClicked, this, &First::handleMessageBoxButton);
        qDebug() << codeString;
        // Show the message box
        msgBox.exec();

    } else {
        // Line edit is empty, show a message or take appropriate action
        QMessageBox::warning(this, "Empty Field", "<font color='red'>Please enter data in the text field.</font>");
    }
}

// Ok Button in msgbox
void First::handleMessageBoxButton(QAbstractButton *button)
{
    QPushButton *clickedButton = qobject_cast<QPushButton *>(button);

    // Check if the clicked button is the "OK" button
    if (clickedButton && clickedButton->text() == tr("OK")) {
        QString userInput = ui->lineEdit->text();
        float balanceToAdd = userInput.toFloat();

        // Create a transaction
        Transaction transaction("Cash in","Easy_Pay wallet",balanceToAdd,RECIVER);

        // Push the transaction into the stack
        ad1.users[registered_user].transactions.push(transaction);

        // Update balance in users map
        ad1.users[registered_user].balance += balanceToAdd;


        updateUIWithTransactions();
        deleteWidget(delete_transactions_history);
        displaytransactioninPageData();
        displayBalance();
        QTimer::singleShot(2000, this, [=]() {

            ui->stackedWidget->setCurrentIndex(2);
        });
        ui->lineEdit->setText("");
    }
}

// VIEW BALANCE SCREEN (Page 5)

// Go to Dashboard
void First::on_pushButton_22_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}

// Go to View History
void First::on_pushButton_19_clicked()
{
    ui->stackedWidget->setCurrentIndex(13);

    ui->label_124->setText(QString::fromStdString(registered_user));
    ui->label_124->setAlignment(Qt::AlignRight);
    QFont font = ui->label_124->font();
    font.setPointSize(18);
    ui->label_124->setFont(font);
}

// Go to My_Profile
void First::on_pushButton_24_clicked()
{
    goToProfile();

}

// Go to Settings
void First::on_pushButton_26_clicked()
{
    ui->stackedWidget->setCurrentIndex(4);
    ui->label_42->setText(QString::fromStdString(registered_user));
    ui->label_42->setAlignment(Qt::AlignRight);
    QFont font = ui->label_42->font();
    font.setPointSize(18);
    ui->label_42->setFont(font);
}

//Logout button
void First::on_pushButton_20_clicked()
{
    if(logOut()){
        deleteWidget(delete_transactions_history);
        ui->stackedWidget->setCurrentIndex(7);
    }
}

// Display Wallet Balance && Card Balance && Total Balance
void First::displayBalance() {

    // Check if the user was found
    if (ad1.users[registered_user].user_name != "") {
        // User found

        // Convert balances to string
        QString balanceText = QString::number(ad1.users[registered_user].balance);
        QString cardBalanceText;
        int totalCardBalance = 0;
        for (const Card& card : ad1.users[registered_user].cards) {
            cardBalanceText +=  "  Card balance: " + QString::number(card.card_balance) + "\t\t";
            totalCardBalance += card.card_balance;
        }
        totalCardBalance= totalCardBalance + ad1.users[registered_user].balance;

        QString totalCardBalanceText = QString::number(totalCardBalance);

        qDebug() << balanceText;
        qDebug() << cardBalanceText;
        qDebug() << totalCardBalanceText;
        // Display the Easy pay wallet
        ui->label_37->setText(balanceText);
        // Display the Card's Balance
        ui->label_38->setText(cardBalanceText);
        // Display total card balance in label_87
        ui->label_162->setText(totalCardBalanceText);
    } else {
        ui->label_37->setText("User not found");
        ui->label_38->setText("Card information not available");
        ui->label_162->setText("Total balance not available");
    }
}

// VIEW HISTORY SCREEN (PAGE 11)

//go to dashboard screen(view history)
void First::on_pushButton_137_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}

//go to view balance screen(view history)
void First::on_pushButton_138_clicked()
{
    if(askForPin()){
        ui->stackedWidget->setCurrentIndex(3);
        ui->label_24->setText(QString::fromStdString(registered_user));
        ui->label_24->setAlignment(Qt::AlignRight);
        QFont font = ui->label_24->font();
        font.setPointSize(18);
        ui->label_24->setFont(font);
    }
}

//go to my profile(view history)
void First::on_pushButton_139_clicked()
{
    goToProfile();
}

//go to setting(viewh history)
void First::on_pushButton_141_clicked()
{
    ui->stackedWidget->setCurrentIndex(13);

    ui->label_124->setText(QString::fromStdString(registered_user));
    ui->label_124->setAlignment(Qt::AlignRight);
    QFont font = ui->label_124->font();
    font.setPointSize(18);
    ui->label_124->setFont(font);
}

// log_out (view history)
void First::on_pushButton_135_clicked()
{
    if(logOut()){
        deleteWidget(delete_transactions_history);
        ui->stackedWidget->setCurrentIndex(7);
    }
}

//Display Transactions
void First::displaytransactioninPageData()
{
    QWidget *scrollContent = new QWidget;
    QVBoxLayout *scrollLayout = new QVBoxLayout(scrollContent);

    delete_transactions_history=scrollLayout;

    scrollContent->setLayout(scrollLayout);
    stack <Transaction> temp;
    Transaction transaction;
    int x=ad1.users[registered_user].transactions.size();
    for(int i=0;i<x;i++)
    {
        QPushButton *buttons=new QPushButton(QString("From: %1\t\t\t\t\tTo: %2\t\t\t\t\tDate: %3/%4/%5 %6:%7:%8\t\t\t\t\tStatus: %9\t\t\t\t\tKind: %10\t\t\t\t\t\tAmount: %11")
                                                   .arg(QString::fromStdString(ad1.users[registered_user].transactions.top().from),
                                                        QString::fromStdString(ad1.users[registered_user].transactions.top().to),
                                                        QString::number(ad1.users[registered_user].transactions.top().date.day),
                                                        QString::number(ad1.users[registered_user].transactions.top().date.month),
                                                        QString::number(ad1.users[registered_user].transactions.top().date.year),
                                                        QString::number(ad1.users[registered_user].transactions.top().date.hour),
                                                        QString::number(ad1.users[registered_user].transactions.top().date.minutes),
                                                        QString::number(ad1.users[registered_user].transactions.top().date.seconds),
                                                        QString::fromStdString(transaction.enumToString(ad1.users[registered_user].transactions.top().status)),
                                                        QString::fromStdString(transaction.enumToString2(ad1.users[registered_user].transactions.top().transaction_kind)),
                                                        QString::number(ad1.users[registered_user].transactions.top().amount)));

        buttons->setStyleSheet("background:rgb(31, 31, 31);color:white;height:70px;border-style: solid;border-color:white;border-width: 2px;border-radius: 15px;font: 16px Arial, sans-serif;");
        scrollLayout->addWidget(buttons);
        temp.push(ad1.users[registered_user].transactions.top());
        ad1.users[registered_user].transactions.pop();
    }

    while(!temp.empty()){
        ad1.users[registered_user].transactions.push(temp.top());
        temp.pop();

    }

    ui->scrollArea_9->setWidget(scrollContent);
    ui->scrollArea_9->setWidgetResizable(true);
}

// SETTINGS SCREEN

// Go to Dashboard Screen
void First::on_pushButton_54_clicked()
{
     ui->stackedWidget->setCurrentIndex(2);
}

// Go to View Balance Screen after Pop message
void First::on_pushButton_55_clicked()
{
    if(askForPin()){
        ui->stackedWidget->setCurrentIndex(3);
        ui->label_24->setText(QString::fromStdString(registered_user));
        ui->label_24->setAlignment(Qt::AlignRight);
        QFont font = ui->label_24->font();
        font.setPointSize(18);
        ui->label_24->setFont(font);
    }
}

// Go to View History
void First::on_pushButton_51_clicked()
{
    ui->stackedWidget->setCurrentIndex(13);

    ui->label_124->setText(QString::fromStdString(registered_user));
    ui->label_124->setAlignment(Qt::AlignRight);
    QFont font = ui->label_124->font();
    font.setPointSize(18);
    ui->label_124->setFont(font);
}

// Go to My Profile
void First::on_pushButton_56_clicked()
{
    goToProfile();
}

// Log out Button
void First::on_pushButton_52_clicked()
{
    if(logOut()){
        deleteWidget(delete_transactions_history);
        ui->stackedWidget->setCurrentIndex(7);
    }
}

// ADMIN TRANSACTION (PAGE 22)

//go to admin screen(back/tran)
void First::on_pushButton_125_clicked()
{
    ui->stackedWidget->setCurrentIndex(8);
}

void First::displayDatatransactions()
{
    QWidget *scrollContents = new QWidget;
    QVBoxLayout *scrollLayouts = new QVBoxLayout(scrollContents);

    delete_transactions_admin=scrollLayouts;

    scrollContents->setLayout(scrollLayouts);
    stack <Transaction> temp;
    Transaction transaction;
    int x=ad1.users[registered_user].transactions.size();
    cout << x << endl;
    for(int i=0;i<x;i++)
    {
        cout << ad1.users[registered_user].transactions.top().from << endl;
        QPushButton *buttons=new QPushButton(QString("From: %1\t\t\t\t\tTo: %2\t\t\t\t\tDate: %3/%4/%5 %6:%7:%8\t\t\t\t\tStatus: %9\t\t\t\t\tKind: %10\t\t\t\t\t\tAmount: %11")
                                                   .arg(QString::fromStdString(ad1.users[registered_user].transactions.top().from),
                                                        QString::fromStdString(ad1.users[registered_user].transactions.top().to),
                                                        QString::number(ad1.users[registered_user].transactions.top().date.day),
                                                        QString::number(ad1.users[registered_user].transactions.top().date.month),
                                                        QString::number(ad1.users[registered_user].transactions.top().date.year),
                                                        QString::number(ad1.users[registered_user].transactions.top().date.hour),
                                                        QString::number(ad1.users[registered_user].transactions.top().date.minutes),
                                                        QString::number(ad1.users[registered_user].transactions.top().date.seconds),
                                                        QString::fromStdString(transaction.enumToString(ad1.users[registered_user].transactions.top().status)),
                                                        QString::fromStdString(transaction.enumToString2(ad1.users[registered_user].transactions.top().transaction_kind)),
                                                        QString::number(ad1.users[registered_user].transactions.top().amount)));
        buttons->setStyleSheet("background:rgb(31, 31, 31);color:white;height:70px;border-style: solid;border-color:white;border-width: 2px;border-radius: 15px;font: 16px Arial, sans-serif;");
        scrollLayouts->addWidget(buttons);
        temp.push(ad1.users[registered_user].transactions.top());
        ad1.users[registered_user].transactions.pop();
    }

    while(!temp.empty()){
        ad1.users[registered_user].transactions.push(temp.top());
        temp.pop();

    }

    ui->scrollArea_7->setWidget(scrollContents);
    ui->scrollArea_7->setWidgetResizable(true);

}


//*////////////////////////////////////////////////Ahmed and Lamiaa////////////////////////////////////////

//----------------------------------------page1------------------------------------
//when click on sign_up bttton
void First::on_pushButton_SignUP_clicked()
{
    QRegularExpression password_regex("^[^\\s\\?]{8,}$");
    QRegularExpression pin_regex("^[1-9][0-9]{5}$");


    ui->label_EmptyDataError->setText("");
    ui->label_UserNameError->setText("");
    ui->label_PhoneNumberError->setText("");
    ui->label_PasswordError->setText("");
    ui->label_PinError->setText("");
    ui->label_ConfirmPasswordError->setText("");
    ui->label_ConfirmPinError->setText("");

    QString user_name=ui->lineEdit_UserName->text();
    QString email=ui->lineEdit_Email->text();
    QString phone=ui->lineEdit_Phone->text();
    QString password=ui->lineEdit_Password->text();
    QString confirm_password=ui->lineEdit_confirmPass->text();
    QString pin=ui->lineEdit_Pin->text();
    QString confirm_pin=ui->lineEdit_ConfirmPin->text();
    if(user_name==""||email==""||phone==""||password==""||confirm_password==""||pin==""||confirm_pin=="")
    {
        ui->label_EmptyDataError->setText("There are empty fields please,compelete all data");
    }
    else
    {
        string check_user_name=user_name.toStdString();
        string check_phone=phone.toStdString();

        int check=ad1.check_signin_data(check_user_name,check_phone);
        if(check==1)
        {
            ui->label_UserNameError->setText("This username is aldready exist please,try again");
        }
        else if(check==2)
        {
            ui->label_PhoneNumberError->setText("This phone number is aldready exist please,try again");
        }
        else if(!password_regex.match(password).hasMatch())
        {
            ui->label_PasswordError->setText("This password is too week please,try again");
        }
        else if(!pin_regex.match(pin).hasMatch())
        {
            ui->label_PinError->setText("This pin is too short please,try again");
        }
        else if(password!=confirm_password)
        {
            ui->label_ConfirmPasswordError->setText("confirmation password isn't correct please,try again");
        }
        else if(pin!=confirm_pin)
        {
            ui->label_ConfirmPinError->setText("confirmation pin isn't correct please,try again");
        }
        else
        {
            // string registered_password=password.toStdString();
            birth_of_date.day=ui->dateEdit->date().day();
            birth_of_date.month=ui->dateEdit->date().month();
            birth_of_date.year=ui->dateEdit->date().year();
            // string registered_email=email.toStdString();
            string registered_pin=pin.toStdString();
            // int registered_pin1=stoi(registered_pin);

            new_user.password=new_user.passwordEnc(password.toStdString());
            new_user.user_name=check_user_name;
            new_user.phone_number=check_phone;
            new_user.Pin=stoi(registered_pin);
            new_user.email=email.toStdString();
            new_user.birth_date=birth_of_date;
            new_user.address_account=new_user.user_name+"@Easy_Pay.com";
            new_user.status="Actived";

            ad1.addUser(new_user);
            ad1.dis();
            registered_user=new_user.user_name;

            displayBalance();
            updateUIWithAccounts();
            updateUIWithMessages();
            updateUIWithTransactions();
            displaytransactioninPageData();

            ui->stackedWidget->setCurrentIndex(2);

            ui->lineEdit_UserName->setText("");
            ui->lineEdit_Email->setText("");
            ui->lineEdit_Phone->setText("");
            ui->lineEdit_Password->setText("");
            ui->lineEdit_confirmPass->setText("");
            ui->lineEdit_Pin->setText("");
            ui->lineEdit_ConfirmPin->setText("");

            ui->label_EmptyDataError->setText("done");
        }
    }
    ui->lineEdit_Password->setEchoMode(QLineEdit::Password);
    ui->pushButton_93->setIcon(QIcon(":/new/prefix1/eye-password-show-svgrepo-com (1).png"));
    ui->lineEdit_ConfirmPin->setEchoMode(QLineEdit::Password);
    ui->pushButton_158->setIcon(QIcon(":/new/prefix1/eye-password-show-svgrepo-com (1).png"));
    ui->lineEdit_Pin->setEchoMode(QLineEdit::Password);
    ui->pushButton_95->setIcon(QIcon(":/new/prefix1/eye-password-show-svgrepo-com (1).png"));
    ui->lineEdit_confirmPass->setEchoMode(QLineEdit::Password);
    ui->pushButton_94->setIcon(QIcon(":/new/prefix1/eye-password-show-svgrepo-com (1).png"));

}

//when click on BackToLogin button
void First::on_pushButton_BackToLogin_clicked()
{
    ui->label_EmptyDataError->setText("");
    ui->label_UserNameError->setText("");
    ui->label_PhoneNumberError->setText("");
    ui->label_PasswordError->setText("");
    ui->label_PinError->setText("");
    ui->label_ConfirmPasswordError->setText("");
    ui->label_ConfirmPinError->setText("");

    ui->lineEdit_UserName->setText("");
    ui->lineEdit_Email->setText("");
    ui->lineEdit_Phone->setText("");
    ui->lineEdit_Password->setText("");
    ui->lineEdit_confirmPass->setText("");
    ui->lineEdit_Pin->setText("");
    ui->lineEdit_ConfirmPin->setText("");

    ui->lineEdit_Password->setEchoMode(QLineEdit::Password);
    ui->pushButton_93->setIcon(QIcon(":/new/prefix1/eye-password-show-svgrepo-com (1).png"));
    ui->lineEdit_ConfirmPin->setEchoMode(QLineEdit::Password);
    ui->pushButton_158->setIcon(QIcon(":/new/prefix1/eye-password-show-svgrepo-com (1).png"));
    ui->lineEdit_Pin->setEchoMode(QLineEdit::Password);
    ui->pushButton_95->setIcon(QIcon(":/new/prefix1/eye-password-show-svgrepo-com (1).png"));
    ui->lineEdit_confirmPass->setEchoMode(QLineEdit::Password);
    ui->pushButton_94->setIcon(QIcon(":/new/prefix1/eye-password-show-svgrepo-com (1).png"));

    ui->stackedWidget->setCurrentIndex(7);
}
//----------------------------------------------------------------------------------
//-----------------------------------------page:2-----------------------------------
//when click on Add in page of add user page:2
void First::on_pushButton_SignUP_2_clicked()
{
    QRegularExpression password_regex("^[^\\s\\?]{8,}$");
    QRegularExpression pin_regex("^[1-9][0-9]{5}$");


    ui->label_EmptyDataError_2->setText("");
    ui->label_UserNameError_5->setText("");
    ui->label_PhoneNumberError_5->setText("");
    ui->label_PasswordError_5->setText("");
    ui->label_ConfirmPasswordError_5->setText("");
    ui->label_PinError_2->setText("");
    ui->label_ConfirmPinError_2->setText("");

    QString user_name=ui->lineEdit_UserName_5->text();
    QString email=ui->lineEdit_Email_5->text();
    QString phone=ui->lineEdit_Phone_5->text();
    QString password=ui->lineEdit_Password_5->text();
    QString confirm_password=ui->lineEdit_confirmPass_5->text();
    QString pin=ui->lineEdit_Pin_2->text();
    QString confirm_pin=ui->lineEdit_ConfirmPin_2->text();
    if(user_name==""||email==""||phone==""||password==""||confirm_password==""||pin==""||confirm_pin=="")
    {
        ui->label_EmptyDataError_2->setText("There are empty fields please,compelete all data");
    }
    else
    {
        string check_user_name=user_name.toStdString();
        string check_phone=phone.toStdString();

        int check=ad1.check_signin_data(check_user_name,check_phone);
        if(check==1)
        {
            ui->label_UserNameError_5->setText("This username is aldready exist please,try again");
        }
        else if(check==2)
        {
            ui->label_PhoneNumberError_5->setText("This phone number is aldready exist please,try again");
        }
        else if(!password_regex.match(password).hasMatch())
        {
            ui->label_PasswordError_5->setText("This password is too week please,try again");
        }
        else if(!pin_regex.match(pin).hasMatch())
        {
            ui->label_PinError_2->setText("This pin is too short please,try again");
        }
        else if(password!=confirm_password)
        {
            ui->label_ConfirmPasswordError_5->setText("confirmation password isn't correct please,try again");
        }
        else if(pin!=confirm_pin)
        {
            ui->label_ConfirmPinError_2->setText("confirmation pin isn't correct please,try again");
        }
        else
        {
            // string registered_password=password.toStdString();
            birth_of_date.day=ui->dateEdit_2->date().day();
            birth_of_date.month=ui->dateEdit_2->date().month();
            birth_of_date.year=ui->dateEdit_2->date().year();
            // string registered_email=email.toStdString();
            string registered_pin=pin.toStdString();
            // int registered_pin1=stoi(registered_pin);

            new_user.password=new_user.passwordEnc(password.toStdString());
            new_user.user_name=check_user_name;
            new_user.phone_number=check_phone;
            new_user.Pin=stoi(registered_pin);
            new_user.email=email.toStdString();
            new_user.birth_date=birth_of_date;
            new_user.address_account=new_user.user_name+"@Easy_Pay.com";
            new_user.status="Actived";

            ad1.addUser(new_user);
            registered_user=new_user.user_name;
            ad1.dis();

            if(delete_first_admin_widget)
                deleteWidget(delete_first_admin_widget);
            if(delete_transactions_admin)
                deleteWidget(delete_transactions_admin);
            if(delete_cards_admin_display)
                deleteWidget(delete_cards_admin_display);

            setFirstAdminPageData();

            ui->stackedWidget->setCurrentIndex(11);

            ui->lineEdit_UserName_5->setText("");
            ui->lineEdit_Email_5->setText("");
            ui->lineEdit_Phone_5->setText("");
            ui->lineEdit_Password_5->setText("");
            ui->lineEdit_confirmPass_5->setText("");
            ui->lineEdit_Pin_2->setText("");
            ui->lineEdit_ConfirmPin_2->setText("");

            ui->label_EmptyDataError_2->setText("done");
        }
    }

    ui->lineEdit_confirmPass_5->setEchoMode(QLineEdit::Password);
    ui->pushButton_163->setIcon(QIcon(":/new/prefix1/eye-password-show-svgrepo-com (1).png"));
    ui->lineEdit_Password_5->setEchoMode(QLineEdit::Password);
    ui->pushButton_164->setIcon(QIcon(":/new/prefix1/eye-password-show-svgrepo-com (1).png"));
    ui->lineEdit_ConfirmPin_2->setEchoMode(QLineEdit::Password);
    ui->pushButton_165->setIcon(QIcon(":/new/prefix1/eye-password-show-svgrepo-com (1).png"));
    ui->lineEdit_Pin_2->setEchoMode(QLineEdit::Password);
    ui->pushButton_166->setIcon(QIcon(":/new/prefix1/eye-password-show-svgrepo-com (1).png"));
}

//when click on cancel in page of add user page:2
void First::on_pushButton_SignUP_3_clicked()
{
    ui->label_EmptyDataError_2->setText("");
    ui->label_UserNameError_5->setText("");
    ui->label_PhoneNumberError_5->setText("");
    ui->label_PasswordError_5->setText("");
    ui->label_ConfirmPasswordError_5->setText("");
    ui->label_PinError_2->setText("");
    ui->label_ConfirmPinError_2->setText("");

    ui->lineEdit_UserName_5->setText("");
    ui->lineEdit_Email_5->setText("");
    ui->lineEdit_Phone_5->setText("");
    ui->lineEdit_Password_5->setText("");
    ui->lineEdit_confirmPass_5->setText("");
    ui->lineEdit_Pin_2->setText("");
    ui->lineEdit_ConfirmPin_2->setText("");

    ui->stackedWidget->setCurrentIndex(11);

    ui->lineEdit_confirmPass_5->setEchoMode(QLineEdit::Password);
    ui->pushButton_163->setIcon(QIcon(":/new/prefix1/eye-password-show-svgrepo-com (1).png"));
    ui->lineEdit_Password_5->setEchoMode(QLineEdit::Password);
    ui->pushButton_164->setIcon(QIcon(":/new/prefix1/eye-password-show-svgrepo-com (1).png"));
    ui->lineEdit_ConfirmPin_2->setEchoMode(QLineEdit::Password);
    ui->pushButton_165->setIcon(QIcon(":/new/prefix1/eye-password-show-svgrepo-com (1).png"));
    ui->lineEdit_Pin_2->setEchoMode(QLineEdit::Password);
    ui->pushButton_166->setIcon(QIcon(":/new/prefix1/eye-password-show-svgrepo-com (1).png"));
}
//----------------------------------------------------------------------------------

//page3
void First::on_pushButton_logOut_clicked()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("Logout");
    msgBox.setText("Are you sure you want to log out?");
    msgBox.setStandardButtons(QMessageBox::Yes      |        QMessageBox::No);
    msgBox.setGeometry(1000, 500, 800, 1000);

    msgBox.setStyleSheet("QMessageBox { border: 2px solid #7A69E9; background-color: black; font-size: 20px; }"
                         "QMessageBox QLabel { color: white; }" // Set text color to white
                         "QPushButton { background-color: #7A69E9; color: white; font-size: 20px; align: ceneter;}");

    int reply = msgBox.exec();

    if (reply == QMessageBox::Yes) {
        //cout<<registered_user<<endl;
        ui->stackedWidget->setCurrentIndex(7);

    }
}

//-----------------------------------------page:8-----------------------------------
//when click on back to login button
void First::on_pushButton_backToSignIn_clicked()
{
    ui->lineEdit_user_name8->setText("");
    ui->lineEdit_password8->setText("");
    ui->label_error8->setText("");
    ui->stackedWidget->setCurrentIndex(0);
}

//when click on log_in button
void First::on_pushButton_logIn_clicked()
{
    string check_user_name=(ui->lineEdit_user_name8->text()).toStdString();
    string check_password=(ui->lineEdit_password8->text()).toStdString();
    if(ui->lineEdit_password8->text()==""||ui->lineEdit_user_name8->text()=="")
    {
        ui->label_error8->setText("There are empty fields please,compelete all data");
    }
    else
    {
        //if the data ifs the data of the admin
        if(check_user_name=="admin@"&&check_password=="123456789admin")
        {
            setFirstAdminPageData();
            // cardScroll();
            // cardScroll_page_10();
            ui->stackedWidget->setCurrentIndex(11);
            ui->lineEdit_user_name8->setText("");
            ui->lineEdit_password8->setText("");
            ui->label_error8->setText("");
        }
        else
        {
            if((ad1.getUserData(check_user_name,check_password)).user_name!=""&&(ad1.getUserData(check_user_name,check_password)).status!="Suspended")
            {
                // if ((ad1.getUserData(check_user_name,check_password)).suspended)
                registered_user=(ad1.getUserData(check_user_name,check_password)).user_name;
                ui->lineEdit_user_name8->setText("");
                ui->lineEdit_password8->setText("");
                ui->label_error8->setText("");

                displayBalance();
                updateUIWithAccounts();
                updateUIWithMessages();
                updateUIWithTransactions();
                displaytransactioninPageData();



                ui->stackedWidget->setCurrentIndex(2);
            }
            else if((ad1.getUserData(check_user_name,check_password)).user_name!=""&&(ad1.getUserData(check_user_name,check_password)).status=="Suspended")
            {
                ui->label_error8->setText("Your account is suspended,please contact the admin");
            }
            else
            {
                ui->label_error8->setText("Can't log in,user name or password may be wrong");
            }
        }
    }
    ui->lineEdit_password8->setEchoMode(QLineEdit::Password);
    ui->pushButton_159->setIcon(QIcon(":/new/prefix1/eye-password-show-svgrepo-com (1).png"));
}

//----------------------------------------------------------------------------------

//this function to fill data of users in firdt page of the admin
void First::setFirstAdminPageData()
{
    ui->label_81->setText("");
    ad1.dis();
    cout<<"///////////////////////////////"<<endl;
    QWidget *scrollContent = new QWidget;
    QVBoxLayout *scrollLayout = new QVBoxLayout(scrollContent);
    //ahmed added
    delete_first_admin_widget=scrollLayout;
    int total_balance=0;

    scrollContent->setLayout(scrollLayout);
    unordered_map<string,User>::iterator it;
    for(it=ad1.users.begin();it!=ad1.users.end();it++)
    {
        if(it->second.user_name=="")
            continue;
        total_balance+=it->second.balance;
        QHBoxLayout *rowLayout = new QHBoxLayout; // Create a layout for each row
        QLabel *lb=new QLabel;
        QPushButton *buttons=new QPushButton(QString::fromStdString((it->first)+"-                                                                  "+(it->second.status)+"                                                                             EGP   "+to_string(it->second.balance)));
        buttons->setStyleSheet("background:rgb(31, 31, 31);color:white;height:70px;border-style: solid;border-color:white;border-width: 2px;border-radius: 15px;font: 16px Arial, sans-serif;");
        QPixmap pix("C:/Users/lamia/OneDrive/Desktop/wallet2004/ll2.png");

        lb->setPixmap(pix.scaled(100,100,Qt::KeepAspectRatio));
        rowLayout->addWidget(lb);
        rowLayout->addWidget(buttons);

        scrollLayout->addLayout(rowLayout);

        connect(buttons, SIGNAL(clicked()), this, SLOT(onUserClick()));

    }

    ui->label_118->setText(QString::fromStdString(to_string(total_balance)));

    ui->scrollArea_6->setWidget(scrollContent);
    ui->scrollArea_6->setWidgetResizable(true);
}

void First::onUserClick()
{
    //ahmed added
    ui->label_81->setText("");
    QPushButton *p = qobject_cast<QPushButton*>(sender());
    string key = p->text().toStdString();
    string key1 = key.substr(0, key.find('-'));
    registered_user=key1;
    cardScroll();
    Wallet_display();
    cardScroll_page_10();
    auto it = ad1.users.find(key1);
    if (it != ad1.users.end()) {
        ui->label_68->setText(QString::fromStdString(it->second.user_name));
        ui->label_71->setText(QString::fromStdString(it->second.user_name));
        ui->label_86->setText(QString::fromStdString(it->second.user_name));
        ui->label_73->setText(QString::fromStdString(it->second.phone_number));
        ui->label_75->setText(QString::fromStdString(it->second.email));
        ui->label_77->setText(QString::fromStdString(to_string(it->second.birth_date.day)+" / "+ to_string(it->second.birth_date.month)+" / " + to_string(it->second.birth_date.year)));
        ui->label_79->setText(QString::fromStdString(it->second.passwordDec(it->second.password)));
        if(it->second.status=="Suspended")
        {
            ui->pushButton_2->setText("Active Account");
        }
        else
        {
            ui->pushButton_2->setText("suspend Account");
        }
        //ahmed added
        deleteWidget(delete_first_admin_widget);

        ui->stackedWidget->setCurrentIndex(8);

    } else {
        cout << "User not found!" << endl;
    }
}

void First::on_pushButton_logOut_4_clicked()
{
    if(logOut()){
        if(delete_first_admin_widget)
            deleteWidget(delete_first_admin_widget);
        if(delete_transactions_admin)
            deleteWidget(delete_transactions_admin);
        if(delete_cards_admin_display)
            deleteWidget(delete_cards_admin_display);

        ui->stackedWidget->setCurrentIndex(7);
    }
}

//when click on Add user in page of add user page:20
void First::on_pushButton_SignUP_5_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}
// ----------------------- Buttons in page_9 -----------------------


// Go to All Transactions from Admin page
void First::on_pushButton_clicked()
{
    displayDatatransactions();
    ui->stackedWidget->setCurrentIndex(12);
    ui->label_121->setText(QString::fromStdString(registered_user));
}
// Back
void First::on_pushButton_7_clicked()
{
    setFirstAdminPageData();
    ui->stackedWidget->setCurrentIndex(11);
}
// Edit Account
void First::on_pushButton_4_clicked()
{
    ui->lineEdit_9->setText(QString::fromStdString(ad1.users[registered_user].phone_number));
    ui->lineEdit_8->setText(QString::fromStdString(ad1.users[registered_user].email));
    ui->lineEdit_11->setText(QString::fromStdString(ad1.users[registered_user].passwordDec(ad1.users[registered_user].password)));
    ui->lineEdit_10->setText(QString::fromStdString(to_string(ad1.users[registered_user].Pin)));
    ui->stackedWidget->setCurrentIndex(9);
}

// Delete Account

void First::on_pushButton_3_clicked() {

   // ui->label_136->hide();

    QMessageBox::StandardButton reply = QMessageBox::question(this, "Delete Account!",
                                                              "Are you sure you want to delete the account?", QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        // auto indexToDelete = ui->label_68->text().toStdString();
         auto it = ad1.users.find(registered_user);
        ad1.users.erase(it);
         balanceLabel->setText("");
         balanceLabel->setStyleSheet("color: white; font-size: 15px;");
        setFirstAdminPageData();
        deleteWidget(delete_cards_admin_display);
        cardScroll();
            ui->label_71->setText("");
            ui->label_86->setText("");
            ui->label_73->setText("");
            ui->label_75->setText("");
            ui->label_77->setText("");
            ui->label_79->setText("");
            ui->label_81->setText("The account has been deleted successfully ✅");
            ui->label_81->setStyleSheet("QLabel { color : green; }");
            //ui->label_136->show();


                QTimer::singleShot(2000, this, [=]() {

                    ui->stackedWidget->setCurrentIndex(11);
                });
        // }
        // else {

        //     ui->label_136->setText("Account not found ❌");
        //     ui->label_136->setStyleSheet("QLabel { color : red; }");
        //     ui->label_136->show();
        // }
    }
}

// Suspend Account or Active account
void First::on_pushButton_2_clicked()
{
    ui->label_137->hide();
    QMessageBox::StandardButton reply;
    auto indexToSuspend = ui->label_68->text().toStdString();
    auto it = ad1.users.find(indexToSuspend);
    if(it->second.status=="Actived")
    {
        reply = QMessageBox::question(this, "Suspend Account!",
                                      "Are you sure you want to suspend the account?", QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            auto indexToSuspend = ui->label_68->text().toStdString();
            auto it = ad1.users.find(indexToSuspend);
            if (it != ad1.users.end()) {
                // Proceed with suspending the account
                it->second.status = "Suspended";
                // QString message = QString("Account for user %1 has been suspended.").arg(QString::fromStdString(it->second.user_name));
                // QMessageBox::information(nullptr, "Account Suspended", message);
                ui->label_81->setText("The account has been suspended successfully ✅");
                ui->label_137->setStyleSheet("QLabel { color : green; }");
                ui->label_137->show();
                if(it->second.status == "Suspended")
                {
                    deleteWidget(delete_first_admin_widget);
                    ui->pushButton_2->setText("Active Account");
                }

            }


        }
    }
    else
    {
        reply = QMessageBox::question(this, "Active Account!",
                                      "Are you sure you want to Active the account?", QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            auto indexToSuspend = ui->label_68->text().toStdString();
            auto it = ad1.users.find(indexToSuspend);
            if (it != ad1.users.end()) {
                // Proceed with activing the account
                it->second.status = "Active";
                // QString message = QString("Account for user %1 has been suspended.").arg(QString::fromStdString(it->second.user_name));
                // QMessageBox::information(nullptr, "Account Suspended", message);
                ui->label_81->setText("The account has been Actived successfully ✅");
                ui->label_137->setStyleSheet("QLabel { color : green; }");
                ui->label_137->show();
                if(it->second.status == "Active")
                {

                    ui->pushButton_2->setText("Suspend Account");
                }

            }


        }
    }
}

// Wallet display function
void First::Wallet_display() {
    QWidget *scrollContent = new QWidget;
    QVBoxLayout *scrollLayout = new QVBoxLayout(scrollContent);
    scrollContent->setLayout(scrollLayout);


    QHBoxLayout *rowLayout = new QHBoxLayout; // Create a layout for each row

    // Create a frame to contain the card information
    QFrame *frame = new QFrame;
    frame->setFrameShape(QFrame::StyledPanel);
    frame->setStyleSheet("background-color: rgba(255, 255, 255, 50); border: 2px inset #7A69E9; border-radius: 10px; padding: 5px;");

    // Create a label to display "Easy Pay Wallet"
    QLabel *walletLabel = new QLabel("Easy Pay Wallet");
    walletLabel->setStyleSheet("color: white ; font-size: 20px; font-weight: bold;");
    walletLabel->setAlignment(Qt::AlignCenter);

    // Create a label to display card balance
    balanceLabel = new QLabel;
    QString balanceInfo = "Balance: " + QString::number(ad1.users[registered_user].balance) + " EGP";
    balanceLabel->setText(balanceInfo);
    balanceLabel->setStyleSheet("color: white; font-size: 15px;");

    // Add widgets to frame
    QVBoxLayout *frameLayout = new QVBoxLayout(frame);
    frameLayout->addWidget(walletLabel);
    frameLayout->addWidget(balanceLabel, 0, Qt::AlignHCenter); // Align balance label to the center horizontally

    // Add frame to row layout
    rowLayout->addWidget(frame);

    // Add spacing between widgets
    rowLayout->setSpacing(10); // Adjust spacing as needed

    // Add row layout to scroll layout
    scrollLayout->addLayout(rowLayout);


    // Set the scroll area widget and make it resizable
    ui->scrollArea_8->setWidget(scrollContent);
    ui->scrollArea_8->setWidgetResizable(true);
}

// Card Scroll Function .. to display card information
void First :: cardScroll()
{

    QWidget *scrollContent = new QWidget;
    QVBoxLayout *scrollLayout = new QVBoxLayout(scrollContent);
    scrollContent->setLayout(scrollLayout);

    delete_cards_admin_display=scrollLayout;

    for (int i = 0; i < ad1.users[registered_user].cards.size(); i++) {
        QHBoxLayout *rowLayout = new QHBoxLayout; // Create a layout for each row

        QPixmap pix("C:/Users/lamia/OneDrive/Desktop/wallet2004/CardFormate.png");

        // Create a frame to contain the card information
        QFrame *frame = new QFrame;
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setStyleSheet("background-color: rgba(255, 255, 255, 50);border: 2px outset #7A69E9; border-radius: 10px; padding: 5px;");

        // Create a label to display card information
        QLabel *cardLabel = new QLabel;
        QString cardInfo = QString::fromStdString(ad1.users[registered_user].cards[i].card_holder_name)
                           + " - " + QString::fromStdString(ad1.users[registered_user].cards[i].card_number)
                           + " - Balance: " + QString::number(ad1.users[registered_user].cards[i].card_balance) + " EGP";
        cardLabel->setText(cardInfo);
        cardLabel->setStyleSheet("color: white; font-size: 18px;"); // Set font size to 18px

        // Add card label to frame
        QVBoxLayout *frameLayout = new QVBoxLayout(frame);
        frameLayout->addWidget(cardLabel);

        // Add frame to row layout
        rowLayout->addWidget(frame);

        // Add spacing between widgets
        rowLayout->setSpacing(10); // Adjust spacing as needed

        // Add row layout to scroll layout
        scrollLayout->addLayout(rowLayout);
    }

    // Set the scroll area widget and make it resizable
    ui->scrollArea_4->setWidget(scrollContent);
    ui->scrollArea_4->setWidgetResizable(true);
}
// ----------------------- Buttons in page_10 -----------------------

// Back
void First::on_pushButton_6_clicked()
{
    setFirstAdminPageData();
    ui->stackedWidget->setCurrentIndex(8);
    ui->lineEdit_10->setEchoMode(QLineEdit::Password);
    ui->pushButton_99->setIcon(QIcon(":/new/prefix1/eye-password-show-svgrepo-com (1).png"));
    ui->lineEdit_11->setEchoMode(QLineEdit::Password);
    ui->pushButton_98->setIcon(QIcon(":/new/prefix1/eye-password-show-svgrepo-com (1).png"));
}

// Card Scroll Function .. to display card information
void First :: cardScroll_page_10()
{
    QWidget *scrollContent = new QWidget;
    QVBoxLayout *scrollLayout = new QVBoxLayout(scrollContent);
    scrollContent->setLayout(scrollLayout);

    QHBoxLayout *rowLayout = new QHBoxLayout; // Create a layout for each row

    // Create a frame to contain the card information
    QFrame *frame = new QFrame;
    frame->setFrameShape(QFrame::StyledPanel);
    frame->setStyleSheet("background-color: rgba(255, 255, 255, 50); border: 2px inset #7A69E9; border-radius: 10px; padding: 5px;");

    // Create a label to display "Easy Pay Wallet"
    QLabel *walletLabel = new QLabel("Easy Pay Wallet");
    walletLabel->setStyleSheet("color: white ; font-size: 20px; font-weight: bold;");
    walletLabel->setAlignment(Qt::AlignCenter);

    // Create a QLineEdit to display and edit the balance
    QLineEdit *balanceEdit = new QLineEdit;
    balanceEdit->setText(QString::number(ad1.users[registered_user].balance));
    balanceEdit->setStyleSheet("background-color: white; color: black; font-size: 15px; border-radius: 5px;");
    balanceEdit->setAlignment(Qt::AlignCenter);

    // Create a button to save the edited balance
    QPushButton *saveButton = new QPushButton("Save");
    saveButton->setStyleSheet("background-color: #7A69E9; color: white; font-size: 15px; border-radius: 5px; padding: 5px;");
    // Connect the button's clicked signal to update the balance
    connect(saveButton, &QPushButton::clicked, [=]() {
        bool ok;
        float newBalance = balanceEdit->text().toInt(&ok);
        if (ok) {
            // Update the balance and display a success message
            ad1.users[registered_user].balance = newBalance;
            QMessageBox::information(this, "Success", "Balance updated successfully.");
            QString balanceInfo = "Balance: " + QString::number(ad1.users[registered_user].balance) + " EGP";
            balanceLabel->setText(balanceInfo);
            deleteWidget(delete_first_admin_widget);

            setFirstAdminPageData();
        } else {
            // Display an error message if the entered balance is not valid
            QMessageBox::warning(this, "Error", "Invalid balance value. Please enter a valid integer.");
        }
    });

    // Add widgets to frame
    QVBoxLayout *frameLayout = new QVBoxLayout(frame);
    frameLayout->addWidget(walletLabel);
    frameLayout->addWidget(balanceEdit);
    frameLayout->addWidget(saveButton);

    // Add frame to row layout
    rowLayout->addWidget(frame);

    // Add spacing between widgets
    rowLayout->setSpacing(10); // Adjust spacing as needed

    // Add row layout to scroll layout
    scrollLayout->addLayout(rowLayout);


    // Set the scroll area widget and make it resizable
    ui->scrollArea_5->setWidget(scrollContent);
    ui->scrollArea_5->setWidgetResizable(true);

}

// confirm
void First::on_pushButton_5_clicked()
{
    string Edit_phoneNumber = (ui->lineEdit_9->text()).toStdString();
    string Edit_Email = (ui->lineEdit_8->text()).toStdString();
    string Edit_Password = (ui->lineEdit_11->text()).toStdString();
    int Edit_Pin = stoi( (ui->lineEdit_10->text()).toStdString());
    string nameDelete = ui->label_86->text().toStdString();
    updateUsername(Edit_Email,Edit_phoneNumber,Edit_Pin,Edit_Password);

    ui->label_68->setText(QString::fromStdString(ad1.users[registered_user].user_name));
    ui->label_71->setText(QString::fromStdString(ad1.users[registered_user].user_name));
    ui->label_86->setText(QString::fromStdString(ad1.users[registered_user].user_name));
    ui->label_73->setText(QString::fromStdString(ad1.users[registered_user].phone_number));
    ui->label_75->setText(QString::fromStdString(ad1.users[registered_user].email));
    ui->label_77->setText(QString::fromStdString(to_string(ad1.users[registered_user].birth_date.day)+" / "+ to_string(ad1.users[registered_user].birth_date.month)+" / " + to_string(ad1.users[registered_user].birth_date.year)));
    ui->label_79->setText(QString::fromStdString(ad1.users[registered_user].passwordDec(ad1.users[registered_user].password)));


    deleteWidget(delete_first_admin_widget);
    ui->stackedWidget->setCurrentIndex(8);

    ui->lineEdit_10->setEchoMode(QLineEdit::Password);
    ui->pushButton_99->setIcon(QIcon(":/new/prefix1/eye-password-show-svgrepo-com (1).png"));
    ui->lineEdit_11->setEchoMode(QLineEdit::Password);
    ui->pushButton_98->setIcon(QIcon(":/new/prefix1/eye-password-show-svgrepo-com (1).png"));
}

// ----------------------- Buttons in page_23 -----------------------

// Dashboard
void First::on_pushButton_153_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}

// View Balance
void First::on_pushButton_154_clicked()
{
    if (askForPin()){
    ui->stackedWidget->setCurrentIndex(3);

        ui->label_24->setText(QString::fromStdString(registered_user));
        ui->label_24->setAlignment(Qt::AlignRight);
        QFont font = ui->label_24->font();
        font.setPointSize(18);
        ui->label_24->setFont(font);
    }
}

// Send Money
void First::on_pushButton_152_clicked()
{
    goToSendMoneyPage();
}

// Request Money
void First::on_pushButton_156_clicked()
{
    goToRequestMoneyPage();
}

// View History
void First::on_pushButton_150_clicked()
{
    ui->stackedWidget->setCurrentIndex(13);

    ui->label_124->setText(QString::fromStdString(registered_user));
    ui->label_124->setAlignment(Qt::AlignRight);
    QFont font = ui->label_124->font();
    font.setPointSize(18);
    ui->label_124->setFont(font);
}

// My Profile
void First::on_pushButton_155_clicked()
{
    goToProfile();

}

// Setting
void First::on_pushButton_157_clicked()
{
    ui->stackedWidget->setCurrentIndex(4);
    ui->label_42->setText(QString::fromStdString(registered_user));
    ui->label_42->setAlignment(Qt::AlignRight);
    QFont font = ui->label_42->font();
    font.setPointSize(18);
    ui->label_42->setFont(font);
}

// Log Out
void First::on_pushButton_151_clicked()
{
   if (logOut()) {
        deleteWidget(delete_transactions_history);
        ui->stackedWidget->setCurrentIndex(7);
    }
}

//Confirm
void First::on_pushButton_SignUP_6_clicked()
{
    string check_user_name=(ui->lineEdit_user_name8->text()).toStdString();
    ui->label_64->setText(QString::fromStdString(check_user_name));
    ui->label_133->setText(QString::fromStdString(check_user_name));
    string Edit_phoneNumber = (ui->lineEdit_24->text()).toStdString();
    string Edit_Password = (ui->lineEdit_26->text()).toStdString();
    string Edit_Email = (ui->lineEdit_25->text()).toStdString();
    int Edit_Pin = stoi( (ui->lineEdit_27->text()).toStdString());
    string nameDelete = ui->label_64->text().toStdString();

    updateUsername(Edit_Email,Edit_phoneNumber,Edit_Pin,Edit_Password);

    ui->label_133->setText(QString::fromStdString(registered_user));
    ui->label_133->setAlignment(Qt::AlignRight);
    QFont font = ui->label_133->font();
    font.setPointSize(18);
    ui->label_133->setFont(font);


    QTimer::singleShot(300, this, [=]() {

        ui->stackedWidget->setCurrentIndex(2);
    });

}
// Function to update the user name
void First :: updateUsername( string&newEmail, string& newPhone ,int& pin,string& newPass)
{

    auto it = ad1.users.find(registered_user);
    if (it != ad1.users.end()) {
        it->second.email = newEmail;
        it->second.phone_number = newPhone;
        it->second.Pin = pin;
        it->second.password =it->second.passwordEnc( newPass);
    }
    else
    {
        cout << "User not found" <<endl;
    }
}
//is used to when clicking on go to profile
void First::goToProfile()
{
    ui->stackedWidget->setCurrentIndex(14);

    ui->label_64->setText(QString::fromStdString("Welcome, "+registered_user));
    ui->label_133->setText(QString::fromStdString(registered_user));

    ui->lineEdit_24->setText(QString::fromStdString(ad1.users[registered_user].phone_number));
    ui->lineEdit_25->setText(QString::fromStdString(ad1.users[registered_user].email));
    ui->lineEdit_26->setText(QString::fromStdString(ad1.users[registered_user].passwordDec(ad1.users[registered_user].password)));
    ui->lineEdit_27->setText(QString::fromStdString(to_string(ad1.users[registered_user].Pin)));

    ui->lineEdit_27->setEchoMode(QLineEdit::Password);
    ui->pushButton_162->setIcon(QIcon(":/new/prefix1/eye-password-show-svgrepo-com (1).png"));
    ui->lineEdit_26->setEchoMode(QLineEdit::Password);
    ui->pushButton_161->setIcon(QIcon(":/new/prefix1/eye-password-show-svgrepo-com (1).png"));

    ui->label_133->setText(QString::fromStdString(registered_user));
    ui->label_133->setAlignment(Qt::AlignRight);
    QFont font = ui->label_133->font();
    font.setPointSize(18);
    ui->label_133->setFont(font);
}

//delete the widgets
void First::deleteWidget(QVBoxLayout *delete_widget)
{
    QLayoutItem *child;
    if(child){
    while ((child = delete_widget->takeAt(0)) != nullptr)
    {
        delete child->widget();
        delete child;
    }
    }
}

//---------------------------------------------------**********----------------------------------------------

//-------------------------------------------------
//buttons to show and hide password
void First::on_pushButton_93_clicked()
{
    if(ui->lineEdit_Password->echoMode()==QLineEdit::Password){
        ui->lineEdit_Password->setEchoMode(QLineEdit::Normal);
        ui->pushButton_93->setIcon(QIcon(":/new/prefix1/eye-password-hide-svgrepo-com (1).png"));
    }
    else{
        ui->lineEdit_Password->setEchoMode(QLineEdit::Password);
        ui->pushButton_93->setIcon(QIcon(":/new/prefix1/eye-password-show-svgrepo-com (1).png"));
    }
}

void First::on_pushButton_94_clicked()
{
    if(ui->lineEdit_confirmPass->echoMode()==QLineEdit::Password){
        ui->lineEdit_confirmPass->setEchoMode(QLineEdit::Normal);
        ui->pushButton_94->setIcon(QIcon(":/new/prefix1/eye-password-hide-svgrepo-com (1).png"));
    }
    else{
        ui->lineEdit_confirmPass->setEchoMode(QLineEdit::Password);
        ui->pushButton_94->setIcon(QIcon(":/new/prefix1/eye-password-show-svgrepo-com (1).png"));
    }
}

void First::on_pushButton_95_clicked()
{
    if(ui->lineEdit_Pin->echoMode()==QLineEdit::Password){
        ui->lineEdit_Pin->setEchoMode(QLineEdit::Normal);
        ui->pushButton_95->setIcon(QIcon(":/new/prefix1/eye-password-hide-svgrepo-com (1).png"));
    }
    else{
        ui->lineEdit_Pin->setEchoMode(QLineEdit::Password);
        ui->pushButton_95->setIcon(QIcon(":/new/prefix1/eye-password-show-svgrepo-com (1).png"));
    }
}

void First::on_pushButton_158_clicked()
{
    if(ui->lineEdit_ConfirmPin->echoMode()==QLineEdit::Password){
        ui->lineEdit_ConfirmPin->setEchoMode(QLineEdit::Normal);
        ui->pushButton_158->setIcon(QIcon(":/new/prefix1/eye-password-hide-svgrepo-com (1).png"));
    }
    else{
        ui->lineEdit_ConfirmPin->setEchoMode(QLineEdit::Password);
        ui->pushButton_158->setIcon(QIcon(":/new/prefix1/eye-password-show-svgrepo-com (1).png"));
    }
}

void First::on_pushButton_159_clicked()
{
    if(ui->lineEdit_password8->echoMode()==QLineEdit::Password){
        ui->lineEdit_password8->setEchoMode(QLineEdit::Normal);
        ui->pushButton_159->setIcon(QIcon(":/new/prefix1/eye-password-hide-svgrepo-com (1).png"));
    }
    else{
        ui->lineEdit_password8->setEchoMode(QLineEdit::Password);
        ui->pushButton_159->setIcon(QIcon(":/new/prefix1/eye-password-show-svgrepo-com (1).png"));
    }
}

void First::on_pushButton_160_clicked()
{
    if(ui->lineEdit_3->echoMode()==QLineEdit::Password){
        ui->lineEdit_3->setEchoMode(QLineEdit::Normal);
        ui->pushButton_160->setIcon(QIcon(":/new/prefix1/eye-password-hide-svgrepo-com (1).png"));
    }
    else{
        ui->lineEdit_3->setEchoMode(QLineEdit::Password);
        ui->pushButton_160->setIcon(QIcon(":/new/prefix1/eye-password-show-svgrepo-com (1).png"));
    }
}

void First::on_pushButton_161_clicked()
{
    if(ui->lineEdit_26->echoMode()==QLineEdit::Password){
        ui->lineEdit_26->setEchoMode(QLineEdit::Normal);
        ui->pushButton_161->setIcon(QIcon(":/new/prefix1/eye-password-hide-svgrepo-com (1).png"));
    }
    else{
        ui->lineEdit_26->setEchoMode(QLineEdit::Password);
        ui->pushButton_161->setIcon(QIcon(":/new/prefix1/eye-password-show-svgrepo-com (1).png"));
    }
}

void First::on_pushButton_162_clicked()
{
    if(ui->lineEdit_27->echoMode()==QLineEdit::Password){
        ui->lineEdit_27->setEchoMode(QLineEdit::Normal);
        ui->pushButton_162->setIcon(QIcon(":/new/prefix1/eye-password-hide-svgrepo-com (1).png"));
    }
    else{
        ui->lineEdit_27->setEchoMode(QLineEdit::Password);
        ui->pushButton_162->setIcon(QIcon(":/new/prefix1/eye-password-show-svgrepo-com (1).png"));
    }
}

void First::on_pushButton_98_clicked()
{
    if(ui->lineEdit_11->echoMode()==QLineEdit::Password){
        ui->lineEdit_11->setEchoMode(QLineEdit::Normal);
        ui->pushButton_98->setIcon(QIcon(":/new/prefix1/eye-password-hide-svgrepo-com (1).png"));
    }
    else{
        ui->lineEdit_11->setEchoMode(QLineEdit::Password);
        ui->pushButton_98->setIcon(QIcon(":/new/prefix1/eye-password-show-svgrepo-com (1).png"));
    }
}

void First::on_pushButton_99_clicked()
{
    if(ui->lineEdit_10->echoMode()==QLineEdit::Password){
        ui->lineEdit_10->setEchoMode(QLineEdit::Normal);
        ui->pushButton_99->setIcon(QIcon(":/new/prefix1/eye-password-hide-svgrepo-com (1).png"));
    }
    else{
        ui->lineEdit_10->setEchoMode(QLineEdit::Password);
        ui->pushButton_99->setIcon(QIcon(":/new/prefix1/eye-password-show-svgrepo-com (1).png"));
    }
}

void First::on_pushButton_164_clicked()
{
    if(ui->lineEdit_Password_5->echoMode()==QLineEdit::Password){
        ui->lineEdit_Password_5->setEchoMode(QLineEdit::Normal);
        ui->pushButton_164->setIcon(QIcon(":/new/prefix1/eye-password-hide-svgrepo-com (1).png"));
    }
    else{
        ui->lineEdit_Password_5->setEchoMode(QLineEdit::Password);
        ui->pushButton_164->setIcon(QIcon(":/new/prefix1/eye-password-show-svgrepo-com (1).png"));
    }
}

void First::on_pushButton_163_clicked()
{
    if(ui->lineEdit_confirmPass_5->echoMode()==QLineEdit::Password){
        ui->lineEdit_confirmPass_5->setEchoMode(QLineEdit::Normal);
        ui->pushButton_163->setIcon(QIcon(":/new/prefix1/eye-password-hide-svgrepo-com (1).png"));
    }
    else{
        ui->lineEdit_confirmPass_5->setEchoMode(QLineEdit::Password);
        ui->pushButton_163->setIcon(QIcon(":/new/prefix1/eye-password-show-svgrepo-com (1).png"));
    }
}

void First::on_pushButton_166_clicked()
{
    if(ui->lineEdit_Pin_2->echoMode()==QLineEdit::Password){
        ui->lineEdit_Pin_2->setEchoMode(QLineEdit::Normal);
        ui->pushButton_166->setIcon(QIcon(":/new/prefix1/eye-password-hide-svgrepo-com (1).png"));
    }
    else{
        ui->lineEdit_Pin_2->setEchoMode(QLineEdit::Password);
        ui->pushButton_166->setIcon(QIcon(":/new/prefix1/eye-password-show-svgrepo-com (1).png"));
    }
}

void First::on_pushButton_165_clicked()
{
    if(ui->lineEdit_ConfirmPin_2->echoMode()==QLineEdit::Password){
        ui->lineEdit_ConfirmPin_2->setEchoMode(QLineEdit::Normal);
        ui->pushButton_165->setIcon(QIcon(":/new/prefix1/eye-password-hide-svgrepo-com (1).png"));
    }
    else{
        ui->lineEdit_ConfirmPin_2->setEchoMode(QLineEdit::Password);
        ui->pushButton_165->setIcon(QIcon(":/new/prefix1/eye-password-show-svgrepo-com (1).png"));
    }
}

void First::on_pushButton_134_clicked()
{

}
