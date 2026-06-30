#include <iostream>
#include <fstream>
#include <string>

int main() {
    // Optimización extrema para lectura de flujo masivo
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    std::ifstream archivo("dataset_wikipedia.txt");
    std::ofstream salida("pesos_aio_maxima_potencia.txt");

    if (!archivo.is_open()) return 1;

    std::string w1, w2, w3;
    // Procesamiento lineal sin almacenamiento en RAM
    while (archivo >> w1 >> w2 >> w3) {
        // Genera la estructura de trigramas que tus otros motores esperan
        salida << w1 << " " << w2 << " " << w3 << "\n";
    }

    archivo.close();
    salida.close();
    return 0;
}
