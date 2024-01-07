#include "Domain.c"
#include "Validation.c"

void login(char username, char password) {
    int user_id = validate_user_data(username, password);
}
