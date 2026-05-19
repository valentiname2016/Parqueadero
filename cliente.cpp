#define _WIN32_WINNT 0x0600
#include <iostream>
#include <string>
#include <cstring>
#include <winsock2.h>   // Librería para socket
#include <ws2tcpip.h>   // Para IPs
#include <ctime>
#include "generador.h"  
 
// Parsa usar librería de red (Ws2_32.lib)
#pragma comment(lib, "ws2_32.lib")
 
using namespace std;
 
int main() {
    //Para conexion a servidor
    string ip_servidor = "192.168.1.1";
    int puerto = 8080;
 
    // Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cout << "Error al inicializar Winsock en Windows.\n";
        return -1;
    }
 
    SOCKET socket_cliente = INVALID_SOCKET;
    struct sockaddr_in direccion_servidor;
 
    // Crear socket
    socket_cliente = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_cliente == INVALID_SOCKET) {
        cout << "Error al crear el socket en Windows. Error: " << WSAGetLastError() << "\n";
        WSACleanup();
        return -1;
    }
 
    // Configuracijón para conectar al servidor
    direccion_servidor.sin_family = AF_INET;
    direccion_servidor.sin_port = htons(puerto);
 

    direccion_servidor.sin_addr.s_addr = inet_addr(ip_servidor.c_str());
    if (direccion_servidor.sin_addr.s_addr == INADDR_NONE) {
        cout << "Direccion IP invalida o no soportada.\n";
        closesocket(socket_cliente);
        WSACleanup();
        return -1;
    }
 
    // Conexión entre computadora cliente y servidor
    cout << "Intentando conectar al parqueadero (IP: " << ip_servidor << ")...\n";
    if (connect(socket_cliente, (struct sockaddr *)&direccion_servidor, sizeof(direccion_servidor)) == SOCKET_ERROR) {
        cout << "Error: No se pudo conectar.\n";
        closesocket(socket_cliente);
        WSACleanup();
        return -1;
    }
 
    cout << "Conectado exitosamente al servidor\n";
 
    
    generador mi_generador;
    srand(time(0));
 
    cout << "Iniciando el envio de placas\n";
 
    
    while (true) {
        
        Placa nueva_placa = mi_generador.generarPlaca();
        
        string mensaje = nueva_placa.placaTexto();
 
        cout << "[Cliente Windows] Enviando: " << mensaje << "\n";
 
        send(socket_cliente, mensaje.c_str(), mensaje.length(), 0);
 
        Sleep(5000);
    }
 
    closesocket(socket_cliente);
    WSACleanup();
    return 0;
}