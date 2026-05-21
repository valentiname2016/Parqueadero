#ifndef PARQUEADERO_H
#define PARQUEADERO_H

#include <iostream>
#include <map>

using namespace std;

class Parqueadero
{
private:
    map<string, int> celdas;
    int celda = 1;

public:
    void procesarPlaca(string placa);
};

#endif