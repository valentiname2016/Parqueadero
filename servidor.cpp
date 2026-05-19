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
        // Esta línes permite que pueda percibir cualquier pc
        direccion.sin_addr.s_addr = INADDR_ANY; 
        // Este es el puerto de conexión
        direccion.sin_port = htons(8080);       

        if (bind(server_fd, (struct sockaddr *)&direccion, sizeof(direccion)) < 0) return false;
        if (listen(server_fd, 3) < 0) return false;

        cout << "[C++] Servidor listo. Esperando cliente en puerto 8080...\n";
        return true;
    }

    //función que Python llamará constantemente para ver si llegó una placa
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
        
        int bytes_leidos = recv(socket_cliente, buffer, 1024, MSG_DONTWAIT);
        
        // Si llegaron datos nuevos
        if (bytes_leidos > 0){
            //Se pasa a string la información
            string datos(buffer);
            stringstream ss(datos);
            string serie, hora, s_celda;

            getline(ss, serie, ',');
            getline(ss, hora, ',');
            getline(ss, s_celda, ',');

            if(serie.empty() || hora.empty() || s_celda.empty()) return false;

            int celda_solicitada = stoi(s_celda);


            //Diccionario
            string tipo_accion = "";
            if (celdas.count(serie) > 0){
                celda_solicitada = celdas[serie]; // Recordamos su celda original
                //Se libera el espacio del parqueadero
                celdas.erase(serie);              
                tipo_accion = "SALIDA";
            } else {
                //Se ocupa el espacio del parqueadero
                celdas[serie] = celda_solicitada; 
                tipo_accion = "INGRESO";
            }

            //Se copia la info de la placa para enviarla
            strcpy(resultado->serie, serie.c_str());
            strcpy(resultado->hora, hora.c_str());
            resultado->celda = celda_solicitada;
            strcpy(resultado->accion, tipo_accion.c_str());

            //True para avisar que hay una placa nueva
            return true;
        }
        
        return false;
    }
}