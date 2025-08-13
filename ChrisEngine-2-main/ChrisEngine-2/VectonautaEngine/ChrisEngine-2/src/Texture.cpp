// TextureChrisEngine2.h / .cpp
#include "EngineUtilities.h"
#include "Window.h"
#include <iostream>
#include <optional>

class TextureChrisEngine2 {
private:
    std::string m_name;         // nombre del archivo sin extensión
    std::string m_ext;          // extensión del archivo (ej. "png")
    sf::Texture m_texture;      // textura SFML
    std::optional<sf::Sprite> m_sprite;  // sprite opcional, solo si se cargó la textura

    // Construye la ruta absoluta del asset
    static std::string makeFullPath(const std::string& base, const std::string& ext) {
        return "bin/" + base + "." + ext; // assets bajo "bin/"
    }

public:
    // Constructor: carga la textura y crea el sprite centrado
    TextureChrisEngine2(const std::string& textureName, const std::string& extension)
        : m_name(textureName), m_ext(extension)
    {
        const std::string path = makeFullPath(m_name, m_ext);
        if (!m_texture.loadFromFile(path)) {
            m_sprite.reset(); // si falla, sprite nulo
            std::cerr << "[TextureChrisEngine2] Cannot load: " << path << "\n";
            return;
        }

        // Crea sprite y centra origen
        m_sprite.emplace(m_texture);
        const auto bounds = m_sprite->getLocalBounds();
        const auto sz = bounds.size;
        m_sprite->setOrigin(sf::Vector2f{ sz.x * 0.5f, sz.y * 0.5f });
    }

    // Cambiar posición
    void setPosition(const sf::Vector2f& p) {
        if (m_sprite) m_sprite->setPosition(p);
    }

    // Rotación en grados
    void setRotation(float degrees) {
        if (m_sprite) m_sprite->setRotation(sf::degrees(degrees));
    }

    // Escala (x, y)
    void setScale(const sf::Vector2f& s) {
        if (m_sprite) m_sprite->setScale(s);
    }

    // Dibuja en ventana
    void render(const EngineUtilities::TSharedPointer<Window>& window) {
        if (m_sprite) window->draw(*m_sprite);
    }
};

// Función auxiliar para cargar todos los assets de ChrisEngine-2
void loadChrisEngine2Textures(ResourceManagerChrisEngine2& rm) {
    rm.loadTexture("princesa", "png");
    rm.loadTexture("sonic", "png");
    rm.loadTexture("virdo", "png");
    rm.loadTexture("wario", "png");
    rm.loadTexture("pista de carreras", "png");
}
