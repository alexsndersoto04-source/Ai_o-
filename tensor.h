#ifndef TENSOR_H
#define TENSOR_H

#include <vector>
#include <cstddef>
#include <cstdint>

// ===================================================================
// MOTOR 1: INFERENCIA (PIEZA 2: CONTENEDOR DE TENSORES CUANTIZADOS)
// OBJETIVO: Definir el molde de 1 byte (INT8) para las neuronas
// ===================================================================

struct Tensor {
    std::vector<size_t> forma;  // Dimensiones del tensor (ej. [filas, columnas])
    std::vector<int8_t> datos;  // Las neuronas empaquetadas en 1 byte cada una
    
    // Parámetros de cuantización para rescatar la precisión matemática en la nube
    float escala;
    int32_t punto_cero;

    // Devuelve el número total de neuronas dentro de este bloque
    size_t obtener_numero_elementos() const {
        if (forma.empty()) return 0;
        size_t elementos = 1;
        for (size_t dim : forma) {
            elementos *= dim;
        }
        return elementos;
    }
};

#endif
