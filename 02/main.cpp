#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

float toDegrees(float radians)
{
    return float(double(radians) * 180.f / M_PI);
}

void updateRotation(sf::ConvexShape &arrow, float dt, const sf::Vector2f &distanceXY)
{
    const float MAX_ROTATION_SPEED = 90; // Pi/2
    if (distanceXY.length() > 0.5f)
    {
        const float currAngle = arrow.getRotation().asDegrees();
        const float radians = atan2(distanceXY.y, distanceXY.x);
        float mouseAngle = toDegrees(radians);
        if (mouseAngle <= 0)
        {
            mouseAngle += 360;
        }
        float angle = mouseAngle - currAngle;
        if ( angle > -0.5f && angle < 0.5f) 
        {
            // std::cout << "inside=" << angle << std::endl;
            arrow.setRotation(sf::degrees(mouseAngle));
            return;
        }
        if (abs(angle) > 180)
        {
            if (angle <= 0)
            {
                angle = 360 + angle;
            }
            else
            {
                angle = angle - 360;
            }
        }
        float rotationSpeed = MAX_ROTATION_SPEED * dt; // без деления на 100 поворачивает очень быстро.
        if (angle < 0) rotationSpeed = - rotationSpeed;
        arrow.setRotation(sf::degrees(currAngle + rotationSpeed));
    }
}

void updateMotion(const sf::Vector2f &currPos, sf::ConvexShape &arrow, float dt, const sf::Vector2f &distanceXY)
{
    const float MAX_SPEED_PX = 20;
    const float distance = distanceXY.length();
    // добавил условие иначе стрелка исчезала, видимо из-за деления на 0
    if (distance > 0)
    {
        const float cos = distanceXY.x / distance;
        const float sin = distanceXY.y / distance;
        sf::Vector2f ratioXY = {cos, sin};
        float speed = MAX_SPEED_PX * dt;
        arrow.setPosition(currPos + speed * ratioXY);
    }
}

void update(const sf::Vector2f &mousePos, sf::ConvexShape &arrow, sf::Clock &clock)
{
    const float dt = clock.restart().asSeconds();

    const sf::Vector2f currPos = arrow.getPosition();
    const sf::Vector2f distanceXY = mousePos - currPos;

    updateRotation(arrow, dt, distanceXY);
    updateMotion(currPos, arrow, dt, distanceXY);
}

void initArrow(sf::ConvexShape &arrow)
{
    arrow.setPointCount(7);
    arrow.setPoint(0, {0, 0});
    arrow.setPoint(1, {-60, -70});
    arrow.setPoint(2, {-60, -35});
    arrow.setPoint(3, {-120, -35});
    arrow.setPoint(4, {-120, 35});
    arrow.setPoint(5, {-60, 35});
    arrow.setPoint(6, {-60, 70});
    arrow.setFillColor(sf::Color::Yellow);
    arrow.setOutlineThickness(3);
    arrow.setOutlineColor(sf::Color::Black);
    arrow.setPosition({400, 300});
}

void onMouseMove(const sf::Event::MouseMoved &event, sf::Vector2f &mousePos)
{
    mousePos = {float(event.position.x), float(event.position.y)};
}

void pollEvents(sf::RenderWindow &window, sf::Vector2f &mousePos)
{
    while (const std::optional ev = window.pollEvent())
    {
        if (ev->is<sf::Event::Closed>())
        {
            window.close();
        }
        if (const auto *mouseMove = ev->getIf<sf::Event::MouseMoved>())
        {
            onMouseMove(*mouseMove, mousePos);
        }
    }
}

void redrawFrame(sf::RenderWindow &window, sf::ConvexShape &arrow)
{
    window.clear(sf::Color::White);
    window.draw(arrow);
    window.display();
}

int main()
{
    constexpr unsigned WINDOW_WIDTH = 800;
    constexpr unsigned WINDOW_HEIGHT = 600;
    sf::ContextSettings settings;
    settings.antiAliasingLevel = 8;
    sf::RenderWindow window(
        sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}),
        "Arrow moves",
        sf::Style::Default,
        sf::State::Windowed,
        settings);

    sf::Clock clock;
    sf::ConvexShape arrow;
    sf::Vector2f mousePos;
    initArrow(arrow);

    while (window.isOpen())
    {
        pollEvents(window, mousePos);
        update(mousePos, arrow, clock);
        redrawFrame(window, arrow);
    }
}
