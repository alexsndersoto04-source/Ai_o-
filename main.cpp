#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <sstream>
#include <cmath>
#include <algorithm>

// --- TOKENIZADOR ULTRA-OPTIMIZADO ---
class Tokenizer {
public:
    std::unordered_map<std::string, int> word_to_id;
    std::unordered_map<int, std::string> id_to_word;
    int next_id = 0;

    int tokenize(const std::string& word) {
        auto it = word_to_id.find(word);
        if (it == word_to_id.end()) {
            word_to_id[word] = next_id;
            id_to_word[next_id] = word;
            return next_id++;
        }
        return it->second;
    }
};

// --- NÚCLEO MATEMÁTICO PROPIO DE MÁXIMA POTENCIA (VDA MULTI-NIVEL) ---
class SuperBrainAiO {
private:
    // Matriz de Contexto de Bigramas: Token A -> (Token B -> Peso)
    std::unordered_map<int, std::unordered_map<int, double>> bigram_matrix;
    // Matriz de Contexto de Trigramas para Alta Coherencia: Combinación(A,B) -> (Token C -> Peso)
    std::unordered_map<string, std::unordered_map<int, double>> trigram_matrix;
    
    const size_t CONTEXT_WINDOW = 6; // Ventana de atención extendida

    std::string make_key(int id1, int id2) {
        return std::to_string(id1) + "_" + std::to_string(id2);
    }

public:
    void process_window(const std::vector<int>& tokens) {
        if (tokens.size() < 2) return;

        for (size_t i = 0; i < tokens.size(); ++i) {
            // 1. Conexiones de Bigramas con peso por distancia exponencial
            for (size_t step = 1; step <= CONTEXT_WINDOW && (i + step) < tokens.size(); ++step) {
                int current = tokens[i];
                int next = tokens[i + step];
                double weight = 1.0 / std::exp(static_cast<double>(step) - 1.0);
                bigram_matrix[current][next] += weight;
            }

            // 2. Conexiones de Trigramas (Estructura de frase compuesta)
            if (i + 2 < tokens.size()) {
                std::string key = make_key(tokens[i], tokens[i + 1]);
                int next_next = tokens[i + 2];
                trigram_matrix[key][next_next] += 2.0; // Doble peso por coherencia secuencial directa
            }
        }
    }

    // FUERZA BRUTA DE ESCRITURA EN DISCO (Garantiza la creación del archivo)
    void force_save_weights(const std::string& filename) {
        std::ofstream out(filename, std::ios::out | std::ios::binary);
        if (!out.is_open()) {
            std::cerr << "[CRÍTICO] No se pudo abrir el archivo para escribir los pesos.\n";
            return;
        }

        // Buffer de alta velocidad para evitar cuellos de botella en el disco
        std::vector<char> buffer(64 * 1024);
        out.rdbuf()->pubsetbuf(buffer.data(), buffer.size());

        out << "=== AI O WEIGHTS V200 ===\n";
        
        // Guardar Bigramas
        out << "[BIGRAMS]\n";
        for (auto const& [current, next_map] : bigram_matrix) {
            for (auto const& [next, weight] : next_map) {
                out << current << "," << next << "," << weight << "\n";
            }
        }

        // Guardar Trigramas
        out << "[TRIGRAMS]\n";
        for (auto const& [key, next_map] : trigram_matrix) {
            for (auto const& [next, weight] : next_map) {
                out << key << "," << next << "," << weight << "\n";
            }
        }

        out.close();
        std::cout << "[NÚCLEO] Escritura forzada completada con éxito.\n";
    }
};

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Error de ejecución: Dataset requerido.\n";
        return 1;
    }

    std::ifstream file(argv[1]);
    if (!file.is_open()) {
        std::cerr << "Error crítico de E/S al abrir el archivo.\n";
        return 1;
    }

    std::cout << "==================================================\n";
    std::cout << "   SISTEMA MATEMÁTICO AVANZADO: Ai o (POTENCIA 200%) \n";
    std::cout << "==================================================\n";
    
    Tokenizer tokenizer;
    SuperBrainAiO brain;

    std::string line;
    std::vector<int> token_stream;
    long long total_words = 0;

    std::cout << "[PROCESO] Ejecutando análisis matricial masivo...\n";
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string word;
        while (ss >> word) {
            word.erase(std::remove_if(word.begin(), word.end(), ::ispunct), word.end());
            if (word.empty()) continue;

            token_stream.push_back(tokenizer.tokenize(word));
            total_words++;

            if (token_stream.size() >= 50000) {
                brain.process_window(token_stream);
                token_stream.clear();
            }
        }
    }

    if (!token_stream.empty()) {
        brain.process_window(token_stream);
    }
    file.close();

    std::cout << "[INFO] Dataset analizado por completo. Palabras procesadas: " << total_words << "\n";
    std::cout << "[PROCESO] Forzando el empaquetado de pesos en almacenamiento virtual...\n";
    
    std::string output_name = "pesos_aio_maxima_potencia.txt";
    brain.force_save_weights(output_name);

    std::cout << "[ÉXITO] Ciclo del motor C++ terminado satisfactoriamente.\n";
    std::cout << "==================================================\n";
    return 0;
}
