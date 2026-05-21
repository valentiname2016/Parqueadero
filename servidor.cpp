#include <iostream>
#include <string>
#include <map>
#include <sstream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

using namespace std;

//Estructura para empaquetar los datos de la placa
struct DatosPlaca {
    char serie[10];
    char hora[10];
    int celda;
    char accion[10]; 
};

//Diccionario con información de la placa, celda
map<string, int> celdas; 
int server_fd = -1;
int socket_cliente = -1;

//Todo lo que esté dentro de extern "C" puede ser leído por Python
extern "C" {

    //función para encender el servidor socket
    bool iniciarServidor() {
        struct sockaddr_in direccion;
        int opt = 1;

        server_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (server_fd < 0) return false;

        setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

        direccion.sin_family = AF_INET;
        //percibe el computador del cliente
        direccion.sin_addr.s_addr = INADDR_ANY; 
        //Puerto de conexión
        direccion.sin_port = htons(8080);       

        if (bind(server_fd, (struct sockaddr *)&direccion, sizeof(direccion)) < 0) return false;
        if (listen(server_fd, 3) < 0) return false;

        cout << "[C++] Servidor listo. Esperando cliente en puerto 8080...\n";
        return true;
    }

    //función de Python para verificar si llegó una placa
    bool recibirPlaca(DatosPlaca* resultado) {
        
        if (socket_cliente == -1) {
            struct sockaddr_in address;
            
            socklen_t addrlen = sizeof(address); 
        
            socket_cliente = accept(server_fd, (struct sockaddr *)&address, &addrlen);
            
            if (socket_cliente < 0) {
                socket_cliente = -1;
                return false;
            }
            cout << "El usuario se ha conectado al socket\n";
        }

            char buffer[1024] = {0};
            
            int bytes_leidos = recv(socket_cliente, buffer, 1023, MSG_DONTWAIT);
            

        if (bytes_leidos > 0) {
            buffer[bytes_leidos] = '\0'; 

            string mensaje_limpio(buffer);
            while (!mensaje_limpio.empty() && (mensaje_limpio.back() == '\n' || mensaje_limpio.back() == '\r')) {
                mensaje_limpio.pop_back();
            }

            cout << "[C++] Procesando mensaje limpio: " << mensaje_limpio << "\n";

            strncpy(buffer, mensaje_limpio.c_str(), sizeof(buffer) - 1);
            buffer[sizeof(buffer) - 1] = '\0';

            char serie_aux[20] = {0};
            char hora_aux[20] = {0};
            char accion_aux[20] = {0};
            int celda_aux = 0;

            int asignados = sscanf(buffer, "%[^,],%[^,],%d,%[^,]", serie_aux, hora_aux, &celda_aux);

            if (asignados >= 3) { 
                resultado->celda = celda_aux;
                
                static char serie_persistente[20];
                static char hora_persistente[20];
                static char accion_persistente[20];
                
                strcpy(serie_persistente, serie_aux);
                strcpy(hora_persistente, hora_aux);
                
                strcpy(accion_persistente, accion_aux); 

                strcpy(resultado->serie, serie_persistente);
                strcpy(resultado->hora, hora_persistente);
                strcpy(resultado->accion, accion_persistente);

                cout << "[C++] Estructura armada con exito para Python. Celda: " << resultado->celda << "\n";
                return true;
            }
        }
        return false;
    }
}