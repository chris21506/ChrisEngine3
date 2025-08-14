// =================== ChrisEngine Texture ===================
#include <iostream>
#include "EngineUtilities.h"
#include "Window.h"
#include "SFML/Graphics.hpp"

class ChrisEngineTexture {
private:
    std::string m_name;    // nombre base del sprite
    std::string m_ext;     // extensión (png)
    std::optional<sf::Sprite> m_sprite;   // sprite de SFML
    sf::Texture m_texture; // textura subyacente

    // Construye ruta absoluta del archivo
    static std::string makeFullPath(const std::string& base, const std::string& ext) {
        return "bin/" + base + "." + ext;
    }

public:
    // Constructor: carga la textura y prepara el sprite
    ChrisEngineTexture(const std::string& textureName, const std::string& extension)
        : m_name(textureName), m_ext(extension)
    {
        const std::string path = makeFullPath(m_name, m_ext);
        if (!m_texture.loadFromFile(path)) {
            m_sprite.reset();
            std::cerr << "[ChrisEngineTexture] Cannot load: " << path << "\n";
            return;
        }

        // Crear sprite y asignar textura
        m_sprite.emplace(m_texture);

        // Centrar origen del sprite para rotación y escalado
        const auto bounds = m_sprite->getLocalBounds();
        const auto sz = bounds.size;
        m_sprite->setOrigin(sf::Vector2f{ sz.x * 0.5f, sz.y * 0.5f });
    }

    // Posición absoluta
    void setPosition(const sf::Vector2f& p) {
        if (m_sprite) m_sprite->setPosition(p);
    }

    // Rotación en grados
    void setRotation(float degrees) {
        if (m_sprite) m_sprite->setRotation(sf::degrees(degrees));
    }

    // Escala no uniforme
    void setScale(const sf::Vector2f& s) {
        if (m_sprite) m_sprite->setScale(s);
    }

    // Dibujar sprite en ventana
    void render(const EngineUtilities::TSharedPointer<Window>& window) {
        if (!m_sprite) return;
        window->draw(*m_sprite);
    }
};

// =================== Ejemplo de uso de los recursos ===================
void loadRaceSprites() {
    auto princesa = EngineUtilities::MakeShared<ChrisEngineTexture>("princesa", "png");
    auto sonic = EngineUtilities::MakeShared<ChrisEngineTexture>("sonic", "png");
    auto virdo = EngineUtilities::MakeShared<ChrisEngineTexture>("virdo", "png");
    auto wario = EngineUtilities::MakeShared<ChrisEngineTexture>("wario", "png");
    auto pista = EngineUtilities::MakeShared<ChrisEngineTexture>("pista de carreras", "png");
}
