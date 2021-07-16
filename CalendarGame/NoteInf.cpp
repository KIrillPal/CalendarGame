#pragma once
#include "NoteInf.h"



NoteInf::NoteInf()
{
}

NoteInf::NoteInf(sf::String txt, int64_t dte, uint16_t tme)
{
    if (tme == -1) hasTime = false;
    else time = tme;
    text = txt;
    date = dte;
}


void NoteInf::operator=(NoteInf inf)
{
    text = inf.text;
    hasTime = inf.hasTime;
    time = inf.time;
    date = inf.date;
    rank = inf.rank;
}

bool NoteInf::operator<(NoteInf inf)
{
    return (date < inf.date || (date == inf.date && time < inf.time));
}

NoteInf::~NoteInf()
{
}
