#ifndef PARQUEADERO_H
#define PARQUEADERO_H

#include "placa.h"
#include <map>

class Parqueadero
{
private:
    std::map<int, Placa> celdas;

public:
    void registrarEntrada(Placa p);
    void liberarCelda(int celda);
    bool placaExiste(std::string numero);
    void procesarPlaca(Placa p);
};

#endif