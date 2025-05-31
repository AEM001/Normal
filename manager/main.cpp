#include "include/system.h"
#include <iostream>
#include <windows.h>

int main() {
    // Set console code page to UTF-8 so console correctly prints unicode characters
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    System system;
    system.run();
    return 0;
}
