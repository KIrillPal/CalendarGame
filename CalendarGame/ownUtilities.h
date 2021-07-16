#pragma once
#include <SFML\Graphics.hpp>
#include <vector>

class ownUtilities
{
public:
    ownUtilities();
    // Constants //
    const sf::String fontFamily = "fonts/AGENCYB.ttf";
    const float USS = 2;
    const uint64_t MAX_YEAR_ABS = 99999999, YEAR = (1LL << 63) - (1LL << 32);
    const uint64_t MONTH = (1LL << 32) - (1LL << 24), DAY = (1LL << 24) - (1LL << 16);
    const uint16_t btwm[12] = { 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334 };
    std::vector<char*> MONTH_NAME = { "JANUARY", "FEBRUARY", "MARCH", "APRIL", "MAY", "JUNE", "JULY",
        "AUGUST", "SEPTEMBER", "OCTOBER", "NOVEMBER", "DECEMBER" };
    std::vector<char*> weekDays = { "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday" };
    // Fields //
    sf::Sprite *noteGrabBack, *noteDelBack, *noteThrowBack, *noteUpBack, *noteDownBack, *dayBack, *dayOff, *dayNone;
    sf::Sprite *noteFlags[4];
    std::vector<sf::Sprite *> NoteShapes, noteLabels;
    std::map<int64_t, int16_t> dateMap, flagMap;
    char * filePath = "dates.inf";
    // Methods //
    sf::Sprite * createBackground(sf::FloatRect box, sf::String texturePath);
    void fillNoteShapes();
    void clearNoteShapes();
    int64_t dateHash(int y, uint8_t mon, uint8_t day, uint8_t h, uint8_t m);
    bool isYearLeap(int64_t y);
    ~ownUtilities();
    uint8_t weekDay(int64_t y, uint64_t m, uint64_t d);
    int64_t getY(uint64_t date);
    uint8_t getM(uint64_t date);
    uint8_t getD(uint64_t date);
    bool getAC(uint64_t date);
    int64_t absY(int64_t y, bool ac);
    void loadNoteLabels();
    void deleteNoteLabels();
    void parseRead();
    void parseWrite();
};

