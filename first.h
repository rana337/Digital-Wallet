#ifndef FIRST_H
#define FIRST_H

#include <QMainWindow>
#include "admin.h"
#include <QAbstractButton>
#include<QVBoxLayout>
#include<QLabel>

namespace Ui {
class First;
}

class First : public QMainWindow
{
    Q_OBJECT

public:
    explicit First(QWidget *parent = nullptr);
    Admin ad1;





    string registered_user;

    Date birth_of_date;
    User new_user;


    QVBoxLayout *delete_first_admin_widget;
    QVBoxLayout *delete_transactions_history;
    QVBoxLayout *delete_transactions_admin;
    QVBoxLayout *delete_cards_admin_display;
    QLabel *balanceLabel;


    void setFirstAdminPageData();
    void cardScroll();
    void updateUsername(string&newEmail, string&newPhone ,int& pin,string& newPass);
    void  cardScroll_page_10();
    void deleteWidget(QVBoxLayout *delete_widget);
    void  Wallet_display();
    void goToProfile();
    void deleteWidget2(QLayout *delete_widget);

    ~First();

private slots:


    void on_pushButton_8_clicked();

    void on_pushButton_101_clicked();

    void on_pushButton_100_clicked();

    void on_pushButton_119_clicked();

    void on_pushButton_53_clicked();

    void on_pushButton_102_clicked();

    void on_pushButton_103_clicked();

    void on_pushButton_104_clicked();

    void on_pushButton_37_clicked();

    void on_pushButton_21_clicked();

    void on_pushButton_69_clicked();

    void on_pushButton_86_clicked();

    void on_pushButton_136_clicked();


    void on_lineEdit_16_textChanged(const QString &arg1);

    void on_lineEdit_17_textChanged(const QString &arg1);

    void on_lineEdit_12_textChanged(const QString &arg1);

    void on_pushButton_123_clicked();

    void on_pushButton_107_clicked();

    void on_pushButton_108_clicked();

    void on_pushButton_106_clicked();

    void on_pushButton_105_clicked();

    void on_pushButton_92_clicked();

    void on_pushButton_140_clicked();



    void on_pushButton_41_clicked();

    void on_pushButton_25_clicked();

    void on_pushButton_57_clicked();

    void on_pushButton_73_clicked();

    void on_pushButton_90_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_35_clicked();

    void on_pushButton_40_clicked();

    void on_pushButton_42_clicked();

    void on_pushButton_70_clicked();

    void on_pushButton_71_clicked();

    void on_pushButton_67_clicked();

    void on_pushButton_72_clicked();

    void on_pushButton_74_clicked();

    void on_pushButton_22_clicked();

    void on_pushButton_19_clicked();

    void on_pushButton_24_clicked();

    void on_pushButton_26_clicked();

    void on_pushButton_125_clicked();

    void on_pushButton_137_clicked();

    void on_pushButton_138_clicked();

    void on_pushButton_139_clicked();

    void on_pushButton_141_clicked();

    void displayBalance();

    void on_pushButton_75_clicked();

    void handleMessageBoxButton(QAbstractButton *button);

    void updateUIWithMessages();

    void updateUIWithAccounts();

    void updateUIWithTransactions();

    void on_pushButton_39_clicked();

    void displaytransactioninPageData();

    void displayDatatransactions();

    void on_pushButton_36_clicked();

    void on_pushButton_20_clicked();

    void on_pushButton_68_clicked();

    void on_pushButton_135_clicked();

    void on_pushButton_54_clicked();

    void on_pushButton_55_clicked();

    void on_pushButton_51_clicked();

    void on_pushButton_56_clicked();

    void on_pushButton_52_clicked();

    void on_pushButton_120_clicked();

    void on_pushButton_121_clicked();

    void on_pushButton_117_clicked();

    void on_pushButton_122_clicked();

    void on_pushButton_124_clicked();

    void on_pushButton_118_clicked();

    void on_pushButton_87_clicked();

    void on_pushButton_88_clicked();

    void on_pushButton_84_clicked();

    void on_pushButton_89_clicked();

    void on_pushButton_91_clicked();

    void on_pushButton_85_clicked();

    bool askForPin();

    void goToSendMoneyPage();

    void goToRequestMoneyPage();

    bool logOut();







    void on_pushButton_logOut_clicked();

    void on_pushButton_SignUP_clicked();

    void on_pushButton_BackToLogin_clicked();

    void on_pushButton_backToSignIn_clicked();

    void on_pushButton_logIn_clicked();

    void onUserClick();

    void on_pushButton_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_153_clicked();

    void on_pushButton_154_clicked();

    void on_pushButton_152_clicked();

    void on_pushButton_156_clicked();

    void on_pushButton_150_clicked();

    void on_pushButton_155_clicked();

    void on_pushButton_157_clicked();

    void on_pushButton_151_clicked();

    void on_pushButton_SignUP_6_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_SignUP_2_clicked();

    void on_pushButton_SignUP_3_clicked();

    void on_pushButton_SignUP_5_clicked();

    void on_pushButton_logOut_4_clicked();

    void on_pushButton_93_clicked();

    void on_pushButton_94_clicked();

    void on_pushButton_95_clicked();

    void on_pushButton_158_clicked();

    void on_pushButton_159_clicked();

    void on_pushButton_160_clicked();

    void on_pushButton_161_clicked();

    void on_pushButton_162_clicked();

    void on_pushButton_98_clicked();

    void on_pushButton_99_clicked();

    void on_pushButton_164_clicked();

    void on_pushButton_163_clicked();

    void on_pushButton_166_clicked();

    void on_pushButton_165_clicked();

    void on_pushButton_134_clicked();

private:
    Ui::First *ui;
};

#endif // FIRST_H
