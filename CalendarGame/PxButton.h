#pragma once
#include "ownUtilities.h"
class PxButton
{
    const float USS = 2;
public:
// Constants //
// Fields //
    float brightOver = 0.8, brightPress = 0.6, brightBox = 0.7, MAX_FONTSIZE = 200;
    sf::Sprite * backGround = nullptr;
    sf::Font mainFont;
    sf::Text mainText;
    sf::Color boxColor = sf::Color::White, 
        overColor = sf::Color::White, 
        pressColor = sf::Color::White;
    sf::FloatRect control;
    bool isPressed = false, isOver = false, hasSpecialSprite = false, isEnabled = false;
    float fontSize, textSize;
// Methods //
    PxButton();
    PxButton(sf::FloatRect rect, sf::Color c, float fs, char * txt, sf::Font& f);
    void setColor(sf::Color c);
    void setSprite(sf::Sprite * sprt);
    void setPosition(float x, float y);
    void update();
    bool checkPressed(sf::FloatRect box, sf::Vector2f mindPos, float x, float y);
    bool checkOver(sf::FloatRect box, sf::Vector2f mindPos, float x, float y);
    bool checkReleased(sf::FloatRect box, sf::Vector2f mindPos, float x, float y);
    ~PxButton();
};

