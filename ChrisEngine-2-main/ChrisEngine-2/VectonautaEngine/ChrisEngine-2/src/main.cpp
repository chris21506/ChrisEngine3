// main_ChrisEngine2.cpp
#include "BaseAppChrisEngine2.h"
#include <iostream>

// Punto de entrada de ChrisEngine-2
int main() {
    try {
        // Crea la aplicación principal de ChrisEngine-2
        BaseAppChrisEngine2 app;

        // Inicializa personajes y pista
        app.addRacer("princesa.png");
        app.addRacer("sonic.png");
        app.addRacer("virdo.png");
        app.addRacer("wario.png");

        app.setTrack("pista de carreras.png");

        // Ejecuta ciclo principal: init → loop(update/render) → destroy
        int result = app.run();

        // Si retorna distinto de 0, reporta error
        if (result != 0) {
            std::cerr << "ChrisEngine-2 exited with error code: " << result << '\n';
        }

        return result; // 0 = éxito
    }
    catch (const std::exception& e) {
        // Captura cualquier excepción no controlada
        std::cerr << "Unhandled exception in ChrisEngine-2: " << e.what() << '\n';
        return -1;
    }
}
