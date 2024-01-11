#include "Domain.c"
#include "time.h"

void setup() {
    /**
      Function that loads all of the data from the files
    into memory
    */
    setup_users();
    setup_accounts();
    setup_transactions();
}

void logIn(char username[], char password[]) {
    user_id = validateUserData(username, password);
}

void logOut() {
    user_id = 0;
}

void deselectAccount() {
    acc_id = 0;
}

void toggleAccount() {
    /**
      Function that sets the status of an account:
      1 - if it was inactive
      0 - if it was active
      updates the user file
    */
    if(user_list[user_id].accounts[acc_id].status)
        user_list[user_id].accounts[acc_id].status = 0;
    else
        user_list[user_id].accounts[acc_id].status = 1;
    update_accounts_file(user_id, user_list[user_id].filename);
}

_Bool changeAccountName(char name[]) {
    /**
      Function that attempts to modify the name of an account
    and update the user file
      returns 1 if name was valid and 0 otherwise
    */
    if(validateName(name)) {
        strcpy(user_list[user_id].accounts[acc_id].name, name);
        update_accounts_file(user_id, user_list[user_id].filename);
        return 1;
    }
    return 0;
}

_Bool recordTransaction(struct Transaction new_transaction) {
    /**
      Function that stores a transaction in the array of
    the current account
      returns 1 if new_transaction is valid and 0, otherwise
    */
    if(validateTransaction(new_transaction))
    {
        int n = ++user_list[user_id].accounts[acc_id].num_of_transactions;
        user_list[user_id].accounts[acc_id].transactions[n] = new_transaction;
        if(strcmp(new_transaction.type, "income") == 0)
            user_list[user_id].accounts[acc_id].balance += new_transaction.amount;
        else
            user_list[user_id].accounts[acc_id].balance -= new_transaction.amount;
        update_accounts_file(user_id, user_list[user_id].filename);
        update_transactions_file(user_id, acc_id, user_list[user_id].accounts[acc_id].filename);
        return 1;
    }
    return 0;
}

_Bool recordTransfer(int destination_id, struct Transaction new_transaction) {
    /**
      Function that stores a transaction in the array of
    the current account, marked as expense and another one
    in the array of the destination-account, marked as income
      destination_id - id of the account we want to transfer to
      returns 1 if new_transaction is valid and 0, otherwise
    */
    if(validateTransaction(new_transaction))
    {
        int n = ++user_list[user_id].accounts[destination_id].num_of_transactions;
        user_list[user_id].accounts[destination_id].transactions[n] = new_transaction;
        if(strcmp(new_transaction.type, "income") == 0)
            user_list[user_id].accounts[destination_id].balance += new_transaction.amount;
        else
            user_list[user_id].accounts[destination_id].balance -= new_transaction.amount;
        update_accounts_file(user_id, user_list[user_id].filename);
        update_transactions_file(user_id, destination_id, user_list[user_id].accounts[destination_id].filename);
        return 1;
    }
    return 0;
}

_Bool recordPayment(int destination_id, struct Transaction new_transaction) {
    /**
      Function that stores a transaction in the array of
    the current account, marked as expense and another one
    in the first(main) account of the destination-user,
    marked as income
      destination_id - id of the user we want to pay
      returns 1 if transaction is valid and 0, otherwise
    */
    if(validateTransaction(new_transaction))
    {
        int n = ++user_list[destination_id].accounts[1].num_of_transactions;
        user_list[destination_id].accounts[1].transactions[n] = new_transaction;
        if(strcmp(new_transaction.type, "income") == 0)
            user_list[destination_id].accounts[1].balance += new_transaction.amount;
        else
            user_list[destination_id].accounts[1].balance -= new_transaction.amount;
        update_accounts_file(destination_id, user_list[destination_id].filename);
        update_transactions_file(destination_id, 1, user_list[destination_id].accounts[1].filename);
        return 1;
    }
    return 0;
}

void updateAudit(_Bool logged) {
    /**
      Function that enters a new log into the audit file
      logged - boolean that checks if current user has logged
    in(1) or logged out(0)
    */
    FILE *fh = fopen("audit.txt", "a");

    time_t t;
    struct tm *currentTime;
    time(&t);
    currentTime = localtime(&t);

    fprintf(fh, "%02d/%02d/%04d %02d:%02d.....User %s logged ",
            currentTime->tm_mday,
            currentTime->tm_mon + 1,
            currentTime->tm_year + 1900,
            currentTime->tm_hour, currentTime->tm_min,
            user_list[user_id].name);
    if(logged)
        fprintf(fh, "in.\n");
    else
        fprintf(fh, "out.\n");

    fclose(fh);
}
