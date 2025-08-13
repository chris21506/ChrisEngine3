// BaseApp para ChrisEngine-2
#include "BaseAppChrisEngine2.h"
#include "Prerequisites.h"
#include "Window.h"
#include "EngineGUI.h"
#include "ChrisEngine2Racer.h"
#include "ECS/Transform.h"
#include "CShape.h"

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>

#include <cmath>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <filesystem>

// --- Helpers geométricos y path ---
namespace {
    inline float vlen(const sf::Vector2f& v) { return std::sqrt(v.x * v.x + v.y * v.y); }
    inline sf::Vector2f vnorm(const sf::Vector2f& v) {
        float L = vlen(v);
        return (L > 1e-6f) ? sf::Vector2f{ v.x / L, v.y / L } : sf::Vector2f{ 0.f, 0.f };
    }
    inline sf::Vector2f vperp(const sf::Vector2f& v) { return sf::Vector2f{ -v.y, v.x }; }

    // Asset path
    static std::filesystem::path assetPath(const std::string& rel) {
        auto p = std::filesystem::current_path() / "bin" / rel;
        std::error_code ec;
        return std::filesystem::weakly_canonical(p, ec);
    }

    // Funciones para densificar, offset, dibujar paths, guardar/cargar rutas
    // (igual que tu código original, adaptadas si quieres)
}

// --- Estado global de edición y jugador ---
static bool s_editMode = false;
static std::vector<sf::Vector2f> s_editPts;
static sf::Vector2f s_playerVel{ 0.f,0.f };
static float s_playerAngleDeg = 0.f;
static bool s_playerInit = false;

// --- Parámetros de control jugador ---
static float s_pcAccel = 400.f;
static float s_pcTurnRad = 2.6f;
static float s_pcMaxSpeed = 300.f;
static float s_pcFriction = 0.90f;

// ---------------- BaseAppChrisEngine2 ----------------

BaseAppChrisEngine2::~BaseAppChrisEngine2() {}

bool BaseAppChrisEngine2::init() {
    // Ventana
    m_windowPtr = EngineUtilities::MakeShared<Window>(1920, 1080, "ChrisEngine-2");
    if (!m_windowPtr) return false;

    gui.init(m_windowPtr);

    // Cargar pista "pista de carreras.png"
    if (!resourceMan.loadTexture("Sprites/Track", "pista de carreras.png")) {
        MESSAGE("BaseAppChrisEngine2", "init", "Cannot load pista de carreras.png");
    }

    auto trackTex = resourceMan.getTexture("Sprites/Track");
    m_trackActor = EngineUtilities::MakeShared<Actor>("Track");
    {
        auto sh = m_trackActor->getComponent<CShape>();
        sh->createShape(ShapeType::RECTANGLE);
        sh->setFillColor(sf::Color::White);
        auto sz = trackTex->getTexture().getSize();
        if (auto r = dynamic_cast<sf::RectangleShape*>(sh->getShape())) {
            r->setSize({ float(sz.x), float(sz.y) });
            r->setOrigin({ 0.f,0.f });
        }
        sh->setScale({ 1920.f / float(sz.x), 1080.f / float(sz.y) });
    }
    m_trackActor->setTexture(trackTex);
    m_trackActor->getComponent<Transform>()->setPosition({ 0.f,0.f });

    // Cargar ruta por defecto si existe
    std::vector<sf::Vector2f> loaded;
    if (loadPathTxt("Paths/track.path", loaded)) {
        s_editPts = loaded;
        if (!s_editPts.empty() && vlen(s_editPts.front() - s_editPts.back()) > 5.f)
            s_editPts.push_back(s_editPts.front());
        m_path = densifyClosed(s_editPts, 30.f);
    }

    // --- Crear corredores: Princesa, Sonic, Virdo, Wario ---
    auto r1 = EngineUtilities::MakeShared<ChrisEngine2Racer>("Princesa", 1);
    auto r2 = EngineUtilities::MakeShared<ChrisEngine2Racer>("Sonic", 2);
    auto r3 = EngineUtilities::MakeShared<ChrisEngine2Racer>("Virdo", 3);
    auto r4 = EngineUtilities::MakeShared<ChrisEngine2Racer>("Wario", 4);

    // Asignar carriles si hay ruta
    if (!m_path.empty()) {
        auto lane0 = m_path;
        auto lane1 = offsetClosed(m_path, +12.f);
        auto lane2 = offsetClosed(m_path, -12.f);
        auto lane3 = offsetClosed(m_path, +24.f);

        r1->setPath(lane0); r2->setPath(lane1); r3->setPath(lane2); r4->setPath(lane3);
        if (auto xf = r1->getComponent<Transform>()) xf->setPosition(lane0.front());
        if (auto xf = r2->getComponent<Transform>()) xf->setPosition(lane1.front());
        if (auto xf = r3->getComponent<Transform>()) xf->setPosition(lane2.front());
        if (auto xf = r4->getComponent<Transform>()) xf->setPosition(lane3.front());
    }

    // Texturas de sprites
    resourceMan.loadTexture("Sprites/Princesa", "princesa.png");
    resourceMan.loadTexture("Sprites/Sonic", "sonic.png");
    resourceMan.loadTexture("Sprites/Virdo", "virdo.png");
    resourceMan.loadTexture("Sprites/Wario", "wario.png");

    if (auto t = resourceMan.getTexture("Sprites/Princesa"); !t.isNull()) r1->setTexture(t);
    if (auto t = resourceMan.getTexture("Sprites/Sonic"); !t.isNull()) r2->setTexture(t);
    if (auto t = resourceMan.getTexture("Sprites/Virdo"); !t.isNull()) r3->setTexture(t);
    if (auto t = resourceMan.getTexture("Sprites/Wario"); !t.isNull()) r4->setTexture(t);

    m_racers = { r1, r2, r3, r4 };

    // Meta y vueltas
    m_finishLine = sf::FloatRect{ {1800.f,500.f},{50.f,200.f} };
    for (auto& r : m_racers) { r->setFinishLine(m_finishLine); r->setTotalLaps(3); }

    // Escalar sprites a 48 px
    auto fitSprite = [&](auto& racer, auto& tex) {
        if (tex.isNull()) return;
        auto sz = tex->getTexture().getSize();
        float s = 48.f / float(std::max(sz.x, sz.y));
        if (auto xf = racer->getComponent<Transform>()) xf->setScale({ s,s });
        };
    fitSprite(r1, resourceMan.getTexture("Sprites/Princesa"));
    fitSprite(r2, resourceMan.getTexture("Sprites/Sonic"));
    fitSprite(r3, resourceMan.getTexture("Sprites/Virdo"));
    fitSprite(r4, resourceMan.getTexture("Sprites/Wario"));

    m_playerIdx = -1; s_playerVel = { 0.f,0.f }; s_playerInit = false;
    gui.setRacers(m_racers);
    return true;
}

void BaseAppChrisEngine2::destroy() {
    m_racers.clear();
    m_finishedOrder.clear();
    m_trackActor.reset();
}
