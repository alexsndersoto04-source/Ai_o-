#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <cstdint>

// Estructura central de Ai o: Optimizada para mínima huella de RAM
struct MotorEntrenamiento {
    std::unordered_map<uint64_t, std::unordered_map<int, uint32_t>> pesos;

    // Función en línea para máxima velocidad de ejecución
    inline void registrar_trigrama(int t1, int t2, int t3) {
        // Combina dos IDs en una sola clave de 64 bits para ahorrar memoria
        uint64_t clave = (static_cast<uint64_t>(t1) << 32) | static_cast<uint32_t>(t2);
        pesos[clave][t3]++;
    }
};

int main(int argc, char* argv[]) {
    // OPTIMIZACIÓN CRÍTICA: Acelera drásticamente la lectura de archivos.
    // Esto previene que GitHub cancele el proceso por "Timeout".
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    if (argc < 2) {
        std::cerr << "Error: Se requiere el archivo de dataset.\n";
        return 1;
    }

    MotorEntrenamiento motor;
    std::ifstream archivo(argv[1]);
    if (!archivo.is_open()) {
        std::cerr << "Error crítico: No se pudo abrir el dataset " << argv[1] << "\n";
        return 1;
    }

    std::string palabra;
    std::vector<int> tokens;
    std::unordered_map<std::string, int> diccionario;
    int id_actual = 0;

    // FASE 1: Absorción de datos y tokenización secuencial
    while (archivo >> palabra) {
        auto it = diccionario.find(palabra);
        if (it == diccionario.end()) {
            diccionario[palabra] = id_actual;
            tokens.push_back(id_actual);
            id_actual++;
        } else {
            tokens.push_back(it->second);
        }
    }
    archivo.close(); // Liberamos el archivo inmediatamente para ahorrar recursos

    // FASE 2: Construcción de la matriz estructurada
    if (tokens.size() >= 3) {
        for (size_t i = 0; i < tokens.size() - 2; ++i) {
            motor.registrar_trigrama(tokens[i], tokens[i+1], tokens[i+2]);
        }
    }

    // FASE 3: Escritura limpia para acoplarse con los demás archivos y motores
    std::ofstream salida("pesos_aio.txt");
    if (!salida.is_open()) {
        std::cerr << "Error crítico: No se pudo crear el archivo de pesos.\n";
        return 1;
    }

    for (auto const& [clave, transiciones] : motor.pesos) {
        salida << clave << ":";
        for (auto const& [siguiente, conteo] : transiciones) {
            salida << siguiente << "," << conteo << " ";
        }
        salida << "\n";
    }
    salida.close();

    return 0;
}
