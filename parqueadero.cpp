#include "parqueadero.h"

void Parqueadero::procesarPlaca(string placa)
{
    // Si la placa ya existe
    if(celdas.find(placa) != celdas.end())
    {
        cout << "Placa " << placa << " salio" << endl;

        cout << "Celda liberada: "
             << celdas[placa] << endl;

        celdas.erase(placa);
    }

    // Si no existe
    else
    {
        celdas[placa] = celda;

        cout << "Placa " << placa << " ingreso" << endl;

        cout << "Celda asignada: "
             << celda << endl;

        celda++;
    }
}