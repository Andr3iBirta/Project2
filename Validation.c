#include "Globals.c"

int validateUserData(char username[], char password[]) {
    /**
      Function that checks if the login data is valid
    and returns the id of the user if the name and
    password match
      Returns 0 if data is invalid
    */
    for(int i = 1; i <= num_of_users; i++)
        if(strcmp(username, user_list[i].name) == 0)
            if(strcmp(password, user_list[i].pass) == 0)
                return i;
    return 0;
}

_Bool validateName(char name[]) {
    /**
      Function that checks the validity of an account name:
    returns 1 if name contains no spaces and 0, otherwise
    */
    if(strchr(name, ' '))
        return 0;
    return 1;
}

_Bool validateTransaction(struct Transaction transaction) {
    /**
      Function that checks if all information about 'transaction' is valid
    returns '0' if False and '1' if True
    */
    if(transaction.date.day < 1 || transaction.date.day > 31)
        return 0;
    if(transaction.date.month < 1 || transaction.date.month > 12)
        return 0;
    if(transaction.date.year < 1900)
        return 0;
    if(transaction.amount < 0)
        return 0;
    if(strcmp(transaction.type, "income") && strcmp(transaction.type, "expense"))
        return 0;
    return 1;
}
