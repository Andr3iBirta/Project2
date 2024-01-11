#include "Controller.c"

void clearBuffer() {
    while(getchar() != '\n');
}

void clearScreen() {
    getchar();
    system("cls");
}

void title() {
    printf("    +----------+\n    | HomeBank |\n    +----------+\n");
}

void uiUpdateAccount() {
    char name[101];
    printf("\n Enter a new account name: ");
    clearBuffer();
    fgets(name, 101, stdin);
    if(name[strlen(name) - 1] == '\n')
        name[strlen(name) - 1] = '\0';
    if(changeAccountName(name))
        printf("\n Account name changed successfully.");
    else
        printf("\n Account name cannot contain spaces!");
}

void uiToggleAccount() {
    if(acc_id == 1) {
        printf("\n Cannot deactivate main account!\n");
        return;
    }
    toggleAccount();
    printf("\n %s ", user_list[user_id].accounts[acc_id].name);
    if(user_list[user_id].accounts[acc_id].status)
        printf("is now active.");
    else
        printf("is now inactive.");
}

void uiTransaction() {
    /**
      Function that reads the day, month, year, description, amount and type
    of a transaction
    */
    struct Transaction transaction;
    printf("\n Enter the date(dd/mm/yyyy): ");
    scanf("%d/%d/%d", &transaction.date.day, &transaction.date.month, &transaction.date.year);

    printf(" Enter the description: ");
    while(getchar() != '\n'); ///clears the buffer
    fgets(transaction.description, 251, stdin); ///reads a multi-word string
    ///deletes the 'endline' character from the end of the string
    if(transaction.description[strlen(transaction.description) - 1] == '\n')
        transaction.description[strlen(transaction.description) - 1] = '\0';

    printf(" Enter the amount: ");
    if(scanf("%lf", &transaction.amount) != 1)
        transaction.amount = -1; ///if a non-float input is entered, it is marked as invalid

    printf(" Enter the type: ");
    scanf("%s", transaction.type);

    if(recordTransaction(transaction))
        printf("\n Transaction recorded successfully.");
    else
        printf("\n Transaction invalid.");
}

void uiTransfer() {
    int k = 0;
    for(int i = 1; i <= user_list[user_id].num_of_accounts; i++)
        if(i != acc_id && user_list[user_id].accounts[i].status)
            k++;

    if(!k) {
        printf("\n No other active accounts.");
        return;
    }

    for(int i = 1; i <= user_list[user_id].num_of_accounts; i++) {
        if(i != acc_id && user_list[user_id].accounts[i].status) {
            char str_status[101] = "";
            if(!user_list[user_id].accounts[i].status)
                strcat(str_status, "in");
            strcat(str_status, "active");

            printf("\n %d) %s %.2lf$\n",
                i, user_list[user_id].accounts[i].name,
                user_list[user_id].accounts[i].balance);
        }
    }

    int id;
    printf("\n Select an account to start the transfer: ");
    scanf("%d", &id);
    if(id != 1 && id != 2 && id != 3) {
        printf("\n No account found.");
        clearBuffer();
        return;
    }
    if(id == acc_id || !user_list[user_id].accounts[id].status) {
        printf("\n No account found.");
        clearBuffer();
        return;
    }

    int destination_acc_id = id;

    struct Transaction transaction;
    printf("\n Enter the date(dd/mm/yyyy): ");
    scanf("%d/%d/%d", &transaction.date.day, &transaction.date.month, &transaction.date.year);

    printf(" Enter the description: ");
    while(getchar() != '\n'); ///clears the buffer
    fgets(transaction.description, 251, stdin); ///reads a multi-word string
    ///deletes the 'endline' character from the end of the string
    if(transaction.description[strlen(transaction.description) - 1] == '\n')
        transaction.description[strlen(transaction.description) - 1] = '\0';

    printf(" Enter the amount: ");
    if(scanf("%lf", &transaction.amount) != 1)
    transaction.amount = -1; ///if a non-float input is entered, it is marked as invalid

    strcpy(transaction.type, "expense");

    if(recordTransaction(transaction)) {
        strcpy(transaction.type, "income");
        if(recordTransfer(destination_acc_id, transaction))
            printf("\n Transfer recorded successfully.");
    }
    else
        printf("\n Transfer invalid.");
    clearBuffer();
}

void uiPayment() {
    if(num_of_users == 1) {
        printf("\n No contacts available.");
        clearBuffer();
        return;
    }

    for(int i = 1; i <= num_of_users; i++)
        if(i != user_id)
            printf("\n %d) %s\n", i, user_list[i].name);

    int id;
    printf("\n Select a contact: ");
    scanf("%d", &id);
    if(id == user_id || (id != 1 && id != 2 && id != 3)) {
        printf("\n Contact not found.");
        return;
    }

    int destination_user_id = id;

    struct Transaction transaction;
    printf("\n Enter the date(dd/mm/yyyy): ");
    scanf("%d/%d/%d", &transaction.date.day, &transaction.date.month, &transaction.date.year);

    printf(" Enter the description: ");
    while(getchar() != '\n'); ///clears the buffer
    fgets(transaction.description, 251, stdin); ///reads a multi-word string
    ///deletes the 'endline' character from the end of the string
    if(transaction.description[strlen(transaction.description) - 1] == '\n')
        transaction.description[strlen(transaction.description) - 1] = '\0';

    printf(" Enter the amount: ");
    if(scanf("%lf", &transaction.amount) != 1)
    transaction.amount = -1; ///if a non-float input is entered, it is marked as invalid

    strcpy(transaction.type, "expense");

    if(recordTransaction(transaction)) {
        strcpy(transaction.type, "income");
        if(recordPayment(destination_user_id, transaction))
            printf("\n Payment recorded successfully.");
    }
    else
        printf("\n Payment invalid.");
    clearBuffer();
}

void transactionMenu() {
    if(!user_list[user_id].accounts[acc_id].status) {
        printf(" An inactive account cannot perform transactions!");
        clearBuffer();
        clearScreen();
        return;
    }
    clearScreen();
    int choice;
    do {
        printf("\n ~ Transactions ~\n");
        printf(" 1. Deposit/Withdrawal\n");
        printf(" 2. Transfer\n");
        printf(" 3. Payment\n");
        printf(" 4. Back\n");
        printf(" Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                clearScreen();
                uiTransaction();
                clearBuffer();
                clearScreen();
                break;
            case 2:
                clearScreen();
                uiTransfer();
                clearScreen();
                break;
            case 3:
                clearScreen();
                uiPayment();
                clearScreen();
                break;
            case 4:
                clearScreen();
                break;
            default:
                printf(" Invalid choice. Please enter a valid option.\n");
                clearBuffer();
                clearScreen();
        }
    } while (choice != 4);
}

void accountStatement() {
    /**
      Function that prints information about every transaction in
    current account, sorted by type
    */
    int n = user_list[user_id].accounts[acc_id].num_of_transactions;

    printf("\n ~ %s ~\n\n Balance: %.2lf$\n",
           user_list[user_id].accounts[acc_id].name,
           user_list[user_id].accounts[acc_id].balance);

    if(!n) {
        printf("\n No transactions have been recorded.\n");
        return;
    }

    int k = 0;
    printf("\n~income~\n\n");
    for(int i = n; i >= 1; i--)
        if(strcmp(get_type(user_id, acc_id, i), "income") == 0)
            printf("%2d)date: %02d/%02d/%04d\n   description: %s\n   amount: %.2lf$\n",
            ++k, get_date(user_id, acc_id, i).day, get_date(user_id, acc_id, i).month,
            get_date(user_id, acc_id, i).year, get_description(user_id, acc_id, i),
            get_amount(user_id, acc_id, i));

    k = 0;
    printf("\n~expense~\n\n");
    for(int i = n; i >= 1; i--)
        if(strcmp(get_type(user_id, acc_id, i), "expense") == 0)
            printf("%2d)date: %02d/%02d/%04d\n   description: %s\n   amount: %.2lf$\n\n",
            ++k, get_date(user_id, acc_id, i).day, get_date(user_id, acc_id, i).month,
            get_date(user_id, acc_id, i).year, get_description(user_id, acc_id, i),
            get_amount(user_id, acc_id, i));
}

void accountMenu() {
    int choice;
    do {
        printf("\n ~ %s ~\n", user_list[user_id].accounts[acc_id].name);
        printf(" 1. Record transaction\n");
        printf(" 2. Update\n");
        ///dynamic menu
        if(user_list[user_id].accounts[acc_id].status)
            printf(" 3. Deactivate\n");
        else
            printf(" 3. Activate\n");
        printf(" 4. Statement\n");
        printf(" 5. Back\n");
        printf(" Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                transactionMenu();
                break;
            case 2:
                system("cls");
                uiUpdateAccount();
                clearScreen();
                break;
            case 3:
                uiToggleAccount();
                clearBuffer();
                clearScreen();
                break;
            case 4:
                clearScreen();
                accountStatement();
                clearScreen();
                break;
            case 5:
                deselectAccount();
                clearScreen();
                break;
            default:
                printf(" Invalid choice. Please enter a valid option.\n");
                clearBuffer();
                clearScreen();
        }
    } while (choice != 5);
}

void uiAccountSelection() {
    printf("\n  %s's accounts:\n", user_list[user_id].name);
    printf(" ~-~-~-~-~-~-~\n");
    for(int i = 1; i <= user_list[user_id].num_of_accounts; i++) {
        char str_status[101] = "";
        if(!user_list[user_id].accounts[i].status)
            strcat(str_status, "in");
        strcat(str_status, "active");

        printf(" id: %d\n Name: %s\n Status: %s\n Balance: %.2lf$\n ~-~-~-~-~-~-~\n", i,
            user_list[user_id].accounts[i].name,
            str_status, user_list[user_id].accounts[i].balance);
    }
    int id;
    printf(" Select an account by entering its id: ");
    scanf("%d", &id);
    if(id == 1 || id == 2 || id == 3)
    {
        acc_id = id;
        printf(" %s selected.", user_list[user_id].accounts[acc_id].name);
    }
    if(!acc_id)
        printf(" No account found.\n");
}

void accountSelectionMenu() {
    while(!acc_id) {
        uiAccountSelection();
        clearBuffer();
        clearScreen();
    }
    accountMenu();
}

void viewContacts() {
    printf("\n ~ Contact list ~\n");
    for(int i = 1; i <= num_of_users; i++)
        if(i != user_id)
            printf("  --> %s\n", user_list[i].name);
}

void userMenu() {
    updateAudit(1);
    int choice;
    do {
        printf("\n ~User Menu~\n");
        printf(" 1. Manage Accounts\n");
        printf(" 2. View Contacts\n");
        printf(" 3. Log out\n");
        printf(" Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                system("cls");
                accountSelectionMenu();
                break;
            case 2:
                viewContacts();
                clearBuffer();
                clearScreen();
                break;
            case 3:
                updateAudit(0);
                logOut();
                printf(" Logging out...\n");
                clearBuffer();
                clearScreen();
                break;
            default:
                printf(" Invalid choice. Please enter a valid option.\n");
                clearBuffer();
                clearScreen();
        }
    } while(choice != 3 && choice != 4);
}

void uiSetup() {
    setup();
    printf("Data loaded into memory...\n\n");
}

void uiLogin() {
    char username[101], password[101];
    printf("\n ~~Log in~~\n");
    printf(" Please enter your credentials:\n");
    printf(" Username: ");
    scanf("%s", username);
    printf(" Password: ");
    scanf("%s", password);
    logIn(username, password);
    if(!user_id)
        printf(" Invalid username or password!\n");
    else
        printf(" Welcome, %s!", user_list[user_id].name);
}

void loginMenu() {
    while(!user_id) {
        uiLogin();
        clearBuffer();
        clearScreen();
    }
    userMenu();
}

void runMenu() {
    uiSetup();

    int choice;
    do {
        title();
        printf(" 1 - Log in\n");
        printf(" 2 - Exit\n");
        printf(" Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                system("cls");
                loginMenu();
                break;
            case 2:
                printf("\n Exiting the application...\n");
                break;
            default:
                printf(" Invalid choice. Please enter a valid option.\n");
                clearBuffer();
                clearScreen();
        }
    } while (choice != 2);
}
