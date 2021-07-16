#pragma once
#include "NoteInf.h"
#include "PxButton.h"
#include "TextBox.h"

class Note
{
public:
// Constants //
    const uint16_t WIDTH = 200;
    const uint16_t MIN_HEIGHT = 33;
    const uint16_t MAX_HEIGHT = 300;
    const float USS = 2;
// Fields //
    NoteInf note;
    sf::Sprite * backGround;
    sf::String cutted;
    TextBox *text;
    TextBox *time;
    sf::Vector2f move_target;
    sf::FloatRect control;
    bool upped = false, isDeleted = false;
    uint16_t stepAnim = 0, txtHeight = MIN_HEIGHT * USS;
    uint16_t Height = MIN_HEIGHT * USS, heightTo = MIN_HEIGHT * USS;
    size_t inArray = 0;
    PxButton *grabBtn, *throwBtn, *deleteBtn, *redoBtn, *upBtn, *downBtn;
// Methods //
    Note();
    Note(NoteInf inf, ownUtilities * owt, sf::Font& bF, sf::Font& tF);
    void setOfMin(float px, float py);
    void setOfMax(float px, float py);
    void setOpened();
    void setClosed();
    void update();
    void setPosition(float x, float y);
    ~Note();
};

