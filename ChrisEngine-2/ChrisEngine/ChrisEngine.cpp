// ActorChrisEngine2.h / .cpp
#include "CShapeChrisEngine2.h"
#include "TransformChrisEngine2.h"
#include "TextureChrisEngine2.h"
#include "WindowChrisEngine2.h"
#include "EngineUtilities.h"
#include <vector>
#include <memory>
#include <string>

class ActorChrisEngine2 {
public:
    std::string m_name; // Nombre del actor ("Track" o personaje)
    std::vector<EngineUtilities::TSharedPointer<void>> components;

    // Actualiza posición, escala y rotación de los componentes
    void update(float dt) {
        (void)dt;
        auto xf = getComponent<TransformChrisEngine2>();
        if (!xf) return;

        // Actualiza CShape si existe
        if (auto shape = getComponent<CShapeChrisEngine2>()) {
            if (auto raw = shape->getShape())
                xf->applyTo(*raw);
        }

        // Actualiza Texture si existe
        if (auto tex = getComponent<TextureChrisEngine2>()) {
            tex->setPosition(xf->getPosition());
            auto s = xf->getScale();
            if (s.x == 0.f && s.y == 0.f) s = { 1.f, 1.f }; // fallback
            tex->setScale(s);
            tex->setRotation(xf->getRotation());
        }
    }

    // Dibuja actor en la ventana
    void render(const EngineUtilities::TSharedPointer<WindowChrisEngine2>& window) {
        // Track dibuja solo su CShape
        if (m_name == "Track") {
            if (auto shape = getComponent<CShapeChrisEngine2>())
                shape->render(window);
            return;
        }

        // Racers dibujan su sprite
        if (auto tex = getComponent<TextureChrisEngine2>())
            tex->render(window);
    }

    // Asigna textura al actor
    void setTexture(const EngineUtilities::TSharedPointer<TextureChrisEngine2>& texture) {
        if (texture.isNull()) return;

        // Reemplaza o agrega el componente Texture
        bool replaced = false;
        for (auto& comp : components) {
            if (comp.template dynamic_pointer_cast<TextureChrisEngine2>()) {
                comp = texture;
                replaced = true;
                break;
            }
        }
        if (!replaced) components.push_back(texture);

        // Si es Track, también asigna la textura al CShape
        if (m_name == "Track") {
            if (auto shape = getComponent<CShapeChrisEngine2>())
                shape->setTexture(texture);
        }
    }

    // Plantilla para obtener componentes del actor
    template <typename T>
    EngineUtilities::TSharedPointer<T> getComponent() {
        for (auto& comp : components) {
            if (auto casted = comp.template dynamic_pointer_cast<T>())
                return casted;
        }
        return EngineUtilities::TSharedPointer<T>();
    }
};

// Ejemplo de inicialización de actores para ChrisEngine-2
void setupChrisEngine2Actors(std::vector<ActorChrisEngine2>& actors,
    ResourceManagerChrisEngine2& rm) {
    // Pista
    ActorChrisEngine2 track;
    track.m_name = "Track";
    track.components.push_back(EngineUtilities::MakeShared<CShapeChrisEngine2>(ShapeType::RECTANGLE));
    track.setTexture(rm.getTexture("pista de carreras"));
    actors.push_back(track);

    // Personajes
    std::vector<std::string> names = { "princesa.png", "sonic.png", "virdo.png", "wario.png" };
    for (auto& name : names) {
        ActorChrisEngine2 racer;
        racer.m_name = name;
        racer.components.push_back(EngineUtilities::MakeShared<TransformChrisEngine2>());
        racer.setTexture(rm.getTexture(name));
        actors.push_back(racer);
    }
}
