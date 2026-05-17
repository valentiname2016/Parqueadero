#include <iostream>
#include <chrono>   // Para el reloj del sistema
#include <ctime>    // Para convertir a tiempo local
#include <iomanip>  // Para usar std::put_time
#include <sstream>  // Para poder guardar el formato en un string
#include "placa.h"

using namespace std;

Placa::Placa(){

    serie = "";
    hora = "";
    celda = 0;
}

void Placa::generarAleatoria(){

    serie = "";

    for(int i=0; i<3; i++){
        serie += 'A' + rand()%26;
    }

    for(int i=0; i<3; i++){
        serie += '0' + rand()%10;
    }

    //Se captura la hora actual
    auto ahora = std::chrono::system_clock::now();
    
    //Se convierte al formato de tiempo de C y luego al tiempo local
    std::time_t tiempo_c = std::chrono::system_clock::to_time_t(ahora);
    std::tm* tiempo_local = std::localtime(&tiempo_c);
    
    //Se usa un stringstream() para formatear la hora como texto
    std::stringstream ss;
    ss << std::put_time(tiempo_local, "%H:%M:%S");
    
    //Se guarda en hora
    hora = ss.str();

    celda = rand()%10 + 1;

}