#include <SFML/Graphics.hpp>
#include <optional>
#include <cmath>

using namespace sf;

class SpriteAnimation {
public:
    SpriteAnimation(const sf::Texture &texture)
        : m_sprite(texture) {
    }

    void setAnimation(
        Vector2i size,
        Vector2i position,
        int count,
        int currentFrame = 0,
        bool isRepeat = true
    ) {
        m_frameSize = size;
        m_startFrame = position;
        m_frameCount = count;
        m_currentFrame = currentFrame;
        m_repeat = isRepeat;
        updateTextureRect();
    };

    void nextFrame() {
        if (m_frameCount <= 0) return;
        m_currentFrame = (m_currentFrame + 1) % m_frameCount;
        updateTextureRect();
    }

    void draw(sf::RenderWindow &window) {
        window.draw(m_sprite);
    }

    void setPosition(sf::Vector2f pos) {
        m_sprite.setPosition(pos);
    }

    void setScale(sf::Vector2f scale) {
        m_sprite.setScale(scale);
    }

    void setOrigin(sf::Vector2f origin) {
        m_sprite.setOrigin(origin);
    }

    Vector2f getPosition() {
        return m_sprite.getPosition();
    }

private:
    void updateTextureRect() {
        sf::Vector2i offset = m_startFrame + sf::Vector2i{
                                  m_currentFrame * m_frameSize.x,
                                  0
                              };
        m_sprite.setTextureRect(sf::IntRect(offset, m_frameSize));
    }

    sf::Sprite m_sprite;
    sf::Vector2i m_frameSize{0, 0};
    sf::Vector2i m_startFrame{0, 0};
    int m_frameCount = 0;
    int m_currentFrame = 0;
    bool m_repeat = true;
    float m_time = 0.f;
};

class Cat {
public:
    Cat(const sf::Texture &texture)
        : m_animatedSprite(texture) {
        m_animatedSprite.setAnimation({64, 64}, {296, 256}, 5);
        m_animatedSprite.setOrigin({32.f, 32.f});
    }

    void update(float dt, const sf::Vector2f &targetPos) {
        const float MAX_SPEED_PX = 100.f;
        const float FRAME_DISTANCE = 50.f;


        Vector2f currPos = m_animatedSprite.getPosition();
        Vector2f distanceXY = targetPos - currPos;

        float distance = std::sqrt(distanceXY.x * distanceXY.x + distanceXY.y * distanceXY.y);

        if (distance > 3.f) {
            sf::Vector2f direction = distanceXY / distance;
            float speed = MAX_SPEED_PX * dt;
            Vector2f newPos = currPos + direction * speed;

            float deltaDist = std::sqrt(
                (newPos.x - currPos.x) * (newPos.x - currPos.x) +
                (newPos.y - currPos.y) * (newPos.y - currPos.y)
            );
            m_distanceAccumulator += deltaDist;

            if (m_distanceAccumulator >= FRAME_DISTANCE) {
                m_animatedSprite.nextFrame();
            }

            m_animatedSprite.setPosition(newPos);
        }
    }


    void draw(sf::RenderWindow &window) {
        m_animatedSprite.draw(window);
    }

    void setPosition(Vector2f pos) {
        m_animatedSprite.setPosition(pos);
    }

    Vector2f getPosition() {
        return m_animatedSprite.getPosition();
    }

private:
    SpriteAnimation m_animatedSprite;
    float m_speed = 100.f;
    float m_distanceAccumulator  = 0.f;
};

int main() {
    RenderWindow window(VideoMode({800u, 600u}), "Cat walking");

    Image image;
    if (!image.loadFromFile("../images/WhiteCat.png"))
        return -1;

    Texture texture;
    if (!texture.loadFromImage(image))
        return -1;

    Cat cat(texture);
    cat.setPosition({100.f, 100.f});

    CircleShape cursorTarget(8.f);
    cursorTarget.setFillColor(Color::Red);
    cursorTarget.setOrigin({8.f, 8.f});

    Clock clock;

    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();

        std::optional<Event> eventOpt;
        while ((eventOpt = window.pollEvent())) {
            const auto &event = *eventOpt;
            if (event.is<Event::Closed>())
                window.close();
        }

        Vector2i mousePos = Mouse::getPosition(window);
        Vector2f targetPos(mousePos);
        cursorTarget.setPosition(targetPos);

        cat.update(dt, targetPos);

        window.clear();
        cat.draw(window);
        window.draw(cursorTarget);
        window.display();
    }

    return 0;
}
