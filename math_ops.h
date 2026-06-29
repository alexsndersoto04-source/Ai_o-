#ifndef MATH_OPS_H
#define MATH_OPS_H

#include "tensor.h"
#include <cmath>
#include <vector>

// ===================================================================
// MOTOR 1: INFERENCIA (PIEZA 3: OPERACIONES MATEMÁTICAS DE BAJO NIVEL)
// OBJETIVO: Multiplicación de matrices de neuronas cuantizadas en 8 bits
// ===================================================================

class MathOps {
public:
    // Multiplica una matriz de neuronas por un vector de impulsos (Pensamiento de la IA)
    // Diseñado para que GitHub Actions lo procese de forma lineal y ultra veloz
    static void multiplicar_matriz_vector(const Tensor& matriz, const std::vector<float>& entrada, std::vector<float>& salida) {
        size_t filas = matriz.forma[0];
        size_t columnas = matriz.forma[1];
        
        salida.assign(filas, 0.0f);

        // Des-cuantización sobre la marcha: convierte el byte físico a valor matemático
        for (size_t i = 0; i < filas; ++i) {
            float suma = 0.0f;
            for (size_t j = 0; j < columnas; ++j) {
                size_t indice = i * columnas + j;
                // Fórmula de rescate de precisión: (peso_int8 - punto_cero) * escala
                float peso_descuantizado = (static_cast<float>(matriz.datos[indice]) - matriz.punto_cero) * matriz.escala;
                suma += peso_descuantizado * entrada[j];
            }
            // Función de activación biológica (GELU aproximada para alta coherencia en el habla)
            salida[i] = suma / (1.0f + std::exp(-1.702f * suma));
        }
    }

    // Softmax: Distribución de probabilidad para que la IA elija la palabra más coherente y con sentido
    static void softmax(std::vector<float>& valores) {
        float max_val = valores[0];
        for (float v : valores) {
            if (v > max_val) max_val = v;
        }

        float suma = 0.0f;
        for (size_t i = 0; i < valores.size(); ++i) {
            valores[i] = std::exp(valores[i] - max_val);
            suma += valores[i];
        }

        for (size_t i = 0; i < valores.size(); ++i) {
            valores[i] /= suma;
        }
    }
};

#endif
