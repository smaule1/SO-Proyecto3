#include <Windows.h>
#include <stdio.h>

int ping(HANDLE port_handle){
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
    }        
    return FALSE;
}


int main(){
    //Crear handle del puerto
    HANDLE port_handle;
    port_handle = CreateFileA("COM2", GENERIC_READ | GENERIC_WRITE,
    0, NULL, OPEN_EXISTING,0, NULL);

    if(port_handle == INVALID_HANDLE_VALUE){
        printf("no sirvió .-.\n");
        char err_msg[256];
        DWORD err_num = GetLastError();
        FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL, err_num, 0, err_msg, sizeof(err_msg), NULL);
        printf("ERROR: %s\n", err_msg);
    }else{
        printf("sí sirvio :)\n");        
    }

    // Configurar puerto COM
    DCB port_config = {0};
    port_config.DCBlength = sizeof(port_config);
    
    int status = GetCommState(port_handle, &port_config);

    //config
    port_config.BaudRate = 9600;
    port_config.ByteSize = 8;
    port_config.Parity = NOPARITY;
    port_config.StopBits = ONESTOPBIT;
    SetCommState(port_handle, &port_config);

    COMMTIMEOUTS timeouts = {0};
    timeouts.ReadIntervalTimeout = 20;
    timeouts.ReadTotalTimeoutMultiplier = 1;
    timeouts.ReadTotalTimeoutConstant = 50;
    timeouts.WriteTotalTimeoutConstant = 50;
    timeouts.WriteTotalTimeoutMultiplier = 1;
    SetCommTimeouts(port_handle, &timeouts);


    
    // Receive Loop
    SetCommMask(port_handle, EV_RXCHAR);
    DWORD event_mask;
    int connected = FALSE;
    while(TRUE){
        WaitCommEvent(port_handle, &event_mask, NULL);
        if(event_mask & EV_RXCHAR){

            if(!connected){ 
                connected = ping(port_handle);
                continue;
            }

            break;

        }
    }
    
    CloseHandle(port_handle);
    return 0;
}



