#pragma once
#include "PxButton.h"

PxButton::PxButton()
{

}

PxButton::PxButton(sf::FloatRect rect, sf::Color c, float fs, char * txt, sf::Font& f)
{
// Set variables //
    control = rect;
    setColor(c);
    textSize = fs;
    isEnabled = true;
// Prepare visible text //
   mainText = sf::Text(txt, f, 1);
// Get font size //
    uint16_t first = 0, last = MAX_FONTSIZE;
    while (last - first > 1)
    {
        uint16_t middle = first + (last-first) / 2;
        mainText.setCharacterSize(middle);
        if (mainText.getLocalBounds().width < fs*USS)
            first = middle;
        else last = middle;
    }
// Set text options //
    mainText.setCharacterSize(first);
    sf::FloatRect mt = mainText.getGlobalBounds();
    mainText.setFillColor(sf::Color::Black);
    mainText.setOrigin(mt.left + mt.width / 2, 
        mt.top + mt.height / 2);
}

void PxButton::setColor(sf::Color c)
{
    boxColor.r = c.r * brightBox;
    boxColor.g = c.g * brightBox;
    boxColor.b = c.b * brightBox;
    overColor.r = c.r * brightOver;
    overColor.g = c.g * brightOver;
    overColor.b = c.b * brightOver;
    pressColor.r = c.r * brightPress;
    pressColor.g = c.g * brightPress;
    pressColor.b = c.b * brightPress;
    update();
}

void PxButton::setSprite(sf::Sprite * sprt)
{
    backGround = sprt;
    backGround->setPosition(control.left, control.top);
    update();
}

void PxButton::setPosition(float x, float y)
{
    backGround->setPosition(x, y);
    control.left = x, control.top = y;
}

void PxButton::update()
{
    if (backGround)
    {
        if (isPressed) backGround->setColor(pressColor);
        else if (isOver) backGround->setColor(overColor);
        else backGround->setColor(boxColor);
    }
}

bool PxButton::checkPressed(sf::FloatRect box, sf::Vector2f mindPos, float x, float y)
{
    if (!isPressed && isEnabled)
    {
        sf::FloatRect real;
        real.left = control.left - mindPos.x;
        real.top = control.top - mindPos.y;
        real.width = control.width * USS;
        real.height = control.height * USS;
        if (real.contains(x - box.left, y - box.top))
        {
            isPressed = true;
            update();
            return true;
        }
    }
    return false;
}

bool PxButton::checkOver(sf::FloatRect box, sf::Vector2f mindPos, float x, float y)
{
    if (!isPressed && isEnabled)
    {
        sf::FloatRect real;
        real.left = control.left - mindPos.x;
        real.top = control.top - mindPos.y;
        real.width = control.width * USS;
        real.height = control.height * USS;
        if (real.contains(x - box.left, y - box.top))
        {
            isOver = true;
            update();
            return true;
        }
        else if (isOver)
        {
            isOver = false;
            update();
        }
    }
    return false;
}

bool PxButton::checkReleased(sf::FloatRect box, sf::Vector2f mindPos, float x, float y)
{
    if (isPressed && isEnabled)
    {
        sf::FloatRect real;
        real.left = control.left - mindPos.x;
        real.top = control.top - mindPos.y;
        real.width = control.width * USS;
        real.height = control.height * USS;
        //printf("%f %f %f %f\n", real.left, real.top, real.width, real.height);
        isPressed = false;
        update();
        if (real.contains(x - box.left, y - box.top))
            return true;
    }
    return false;
}


PxButton::~PxButton()
{
    if (!hasSpecialSprite)
    {
        delete backGround->getTexture();
        delete backGround;
    }
}
