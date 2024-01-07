#include <string.h>
#include "entities.h"
#include "globals.c"

void setup_users() {
    FILE *file = fopen("user_data.txt", "r");

    char name[101], pass[101], file[101];
    while(fscanf("%s %s %s", name, pass, file) == 3) {
        strcpy(user_list[++num_of_users].name, name);
        strcpy(user_list[++num_of_users].pass, pass);
        strcpy(user_list[++num_of_users].file, file);
    }
}
