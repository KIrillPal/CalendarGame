#pragma once
class TextBox
{
    const float USS = 2;
public:
// Constants //
    const uint16_t MOD = 50;
// Fields //
    sf::String text, last_text = "";
    sf::Text resultBox;
    sf::Font mainFont;
    sf::FloatRect bounds;
    bool isActive = false, isFocused = false, isCursorShowed = false;
    sf::Vector2f cursorPos;
    uint16_t timer = 0, cursor = 0, characterDown = 30, charH = 20;
// Methods //
    TextBox(sf::String txt, sf::FloatRect mb, uint16_t sz, sf::Font& tF);
    sf::String modify(sf::String &txt);
    void setOptimal();
    void setPosition(float x, float y);
    void setFocused();
    bool offFocused();
    void update();
    void showAt(sf::RenderWindow * window, int16_t delta_h);
    void checkPressed(sf::FloatRect box, sf::Vector2f mindPos, float x, float y);
    bool checkKeyPressed(sf::Event::KeyEvent e);
    void checkInput(char key);
    void updateCursor();
    TextBox();
    ~TextBox();
};