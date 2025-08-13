#pragma once

#include <ChrisEngine2Prerequisites.h>
#include <unordered_map>
#include <ECS/Texture.h>

/**
 * @class ChrisEngine2ResourceManager
 * @brief Administra la carga y almacenamiento de texturas para ChrisEngine-2.
 *
 * - Carga automáticamente los personajes: princesa.png, sonic.png, virdo.png, wario.png
 * - Carga la pista: pista de carreras.png
 */
class ChrisEngine2ResourceManager {
public:
    /**
     * @brief Carga una textura y la almacena bajo la clave fileName.
     * @param fileName Nombre base del archivo (sin extensión).
     * @param extension Extensión del archivo (por defecto "png").
     * @return true si la textura ya existía o se cargó correctamente.
     */
    bool loadTexture(const std::string& fileName, const std::string& extension = "png");

    /**
     * @brief Devuelve la textura cargada con fileName, o la textura por defecto si no existe.
     */
    EngineUtilities::TSharedPointer<Texture> getTexture(const std::string& fileName);

    /**
     * @brief Carga automáticamente todas las texturas de los personajes y la pista.
     */
    void loadDefaultResources() {
        for (const auto& character : ChrisEngine2Info::characters) {
            loadTexture(character);
        }
        loadTexture(ChrisEngine2Info::track);
    }

private:
    // Mapa de texturas cargadas: clave = fileName, valor = puntero compartido a Texture
    std::unordered_map<std::string, EngineUtilities::TSharedPointer<Texture>> m_textures;
};
