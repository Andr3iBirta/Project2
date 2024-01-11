#include <string.h>
#include "entities.h"
#include "Validation.c"

struct Date get_date(int user_id, int acc_id, int index) {
    return user_list[user_id].accounts[acc_id].transactions[index].date;
}
char* get_description(int user_id, int acc_id, int index) {
    return user_list[user_id].accounts[acc_id].transactions[index].description;
}
float get_amount(int user_id, int acc_id, int index) {
    return user_list[user_id].accounts[acc_id].transactions[index].amount;
}
char* get_type(int user_id, int acc_id, int index) {
    return user_list[user_id].accounts[acc_id].transactions[index].type;
}

void set_date(int user_id, int acc_id, int index, int new_day, int new_month, int new_year) {
    user_list[user_id].accounts[acc_id].transactions[index].date.day = new_day;
    user_list[user_id].accounts[acc_id].transactions[index].date.month = new_month;
    user_list[user_id].accounts[acc_id].transactions[index].date.year = new_year;
}
void set_description(int user_id, int acc_id, int index, char* new_description) {
    strcpy(user_list[user_id].accounts[acc_id].transactions[index].description , new_description);
}
void set_amount(int user_id, int acc_id, int index, float new_amount) {
    user_list[user_id].accounts[acc_id].transactions[index].amount = new_amount;
}
void set_type(int user_id, int acc_id, int index, char* new_type) {
    strcpy(user_list[user_id].accounts[acc_id].transactions[index].type, new_type);
}

void setup_users() {
    /**
      Function that loads all the users into memory
    */
    FILE *fh = fopen("user_data.txt", "r");

    char name[101], pass[101], filename[101];
    int num;
    while(fscanf(fh, "%s %s %s %d", name, pass, filename, &num) == 4) {
        strcpy(user_list[++num_of_users].name, name);
        strcpy(user_list[num_of_users].pass, pass);
        strcpy(user_list[num_of_users].filename, filename);
        user_list[num_of_users].num_of_accounts = num;
    }

    fclose(fh);
}

void setup_accounts() {
    /**
      Function that loads all the accounts into memory
    */
    int status;
    char name[101], filename[101];
    double balance;

    for(int i = 1; i <= num_of_users; i++) {
        FILE *fh = fopen(user_list[i].filename, "r");

        for(int j = 1; j <= user_list[i].num_of_accounts; j++) {
            fscanf(fh, "%d %s %s %lf", &status, name, filename, &balance);
            user_list[i].accounts[j].status = status;
            strcpy(user_list[i].accounts[j].name, name);
            strcpy(user_list[i].accounts[j].filename, filename);
            user_list[i].accounts[j].balance = balance;
        }

        fclose(fh);
    }
}

void setup_transactions() {
    /**
      Function that loads all the transactions into memory
    */
    int day, month, year, n;
    double amount;
    char description[251], type[10];

    for(int i = 1; i <= num_of_users; i++) {

        for(int j = 1; j <= user_list[i].num_of_accounts; j++) {

            FILE *fh = fopen(user_list[i].accounts[j].filename, "r");

            while (fscanf(fh, "%d/%d/%d %[^\n] %s %lf", &day, &month, &year, description, type, &amount) == 6) {
                n = ++user_list[i].accounts[j].num_of_transactions;
                set_date(i, j, n, day, month, year);
                set_description(i, j, n, description);
                set_type(i, j, n, type);
                set_amount(i, j, n, amount);
            }

            fclose(fh);
        }
    }
}

void update_accounts_file(int crt_user_id, char filename[]) {
    /**
      Function that refreshes the accounts file
    */
    FILE *fh = fopen(filename, "w");

    int status;
    char name[101], fn[101];
    double balance;

    for(int i = 1; i <= 3; i++) {
        status = user_list[crt_user_id].accounts[i].status;
        strcpy(name, user_list[crt_user_id].accounts[i].name);
        strcpy(fn, user_list[crt_user_id].accounts[i].filename);
        balance = user_list[crt_user_id].accounts[i].balance;

        fprintf(fh, "%d %s %s %.2lf\n", status, name, fn, balance);
    }

    fclose(fh);
}

void update_transactions_file(int crt_user_id, int crt_acc_id, char filename[]) {
    /**
      Function that attempts to save information about every transaction
    in 'transactions', in 'filename'
    */
    ///opens the file for the purpose of writing
    FILE *file = fopen(filename, "w");

    ///placeholders that ensure ease of access and code-readability
    int day, month, year, n = user_list[crt_user_id].accounts[crt_acc_id].num_of_transactions;
    float amount;
    char description[251], type[10];

    ///cycles through every transaction
    for(int i = 1; i <= n; i++) {
        day = get_date(crt_user_id, crt_acc_id, i).day;
        month = get_date(crt_user_id, crt_acc_id, i).month;
        year = get_date(crt_user_id, crt_acc_id, i).year;
        amount = get_amount(crt_user_id, crt_acc_id, i);
        strcpy(description, get_description(crt_user_id, crt_acc_id, i));
        strcpy(type, get_type(crt_user_id, crt_acc_id, i));

        ///saves a new transaction in the file
        fprintf(file, "%d/%d/%d %s\n %s %.2lf\n", day, month, year, description, type, amount);
    }

    ///closes the file for further use
    fclose(file);
}
