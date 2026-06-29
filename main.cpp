#include "tokenizer.h"
#include "tensor.h"
#include "math_ops.h"
#include "arena.h"
#include "model_loader.h"
#include "model.h"
#include "quantizer.h"
#include "loss.h"
#include "optimizer.h"
#include "dataset.h"
#include "downloader.h"
#include "train.h"
#include <iostream>

// ===================================================================
// ARCHIVO FINAL DE CONTROL: PUNTO DE ENTRADA GENERAL (main.cpp)
// OBJETIVO: Integrar los 4 motores del sistema para verificar su compilación
// ===================================================================

int main() {
    std::cout << "==================================================" << std::endl;
    std::cout << "   INICIANDO SISTEMA CENTRAL DE CONTROL: Ai o" << std::endl;
    std::cout << "==================================================" << std::endl;

    try {
        // 1. Configuración de parámetros base para el bloque de control
        size_t dimension_oculta = 256; 
        size_t total_neuronas_simuladas = 100000;
        
        std::cout << "[FASE 1] Inicializando el Cerebro de Inferencia..." << std::endl;
        AiOModel modelo(dimension_oculta, total_neuronas_simuladas);

        // 2. Inicialización del Optimizador y del Pipeline de Datos Masivos
        std::cout << "[FASE 2] Levantando los Motores de Optimización y Datos..." << std::endl;
        Optimizer optimizador(0.001f); // Tasa de aprendizaje (Learning Rate)
        DatasetPipeline pipeline(128); // Bloques de contexto de 128 tokens por paso
        
        // 3. Unificación en el Motor de Ciclos Maestro
        std::cout << "[FASE 3] Acoplando el Orquestador de Ciclos..." << std::endl;
        TrainingEngine motor_entrenamiento(modelo, pipeline, optimizador, 2); // 2 Épocas de prueba

        std::cout << "\n[ESTADO] Todos los módulos acoplados con éxito absoluto." << std::endl;
        std::cout << "[ESTADO] Sistema listo para recibir el flujo de GitHub Actions." << std::endl;
        std::cout << "==================================================" << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "\n[ERROR CRÍTICO DEL SISTEMA]: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
