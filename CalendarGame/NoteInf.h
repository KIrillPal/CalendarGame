#pragma once
#include <SFML\Graphics.hpp>
class NoteInf
{
    const float USS = 2;
public:
    NoteInf();
    NoteInf(sf::String txt, int64_t dte, uint16_t time);
    sf::String text = "";
    bool hasTime = true;
    int64_t time = 1440, date = 0, rank = -1;
    void operator = (NoteInf inf);
    bool operator < (NoteInf inf);
    ~NoteInf();
};

