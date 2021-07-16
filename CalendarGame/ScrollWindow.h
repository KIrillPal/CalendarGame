#pragma once
#include "Note.h"

class ScrollWindow
{
    const float USS = 2;
public:
// Constants //
    const sf::Vector2f accl = sf::Vector2f(0.32f, 0.32f);
    const sf::Vector2f deaccl_f = sf::Vector2f(0.96f, 0.96f);
    const sf::Vector2f deaccl = sf::Vector2f(0.68f, 0.68f);
// Fields //
    sf::Vector2f mindPos, mindSize, speed, mindBounds;
    sf::FloatRect control;
    sf::RenderTexture mainField;
// Methods //
    ScrollWindow();
    ScrollWindow(sf::FloatRect real, sf::FloatRect mind);
    void update();
    void moveWindow(float dx, float dy);
    void draw(PxButton * btn);
    void draw(TextBox * txb, int16_t delta_h);
    void draw(Note * nte);
    void draw(sf::FloatRect rect, sf::Color c);
    void draw(sf::Sprite * spr);
    void openDrawing();
    bool checkPressed(float x, float y);
    bool checkMove(float dx, float dy);
    void uniteDrawing(sf::RenderWindow * window);
    bool isPressed = false;
    ~ScrollWindow();
};
