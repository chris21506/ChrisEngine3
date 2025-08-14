// ChrisEngineApp.cpp
#include "ChrisEngineApp.h"
#include "ChrisEnginePrerequisites.h"
#include "Window.h"
#include "EngineGUI.h"
#include "ChrisEngineRacer.h"
#include "ECS/Transform.h"
#include "CShape.h"

#include <vector>
#include <cmath>
#include <iostream>
#include <filesystem>

// ---------------- Helper geométrico ----------------
namespace {
    inline float vlen(const Vector2f& v) { return sqrt(v.x * v.x + v.y * v.y); }
    inline Vector2f vnorm(const Vector2f& v) {
        float L = vlen(v); return (L > 1e-6f) ? Vector2f{ v.x / L, v.y / L } : Vector2f{ 0.f,0.f };
    }
    inline Vector2f vperp(const Vector2f& v) { return Vector2f{ -v.y, v.x }; }

    // Densificar polilínea cerrada
    std::vector<Vector2f> densifyClosed(const std::vector<Vector2f>& pts, float maxSegLen) {
        std::vector<Vector2f> out;
        if (pts.size() < 2) return out;
        int N
            // ==================== ChrisEngine BaseApp ====================
            class ChrisEngine : public BaseApp {
            std::vector<TSharedPointer<A_Racer>> m_racers;
            TSharedPointer<Actor> m_trackActor;
            std::vector<sf::Vector2f> m_path;
            int m_playerIdx = -1;

            public:
                bool init() {
                    m_windowPtr = MakeShared<Window>(1920, 1080, "ChrisEngine");
                    gui.init(m_windowPtr);

                    // Carga pista
                    resourceMan.loadTexture("Sprites/PistaDeCarreras", "png");
                    auto trackTex = resourceMan.getTexture("Sprites/PistaDeCarreras");

                    m_trackActor = MakeShared<Actor>("Pista");
                    auto sh = m_trackActor->getComponent<CShape>();
                    sh->createShape(ShapeType::RECTANGLE);
                    sh->setFillColor(sf::Color::White);
                    sh->setScale({ 1920.f / trackTex->getTexture().getSize().x,
                                  1080.f / trackTex->getTexture().getSize().y });
                    m_trackActor->setTexture(trackTex);

                    // Crea corredores: Princesa, Sonic, Virdo, Wario
                    auto r1 = MakeShared<A_Racer>("Princesa", 1);
                    auto r2 = MakeShared<A_Racer>("Sonic", 2);
                    auto r3 = MakeShared<A_Racer>("Virdo", 3);
                    auto r4 = MakeShared<A_Racer>("Wario", 4);

                    // Asigna texturas
                    resourceMan.loadTexture("Sprites/Princesa", "png");
                    resourceMan.loadTexture("Sprites/Sonic", "png");
                    resourceMan.loadTexture("Sprites/Virdo", "png");
                    resourceMan.loadTexture("Sprites/Wario", "png");

                    if (auto t = resourceMan.getTexture("Sprites/Princesa"); !t.isNull()) r1->setTexture(t);
                    if (auto t = resourceMan.getTexture("Sprites/Sonic"); !t.isNull()) r2->setTexture(t);
                    if (auto t = resourceMan.getTexture("Sprites/Virdo"); !t.isNull()) r3->setTexture(t);
                    if (auto t = resourceMan.getTexture("Sprites/Wario"); !t.isNull()) r4->setTexture(t);

                    m_racers = { r1,r2,r3,r4 };
                    return true;
                }
        };
