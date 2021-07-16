#pragma once
#include "ownUtilities.h"
#include "TextBox.h"
#include <string>


TextBox::TextBox(sf::String txt , sf::FloatRect mb, uint16_t sz, sf::Font& tF)
{
    text = txt;
    bounds = mb;

    resultBox = sf::Text(txt, tF, sz);
    resultBox.setPosition(mb.left, mb.top);
    resultBox.setFillColor(sf::Color::Black);
    setOptimal();
    charH = resultBox.getLocalBounds().height;
    updateCursor();
}

void TextBox::setOptimal()
{
    uint16_t first = 0, last = 70;
    while (last - first > 1)
    {
        uint16_t middle = first + (last - first) / 2;
        resultBox.setString(std::string(middle, 'A'));
        if (resultBox.getLocalBounds().width <= bounds.width)
            first = middle;
        else last = middle;
    }
    characterDown = first;
}

void TextBox::setPosition(float x, float y)
{
    resultBox.setPosition(x, y);
    bounds.left = x, bounds.top = y;
}

sf::String TextBox::modify(sf::String &txt)
{
    sf::String res;
    for (int i = 0; i < txt.getSize(); ++i)
    {
        res += txt[i];
        if ((i+1) % characterDown == 0 && i+1 != txt.getSize())
            res += '\n';
    }
    return res;
}

void TextBox::setFocused()
{
    if (isActive)
    {
        isFocused = true;
        last_text = text;
        timer = 0;
    }
}

bool TextBox::offFocused()
{
    bool res = isFocused;
    isFocused = false;
    isCursorShowed = false;
    return res;
}

void TextBox::update()
{
    if (isFocused && ++timer % MOD == 0)
        timer = 0, isCursorShowed ^= 1;
}

void TextBox::showAt(sf::RenderWindow * window, int16_t delta_h)
{
    window->draw(resultBox);
    if (isCursorShowed)
    {
        sf::RectangleShape cursorShape(sf::Vector2f(2, charH * 1.2));
        cursorShape.setPosition(cursorPos.x-1, cursorPos.y + delta_h);
        cursorShape.setFillColor(sf::Color::Black);
        window->draw(cursorShape);
    }
}

void TextBox::checkPressed(sf::FloatRect box, sf::Vector2f mindPos, float x, float y)
{
    sf::FloatRect bnd = resultBox.getGlobalBounds();
    bnd.left -= mindPos.x;
    bnd.top -= mindPos.y;
    bnd.width += charH;
    if (bnd.height == 0) bnd.height = charH;
    if (isActive && bnd.contains(x - box.left, y - box.top))
    {
        x -= box.left - mindPos.x;
        y -= box.top - mindPos.y;
        //printf("%f %f\n", box.left, box.top);
        if (!isFocused) setFocused();
        sf::String str = resultBox.getString();
        float minPath = -1;
        int minCh = -1, n = str.getSize();
        sf::Vector2f curs;
        for (size_t i = 0, j = 0; i <= n; ++i)
        {
            if (str[i] == '\n') continue;
            sf::Vector2f chPos = resultBox.findCharacterPos(i);
            float path = (x - chPos.x) * (x - chPos.x) + (y - chPos.y - charH) * (y - chPos.y - charH);
            if (minPath == -1 || path < minPath)
            {
                minPath = path;
                minCh = j;
                curs = chPos;
            }
            ++j;
        }
        cursorPos = curs;
        cursor = minCh;
    }
}

bool TextBox::checkKeyPressed(sf::Event::KeyEvent e)
{
    if (isActive && isFocused)
    {
        if (e.code == sf::Keyboard::Enter)
        {
            offFocused();
            return true;
        }
        else if (e.code == sf::Keyboard::Escape)
        {
            offFocused();
            text = last_text;
            updateCursor();
        }
        else if (e.code == sf::Keyboard::BackSpace && cursor > 0)
        {
            text.erase(cursor - 1);
            --cursor;
            updateCursor();
        }
        else if (e.code == sf::Keyboard::Delete && cursor < text.getSize())
        {
            text.erase(cursor);
            updateCursor();
        }
        else if (e.code == sf::Keyboard::Left && cursor > 0)
        {
            --cursor;
            updateCursor();
        }
        else if (e.code == sf::Keyboard::Right && cursor < text.getSize())
        {
            ++cursor;
            updateCursor();
        }
        else if (e.code == sf::Keyboard::Up)
        {
            checkPressed(sf::FloatRect(), sf::Vector2f(), cursorPos.x, cursorPos.y - charH);
        }
        else if (e.code == sf::Keyboard::Down)
        {
            checkPressed(sf::FloatRect(), sf::Vector2f(), cursorPos.x, cursorPos.y + 2*charH );
        }
    }
    return false;
}

void TextBox::checkInput(char key)
{
    if (isActive && isFocused && key != 8 && key != 9 && key != 27 &&
        key != 22 && key != 25 && key != 26)
    {
        if ((text.getSize() ) % characterDown == 0
            && resultBox.getGlobalBounds().height > bounds.height)
            return;
        text.insert(cursor, key);
        ++cursor;
        updateCursor();
    }
}

void TextBox::updateCursor()
{
    resultBox.setString(modify(text));
    cursorPos = resultBox.findCharacterPos(cursor + cursor / characterDown);
}



TextBox::TextBox()
{
}


TextBox::~TextBox()
{
}
