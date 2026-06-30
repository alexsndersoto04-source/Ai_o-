#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <cstdint>

// Optimizamos la estructura de la matriz usando una clave única de 64 bits para los dos primeros tokens
// Clave = (t1 << 32) | t2. El tercer token (t3) apunta al contador.
std::unordered_map<uint64_t, std::unordered_map<int, int>> pesos;

int main() {
    // REGLA DE ORO DE VELOCIDAD: Desactivamos la sincronización con C para lectura ultra rápida
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    std::string nombre_archivo = "dataset_wikipedia.txt";
    std::ifstream archivo(nombre_archivo);

    if (!archivo.is_open()) {
        std::cerr << "Error crítico: No se pudo abrir el archivo " << nombre_archivo << "\n";
        return 1;
    }

    std::cout << "Iniciando Motor 5 de Ai o (Flujo Lineal de Streaming)...\n";
    std::cout << "Procesando dataset directamente en flujo continuo para proteger la RAM...\n";

    int t1 = -1, t2 = -1, t3 = -1;
    long long trigramas_procesados = 0;

    // Procesamiento en flujo continuo (Streaming)
    // Guardamos solo los IDs numéricos necesarios para la ventana del trigrama
    while (archivo >> t3) {
        if (t1 != -1 && t2 != -1) {
            // Combinamos t1 y t2 en una sola clave eficiente de 64 bits
            uint64_t clave = (static_cast<uint64_t>(t1) << 32) | static_cast<uint32_t>(t2);
            
            // Registramos el aprendizaje directamente en el "cuaderno" de pesos
            pesos[clave][t3]++;
            trigramas_procesados++;

            // Cada 10 millones de trigramas mostramos un progreso para saber que sigue vivo
            if (trigramas_procesados % 10000000 == 0) {
                std::cout << "-> Conexiones matemáticas registradas: " << trigramas_procesados << "\n";
            }
        }
        // Desplazamiento lineal: Desechamos el token antiguo y movemos la ventana hacia adelante
        t1 = t2;
        t2 = t3;
    }

    archivo.close(); // El archivo se cierra, la RAM del texto está totalmente limpia
    std::cout << "¡Absorción completada con éxito! Total de trigramas: " << trigramas_procesados << "\n";
    std::cout << "Escribiendo matriz de conocimiento en pesos_aio.txt...\n";

    // FASE DE ESCRITURA: Volcamos el cuaderno de la RAM al archivo físico final
    std::ofstream archivo_salida("pesos_aio.txt");
    if (!archivo_salida.is_open()) {
        std::cerr << "Error al crear el archivo de salida de pesos.\n";
        return 1;
    }

    for (const auto& par_primeros : pesos) {
        uint64_t clave = par_primeros.first;
        // Reconstruimos los IDs originales de t1 y t2 a partir de la clave de 64 bits
        int orig_t1 = static_cast<int>(clave >> 32);
        int orig_t2 = static_cast<int>(clave & 0xFFFFFFFF);

        for (const auto& par_tercero : par_primeros.second) {
            int orig_t3 = par_tercero.first;
            int contador = par_tercero.second;

            // Formato compacto: t1 t2 t3 contador
            archivo_salida << orig_t1 << " " << orig_t2 << " " << orig_t3 << " " << contador << "\n";
        }
    }

    archivo_salida.close();
    std::cout << "=== PROCESO FINALIZADO CON ÉXITO: pesos_aio.txt ESTÁ LISTO ===\n";

    return 0;
}
