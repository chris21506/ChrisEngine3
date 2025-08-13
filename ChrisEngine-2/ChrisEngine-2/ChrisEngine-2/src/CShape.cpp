// CShape para ChrisEngine-2
#include "CShapeChrisEngine2.h"
#include "Window.h"

// Constructor por defecto: crea un placeholder (CIRCLE)
CShapeChrisEngine2::CShapeChrisEngine2()
    : Component(ComponentType::SHAPE)
    , m_shapeType(ShapeType::EMPTY)
{
    createShape(ShapeType::CIRCLE);
}

// Constructor con tipo solicitado
CShapeChrisEngine2::CShapeChrisEngine2(ShapeType shapeType)
    : Component(ComponentType::SHAPE)
    , m_shapeType(ShapeType::EMPTY)
{
    createShape(shapeType);
}

// Crear forma según ShapeType
void CShapeChrisEngine2::createShape(ShapeType shapeType) {
    m_shapeType = shapeType;
    switch (shapeType) {
    case ShapeType::CIRCLE: {
        auto circleSP = EngineUtilities::MakeShared<sf::CircleShape>(10.f);
        circleSP->setFillColor(sf::Color::Green);
        m_shapePtr = circleSP;
        break;
    }
    case ShapeType::RECTANGLE: {
        auto rectSP = EngineUtilities::MakeShared<sf::RectangleShape>(sf::Vector2f(100.f, 50.f));
        rectSP->setFillColor(sf::Color::White);
        m_shapePtr = rectSP;
        break;
    }
    case ShapeType::TRIANGLE: {
        auto convex = EngineUtilities::MakeShared<sf::ConvexShape>(3);
        convex->setPoint(0, { 0.f,0.f });
        convex->setPoint(1, { 50.f,100.f });
        convex->setPoint(2, { 100.f,0.f });
        convex->setFillColor(sf::Color::Blue);
        m_shapePtr = convex;
        break;
    }
    case ShapeType::POLYGON: {
        auto poly = EngineUtilities::MakeShared<sf::ConvexShape>(5);
        poly->setPoint(0, { 0.f,0.f });
        poly->setPoint(1, { 50.f,100.f });
        poly->setPoint(2, { 100.f,0.f });
        poly->setPoint(3, { 75.f,-50.f });
        poly->setPoint(4, { -25.f,-50.f });
        poly->setFillColor(sf::Color::Red);
        m_shapePtr = poly;
        break;
    }
    default:
        m_shapePtr.reset();
        ERROR("CShapeChrisEngine2", "createShape", "Unknown shape type");
        return;
    }
}

// Ciclo de vida ECS (sin lógica especial)
void CShapeChrisEngine2::start() {}
void CShapeChrisEngine2::update(float) {}
void CShapeChrisEngine2::destroy() {}

// Render
void CShapeChrisEngine2::render(const EngineUtilities::TSharedPointer<Window>& window) {
    if (m_shapePtr) window->draw(*m_shapePtr);
    else ERROR("CShapeChrisEngine2", "render", "Shape not initialized");
}

// Transformaciones
void CShapeChrisEngine2::setPosition(float x, float y) {
    if (m_shapePtr) m_shapePtr->setPosition({ x,y });
    else ERROR("CShapeChrisEngine2", "setPosition", "Shape not initialized");
}
void CShapeChrisEngine2::setPosition(const sf::Vector2f& pos) {
    if (m_shapePtr) m_shapePtr->setPosition(pos);
    else ERROR("CShapeChrisEngine2", "setPosition", "Shape not initialized");
}
void CShapeChrisEngine2::setFillColor(const sf::Color& color) {
    if (m_shapePtr) m_shapePtr->setFillColor(color);
    else ERROR("CShapeChrisEngine2", "setFillColor", "Shape not initialized");
}
void CShapeChrisEngine2::setRotation(float angleDegrees) {
    if (m_shapePtr) m_shapePtr->setRotation(sf::degrees(angleDegrees));
    else ERROR("CShapeChrisEngine2", "setRotation", "Shape not initialized");
}
void CShapeChrisEngine2::setScale(const sf::Vector2f& scl) {
    if (m_shapePtr) m_shapePtr->setScale(scl);
    else ERROR("CShapeChrisEngine2", "setScale", "Shape not initialized");
}

// Obtener puntero crudo a la forma
sf::Shape* CShapeChrisEngine2::getShape() {
    return m_shapePtr.get();
}

// Asignar textura (asegúrate que la textura viva mientras se renderiza)
void CShapeChrisEngine2::setTexture(const EngineUtilities::TSharedPointer<Texture>& texture) {
    if (m_shapePtr && texture && !texture.isNull())
        m_shapePtr->setTexture(&texture->getTexture());
}
