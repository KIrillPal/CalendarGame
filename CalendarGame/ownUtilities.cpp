#include "ownUtilities.h"



ownUtilities::ownUtilities()
{
}


ownUtilities::~ownUtilities()
{
}

uint8_t ownUtilities::weekDay(int64_t y, uint64_t m, uint64_t d)
{
    if (y > 0)
    {
        uint64_t day = 365LL * y + (y - 1) / 400 - (y - 1) / 100 + (y - 1) / 4;
        day += btwm[m - 1] + d + 5 + (isYearLeap(y) && m > 2);
        return (day % 7 + 7) % 7;
    }
    else
    {
        int64_t day = 365LL * y + (y + 1) / 400 - (y + 1) / 100 + (y + 1) / 4 - (y < 0);
        day += btwm[m - 1] - 365 + d - (isYearLeap(y) && m <= 2) - 1;
        return (day % 7 + 7) % 7;
    }
}

int64_t ownUtilities::getY(uint64_t date)
{
    return (date & YEAR) >> 32;
}

uint8_t ownUtilities::getM(uint64_t date)
{
    return (date & MONTH) >> 24;
}

uint8_t ownUtilities::getD(uint64_t date)
{
    return (date & DAY) >> 16;
}

bool ownUtilities::getAC(uint64_t date)
{
    return date >> 63;
}

int64_t ownUtilities::absY(int64_t y, bool ac)
{
    return ac ? y : -y + 1;
}

void ownUtilities::loadNoteLabels()
{
    for (int i = 1; i <= 4; ++i)
    {
        std::string pth = "textures/noteFlags/" + std::to_string(i) + ".png";
        noteFlags[i-1] = createBackground(sf::FloatRect(0, 0, 13, 27), pth);
    }
    noteLabels.resize(10);
    for (int i = 1; i <= 10; ++i)
    {
        std::string pth = "textures/noteMarks/" + std::to_string(i) + ".png";
        noteLabels[i-1] = createBackground(sf::FloatRect(0, 0, 13, 13), pth);
        noteLabels[i - 1]->setOrigin(4, 9);
    }
}

void ownUtilities::deleteNoteLabels()
{
    for (int i = 1; i <= 4; ++i)
        delete noteFlags[i - 1];
    for (int i = 1; i <= 10; ++i)
        delete noteLabels[i - 1];
}

sf::Sprite* ownUtilities::createBackground(sf::FloatRect box, sf::String texturePath = "")
{
    sf::Texture * back = new sf::Texture();
    if (texturePath == "")
    {
        const int BORDER = 3;
        sf::Image backImage;
        // Fill area //
        backImage.create(box.width, box.height);
        for (int i = 0; i < box.height; ++i)
            for (int j = 0; j < box.width; ++j)
                backImage.setPixel(j, i, sf::Color(204, 204, 204, 255));
        // Round corners //
        for (int corner = 0; corner < 4; ++corner)
        {
            int x0 = (corner & 1) * (box.width - 1);
            int y0 = ((corner & 2) >> 1) * (box.height - 1);
            for (int x = x0 - 1; x <= x0 + 1; ++x)
                for (int y = y0 - 1; y <= y0 + 1; ++y)
                    if ((x - x0)*(y - y0) == 0 && x >= 0 && y >= 0 && x < box.width && y < box.height)
                        backImage.setPixel(x, y, sf::Color(0, 0, 0, 0));
        }
        // Fill inner area //
        for (int i = BORDER; i < box.height - BORDER; ++i)
            for (int j = BORDER; j < box.width - BORDER; ++j)
                backImage.setPixel(j, i, sf::Color(255, 255, 255, 255));
        // Draw border lines //
        for (int i = BORDER; i < box.height - BORDER; ++i)
        {
            backImage.setPixel(BORDER - 1, i, sf::Color(228, 228, 228, 255));
            backImage.setPixel(box.width - BORDER, i, sf::Color(228, 228, 228, 255));
        }
        for (int j = BORDER; j < box.width - BORDER; ++j)
        {
            backImage.setPixel(j, BORDER - 1, sf::Color(228, 228, 228, 255));
            backImage.setPixel(j, box.height - BORDER, sf::Color(228, 228, 228, 255));
        }
        back->loadFromImage(backImage);
    }
    else back->loadFromFile(texturePath);
    back->setSmooth(false);
    sf::Sprite *res = new sf::Sprite(*back);
    res->setPosition(box.left, box.top);
    res->setScale(USS, USS);
    return res;
}
void ownUtilities::fillNoteShapes()
{
    const size_t MAX_HEIGHT = 300, MIN_HEIGHT = 33, WIDTH = 200;
    NoteShapes.resize(MAX_HEIGHT);
    for (size_t i = MIN_HEIGHT; i < MAX_HEIGHT; ++i)
    {
        NoteShapes[i] = createBackground(sf::FloatRect(0, 0, WIDTH, i));
    }
    noteGrabBack = createBackground(sf::FloatRect(0, 0, 18, 27), "textures/grab.png");
    noteDelBack = createBackground(sf::FloatRect(0, 0, 19, 27), "textures/delete.png");
    noteThrowBack = createBackground(sf::FloatRect(0, 0, 19, 27), "textures/throw.png");
    noteUpBack = createBackground(sf::FloatRect(0, 0, 18, 27), "textures/up.png");
    noteDownBack = createBackground(sf::FloatRect(0, 0, 18, 27), "textures/down.png");
}

void ownUtilities::clearNoteShapes()
{
    for (int i = 0; i < NoteShapes.size(); ++i)
        if (NoteShapes[i])
        {
            delete NoteShapes[i]->getTexture();
            delete NoteShapes[i];
        }
    delete noteGrabBack;
    delete noteDelBack;
    delete noteThrowBack;
    delete noteUpBack;
    delete noteDownBack;
}

int64_t ownUtilities::dateHash(int y, uint8_t mon, uint8_t day, uint8_t h, uint8_t m)
{
    uint64_t hs = 0;
    hs |= ((uint64_t)y << 32);
    hs |= ((uint64_t)mon << 24);
    hs |= ((uint64_t)day << 16);
    hs |= ((uint64_t)h << 8);
    hs |= (uint64_t)m;
    return hs;
}

bool ownUtilities::isYearLeap(int64_t y)
{
    return (y % 400 == 0 || y % 100 && y % 4 == 0);
}
