#include "Globals.h"

int validate_user_data(char username, char password) {
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
