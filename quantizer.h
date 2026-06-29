#ifndef QUANTIZER_H
#define QUANTIZER_H

#include "tensor.h"
#include <vector>
#include <cmath>
#include <algorithm>
#include <cstdint>

// ===================================================================
// MOTOR 2: OPTIMIZACIÓN (PIEZA 7: CUANTIZADOR DINÁMICO DE 8 BITS)
// OBJETIVO: Prensar datos flotantes de 32 bits a 8 bits (1 byte) sin perder precisión
// ===================================================================

class Quantizer {
public:
    // Toma un vector de neuronas en alta precisión (float) y lo comprime dentro de nuestro Tensor de 8 bits
    static void cuantizar_dinamico(const std::vector<float>& entrada, Tensor& tensor_destino) {
        if (entrada.empty()) return;

        // 1. Encontrar los valores máximos y mínimos absolutos para no romper la escala biológica
        float min_val = entrada[0];
        float max_val = entrada[0];
        for (float v : entrada) {
            if (v < min_val) min_val = v;
            if (v > max_val) max_val = v;
        }

        // 2. Calcular la escala matemática de empaquetado para el rango de un byte con signo (-128 a 127)
        float rango = std::max(std::abs(min_val), std::abs(max_val));
        if (rango == 0.0f) {
            tensor_destino.escala = 1.0f;
            tensor_destino.punto_cero = 0;
            std::fill(tensor_destino.datos.begin(), tensor_destino.datos.end(), 0);
            return;
        }

        // Escala = Rango Máximo / 127 (Límite del bit de signo en INT8)
        tensor_destino.escala = rango / 127.0f;
        tensor_destino.punto_cero = 0; // Cuantización simétrica optimizada para CPU en la nube

        // 3. Redimensionar el tensor destino para asegurar espacio exacto sin desperdicio
        tensor_destino.forma = { entrada.size() };
        tensor_destino.datos.resize(entrada.size());

        // 4. Transformación y empaquetado de cada neurona individual
        for (size_t i = 0; i < entrada.size(); ++i) {
            // Dividimos el valor real entre la escala y redondeamos al entero más cercano
            float valor_escalado = std::round(entrada[i] / tensor_destino.escala);
            
            // Forzamos por seguridad que el número quede estrictamente dentro del límite de 1 byte
            if (valor_escalado > 127.0f) valor_escalado = 127.0f;
            if (valor_escalado < -128.0f) valor_escalado = -128.0f;

            tensor_destino.datos[i] = static_cast<int8_t>(valor_escalado);
        }
    }
};

#endif
