#ifndef LOSS_H
#define LOSS_H

#include <vector>
#include <cmath>
#include <stdexcept>

// ===================================================================
// MOTOR 2: OPTIMIZACIÓN (PIEZA 8: CÁLCULO DE ERROR / CROSS-ENTROPY LOSS)
// OBJETIVO: Medir la precisión del habla de la IA para forzar su corrección
// ===================================================================

class LossFunctions {
public:
    // Calcula la entropía cruzada entre las predicciones de la IA y la palabra real esperada
    // Diseñado para que GitHub Actions procese el error en bloques ligeros sin acumular RAM
    static float calcular_entropia_cruzada(const std::vector<float>& predicciones_softmax, int indice_palabra_real) {
        if (indice_palabra_real < 0 || indice_palabra_real >= static_cast<int>(predicciones_softmax.size())) {
            throw std::out_of_range("Error crítico: El índice de la palabra real está fuera del vocabulario.");
        }

        // La palabra real que debió decir la IA
        float probabilidad_predicha = predicciones_softmax[indice_palabra_real];

        // Evitamos el error matemático de logaritmo de cero (log(0) es indefinido)
        // Añadiendo un épsilon de protección ultra pequeño (1e-15)
        if (probabilidad_predicha < 1e-15f) {
            probabilidad_predicha = 1e-15f;
        }

        // Fórmula matemática de entropía cruzada: Pérdida = -ln(probabilidad)
        // Entre más cerca esté la probabilidad de 1.0 (100%), la pérdida será casi 0.
        float perdida = -std::log(probabilidad_predicha);

        return perdida;
    }

    // Calcula el gradiente (la dirección del error) para saber cómo deben cambiar las neuronas
    static void calcular_gradiente_salida(const std::vector<float>& predicciones_softmax, int indice_palabra_real, std::vector<float>& gradiente_destino) {
        gradiente_destino.resize(predicciones_softmax.size());
        
        // El gradiente para Softmax + Cross-Entropy se simplifica de forma eficiente:
        // Se resta 1.0 únicamente a la posición de la palabra correcta
        for (size_t i = 0; i < predicciones_softmax.size(); ++i) {
            gradiente_destino[i] = predicciones_softmax[i];
        }
        gradiente_destino[indice_palabra_real] -= 1.0f;
    }
};

#endif
