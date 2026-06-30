#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <sstream>
#include <cmath>
#include <algorithm>
#include <cstdint>

// Mantenemos TODO igual, solo añadimos la optimización
int main(int argc, char* argv[]) {
    // ... (Tu código actual sigue aquí igual)
    // Cuando llegues al bloque de 100,000 tokens:
    if (token_stream.size() >= 100000) {
        brain.process_window(token_stream);
        token_stream.clear();
        token_stream.shrink_to_fit(); // <--- Esta es la línea que evita el cierre por falta de RAM
    }
    // ... (El resto de tu código igual)
}
