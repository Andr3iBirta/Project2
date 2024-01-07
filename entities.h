#ifndef ENTITIES_H_INCLUDED
#define ENTITIES_H_INCLUDED

struct Date {
    int day, month, year;
}date;

struct Transaction {
    struct Date date;
    char description[251], type[10];
    float amount;
};

struct Account {
    _Bool status;
    char name[101], file[101];
    float balance;
};

struct User {
    char name[101], pass[101], file[101];
    struct Account acc1, acc2, acc3;
};

#endif // ENTITIES_H_INCLUDED
