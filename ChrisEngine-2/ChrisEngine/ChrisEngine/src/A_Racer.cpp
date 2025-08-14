#pragma once

#include "ChrisEnginePrerequisites.h"
#include "ECS/Transform.h"
#include <vector>
#include <cmath>

/**
 * @class ChrisEngineRacer
 * @brief Representa un coche/corredor en la pista de ChrisEngine.
 */
class ChrisEngineRacer : public Actor {
public:
    /**
     * @brief Constructor: recibe el nombre del personaje.
     */
    ChrisEngineRacer(const std::string& characterName)
        : Actor(characterName) {
    }

    /**
     * @brief Define el camino/pista por la que se moverá el corredor.
     */
    void setPath(const std::vector<sf::Vector2f>& pathPoints) {
        path = pathPoints;
        if (!path.empty()) {
            auto xf = getComponent<Transform>();
            xf->setPosition(path.front());
            xf->setRotation(0.f);
        }
        currentWaypointIndex = (path.size() > 1 ? 1 : 0);
        Actor::update(0.f); // sincroniza sprite
    }

    /**
     * @brief Reinici*
