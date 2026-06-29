#ifndef MODEL_LOADER_H
#define MODEL_LOADER_H

#include "tensor.h"
#include <string>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdexcept>

// ===================================================================
// MOTOR 1: INFERENCIA (PIEZA 5: CARGADOR DE MODELO POR MMAP)
// OBJETIVO: Conectar archivos de 750GB al Tensor sin usar la RAM del servidor
// ===================================================================

class ModelLoader {
private:
    int fd; // Descriptor del archivo físico del modelo
    size_t tamano_archivo;
    int8_t* datos_mapeados;

public:
    ModelLoader() : fd(-1), tamano_archivo(0), datos_mapeados(nullptr) {}

    // Desconecta el archivo de forma segura al cerrar el motor
    ~ModelLoader() {
        if (datos_mapeados != MAP_FAILED && datos_mapeados != nullptr) {
            munmap(datos_mapeados, tamano_archivo);
        }
        if (fd != -1) {
            close(fd);
        }
    }

    // Abre el archivo del modelo y lo mapea directo a la estructura virtual
    void mapear_modelo(const std::string& ruta_archivo, size_t bytes_modelo) {
        tamano_archivo = bytes_modelo;

        // 1. Abrimos el archivo binario en modo lectura bajo nivel
        fd = open(ruta_archivo.c_str(), O_RDONLY);
        if (fd == -1) {
            throw std::runtime_error("Error: No se pudo abrir el archivo físico del modelo (.bin).");
        }

        // 2. Aplicamos la magia de mmap: Enlazamos el disco con el direccionamiento virtual
        datos_mapeados = (int8_t*)mmap(nullptr, tamano_archivo, PROT_READ, MAP_SHARED, fd, 0);
        if (datos_mapeados == MAP_FAILED) {
            close(fd);
            fd = -1;
            throw std::runtime_error("Error crítico: Falló el mapeo de memoria (mmap) en el servidor.");
        }
    }

    // Inyecta los pesos del disco directamente en un Tensor específico sin pasar por la RAM
    void cargar_pesos_en_tensor(Tensor& tensor, size_t offset_inicio, const std::vector<size_t>& forma) {
        tensor.forma = forma;
        size_t numero_elementos = tensor.obtener_numero_elementos();

        // Enlazamos el puntero de datos del Tensor al punto exacto del archivo mapeado
        tensor.datos.assign(datos_mapeados + offset_inicio, datos_mapeados + offset_inicio + numero_elementos);
    }
};

#endif
