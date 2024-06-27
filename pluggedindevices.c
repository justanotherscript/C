#include <Windows.h>
#include <stdio.h>

int main() {
    HKEY hKey;
    if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Enum", 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
        // Successfully opened the registry key, now you can work with it
        RegCloseKey(hKey); // Don't forget to close the key when done
    } else {
        printf("Error opening registry key\n");
    }
    return 0;
}
