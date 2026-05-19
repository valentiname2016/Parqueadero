#include "Parqueadero.h"

void Parqueadero::registrarEntrada(Placa p)
{
    celdas[p.getCelda()] = p;
}

void Parqueadero::liberarCelda(int celda)
{
    celdas.erase(celda);
}

bool Parqueadero::placaExiste(std::string numero)
{
    for (auto &par : celdas)
    {
        if (par.second.getNumero() == numero)
            return true;
    }
    return false;
}

void Parqueadero::procesarPlaca(Placa p)
{
    if (!placaExiste(p.getNumero()))
        registrarEntrada(p);
}