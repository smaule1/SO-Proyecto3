#include <Windows.h>
#include <stdio.h>
#include <powrprof.h>
#include <stdlib.h>
#pragma comment(lib, "PowrProf.lib")

/*int ping(HANDLE port_handle){
    char receive_buffer[64] = {0};
    DWORD bytes_received;
    ReadFile(port_handle, receive_buffer, sizeof(receive_buffer),
    &bytes_received, NULL);
    if(bytes_received > 0){
        if(strcmp(receive_buffer, "ping") == 0){
            char* str = "pong";
            WriteFile(port_handle, str, strlen(str), NULL, NULL);
            return TRUE;
        }
        printf("%s\n", receive_buffer);
    }
    return FALSE;
}*/

int read_btn(HANDLE port_handle) {
    char receive_buffer[64] = { 0 };
    int value;
    DWORD bytes_received;
    ReadFile(port_handle, receive_buffer, sizeof(receive_buffer),
        &bytes_received, NULL);
    if (bytes_received > 0) {
        printf("%s\n", receive_buffer);
        if(receive_buffer[0] >= '1' && receive_buffer[0] <= '4') {
            WriteFile(port_handle, "r", 1, NULL, NULL);
        }

        if (strcmp(receive_buffer, "1") == 0) {
            ShellExecute(NULL, L"open", L"taskmgr.exe", NULL, NULL, SW_SHOWNORMAL);
            return TRUE;
        }
        else if (strcmp(receive_buffer, "2") == 0) {
            SetSuspendState(FALSE, TRUE, FALSE);
        }
        else if (strcmp(receive_buffer, "3") == 0) {
            system("shutdown /r /t 0");
        }
        else if (strcmp(receive_buffer, "4") == 0) {
            system("shutdown /s /t 0");
        }
        return TRUE;
    }
    return FALSE;
}



int main() {
    //Crear handle del puerto
    HANDLE port_handle;
    port_handle = CreateFileA("COM6", GENERIC_READ | GENERIC_WRITE,
        0, NULL, OPEN_EXISTING, 0, NULL);

    if (port_handle == INVALID_HANDLE_VALUE) {
        printf("Error\n");
        char err_msg[256];
        DWORD err_num = GetLastError();
        FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL, err_num, 0, err_msg, sizeof(err_msg), NULL);
        printf("ERROR: %s\n", err_msg);
    }
    else {
        printf("Connected to COM port\n");
    }

    // Configurar puerto COM
    DCB port_config = { 0 };
    port_config.DCBlength = sizeof(port_config);

    int status = GetCommState(port_handle, &port_config);    

    //config
    port_config.BaudRate = 9600;
    port_config.ByteSize = 8;
    port_config.Parity = NOPARITY;
    port_config.StopBits = ONESTOPBIT;
    SetCommState(port_handle, &port_config);    

    COMMTIMEOUTS timeouts = { 0 };
    timeouts.ReadIntervalTimeout = 20;
    timeouts.ReadTotalTimeoutMultiplier = 1;
    timeouts.ReadTotalTimeoutConstant = 50;
    timeouts.WriteTotalTimeoutConstant = 50;
    timeouts.WriteTotalTimeoutMultiplier = 1;
    SetCommTimeouts(port_handle, &timeouts);    

    // Receive Loop
    SetCommMask(port_handle, EV_RXCHAR);    
    DWORD event_mask = 0;
    while (TRUE) {
        status = WaitCommEvent(port_handle, &event_mask, NULL);        
        
        if (event_mask & EV_RXCHAR) {            
            read_btn(port_handle);
        }
    }
    CloseHandle(port_handle);
    return 0;
}
