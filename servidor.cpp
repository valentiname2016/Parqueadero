#include <iostream>
#include <string>
#include <map>
#include <sstream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h> // Para configurar sockets no bloqueantes si es necesario

using namespace std;

// Estructura corregida: Ahora usa punteros (char*) para acoplarse con ctypes.c_char_p de Python
struct DatosPlaca {
    const char* serie;
    const char* hora;
    int celda;
    const char* accion; 
};

map<string, int> celdas; 
int server_fd = -1;
int socket_cliente = -1;

extern "C" {

    bool iniciarServidor() {
        struct sockaddr_in direccion;
        int opt = 1;

        server_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (server_fd < 0) return false;

        setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

        direccion.sin_family = AF_INET;
        direccion.sin_addr.s_addr = INADDR_ANY; 
        direccion.sin_port = htons(8080);       

        if (bind(server_fd, (struct sockaddr *)&direccion, sizeof(direccion)) < 0) return false;
        if (listen(server_fd, 3) < 0) return false;

        // Configurar el servidor principal como NO BLOQUEANTE para que accept() no congele la interfaz
        fcntl(server_fd, F_SETFL, O_NONBLOCK);

        cout << "Servidor listo. Esperando cliente en puerto 8080 (Modo No Bloqueante)...\n";
        return true;
    }

    bool recibirPlaca(DatosPlaca* resultado) {
        // Intentar aceptar al cliente si no está conectado
        if (socket_cliente == -1) {
            struct sockaddr_in address;
            socklen_t addrlen = sizeof(address); 
        
            socket_cliente = accept(server_fd, (struct sockaddr *)&address, &addrlen);
            
            if (socket_cliente < 0) {
                socket_cliente = -1; // No hay nadie intentando conectarse aún
                return false;
            }
            // Cuando se conecta, configuramos el canal de lectura como NO BLOQUEANTE también
            fcntl(socket_cliente, F_SETFL, O_NONBLOCK);
            cout << "¡El usuario se ha conectado al socket!\n";
        }

        char buffer[1024] = {0};
        // Leemos con MSG_DONTWAIT para que si no hay datos en ese milisegundo, la función retorne inmediatamente
        int bytes_leidos = recv(socket_cliente, buffer, 1023, MSG_DONTWAIT);
            
        if (bytes_leidos > 0) {
            buffer[bytes_leidos] = '\0'; 

            string mensaje_limpio(buffer);
            while (!mensaje_limpio.empty() && (mensaje_limpio.back() == '\n' || mensaje_limpio.back() == '\r')) {
                mensaje_limpio.pop_back();
            }

            strncpy(buffer, mensaje_limpio.c_str(), sizeof(buffer) - 1);
            buffer[sizeof(buffer) - 1] = '\0';

            char serie_aux[20] = {0};
            char hora_aux[20] = {0};
            int celda_aux = 0;

            // Valentina envía: "SERIE,HORA,CELDA" (3 elementos)
            int asignados = sscanf(buffer, "%[^,],%[^,],%d", serie_aux, hora_aux, &celda_aux);

            if (asignados >= 3) {
                string placa = serie_aux;
                static char accion_aux[20] = {0};

                // Lógica del mapa/diccionario para controlar ingresos y salidas
                if (celdas.find(placa) == celdas.end()) {
                    celdas[placa] = celda_aux;
                    strcpy(accion_aux, "INGRESO");
                } else {
                    celda_aux = celdas[placa];
                    celdas.erase(placa);
                    strcpy(accion_aux, "SALIDA");
                }
                
                // Memoria estática persistente para que Python pueda leer los punteros sin que se borren
                static char serie_persistente[20];
                static char hora_persistente[20];
                
                strcpy(serie_persistente, serie_aux);
                strcpy(hora_persistente, hora_aux);
                
                // Asignamos las direcciones de los punteros de forma exacta
                resultado->celda = celda_aux;
                resultado->serie = serie_persistente;
                resultado->hora = hora_persistente;
                resultado->accion = accion_aux;

                cout << "[C++ exitoso] Placa: " << resultado->serie << " | Accion: " << resultado->accion << " | Celda: " << resultado->celda << "\n";
                return true; 
            }
        } else if (bytes_leidos == 0) {
            // Si recv devuelve 0, significa que Valentina cerró el programa o se desconectó el cable
            cout << "Cliente desconectado. Reabriendo canal de escucha...\n";
            close(socket_cliente);
            socket_cliente = -1;
        }
        return false;
    }
}