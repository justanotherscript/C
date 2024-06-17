#include <stdio.h>
#include <Windows.h>
#include <stdlib.h>

int main() {



	
    // Check if the program runs in Windows
    #ifdef _WIN32

    // Check if the program is Admin
    BOOL isAdmin = FALSE;
    HANDLE hToken = NULL;

    // Open the process token
    if (OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken)) {
        TOKEN_ELEVATION elevation;
        DWORD dwSize = sizeof(TOKEN_ELEVATION);

        // Get the elevation information
        if (GetTokenInformation(hToken, TokenElevation, &elevation, dwSize, &dwSize)) {
            isAdmin = elevation.TokenIsElevated;
        }

        // Close the token handle
        CloseHandle(hToken);
    }

    if (!isAdmin) {
        // Display a popup message
        MessageBox(NULL, "This program must be run as an administrator.", "Error", MB_ICONERROR);

        // Exit the program
        ExitProcess(1);
    }

    // Rest of your program code here...

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


// Checking for mac OS with 
// __APPLE__ macro 
#elif __APPLE__ 

    // Check if the effective user ID is not equal to 0 (root user)
    if (geteuid() != 0) {
    printf("This program requires root privileges to run.\n");
    exit(1);
	}

	

       // Check if the program is in startup
    if(system("ls /Library/StartupItems/USBGuard") != 0) {
        // Copy the program to startup
        system("cp USBGuard /Library/StartupItems/USBGuard");
        printf("Program copied to startup successfully.\n");
    } else {
        printf("Program is already set to run at startup.\n");
    }

// If neither of them is present 
// then this is printed... 
#else 
	printf("Sorry, the system is"
		"not  supported yet.\n"); 
#endif 

    return 0;
}
