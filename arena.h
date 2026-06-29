#ifndef ARENA_H
#define ARENA_H

#include <cstddef>
#include <cstdint>
#include <stdexcept>
#include <vector>

// ===================================================================
// MOTOR 1: INFERENCIA (PIEZA 4: GESTOR MANUAL DE MEMORIA ARENA)
// OBJETIVO: Evitar la fragmentación y mantener la RAM de la nube en cero
// ===================================================================

class MemoryArena {
private:
    size_t capacidad_total;
    size_t offset_actual;
    uint8_t* buffer_lineal;

public:
    // Constructor que inicializa la Arena con un tamaño fijo predeterminado
    MemoryArena(size_t bytes_a_reservar) : capacidad_total(bytes_a_reservar), offset_actual(0) {
        // Reservamos un único bloque sólido de memoria contigua en el servidor
        buffer_lineal = new uint8_t[capacidad_total];
    }

    // Destructor para limpiar la memoria al cerrar el ciclo sin dejar residuos
    ~MemoryArena() {
        delete[] buffer_lineal;
    }

    // Función para asignar espacio dentro de la arena de forma ultra veloz (Complejidad O(1))
    void* asignar(size_t tamaño_bytes) {
        // Alineación de memoria a 8 bytes para optimizar el rendimiento del procesador en la nube
        size_t tamaño_alineado = (tamaño_bytes + 7) & ~7;

        if (offset_actual + tamaño_alineado > capacidad_total) {
            throw std::bad_alloc(); // Evita desbordamientos si el bloque supera la capacidad
        }

        // Entregamos el puntero exacto del espacio reservado
        void* puntero_disponible = &buffer_lineal[offset_actual];
        offset_actual += tamaño_alineado; // Avanzamos el marcador linealmente
        return puntero_disponible;
    }

    // Reseteo instantáneo: pone el marcador a cero sin ciclos pesados de CPU
    void resetear() {
        offset_actual = 0;
    }

    // Verificadores de estado del gestor de memoria
    size_t obtener_capacidad() const { return capacidad_total; }
    size_t obtener_espacio_usado() const { return offset_actual; }
    size_t obtener_espacio_libre() const { return capacidad_total - offset_actual; }
};

#endif
