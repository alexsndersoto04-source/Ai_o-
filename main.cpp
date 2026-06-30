#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <sstream>
#include <algorithm>
#include <cstdint>

class Tokenizer {
public:
    std::unordered_map<std::string, int> word_to_id;
    int next_id = 0;
    int tokenize(const std::string& word) {
        if (word_to_id.find(word) == word_to_id.end()) word_to_id[word] = next_id++;
        return word_to_id[word];
    }
};

class AiOMotor {
public:
    std::unordered_map<std::uint64_t, std::unordered_map<int, double>> trigram_matrix;

    void process_stream(const std::vector<int>& tokens) {
        if (tokens.size() < 3) return;
        for (size_t i = 0; i < tokens.size() - 2; ++i) {
            std::uint64_t key = (static_cast<std::uint64_t>(tokens[i]) << 32) | tokens[i+1];
            trigram_matrix[key][tokens[i+2]] += 1.0;
        }
    }
};

int main(int argc, char* argv[]) {
    Tokenizer tokenizer;
    AiOMotor motor;
    std::ifstream file(argv[1]);
    std::string line;
    std::vector<int> buffer;

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string word;
        while (ss >> word) {
            buffer.push_back(tokenizer.tokenize(word));
            // REDUCIDO A 2,000 PARA EVITAR EL CANCELAMIENTO POR TIEMPO/RAM
            if (buffer.size() >= 2000) {
                motor.process_stream(buffer);
                buffer.clear();
                buffer.shrink_to_fit();
            }
        }
    }
    return 0;
}
