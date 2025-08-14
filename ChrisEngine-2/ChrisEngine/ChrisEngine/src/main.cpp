// =================== ChrisEngine Main ===================
#include "ChrisEngineApp.h"
#include <iostream>

int main() {
    try {
        // Instancia la aplicación principal de ChrisEngine
        ChrisEngineApp app;

        // Inicializa la pista y los personajes
        app.loadTrack("pista de carreras.png");  // Nombre de la pista
        app.addRacer("princesa.png");
        app.addRacer("sonic.png");
        app.addRacer("virdo.png");
        app.addRacer("wario.png");

        // Ejecuta el ciclo principal: init → loop(update/render) → destroy
        int result = app.run();

        // Reporta si hay un código de error
        if (result != 0)
            std::cerr << "ChrisEngine exited with error code: " << result << "\n";

        return result;  // 0 si todo bien, distinto de 0 en error controlado
    }
    catch (const std::exception& e) {
        // Manejo de excepciones no controladas
        std::cerr << "Unhandled exception in ChrisEngine: " << e.what() << "\n";
        return -1;
    }
}
