#include "Note.h"
#include <string>


Note::Note()
{
}

Note::Note(NoteInf inf, ownUtilities * owt, sf::Font& bF, sf::Font& tF)
{
    // Set variables //
    note = inf;
    control.width = WIDTH;
    control.height = MIN_HEIGHT;
    backGround = owt->NoteShapes[MIN_HEIGHT];
    // Set buttons //
    grabBtn = new PxButton(sf::FloatRect(7 * USS, 3 * USS, 18, 27), sf::Color::White, 15, "", bF);
    grabBtn->setSprite(owt->noteGrabBack);
    grabBtn->hasSpecialSprite = true;
    deleteBtn = new PxButton(sf::FloatRect((WIDTH - 24) * USS, 3 * USS, 19, 27), sf::Color::White, 15, "", bF);
    deleteBtn->setSprite(owt->noteDelBack);
    deleteBtn->hasSpecialSprite = true;
    throwBtn = new PxButton(sf::FloatRect((WIDTH - 45) * USS, 3 * USS, 19, 27), sf::Color::White, 15, "", bF);
    throwBtn->setSprite(owt->noteThrowBack);
    throwBtn->hasSpecialSprite = true;
    upBtn = new PxButton(sf::FloatRect((WIDTH - 65) * USS, 3 * USS, 18, 27), sf::Color::White, 15, "", bF);
    upBtn->setSprite(owt->noteUpBack);
    upBtn->hasSpecialSprite = true;
    downBtn = new PxButton(sf::FloatRect((WIDTH - 65) * USS, 3 * USS, 18, 27), sf::Color::White, 15, "", bF);
    downBtn->setSprite(owt->noteDownBack);
    downBtn->hasSpecialSprite = true;
    redoBtn = new PxButton(sf::FloatRect((WIDTH - 24) * USS, 8 * USS, 18, 18), sf::Color::Red, 15, "", bF);
    redoBtn->setSprite(owt->noteUpBack);
    redoBtn->hasSpecialSprite = true;
    sf::String ts = std::to_string(note.time / 60) + ':' + std::to_string(note.time % 60);
    if (note.time % 60 == 0) ts += '0';
    time = new TextBox(ts, sf::FloatRect((WIDTH - 65) * USS, 16 * USS, (WIDTH - 90) * USS, 18 * USS), 15, tF);
    sf::FloatRect tsz = time->resultBox.getGlobalBounds();
    time->resultBox.setOrigin(tsz.width, tsz.height);
    text = new TextBox(note.text, sf::FloatRect(25 * USS, 16 * USS, (WIDTH - 90) * USS, 18 * USS), 15, tF);
    if (text->characterDown < note.text.getSize())
        cutted = note.text.substring(0, text->characterDown - 4) + "...";
    else
    {
        cutted = note.text;
        downBtn->isEnabled = false;
    }
    setOpened();
    setClosed();
}

void Note::setOfMin(float px, float py)
{
    grabBtn->setPosition(control.left + 7 * USS, control.top + 3 * USS);
    deleteBtn->setPosition(control.left + (WIDTH - 24) * USS, control.top + 3 * USS);
    throwBtn->setPosition(control.left + (WIDTH - 45) * USS, control.top + 3 * USS);
    downBtn->setPosition(control.left + (WIDTH - 65) * USS, control.top + 3 * USS);
    text->setPosition(control.left + 25 * USS, control.top + 18 * USS);
    if(downBtn->isEnabled)
        time->setPosition(control.left + (WIDTH - 68) * USS, control.top + 18 * USS);
    else time->setPosition(control.left + (WIDTH - 48) * USS, control.top + 18 * USS);
    
}

void Note::setOfMax(float px, float py)
{
    deleteBtn->setPosition(control.left + (WIDTH - 24) * USS, control.top + txtHeight - 30 * USS);
    throwBtn->setPosition(control.left + (WIDTH - 45) * USS, control.top + txtHeight - 30 * USS);
    upBtn->setPosition(control.left + (WIDTH - 65) * USS, control.top + txtHeight - 30 * USS);
    text->setPosition(control.left + 12 * USS, control.top + 9 * USS);
    time->setPosition(control.left + (WIDTH - 68) * USS, control.top + txtHeight - 15 * USS);
}

void Note::setOpened()
{
    if (!upped)
    {
        upped = true;
        text->bounds.width = (WIDTH - 6) * USS;
        text->text = note.text;
        text->setOptimal();
        text->updateCursor();
        downBtn->isEnabled = false;
        upBtn->isEnabled = true;
        txtHeight = text->resultBox.getGlobalBounds().height + 36 * USS;
        if (txtHeight > MAX_HEIGHT * USS) 
            txtHeight = MAX_HEIGHT * USS;
        text->resultBox.setOrigin(0, 0);
        heightTo = txtHeight;
    }
}

void Note::setClosed()
{
    if (upped)
    {
        upped = false;
        text->bounds.width = (WIDTH - 90) * USS;
        text->text = cutted;
        text->setOptimal();
        text->updateCursor();
        downBtn->isEnabled = (text->characterDown < note.text.getSize());
        upBtn->isEnabled = false;
        sf::FloatRect tsz = text->resultBox.getGlobalBounds();
        text->resultBox.setOrigin(0, tsz.height);
        heightTo = MIN_HEIGHT * USS;
    }
}

void Note::update()
{
    if (Height != heightTo)
    {
        uint16_t h = (txtHeight + MIN_HEIGHT * USS) / 2;
        if (Height == h && upped)
            setClosed();
        else if(Height == h)
            setOpened();
        if (Height < heightTo) 
            ++Height;
        else --Height;
    }
    //printf("%d\n", Height);
}

void Note::setPosition(float x, float y)
{
    control.left = x, control.top = y;
}


Note::~Note()
{
    delete grabBtn;
    delete throwBtn;
    delete deleteBtn;
    delete redoBtn;
    delete upBtn;
    delete downBtn;
    delete text;
    delete time;
}
