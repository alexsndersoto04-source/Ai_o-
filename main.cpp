#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <cstdint>

// Usamos un optimizador para convertir palabras (strings) en IDs numéricos únicos en caliente para no saturar la RAM
std::unordered_map<std::string, int> vocabulario;
int proximo_id = 0;

// Estructura ultra-compacta para los pesos de los trigramas
// Clave de 64 bits = (id_palabra1 << 32) | id_palabra2
std::unordered_map<uint64_t, std::unordered_map<int, int>> pesos;

// Función rápida para obtener o registrar el ID de una palabra
inline int obtener_id(const std::string& palabra) {
    auto it = vocabulario.find(palabra);
    if (it != vocabulario.end()) {
        return it->second;
    }
    vocabulario[palabra] = proximo_id;
    return proximo_id++;
}

int main() {
    // Desactivamos sincronización para lectura a máxima velocidad de disco
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    std::string nombre_archivo = "dataset_wikipedia.txt";
    std::ifstream archivo(nombre_archivo);

    if (!archivo.is_open()) {
        std::cerr << "Error critico: No se pudo abrir el archivo " << nombre_archivo << "\n";
        return 1;
    }

    std::cout << "Iniciando Motor 5 Real de Ai o (Streaming de Texto Plano)...\n";
    std::cout << "Procesando dataset de 12GB en flujo continuo...\n";

    std::string palabra_actual;
    int t1 = -1, t2 = -1, t3 = -1;
    long long trigramas_procesados = 0;

    // LEER TEXTO PLANO PALABRA POR PALABRA
    while (archivo >> palabra_actual) {
        // Convertimos la palabra de texto a su ID numérico inmediatamente
        t3 = obtener_id(palabra_actual);

        if (t1 != -1 && t2 != -1) {
            // Combinamos t1 y t2 en una clave única de 64 bits
            uint64_t clave = (static_cast<uint64_t>(t1) << 32) | static_cast<uint32_t>(t2);
            
            // Registramos el aprendizaje en el cuaderno de pesos
            pesos[clave][t3]++;
            trigramas_procesados++;

            if (trigramas_procesados % 10000000 == 0) {
                std::cout << "-> Conexiones matematicas registradas: " << trigramas_procesados << "\n";
            }
        }
        // Desplazamiento de la ventana de contexto
        t1 = t2;
        t2 = t3;
    }

    archivo.close();
    std::cout << "Absorcion completada. Total de trigramas reales procesados: " << trigramas_procesados << "\n";

    // NOMBRE EXACTO QUE BUSCA GITHUB ACTIONS
    std::string nombre_salida = "pesos_aio_maxima_potencia.txt";
    std::cout << "Escribiendo matriz de conocimiento en " << nombre_salida << "...\n";

    std::ofstream archivo_salida(nombre_salida);
    if (!archivo_salida.is_open()) {
        std::cerr << "Error al crear el archivo de salida.\n";
        return 1;
    }

    // Volcado del conocimiento acumulado al almacenamiento físico
    for (const auto& par_primeros : pesos) {
        uint64_t clave = par_primeros.first;
        int orig_t1 = static_cast<int>(clave >> 32);
        int orig_t2 = static_cast<int>(clave & 0xFFFFFFFF);

        for (const auto& par_tercero : par_primeros.second) {
            int orig_t3 = par_tercero.first;
            int contador = par_tercero.second;

            // Formato: ID1 ID2 ID3 Frecuencia
            archivo_salida << orig_t1 << " " << orig_t2 << " " << orig_t3 << " " << contador << "\n";
        }
    }

    archivo_salida.close();
    std::cout << "=== PROCESO FINALIZADO CON EXITO: " << nombre_salida << " ESTA LISTO ===\n";

    return 0;
}
