// WindowChrisEngine2.h / .cpp
#include "EngineUtilities.h"
#include <SFML/Graphics.hpp>
#include <functional>
#include <iostream>

class WindowChrisEngine2 {
private:
    EngineUtilities::TUniquePointer<sf::RenderWindow> m_windowPtr;
    sf::View m_view;            // vista por defecto
    sf::Clock clock;            // reloj interno para deltaTime
public:
    sf::Time deltaTime;         // tiempo entre frames

    // Constructor: crea la ventana y configura FPS / vista
    WindowChrisEngine2(int width = 1280, int height = 720, const std::string& title = "ChrisEngine-2") {
        m_windowPtr = EngineUtilities::MakeUnique<sf::RenderWindow>(
            sf::VideoMode(static_cast<unsigned>(width), static_cast<unsigned>(height)), title
        );
        if (m_windowPtr && m_windowPtr->isOpen()) {
            m_windowPtr->setFramerateLimit(60);
            m_view = m_windowPtr->getDefaultView();
            MESSAGE("WindowChrisEngine2", "Window", "Created successfully");
        }
        else {
            ERROR("WindowChrisEngine2", "Window", "Failed to create RenderWindow");
        }
    }

    ~WindowChrisEngine2() { destroy(); }

    // Procesa eventos SFML y despacha a callback
    void handleEvents(const std::function<void(const sf::Event&)>& callback) {
        if (!m_windowPtr) return;
        while (auto event = m_windowPtr->pollEvent()) {
            if (callback) callback(*event);
            if (event->is<sf::Event::Closed>()) close();
        }
    }

    bool isOpen() const { return m_windowPtr ? m_windowPtr->isOpen() : false; }

    void clear(const sf::Color& color = sf::Color::Black) {
        if (m_windowPtr) m_windowPtr->clear(color);
    }

    void draw(const sf::Drawable& drawable, const sf::RenderStates& states = {}) {
        if (m_windowPtr) m_windowPtr->draw(drawable, states);
    }

    void display() { if (m_windowPtr) m_windowPtr->display(); }

    void update() { if (m_windowPtr) deltaTime = clock.restart(); }

    void render() { /* placeholder para overlays o debug */ }

    void close() { if (m_windowPtr && m_windowPtr->isOpen()) m_windowPtr->close(); }

    void destroy() {
        if (m_windowPtr) {
            if (m_windowPtr->isOpen()) m_windowPtr->close();
            m_windowPtr.reset();
        }
    }

    sf::RenderWindow& getInternal() { return *m_windowPtr; }

    // Función auxiliar para centrar la vista en la pista de carreras
    void centerOnTrack() {
        sf::Vector2f trackCenter(640.f, 360.f); // posición central estándar
        m_view.setCenter(trackCenter);
        if (m_windowPtr) m_windowPtr->setView(m_view);
    }
};
