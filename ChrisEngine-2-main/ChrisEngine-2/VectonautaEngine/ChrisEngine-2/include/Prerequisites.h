#pragma once

/**
 * @file ChrisEngine2Prerequisites.h
 * @brief Includes estándar, utilidades del motor, ImGui, SFML y macros comunes para ChrisEngine-2.
 *
 * - Incluye los personajes: princesa.png, sonic.png, virdo.png, wario.png
 * - La pista: pista de carreras.png
 */

 // === Standard Libraries ===
#include <iostream>     
#include <string>       
#include <sstream>      
#include <vector>       
#include <thread>       
#include <map>          
#include <fstream>      
#include <unordered_map>

// === Engine Utilities ===
#include "Memory/TSharedPointer.h"  
#include "Memory/TStaticPtr.h"     
#include "Memory/TUniquePtr.h"     

// === ImGui ===
#include <imgui.h>
#include <imgui-SFML.h>          

// === SFML ===
#include <SFML/Graphics.hpp>      

// === Macros ===

/// Libera un puntero y lo pone en nullptr
#define SAFE_PTR_RELEASE(x) if ((x) != nullptr) { delete (x); (x) = nullptr; }

/// Mensaje de creación de recurso
#define MESSAGE(classObj, method, state)                      \
    do {                                                      \
        std::ostringstream os_;                               \
        os_ << classObj << "::" << method                    \
            << " : [CREATION OF RESOURCE: " << state << "]";  \
        std::cerr << os_.str();                               \
    } while (0)

/// Mensaje de error y termina la aplicación
#define ERROR(classObj, method, errorMSG)                     \
    do {                                                      \
        std::ostringstream os_;                               \
        os_ << "ERROR: " << classObj << "::" << method       \
            << " : Error in data from params [" << errorMSG << "]"; \
        std::cerr << os_.str();                               \
        std::exit(1);                                        \
    } while (0)

// === Enumeraciones ===
enum ShapeType {
    EMPTY = 0,      ///< Sin shape
    CIRCLE = 1,     ///< Círculo
    RECTANGLE = 2,  ///< Rectángulo
    TRIANGLE = 3,   ///< Triángulo (ConvexShape)
    POLYGON = 4     ///< Polígono (>=5 puntos)
};

// === Información de ChrisEngine-2 ===
namespace ChrisEngine2Info {
    const std::vector<std::string> characters = {
        "princesa.png",
        "sonic.png",
        "virdo.png",
        "wario.png"
    };

    const std::string track = "pista de carreras.png";
}
