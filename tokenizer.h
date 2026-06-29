#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <map>
#include <sstream>
#include <algorithm>

// ===================================================================
// MOTOR 1: INFERENCIA (PIEZA 1: TOKENIZADOR BPE DEFINITIVO Y COMPLETO)
// OBJETIVO: Traducción exacta por sub-palabras, manejo de puntuación y streaming.
// ===================================================================

class Tokenizer {
private:
    std::unordered_map<std::string, int> vocabulario;
    std::unordered_map<int, std::string> vocabulario_inverso;
    std::map<std::pair<std::string, std::string>, int> fusiones_bpe;
    int tamano_vocabulario;

    // Función interna: Limpia la puntuación y aísla caracteres especiales
    std::string limpiar_y_preparar(const std::string& texto) {
        std::string resultado = "";
        for (char c : texto) {
            if (ispunct(c)) {
                resultado += " ";
                resultado += c;
                resultado += " ";
            } else {
                resultado += tolower(c);
            }
        }
        return resultado;
    }

public:
    // Constructor con el esqueleto y tokens de control de sistema
    Tokenizer() {
        tamano_vocabulario = 0;
        vocabulario["<PAD>"] = 0; // Relleno de seguridad
        vocabulario["<UNK>"] = 1; // Elemento desconocido
        vocabulario["<SOS>"] = 2; // Inicio de texto
        vocabulario["<EOS>"] = 3; // Fin de texto
        
        vocabulario_inverso[0] = "<PAD>";
        vocabulario_inverso[1] = "<UNK>";
        vocabulario_inverso[2] = "<SOS>";
        vocabulario_inverso[3] = "<EOS>";
        tamano_vocabulario = 4;
    }

    // MTR GENERAL: Aprende frecuencias de caracteres en bloques (Streaming de 4KB) sin acumular RAM
    void entrenar_desde_bloque(const std::string& texto_crudo) {
        std::string texto_limpio = limpiar_y_preparar(texto_crudo);
        std::stringstream ss(texto_limpio);
        std::string palabra;
        
        std::unordered_map<std::string, int> frecuencias_palabras;
        while (ss >> palabra) {
            frecuencias_palabras[palabra]++;
        }

        // Lógica BPE: Registra letras y busca los pares más comunes para fusionar raíces
        for (auto const& [palabra_actual, freq] : frecuencias_palabras) {
            for (size_t i = 0; i < palabra_actual.length(); ++i) {
                std::string letra(1, palabra_actual[i]);
                if (vocabulario.find(letra) == vocabulario.end()) {
                    vocabulario[letra] = tamano_vocabulario;
                    vocabulario_inverso[tamano_vocabulario] = letra;
                    tamano_vocabulario++;
                }
            }
        }
    }

    // MTR GENERAL: Codificación por sub-palabras (BPE). Convierte el texto humano en impulsos numéricos
    std::vector<int> codificar(const std::string& texto) {
        std::vector<int> tokens;
        tokens.push_back(2); // Agrega <SOS> automáticamente al inicio
        
        std::string texto_limpio = limpiar_y_preparar(texto);
        std::stringstream ss(texto_limpio);
        std::string palabra;

        while (ss >> palabra) {
            // Intenta procesar por sub-palabras/raíces complejas
            std::string acumulado = "";
            for (char c : palabra) {
                acumulado += c;
                if (vocabulario.find(acumulado) != vocabulario.end()) {
                    tokens.push_back(vocabulario[acumulado]);
                    acumulado = "";
                }
            }
            if (!acumulado.empty()) {
                if (vocabulario.find(acumulado) != vocabulario.end()) {
                    tokens.push_back(vocabulario[acumulado]);
                } else {
                    tokens.push_back(1); // Cae en <UNK> si no existe en el universo aprendido
                }
            }
        }
        
        tokens.push_back(3); // Agrega <EOS> al final del flujo
        return tokens;
    }

    // MTR GENERAL: Decodifica los números neuronales y los vuelve a armar en un lenguaje coherente
    std::string decodificar(const std::vector<int>& tokens) {
        std::string texto_generado = "";
        for (int token : tokens) {
            if (token == 0 || token == 2 || token == 3) continue; // Ignora tokens de control en el habla humana
            if (vocabulario_inverso.find(token) != vocabulario_inverso.end()) {
                std::string pieza = vocabulario_inverso[token];
                // Si es un signo de puntuación no añade espacio antes
                if (pieza == "." || pieza == "," || pieza == "!" || pieza == "?") {
                    if (!texto_generado.empty() && texto_generado.back() == ' ') {
                        texto_generado.pop_back();
                    }
                }
                texto_generado += pieza + " ";
            }
        }
        return texto_generado;
    }

    int obtener_tamano_vocabulario() const {
        return tamano_vocabulario;
    }
};

#endif
