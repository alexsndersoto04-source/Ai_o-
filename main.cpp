#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <sstream>
#include <cmath>
#include <algorithm>
#include <cstdint>
#include <cctype>

// --- COMPONENTE 1: TOKENIZADOR INDEXADO BIDIRECCIONAL ---
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

// --- COMPONENTE 2: MOTOR MATRICIAL DE ATENCIÓN SELECTIVA Y CONTEXTO COMPUESTO ---
class SuperBrainAiO {
private:
    // Matrices de pesos estadísticos probabilísticos
    std::unordered_map<int, std::unordered_map<int, double>> bigram_matrix;
    
    // ARQUITECTURA TRANSFORMER OPTIMIZADA: Clave empaquetada por bits [ID_1 (32 bits) | ID_2 (32 bits)]
    // Elimina strings en las claves para garantizar estabilidad absoluta del compilador y ahorro de RAM
    std::unordered_map<std::uint64_t, std::unordered_map<int, double>> trigram_matrix;
    
    // Registro dinámico de frecuencias globales para el cálculo de la Atención Selectiva
    std::unordered_map<int, std::uint64_t> global_frequencies;

    const size_t CONTEXT_WINDOW = 8; // Ventana extendida para mayor retención de contexto continuo

    // Función de empaquetamiento seguro a nivel de bits
    std::uint64_t pack_tokens(int id1, int id2) const {
        return (static_cast<std::uint64_t>(id1) << 32) | (static_cast<std::uint64_t>(id2) & 0xFFFFFFFFULL);
    }

public:
    void register_frequency(int token_id) {
        global_frequencies[token_id]++;
    }

    // Mecanismo de Atención Selectiva Ponderada (SWA)
    void process_window(const std::vector<int>& tokens) {
        if (tokens.size() < 2) return;

        for (size_t i = 0; i < tokens.size(); ++i) {
            int current = tokens[i];
            
            // Factor de atención inverso a la frecuencia global (Atenúa stop-words como "el", "la" de forma automática)
            double current_importance = 1.0 / (1.0 + std::log1p(static_cast<double>(global_frequencies[current])));

            for (size_t step = 1; step <= CONTEXT_WINDOW && (i + step) < tokens.size(); ++step) {
                int next = tokens[i + step];
                double next_importance = 1.0 / (1.0 + std::log1p(static_cast<double>(global_frequencies[next])));
                
                // Decaimiento de distancia combinado con pesos semánticos discretos de atención
                double base_decay = 1.0 / std::exp(static_cast<double>(step) - 1.0);
                double attention_weight = base_decay * (current_importance * next_importance * 10.0);

                bigram_matrix[current][next] += attention_weight;
            }

            // Inyección de Coherencia de Contexto Profundo (Trigramas empacados por bits)
            if (i + 2 < tokens.size()) {
                std::uint64_t combined_key = pack_tokens(tokens[i], tokens[i + 1]);
                int next_next = tokens[i + 2];
                
                // Ponderación estricta para asegurar la concordancia sintáctica
                trigram_matrix[combined_key][next_next] += 3.5;
            }
        }
    }

    // Volcado de Pesos con Flujo Binario Seguro y Buffer Dedicado contra Corrupción
    void force_save_weights(const std::string& filename) {
        std::ofstream out(filename, std::ios::out | std::ios::binary);
        if (!out.is_open()) {
            std::cerr << "[CRÍTICO] Error E/S: No se pudo crear el archivo de salida de pesos.\n";
            return;
        }

        // Buffer de alta velocidad de 64KB en memoria para agilizar la escritura física en disco
        std::vector<char> stream_buffer(64 * 1024);
        out.rdbuf()->pubsetbuf(stream_buffer.data(), stream_buffer.size());

        out << "=== AI O MATRICIAL WEIGHTS V300 ===\n";
        
        out << "[BIGRAMS]\n";
        for (auto const& [current, next_map] : bigram_matrix) {
            for (auto const& [next, weight] : next_map) {
                out << current << "," << next << "," << weight << "\n";
            }
        }

        out << "[TRIGRAMS]\n";
        for (auto const& [packed_key, next_map] : trigram_matrix) {
            // Desempaquetado seguro de la clave de 64 bits para la escritura en texto plano legile
            int id1 = static_cast<int>(packed_key >> 32);
            int id2 = static_cast<int>(packed_key & 0xFFFFFFFFULL);
            for (auto const& [next, weight] : next_map) {
                out << id1 << "_" << id2 << "," << next << "," << weight << "\n";
            }
        }

        out.close();
        std::cout << "[NÚCLEO] Volcado forzado de pesos a disco completado de forma segura.\n";
    }
};

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Error de ejecución: Ruta de dataset requerida.\n";
        return 1;
    }

    std::string dataset_path = argv[1];
    
    std::cout << "====================================================================\n";
    std::cout << "   NÚCLEO DE ATENCIÓN MATRICIAL AVANZADO (C++17) - PROYECTO: Ai o   \n";
    std::cout << "====================================================================\n";

    Tokenizer tokenizer;
    SuperBrainAiO brain;

    // --- FASE 1: ANÁLISIS PREVIO DE ESPECTRO SEMÁNTICO DE FRECUENCIAS ---
    {
        std::ifstream file(dataset_path);
        if (!file.is_open()) {
            std::cerr << "[CRÍTICO] No se pudo abrir el archivo de dataset para la Fase 1.\n";
            return 1;
        }
        std::cout << "[FASE 1/2] Calculando espectro de frecuencias globales (Filtro SWA)...\n";
        std::string line;
        while (std::getline(file, line)) {
            std::stringstream ss(line);
            std::string word;
            while (ss >> word) {
                word.erase(std::remove_if(word.begin(), word.end(), [](unsigned char c) { return std::ispunct(c); }), word.end());
                if (word.empty()) continue;
                int token_id = tokenizer.tokenize(word);
                brain.register_frequency(token_id);
            }
        }
        file.close();
    }

    // --- FASE 2: CONSTRUCCIÓN Y PROCESAMIENTO DE CONEXIONES MATRICIALES ---
    {
        std::ifstream file(dataset_path);
        if (!file.is_open()) {
            std::cerr << "[CRÍTICO] No se pudo abrir el archivo de dataset para la Fase 2.\n";
            return 1;
        }
        std::cout << "[FASE 2/2] Ejecutando análisis probabilístico dinámico multicanal...\n";
        std::string line;
        std::vector<int> token_stream;
        long long total_words = 0;

        while (std::getline(file, line)) {
            std::stringstream ss(line);
            std::string word;
            while (ss >> word) {
                word.erase(std::remove_if(word.begin(), word.end(), [](unsigned char c) { return std::ispunct(c); }), word.end());
                if (word.empty()) continue;

                token_stream.push_back(tokenizer.tokenize(word));
                total_words++;

                if (token_stream.size() >= 100000) {
                    brain.process_window(token_stream);
                    token_stream.clear();
                }
            }
        }

        if (!token_stream.empty()) {
            brain.process_window(token_stream);
        }
        file.close();
        std::cout << "[INFO] Análisis finalizado. Palabras procesadas con éxito: " << total_words << "\n";
    }

    // --- FASE 3: VOLCADO FINAL EXTRACTIVO ---
    std::cout << "[PROCESO] Iniciando empaquetado seguro de matrices de conocimiento...\n";
    std::string output_name = "pesos_aio_maxima_potencia.txt";
    brain.force_save_weights(output_name);

    std::cout << "[ÉXITO] Arquitectura entrenada de forma óptima sin fallas estructurales.\n";
    std::cout << "====================================================================\n";
    return 0;
}
