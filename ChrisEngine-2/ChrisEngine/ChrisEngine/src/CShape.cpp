// ==================== CShape para ChrisEngine ====================
class CShape : public Component {
    ShapeType m_shapeType;
    TSharedPointer<sf::Shape> m_shapePtr;

public:
    // Constructores
    CShape() : Component(ComponentType::SHAPE), m_shapeType(ShapeType::EMPTY) {
        createShape(ShapeType::CIRCLE); // placeholder
    }
    CShape(ShapeType shapeType) : Component(ComponentType::SHAPE), m_shapeType(ShapeType::EMPTY) {
        createShape(shapeType);
    }

    // Crear shape según tipo
    void createShape(ShapeType shapeType) {
        m_shapeType = shapeType;
        switch (shapeType) {
        case ShapeType::CIRCLE:
            m_shapePtr = MakeShared<sf::CircleShape>(10.f);
            m_shapePtr->setFillColor(sf::Color::Green);
            break;
        case ShapeType::RECTANGLE:
            m_shapePtr = MakeShared<sf::RectangleShape>(sf::Vector2f(100.f, 50.f));
            m_shapePtr->setFillColor(sf::Color::White);
            break;
        case ShapeType::TRIANGLE:
            auto tri = MakeShared<sf::ConvexShape>(3);
            tri->setPoint(0, { 0,0 }); tri->setPoint(1, { 50,100 }); tri->setPoint(2, { 100,0 });
            tri->setFillColor(sf::Color::Blue);
            m_shapePtr = tri;
            break;
        case ShapeType::POLYGON:
            auto poly = MakeShared<sf::ConvexShape>(5);
            poly->setPoint(0, { 0,0 }); poly->setPoint(1, { 50,100 }); poly->setPoint(2, { 100,0 });
            poly->setPoint(3, { 75,-50 }); poly->setPoint(4, { -25,-50 });
            poly->setFillColor(sf::Color::Red);
            m_shapePtr = poly;
            break;
        default:
            m_shapePtr.reset();
            ERROR("CShape", "createShape", "Unknown shape type");
        }
    }

    // Ciclo de vida
    void start() {}
    void update(float) {}
    void destroy() {}

    // Render
    void render(const TSharedPointer<Window>& window) {
        if (m_shapePtr) window->draw(*m_shapePtr);
        else ERROR("CShape", "render", "Shape not initialized");
    }

    // Transformaciones
    void setPosition(float x, float y) { if (m_shapePtr) m_shapePtr->setPosition({ x,y }); }
    void setPosition(const sf::Vector2f& pos) { if (m_shapePtr) m_shapePtr->setPosition(pos); }
    void setFillColor(const sf::Color& col) { if (m_shapePtr) m_shapePtr->setFillColor(col); }
    void setRotation(float deg) { if (m_shapePtr) m_shapePtr->setRotation(sf::degrees(deg)); }
    void setScale(const sf::Vector2f& scl) { if (m_shapePtr) m_shapePtr->setScale(scl); }

    // Getters
    sf::Shape* getShape() { return m_shapePtr.get(); }
    void setTexture(const TSharedPointer<Texture>& tex) {
        if (m_shapePtr && tex && !tex.isNull()) m_shapePtr->setTexture(&tex->getTexture());
    }
};
