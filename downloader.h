#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include <string>
#include <iostream>
#include <cstdlib>
#include <stdexcept>

// ===================================================================
// MOTOR 3: PIPELINE DE DATOS (PIEZA 11: DESCARGADOR AUTOMÁTICO DE DATOS)
// OBJETIVO: Descargar archivos masivos de texto desde internet hacia la nube
// ===================================================================

class DataDownloader {
public:
    // Toma una URL de internet y la guarda en una ruta física local dentro del servidor
    // Diseñado para ejecutarse de forma limpia y transparente en GitHub Actions
    static bool descargar_archivo(const std::string& url, const std::string& ruta_destino_local) {
        std::cout << "[INFO] Iniciando descarga remota de datos masivos..." << std::endl;
        std::cout << "[INFO] Origen: " << url << std::endl;
        std::cout << "[INFO] Destino: " << ruta_destino_local << std::endl;

        // Construimos el comando del sistema utilizando 'curl' de Linux de forma segura
        // -L sigue redirecciones de enlaces, -s oculta la barra de carga interna para no saturar los logs de GitHub, -o define el archivo de salida
        std::string comando = "curl -L -s -o " + ruta_destino_local + " " + url;

        // Ejecutamos el comando en el sistema operativo de la nube
        int codigo_retorno = std::system(comando.c_str());

        // Verificamos si la descarga fue exitosa (el código 0 en Linux significa éxito rotundo)
        if (codigo_retorno == 0) {
            std::cout << "[ÉXITO] Archivo descargado e integrado al pipeline correctamente." << std::endl;
            return true;
        } else {
            // Si falla con curl, aplicamos un sistema de respaldo automático con 'wget'
            std::cout << "[ADVERTENCIA] curl falló o no está disponible. Intentando respaldo con wget..." << std::endl;
            std::string comando_respaldo = "wget -q -O " + ruta_destino_local + " " + url;
            int codigo_retorno_respaldo = std::system(comando_respaldo.c_str());

            if (codigo_retorno_respaldo == 0) {
                std::cout << "[ÉXITO] Archivo descargado con éxito vía motor de respaldo." << std::endl;
                return true;
            }
        }

        // Si ambos sistemas fallan, el pipeline se detiene de forma segura para no entrenar con archivos vacíos
        throw std::runtime_error("Error crítico: No se pudo descargar la fuente de datos masivos desde la URL proveída.");
        return false;
    }
};

#endif
