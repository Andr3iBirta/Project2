#ifndef ENTITIES_H_INCLUDED
#define ENTITIES_H_INCLUDED

struct Date {
    int day, month, year;
}date;

struct Transaction {
    struct Date date;
    char description[251], type[10];
    double amount;
};

struct Account {
    int status, num_of_transactions;
    char name[101], filename[101];
    double balance;
    struct Transaction transactions[101];
};

struct User {
    char name[101], pass[101], filename[101];
    struct Account accounts[10];
    int num_of_accounts;
};

#endif // ENTITIES_H_INCLUDED
