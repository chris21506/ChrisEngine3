#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include <string>
#include <iostream>

// ------------------------------------------------
// Clase Actor: representa corredor o pista
// ------------------------------------------------
class Actor {
    sf::Sprite sprite;
    sf::Texture texture;

public:
    std::string name;

    Actor(const std::string& filename, const std::string& actorName) : name(actorName) {
        if (!texture.loadFromFile(filename)) {
            std::cerr << "Error cargando textura: " << filename << "\n";
        }
        sprite.setTexture(texture);
        // Centrar origen para rotaciones escalado
        sf::FloatRect bounds = sprite.getLocalBounds();
        sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
    }

    void setPosition(float x, float y) { sprite.setPosition(x, y); }
    void setScale(float sx, float sy) { sprite.setScale(sx, sy); }
    void setRotation(float angle) { sprite.setRotation(angle); }

    void render(sf::RenderWindow& window) { window.draw(sprite); }
};

// ------------------------------------------------
// Clase Window: encapsula sf::RenderWindow
// ------------------------------------------------
class ChrisEngineWindow {
    std::unique_ptr<sf::RenderWindow> window;

public:
    ChrisEngineWindow(int width, int height, const std::string& title) {
        window = std::make_unique<sf::RenderWindow>(sf::VideoMode(width, height), title);
        window->setFramerateLimit(60);
    }

    bool isOpen() const { return window->isOpen(); }
    void clear() { window->clear(sf::Color::Black); }
    void display() { window->display(); }
    void draw(sf::Drawable& drawable) { window->draw(drawable); }

    void handleEvents() {
        sf::Event event;
        while (window->pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window->close();
        }
    }

    sf::RenderWindow& getInternal() { return *window; }
};

// ------------------------------------------------
// Función principal
// ------------------------------------------------
int main() {
    ChrisEngineWindow window(1024, 768, "ChrisEngine Carrera");

    // Crear actores
    Actor pista("pista de carreras.png", "Pista");
    pista.setPosition(512, 384); // centro

    Actor princesa("princesa.png", "Princesa");
    princesa.setPosition(200, 600);

    Actor sonic("sonic.png", "Sonic");
    sonic.setPosition(400, 600);

    Actor virdo("virdo.png", "Virdo");
    virdo.setPosition(600, 600);

    Actor wario("wario.png", "Wario");
    wario.setPosition(800, 600);

    std::vector<Actor*> racers = { &princesa, &sonic, &virdo, &wario };

    // Loop principal
    while (window.isOpen()) {
        window.handleEvents();
        window.clear();

        // Dibujar pista primero
        pista.render(window.getInternal());

        // Dibujar corredores
        for (auto* r : racers)
            r->render(window.getInternal());

        window.display();
    }

    return 0;
}
