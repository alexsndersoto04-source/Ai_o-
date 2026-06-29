#ifndef OPTIMIZER_H
#define OPTIMIZER_H

#include "tensor.h"
#include <vector>
#include <cstdint>

// ===================================================================
// MOTOR 2: OPTIMIZACIÓN (PIEZA 9: OPTIMIZADOR DE PESOS / SGD PARA INT8)
// OBJETIVO: Reajustar las neuronas basándose en el error para que la IA aprenda
// ===================================================================

class Optimizer {
private:
    float tasa_aprendizaje; // Learning Rate: Controla qué tan rápido o lento aprende la IA

public:
    // Constructor que define la velocidad de aprendizaje de control
    Optimizer(float alpha = 0.001f) : tasa_aprendizaje(alpha) {}

    // Modifica la tasa de aprendizaje dinámicamente si el entrenamiento lo requiere
    void ajustar_tasa_aprendizaje(float nueva_tasa) {
        tasa_aprendizaje = nueva_tasa;
    }

    // Aplica el descenso de gradiente sobre un Tensor cuantizado de 8 bits
    // Diseñado para que GitHub Actions actualice millones de parámetros sin desbordar la memoria
    void actualizar_pesos(Tensor& tensor_pesos, const std::vector<float>& gradientes) {
        size_t numero_elementos = tensor_pesos.obtener_numero_elementos();

        // Recorremos cada neurona del tensor para aplicar la corrección
        for (size_t i = 0; i < numero_elementos; ++i) {
            // 1. Des-cuantizamos el peso de 8 bits actual para operar en alta precisión flotante
            float peso_real = (static_cast<float>(tensor_pesos.datos[i]) - tensor_pesos.punto_cero) * tensor_pesos.escala;

            // 2. Aplicamos la fórmula fundamental del aprendizaje:
            // Nuevo Peso = Peso Anterior - (Tasa de Aprendizaje * Gradiente del Error)
            peso_real -= tasa_aprendizaje * gradientes[i];

            // 3. Volvemos a empaquetar (cuantizar) el peso corregido directamente en su celda de 1 byte
            float valor_escalado = std::round(peso_real / tensor_pesos.escala);
            
            // Límites de seguridad estrictos de 8 bits para proteger la RAM
            if (valor_escalado > 127.0f) valor_escalado = 127.0f;
            if (valor_escalado < -128.0f) valor_escalado = -128.0f;

            tensor_pesos.datos[i] = static_cast<int8_t>(valor_escalado);
        }
    }
};

#endif
