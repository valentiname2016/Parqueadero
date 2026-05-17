#include <string>
using namespace std;

class Placa{

    private:
        string serie;
        string hora;
        int celda;
    
    public:
        Placa();
        void generarAleatoria();
        string placaTexto();
        string getSerie();
        string getHora();
        int getCelda(); 

};