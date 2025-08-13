#include "ChrisEngine2Racer.h"
#include "ECS/Transform.h"
#include <cmath>
#include <algorithm>

// --- Funciones auxiliares ---
static inline float vlen(const sf::Vector2f& v) {
    return std::sqrt(v.x * v.x + v.y * v.y);
}
static inline sf::Vector2f vnorm(const sf::Vector2f& v) {
    float L = vlen(v);
    return (L > 1e-5f) ? sf::Vector2f{ v.x / L, v.y / L } : sf::Vector2f{ 0.f, 0.f };
}
static inline float clamp01(float x) {
    return std::max(0.f, std::min(1.f, x));
}

// --- Constructor ---
ChrisEngine2Racer::ChrisEngine2Racer(const std::string& name, int /*playerId*/)
    : Actor(name) {
}

// --- Define ruta del corredor ---
void ChrisEngine2Racer::setPath(const std::vector<sf::Vector2f>& pathPoints) {
    path = pathPoints;
    if (!path.empty()) {
        if (auto xf = getComponent<Transform>()) {
            xf->setPosition(path.front());
            xf->setRotation(0.f);
        }
    }
    currentWaypointIndex = (path.size() > 1 ? 1 : 0);
    Actor::update(0.f); // sincroniza sprite con Transform
}

// --- Reinicia estado del corredor ---
void ChrisEngine2Racer::reset() {
    m_currentLap = 0;
    m_place = 0;
    m_crossedLastFrame = false;
    if (!path.empty()) {
        if (auto xf = getComponent<Transform>()) {
            xf->setPosition(path.front());
            xf->setRotation(0.f);
        }
    }
    currentWaypointIndex = (path.size() > 1 ? 1 : 0);
    Actor::update(0.f);
}

// --- Progreso de la vuelta ---
float ChrisEngine2Racer::getProgress() const {
    int N = (int)path.size();
    if (N < 2) return 0.f;
    auto xf = const_cast<ChrisEngine2Racer*>(this)->getComponent<Transform>();
    sf::Vector2f pos = xf ? xf->getPosition() : path.front();
    int cur = currentWaypointIndex % N;
    int prev = (cur + N - 1) % N;
    sf::Vector2f A = path[prev];
    sf::Vector2f B = path[cur];
    float segLen = std::max(1e-4f, vlen(B - A));
    float t = 1.f - clamp01(vlen(B - pos) / segLen);
    return clamp01((prev + t) / float(N));
}

// --- Actualización por frame ---
void ChrisEngine2Racer::update(float deltaTime) {
    if (!isFinished() && path.size() >= 2) {
        doPathFollowing(deltaTime);
        bool inside = m_finishLine.contains(getComponent<Transform>()->getPosition());
        if (inside && !m_crossedLastFrame) ++m_currentLap;
        m_crossedLastFrame = inside;
    }
    Actor::update(deltaTime); // sincroniza Transform -> sprite
}

// --- Lógica de seguimiento del path (Pure Pursuit + Arrive) ---
void ChrisEngine2Racer::doPathFollowing(float dt) {
    auto xf = getComponent<Transform>();
    if (!xf || path.size() < 2) return;

    sf::Vector2f pos = xf->getPosition();
    int N = (int)path.size();
    int i = currentWaypointIndex;
    sf::Vector2f A = path[i];
    sf::Vector2f B = path[(i + 1) % N];
    sf::Vector2f AB = B - A;
    float abLen2 = AB.x * AB.x + AB.y * AB.y;
    if (abLen2 < 1e-6f) { currentWaypointIndex = (i + 1) % N; return; }
    float abLen = std::sqrt(abLen2);

    sf::Vector2f AP = pos - A;
    float t = (AP.x * AB.x + AP.y * AB.y) / abLen2;

    float distToB = std::hypot(pos.x - B.x, pos.y - B.y);
    if (t > 1.f || distToB < arriveRadius) {
        currentWaypointIndex = (i + 1) % N;
        i = currentWaypointIndex;
        A = path[i];
        B = path[(i + 1) % N];
        AB = B - A;
        abLen2 = AB.x * AB.x + AB.y * AB.y;
        abLen = std::sqrt(abLen2);
        if (abLen2 < 1e-6f) return;
        AP = pos - A;
        t = (AP.x * AB.x + AP.y * AB.y) / abLen2;
    }

    float s = std::clamp(t + (lookaheadDistance / std::max(abLen, 1e-3f)), 0.f, 1.f);
    sf::Vector2f pursue = A + AB * s;

    sf::Vector2f to = pursue - pos;
    float d = std::hypot(to.x, to.y);
    if (d > 1e-4f) {
        sf::Vector2f dir = { to.x / d, to.y / d };
        float brakeRadius = lookaheadDistance * 1.2f;
        float speed = (d < brakeRadius) ? (m_maxSpeed * (d / brakeRadius)) : m_maxSpeed;
        pos += dir * speed * dt;

        float angleDeg = std::atan2(dir.y, dir.x) * 180.f / 3.14159265f;
        xf->setRotation(angleDeg + m_spriteAngleOffset);
        xf->setPosition(pos);
    }
}
