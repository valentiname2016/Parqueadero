#include "generador.h"

Placa generador::generarPlaca() {
    Placa nuevaPlaca;
    nuevaPlaca.generarAleatoria();
    return nuevaPlaca;
}