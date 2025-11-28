#include <Windows.h>
#include <initguid.h>
#include <mmdeviceapi.h>
#include <endpointvolume.h>
#include <stdio.h>
#include <objbase.h>

DEFINE_GUID(CLSID_MMDeviceEnumerator, 
0xbcde0395, 0xe52f, 0x467c, 0x8e, 0x3d, 0xc4, 0x57, 0x92, 0x91, 0x69, 0x2e);

DEFINE_GUID(IID_IMMDeviceEnumerator, 
0xa95664d2, 0x9614, 0x4f35, 0xa7, 0x46, 0xde, 0x8d, 0xb6, 0x36, 0x17, 0xe6);

DEFINE_GUID(IID_IAudioEndpointVolume, 
0x5cdf2c82, 0x841e, 0x4546, 0x97, 0x22, 0x0c, 0xf7, 0x2, 0x5, 0x9f, 0x7e);

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
        printf("%s\n", receive_buffer);
    }        
    return FALSE;
}

void set_volume(float volume) {
    HRESULT hr = CoInitialize(NULL);
    IMMDeviceEnumerator *enumerator = NULL;
    IMMDevice *device = NULL;

    //LPCWSTR hyperx_id = L"{0.0.0.00000000}.{D61CB7B5-14D7-4846-AB59-ADCFDD7F21A3}";
    
    hr = CoCreateInstance( &CLSID_MMDeviceEnumerator, NULL, CLSCTX_ALL, &IID_IMMDeviceEnumerator, (void**)&enumerator);

    if (FAILED(hr)) {
        printf("CoCreateInstance failed\n");
        CoUninitialize();
        return;
    }
    //hr = enumerator->lpVtbl->GetDevice(enumerator, hyperx_id, &device);
    hr = enumerator->lpVtbl->GetDefaultAudioEndpoint(enumerator, eRender, eConsole, &device);

    if (FAILED(hr)) {
        printf("GetDevice failed\n");
        CoUninitialize();
        return;
    }

    IAudioEndpointVolume *endpoint = NULL;
    hr = device->lpVtbl->Activate(device, &IID_IAudioEndpointVolume, CLSCTX_INPROC_SERVER, NULL, (void**)&endpoint);

    if (FAILED(hr)) {
        printf("Activate(IAudioEndpointVolume) failed\n", hr);
        CoUninitialize();
        return;
    }
    hr = endpoint->lpVtbl->SetMasterVolumeLevelScalar(endpoint, volume, NULL);
    endpoint->lpVtbl->Release(endpoint);
    device->lpVtbl->Release(device);
    enumerator->lpVtbl->Release(enumerator);
    CoUninitialize();
}

int read_volume_dial(HANDLE port_handle){
    char receive_buffer[64] = {0};
    int value;
    DWORD bytes_received;
    ReadFile(port_handle, receive_buffer, sizeof(receive_buffer),
    &bytes_received, NULL);
    if(bytes_received > 0){
        /*if(strcmp(receive_buffer, "ping") == 0){            
            char* str = "pong";
            WriteFile(port_handle, str, strlen(str), NULL, NULL);
            return TRUE;
        }*/
       printf("%s\n", receive_buffer);
       value = atoi(receive_buffer);
       float volume = (float)value/1023;
       printf("cambia volumen a %.1f\n", volume);
       set_volume(volume);
       return TRUE;
    }
    return FALSE;
}



int main(){
    //Crear handle del puerto
    HANDLE port_handle;
    port_handle = CreateFileA("COM7", GENERIC_READ | GENERIC_WRITE,
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
                printf("Hizo pong? %d\n", connected);
                continue;
            } else {
                printf("Va a cambiar\n");
                read_volume_dial(port_handle);
            }
        }
    }
    
    CloseHandle(port_handle);
    return 0;
}



