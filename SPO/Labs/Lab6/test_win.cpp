#include <windows.h>
#include <iostream>

int main() {
    char username[100];
    DWORD username_len = 100;
    std::cout << GetUserNameA(username, &username_len);
    std::cout << username;
}