%module parqueadero

%{
#include "placa.h"
#include "parqueadero.h"
#include "generador.h"
#include "cliente.h"
#include "servidor.h"
%}

%include "std_string.i"
%include "std_map.i"

%include "parqueadero.h"
%include "generador.h"
%include "placa.h"
%include "cliente.h"
%include "servidor.h"