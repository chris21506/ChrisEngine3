// =================== ChrisEngine ResourceManager ===================
#include <iostream>
#include <filesystem>
#include "EngineUtilities.h"
#include "Texture.h"

class ChrisEngineResourceManager {
private:
    std::map<std::string, EngineUtilities::TSharedPointer<Texture>> m_textures;

public:
    // Carga una textura y la guarda en el mapa
    bool loadTexture(const std::string& fileName, const std::string& extension) {
        // Si ya existe y no es nula, no recargar
        auto it = m_textures.find(fileName);
        if (it != m_textures.end() && !it->second.isNull())
            return true;

        // Construir ruta absoluta del archivo
        std::string fullName = fileName + "." + extension;
        std::filesystem::path fullPath = std::filesystem::absolute(fullName);

        // Crear puntero compartido a la textura
        auto texturePtr = EngineUtilities::MakeShared<Texture>(fileName, extension);

        // Guardar aunque falle para evitar reintentos infinitos
        m_textures[fileName] = texturePtr;

        // Retorna true si la textura cargó correctamente
        return !texturePtr.isNull();
    }

    // Obtener puntero a textura cargada
    EngineUtilities::TSharedPointer<Texture> getTexture(const std::string& fileName) {
        auto it = m_textures.find(fileName);
        if (it != m_textures.end())
            return it->second;

        // No encontrada: puntero nulo
        return EngineUtilities::TSharedPointer<Texture>();
    }

    // Carga todos los sprites de la carrera
    void loadAllResources() {
        loadTexture("princesa", "png");
        loadTexture("sonic", "png");
        l
