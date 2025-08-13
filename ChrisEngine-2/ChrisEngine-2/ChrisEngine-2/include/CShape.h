#pragma once

/**
 * @file ChrisEngine2Shape.h
 * @brief Declares the ChrisEngine2Shape class used to represent 2D shapes for characters and track.
 *
 * - Puede usarse para los personajes: princesa.png, sonic.png, virdo.png, wario.png.
 * - Puede representar la pista: pista de carreras.png.
 */

#include <SFML/Graphics.hpp> // sf::Shape, sf::CircleShape, etc.
#include <Prerequisites.h>   // ShapeType y ComponentType
#include <Memory/TSharedPointer.h>
#include <ECS/Component.h>
#include <ECS/Texture.h>

class Window;

/**
 * @class ChrisEngine2Shape
 * @brief Componente que representa un shape 2D de SFML y puede contener una textura.
 */
class ChrisEngine2Shape : public Component {
public:
    /**
     * @brief Constructor por defecto. Shape vacío inicialmente.
     */
    ChrisEngine2Shape();

    /**
     * @brief Construye un shape del tipo dado inmediatamente.
     * @param shapeType Tipo de primitiva a crear.
     */
    explicit ChrisEngine2Shape(ShapeType shapeType);

    /**
     * @brief Destructor virtual.
     */
    ~ChrisEngine2Shape() override = default;

    /**
     * @brief Llamado una vez al iniciar el componente.
     */
    void start() override;

    /**
     * @brief Actualización por frame del componente.
     * @param deltaTime Tiempo transcurrido desde el último frame en segundos.
     */
    void update(float deltaTime) override;

    /**
     * @brief Renderiza el shape en la ventana dada.
     * @param window Ventana objetivo para dibujar.
     */
    void render(const EngineUtilities::TSharedPointer<Window>& window) override;

    /**
     * @brief Libera recursos del componente.
     */
    void destroy() override;

    /**
     * @brief Crea o reemplaza el shape SFML subyacente con un nuevo tipo.
     * @param shapeType Primitiva a construir (RECTANGLE, CIRCLE, etc.).
     */
    void createShape(ShapeType shapeType);

    /**
     * @brief Define la posición en el mundo del shape.
     * @param x Coordenada X.
     * @param y Coordenada Y.
     */
    void setPosition(float x, float y);

    /**
     * @brief Define la posición en el mundo del shape.
     * @param position Vector de posición.
     */
    void setPosition(const sf::Vector2f& position);

    /**
     * @brief Cambia el color de relleno del shape.
     * @param color Nuevo color a aplicar.
     */
    void setFillColor(const sf::Color& color);

    /**
     * @brief Define la rotación del shape en grados.
     * @param angleDegrees Ángulo en grados (horario).
     */
    void setRotation(float angleDegrees);

    /**
     * @brief Define la escala del shape en cada eje.
     * @param scl Factores de escala (x,y).
     */
    void setScale(const sf::Vector2f& scl);

    /**
     * @brief Obtiene el puntero crudo al shape SFML (puede ser nullptr si no se creó).
     * @return Puntero al sf::Shape subyacente.
     */
    sf::Shape* getShape();

    /**
     * @brief Asigna una textura que se usará como relleno del shape.
     * @param texture Componente de textura (envolviendo un sf::Texture).
     */
    void setTexture(const EngineUtilities::TSharedPointer<Texture>& texture);

private:
    /**
     * @brief Puntero al shape SFML propiedad de este componente.
     */
    EngineUtilities::TSharedPointer<sf::Shape> m_shapePtr;

    /**
     * @brief Tipo de primitiva representada actualmente por este componente.
     */
    ShapeType m_shapeType = ShapeType::EMPTY;
};
