// ResourceManagerChrisEngine2.h / .cpp
#include <iostream>
#include <filesystem>
#include <map>
#include "EngineUtilities.h"
#include "Texture.h"

class ResourceManagerChrisEngine2 {
private:
    // Mapa de texturas cargadas: nombre → puntero compartido a Texture
    std::map<std::string, EngineUtilities::TSharedPointer<Texture>> m_textures;

public:
    // Carga una textura si no estaba cargada
    bool loadTexture(const std::string& fileName, const std::string& extension) {
        // Verifica si ya existe
        auto it = m_textures.find(fileName);
        if (it != m_textures.end() && !it->second.isNull())
            return true; // ya estaba cargada

        // Construye ruta absoluta
        std::string fullName = fileName + "." + extension;
        std::filesystem::path fullPath = std::filesystem::absolute(fullName);

        // Intenta crear la textura
        auto texturePtr = EngineUtilities::MakeShared<Texture>(fileName, extension);

        // Guardar aunque falle para no intentar recargar infinitamente
        m_textures[fileName] = texturePtr;

        // Retorna true si la textura fue cargada correctamente
        return !texturePtr.isNull();
    }

    // Devuelve la textura cargada (puntero nulo si no existe)
    EngineUtilities::TSharedPointer<Texture> getTexture(const std::string& fileName) {
        auto it = m_textures.find(fileName);
        if (it != m_textures.end())
            return it->second;

        // No encontrada → puntero nulo
        return EngineUtilities::TSharedPointer<Texture>();
    }

    // Carga todas las texturas específicas de ChrisEngine-2
    void loadAllTextures() {
        loadTexture("princesa", "png");
        loadTexture("sonic", "png");
        loadTexture("virdo", "png");
        loadTexture("wario", "png");
        loadTexture("pista de carreras", "png");
    }
};
