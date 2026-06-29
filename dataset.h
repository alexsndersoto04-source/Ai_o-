#ifndef DATASET_H
#define DATASET_H

#include "tokenizer.h"
#include <string>
#include <vector>
#include <fstream>
#include <stdexcept>

// ===================================================================
// MOTOR 3: PIPELINE DE DATOS (PIEZA 10: GESTOR DE DATASETS EN STREAMING)
// OBJETIVO: Cargar y fragmentar textos masivos sin acumular RAM en la nube
// ===================================================================

class DatasetPipeline {
private:
    std::ifstream archivo_datos;
    size_t tamano_bloque; // Cuántos tokens procesamos por cada paso eléctrico
    Tokenizer tokenizador_interno;

public:
    // Constructor corregido con la variable exacta
    DatasetPipeline(size_t tamaño_contexto = 512) : tamano_bloque(tamaño_contexto) {}

    // Destructor para cerrar el flujo de archivos de manera segura
    ~DatasetPipeline() {
        if (archivo_datos.is_open()) {
            archivo_datos.close();
        }
    }

    // Abre el archivo de texto masivo descargado en el servidor
    void abrir_fuente_datos(const std::string& ruta_txt) {
        archivo_datos.open(ruta_txt, std::ios::in);
        if (!archivo_datos.is_open()) {
            throw std::runtime_error("Error: No se pudo abrir el archivo de texto masivo para entrenamiento.");
        }
    }

    // MTR GENERAL: Extrae el siguiente bloque numérico del texto sin cargar el archivo completo
    bool obtener_siguiente_lote(std::vector<int>& tokens_entrada, std::vector<int>& tokens_objetivo) {
        tokens_entrada.clear();
        tokens_objetivo.clear();

        std::string linea;
        std::string texto_acumulado = "";

        // Leemos líneas del archivo físico hasta acumular suficiente texto para el bloque
        while (archivo_datos.good() && tokens_entrada.size() < tamano_bloque) {
            std::getline(archivo_datos, linea);
            texto_acumulado += linea + " ";

            // Codificamos temporalmente el fragmento extraído
            std::vector<int> tokens_temporales = tokenizador_interno.codificar(texto_acumulado);
            
            // Si ya llenamos el tamaño del bloque requerido, estructuramos los vectores
            if (tokens_temporales.size() >= tamano_bloque + 1) {
                // Entrada del modelo (X): tokens desde el inicio hasta el penúltimo
                tokens_entrada.assign(tokens_temporales.begin(), tokens_temporales.begin() + tamano_bloque);
                
                // Objetivo de predicción (Y): tokens desplazados un paso a la derecha
                tokens_objetivo.assign(tokens_temporales.begin() + 1, tokens_temporales.begin() + tamano_bloque + 1);
                
                return true; // Lote listo para ser inyectado a las neuronas
            }
        }

        // Si llegamos al final del archivo y quedó un bloque pequeño final, lo procesamos
        if (!texto_acumulado.empty()) {
            std::vector<int> tokens_temporales = tokenizador_interno.codificar(texto_acumulado);
            if (tokens_temporales.size() > 1) {
                size_t tamaño_disponible = tokens_temporales.size() - 1;
                tokens_entrada.assign(tokens_temporales.begin(), tokens_temporales.begin() + tamaño_disponible);
                tokens_objetivo.assign(tokens_temporales.begin() + 1, tokens_temporales.end());
                return true;
            }
        }

        return false; // Se terminaron por completo los datos del archivo (Fin de la Época)
    }

    // Reinicia el puntero del archivo para poder hacer otra pasada de aprendizaje (Nueva Época)
    void reiniciar_pipeline() {
        if (archivo_datos.is_open()) {
            archivo_datos.clear();
            archivo_datos.seekg(0, std::ios::beg);
        }
    }
};

#endif
