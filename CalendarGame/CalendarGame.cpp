#pragma once
#include <chrono>
#include "ScrollWindow.h"
#include "PxButton.h"
#include <string>
#include <iostream>

using namespace sf;

const float USS = 2, HOLIDAY_CHSZ = 17, HOLI_H = 18, HOLI_W = 131 * USS;
const float ProgramFrameTime = 1000 / 240; // 1000ms / 60frames
const size_t WINDOW_WIDTH = 1080, WINDOW_HEIGHT = 650, DIAL_W = 300, DIAL_H = 150, YF_W = 620, YF_H = 450;
const size_t YM_SPACE = 8, YDW_SPACE = 4, YDH_SPACE = 10, YM_TOP = 24, BW = 37, BH = 24;
const sf::String fontFamily = "fonts/Bungee-Regular.ttf";
const sf::String textFontFamily = "fonts/AGENCYB.ttf";
const sf::String PROGRAM_NAME = "Calendar";
const sf::String weekNames[7] = { "MON", "TUE", "WED", "THU", "FRI", "SAT", "SUN" };
int16_t mouseX = 0, mouseY = 0, crtResYear = 1, crtResMonth = 11, crtResDay = 1, selM = 1, selD = 1, selCol = 0, selMonth = 0;
ScrollWindow * lastWas;
int noteStart = 0, windowMode = 1;
sf::Sprite * windowBack;
sf::Font bF, tF;
sf::FloatRect flRect = sf::FloatRect(299, 410, 118, 22);
size_t ym_w = 8 * YM_SPACE + 7 * (YDW_SPACE + BW);
int16_t daysAtM[13] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
uint16_t tdStateH = 0, tdReligH = 0, tdLaborH = 0;
int64_t dateNow = 135;
int64_t nowYear = 1, nowMonth = 1, nowDay = 1;
uint64_t frameTime = 0;
PxButton * yearButtons[6][7];
uint32_t yearFlags[12][6][7], colMonth[7], boxFlags[6][7];
sf::Vector2f dialPos(WINDOW_WIDTH - DIAL_W * USS, WINDOW_HEIGHT - DIAL_H * USS);
// Main controls //
sf::RenderWindow window;
ownUtilities owt;
// Dialog Window //

ScrollWindow *dialScroll;
TextBox *crtInput, *crtPrev, *crtYear, *crtMonth, *crtDay, *crtAPr, *crtAPm, *crtHour, *crtMin;
PxButton *crtClose, *crtOK, *crtLeft, *crtRight, *crtBC;
sf::Sprite * crtBack, *crtTb, *crtYb, *crtMb, *crtDb, *crtHb, *crtMnb;
// Today Information Bar //

TextBox *tdYear, *tdDayNum, *tdMonth, *tdYearNum, *dayInf1, *dayInf2, *dayInf3, *dayInf4, *hdName, *ntName;
PxButton *tdMShow, *tdDShow, *tdLeft, *tdRight, *tdBC, *crtNoteBtn;
ScrollWindow *noteScroll, *tdScroll, *hdScroll;
sf::Sprite *tdYb, *tdDb, *hdState, *hdRelig, *hdLabor, *hdBox, *crtCross, *flSel, *flSet;

std::vector<Note *> dateToday;
std::vector<TextBox *> holiToday;
std::vector<NoteInf> dateHeap, holiHeap;
// Year field //

ScrollWindow *mcUpScroll, *mcScroll, *yfScroll;
PxButton *mcLeft, *mcRight;
sf::Sprite *yfBack;
std::vector<PxButton *> monthButtons(12);
std::vector<TextBox *> weekLabels(7);

bool isDialBC = false, isTodayBC = false;

void enOK()
{
    crtOK->setColor(sf::Color::Green);
    crtOK->isEnabled = true;
}

void disOK()
{
    crtOK->setColor(sf::Color(230, 230, 230, 255));
    crtOK->isEnabled = false;
}

int64_t parse(TextBox * tb)
{
    return std::stoll(tb->text.toAnsiString());
}

void setTxt(TextBox * tb, sf::String txt)
{
    tb->text = txt;
    tb->updateCursor();
}

void setFlagged(PxButton * btn)
{
    sf::FloatRect mt = btn->mainText.getLocalBounds();
    btn->mainText.setOrigin(mt.left + 18,
        mt.top + mt.height / 2);
}

void setUnflagged(PxButton * btn)
{
    sf::FloatRect mt = btn->mainText.getLocalBounds();
    btn->mainText.setOrigin(mt.left + mt.width / 2,
        mt.top + mt.height / 2);
}

void moveButtonDay(int16_t m, int16_t x, int16_t y, int16_t delta)
{
    sf::String nm;
    int16_t d = yearFlags[m][y][x] % 100;
    nm = std::to_string(d);
    sf::FloatRect c = yearButtons[y][x]->control;
    yearButtons[y][x]->setPosition(c.left + (float)ym_w * delta * USS, c.top);
    yearButtons[y][x]->mainText.setString(nm);
    int k = yearFlags[m][y][x] % 1000 / 100;
    boxFlags[y][x] = k;
    if (k >= 5)
        yearButtons[y][x]->isEnabled = false;
    else yearButtons[y][x]->isEnabled = true;
    if (0 < k && k < 5)
    {
        setFlagged(yearButtons[y][x]);
    }
    else setUnflagged(yearButtons[y][x]);
    boxFlags[y][x] += (yearFlags[m][y][x] / 1000LL) * 10;
}

void openDial(sf::String txt, int64_t year, int16_t mon, int16_t day, int16_t h, int16_t m)
{
    windowMode = 2;
    dialScroll->mindPos = sf::Vector2f(WINDOW_WIDTH / 4, WINDOW_HEIGHT / 4);
    if (year < 1)
    {
        isDialBC = 1;
        year = -year + 1;
    }
    else isDialBC = 0;
    setTxt(crtInput, txt);
    setTxt(crtYear, std::to_string(year));
    setTxt(crtMonth, std::to_string(mon));
    setTxt(crtDay, std::to_string(day));
    setTxt(crtHour, std::to_string(h));
    setTxt(crtMin, std::to_string(m));
    sf::Color resC;
    if (isDialBC)
        resC = sf::Color(255, 106, 0, 130);
    else resC = sf::Color(0, 0, 0, 120);
    crtBC->mainText.setFillColor(resC);
    if (txt == "") disOK();
    else enOK();
}

void closeDial()
{
    windowMode = 1;
}

void updTxb(TextBox * tb, int l, int r)
{
    if (tb->text == "")
        tb->text = tb->last_text;
    else
    {
        long long f = parse(tb);
        if(f < l || f > r)
            tb->text = tb->last_text;
        else if (tb->text.getSize() < 2) tb->text = '0' + tb->text;
    }
    tb->updateCursor();
}

void setPos(TextBox * tb, float x, float y, bool cx, bool cy)
{
    sf::FloatRect bnd = tb->resultBox.getLocalBounds();
    x -= cx * bnd.width;
    y -= cy * bnd.height;
    tb->setPosition(x, y);
}

void crtInputOffFocused()
{
    if (crtInput->text == "")
        disOK();
    else enOK();
}

void crtYearOffFocused()
{
    updTxb(crtYear, 1, owt.MAX_YEAR_ABS);
    if (crtYear->text == "")
        crtYear->text = crtYear->last_text;
    int64_t y = parse(crtYear);
    int16_t m = parse(crtMonth);
    int16_t d = parse(crtDay);
    crtLeft->isEnabled = (y > 1);
    crtRight->isEnabled = (y < owt.MAX_YEAR_ABS);
    if (!owt.isYearLeap(y) && m == 2 && d > daysAtM[2])
        setTxt(crtDay, std::to_string(daysAtM[2]));
}

void crtMonthOffFocused()
{
    updTxb(crtMonth, 1, 12);
    int16_t mons = daysAtM[parse(crtMonth)];
    if (parse(crtDay) > mons)
        setTxt(crtDay, std::to_string(mons));
}

void crtDayOffFocused()
{
    if (parse(crtMonth) == 2 && owt.isYearLeap(parse(crtYear)))
        updTxb(crtDay, 1, 29);
    else updTxb(crtDay, 1, daysAtM[parse(crtMonth)]);
}

void initDial()
{
    dialScroll = new ScrollWindow(sf::FloatRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT),
        FloatRect(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, WINDOW_WIDTH * 2 - DIAL_W * USS, WINDOW_HEIGHT * 2 - DIAL_H * USS));
    crtInput = new TextBox("", sf::FloatRect(dialPos.x + 14 * USS, dialPos.y + 25 * USS, DIAL_W * USS - 28 * USS, (DIAL_H - 100) * USS), 20, tF); // Create shape //
    crtInput->isActive = true;
    sf::FloatRect bnds = sf::FloatRect(dialPos.x + 12 * USS, dialPos.y + 8 * USS, DIAL_W * USS - 10 * USS, 200 * USS);
    crtPrev = new TextBox("Enter your text here:", bnds, 15, tF);
    crtPrev->resultBox.setFillColor(sf::Color(128, 128, 128, 255));
    crtBack = owt.createBackground(sf::FloatRect(dialPos.x, dialPos.y, DIAL_W, DIAL_H), "");
    crtTb = owt.createBackground(sf::FloatRect(dialPos.x + 14, dialPos.y + 40, DIAL_W - 15, DIAL_H - 80), "");
    crtTb->setColor(sf::Color(0, 0, 50, 30));
    crtYb = owt.createBackground(sf::FloatRect(dialPos.x + 50 * USS, dialPos.y + (DIAL_H - 58) * USS, 68, 18), "");
    crtClose = new PxButton(sf::FloatRect(dialPos.x + (DIAL_W - 89) * USS,
        dialPos.y + (DIAL_H - 23) * USS, 40, 16), sf::Color::Red, 25, "CLOSE", bF);
    crtClose->mainText.setFillColor(sf::Color::White);
    crtClose->setSprite(owt.createBackground(crtClose->control, ""));
    crtOK = new PxButton(sf::FloatRect(dialPos.x + (DIAL_W - 47) * USS,
        dialPos.y + (DIAL_H - 23) * USS, 40, 16), sf::Color::Green, 15, "OK!", bF);
    crtOK->mainText.setFillColor(sf::Color::White);
    crtOK->setSprite(owt.createBackground(crtOK->control, ""));
    crtLeft = new PxButton(sf::FloatRect(dialPos.x + 30 * USS,
        dialPos.y + (DIAL_H - 58) * USS, 18, 18), sf::Color::White, 1, "", bF);
    crtLeft->setSprite(owt.createBackground(crtLeft->control, "textures/left.png"));
    crtLeft->brightBox = 1;
    crtLeft->brightOver = 0.9;
    crtLeft->brightPress = 0.8;
    crtLeft->setColor(sf::Color::White);
    crtLeft->update();
    crtRight = new PxButton(sf::FloatRect(dialPos.x + 120 * USS,
        dialPos.y + (DIAL_H - 58) * USS, 18, 18), sf::Color::White, 1, "", bF);
    crtRight->setSprite(owt.createBackground(crtRight->control, "textures/right.png"));
    crtRight->brightBox = 1;
    crtRight->brightOver = 0.9;
    crtRight->brightPress = 0.8;
    crtRight->setColor(sf::Color::White);
    crtRight->update();
    crtBC = new PxButton(sf::FloatRect(dialPos.x + 98 * USS, dialPos.y + (DIAL_H - 58) * USS, 22, 18),
        sf::Color::White, 18, "B.C", bF);
    crtBC->mainFont.loadFromFile(owt.fontFamily);
    crtBC->mainText.setFont(crtBC->mainFont);
    crtBC->setSprite(owt.createBackground(crtBC->control, "textures/empty.png"));
    crtBC->brightBox = 1;
    crtBC->brightOver = 0.7;
    crtBC->brightPress = 0.5;
    crtBC->mainText.setFillColor(sf::Color(0, 0, 0, 120));
    crtBC->setColor(sf::Color::White);
    crtBC->update();
    crtYear = new TextBox("2019", sf::FloatRect(dialPos.x + 54 * USS, dialPos.y + (DIAL_H - 57) * USS + 1, 48 * USS, 17), 25, tF);
    crtYear->isActive = true;
    crtMonth = new TextBox("01", sf::FloatRect(dialPos.x + 147 * USS, dialPos.y + (DIAL_H - 57) * USS + 1, 12 * USS, 17), 25, tF);
    crtMonth->isActive = true;
    crtMb = owt.createBackground(sf::FloatRect(dialPos.x + 140 * USS, dialPos.y + (DIAL_H - 58) * USS, 24, 18), "");
    crtDay = new TextBox("02", sf::FloatRect(dialPos.x + 173 * USS, dialPos.y + (DIAL_H - 57) * USS + 1, 12 * USS, 17), 25, tF);
    crtDay->isActive = true;
    crtDb = owt.createBackground(sf::FloatRect(dialPos.x + 166 * USS, dialPos.y + (DIAL_H - 58) * USS, 24, 18), "");
    crtAPr = new TextBox("Date:", sf::FloatRect(dialPos.x + 10 * USS,
        dialPos.y + (DIAL_H - 56) * USS, 120 * USS, 18), 20, tF);
    crtAPr->resultBox.setFillColor(sf::Color(128, 128, 128, 255));
    crtAPm = new TextBox("Time:             :", sf::FloatRect(dialPos.x + 10 * USS,
        dialPos.y + (DIAL_H - 36) * USS, 120 * USS, 18), 20, tF);
    crtAPm->resultBox.setFillColor(sf::Color(128, 128, 128, 255));
    crtHour = new TextBox("03", sf::FloatRect(dialPos.x + 36 * USS + 1, dialPos.y + (DIAL_H - 37) * USS + 1, 12 * USS, 17), 25, tF);
    crtHour->isActive = true;
    crtMin = new TextBox("04", sf::FloatRect(dialPos.x + 63 * USS, dialPos.y + (DIAL_H - 37) * USS + 1, 12 * USS, 17), 25, tF);
    crtMin->isActive = true;
    crtHb = owt.createBackground(sf::FloatRect(dialPos.x + 30 * USS, dialPos.y + (DIAL_H - 38) * USS, 24, 18), "");
    crtMnb = owt.createBackground(sf::FloatRect(dialPos.x + 57 * USS, dialPos.y + (DIAL_H - 38) * USS, 24, 18), "");
}

void initTodayBar()
{
    tdScroll = new ScrollWindow(sf::FloatRect(0, 0, 220 * USS, 500), sf::FloatRect(0, 0, 220 * USS, 300));
    tdYb = owt.createBackground(sf::FloatRect(60, 20, 68, 18), "");
    tdYear = new TextBox(std::to_string(nowYear), sf::FloatRect(68, 23, 48 * USS, 17), 25, tF);
    tdYear->isActive = true;
    tdBC = new PxButton(sf::FloatRect(156, 20, 22, 18), sf::Color::White, 18, "B.C", bF);
    tdBC->mainFont.loadFromFile(owt.fontFamily);
    tdBC->mainText.setFont(tdBC->mainFont);
    tdBC->setSprite(owt.createBackground(tdBC->control, "textures/empty.png"));
    tdBC->brightBox = 1;
    tdBC->brightOver = 0.7;
    tdBC->brightPress = 0.5;
    tdBC->mainText.setFillColor(sf::Color(0, 0, 0, 120));
    tdBC->setColor(sf::Color::White);
    tdBC->update();
    tdLeft = new PxButton(sf::FloatRect(20, 20, 18, 18), sf::Color::White, 1, "", bF);
    tdLeft->setSprite(owt.createBackground(tdLeft->control, "textures/left.png"));
    tdLeft->brightBox = 1;
    tdLeft->brightOver = 0.9;
    tdLeft->brightPress = 0.8;
    tdLeft->setColor(sf::Color::White);
    tdLeft->update();
    tdRight = new PxButton(sf::FloatRect(200, 20, 18, 18), sf::Color::White, 1, "", bF);
    tdRight->setSprite(owt.createBackground(tdRight->control, "textures/right.png"));
    tdRight->brightBox = 1;
    tdRight->brightOver = 0.9;
    tdRight->brightPress = 0.8;
    tdRight->setColor(sf::Color::White);
    tdRight->update();
    tdMShow = new PxButton(sf::FloatRect(244, 20, 38, 18), sf::Color::Red, 21, "OCT", bF);
    tdMShow->mainText.setFillColor(sf::Color::White);
    tdMShow->setSprite(owt.createBackground(tdMShow->control, "textures/m-show.png"));
    tdMShow->brightBox = 1;
    tdMShow->isEnabled = false;
    tdMShow->update();
    tdDShow = new PxButton(sf::FloatRect(324, 20, 24, 18), sf::Color::Red, 14, "14", bF);
    tdDShow->mainText.setFillColor(sf::Color::White);
    tdDShow->setSprite(owt.createBackground(tdDShow->control, "textures/d-show.png"));
    tdDShow->brightBox = 1;
    tdDShow->isEnabled = false;
    tdDShow->update();
    tdDb = owt.createBackground(sf::FloatRect(20, 75, 200, 70), "");
    tdDb->setColor(sf::Color(255, 0, 0, 60));
    tdDayNum = new TextBox("28", sf::FloatRect(35, 87, 100, 30), 60, tF);
    tdDayNum->resultBox.setFillColor(sf::Color(0, 13, 86, 255));
    tdMonth = new TextBox("NOVEMBER", sf::FloatRect(65, 87, 200, 30), 40, tF);
    tdYearNum = new TextBox("2019", sf::FloatRect(35, 87, 250, 30), 60, tF);
    dayInf1 = new TextBox("Friday, working day", sf::FloatRect(35, 87, 200, 30), 15, tF);
    dayInf2 = new TextBox("14.10.2019", sf::FloatRect(35, 87, 200, 30), 20, tF);
    dayInf2->resultBox.setFillColor(sf::Color(0, 13, 86, 255));
    dayInf3 = new TextBox("2019 is a leap year", sf::FloatRect(35, 87, 250, 30), 20, tF);
    dayInf3->resultBox.setFillColor(sf::Color(0, 13, 86, 255));
    dayInf4 = new TextBox("", sf::FloatRect(35, 87, 250, 30), 15, tF);
    dayInf4->resultBox.setFillColor(sf::Color(255, 213, 0, 255));
    setPos(tdDayNum, 35, 125, 0, 1);
    setPos(tdYearNum, 405, 125, 1, 1);
    setPos(tdMonth, 55 + tdDayNum->resultBox.getGlobalBounds().width, 130, 0, 1);
    setPos(dayInf1, 35, 155, 0, 0);
    setPos(dayInf2, 35, 180, 0, 0);
    setPos(dayInf3, 405, 180, 1, 0);
    setPos(dayInf4, 405, 140, 1, 0);
    hdScroll = new ScrollWindow(sf::FloatRect(0, 250, 220 * USS, 80), sf::FloatRect(0, 0, 220 * USS, 300));
    hdState = owt.createBackground(sf::FloatRect(0, 0, 131, 12), "textures/state.png");
    hdRelig = owt.createBackground(sf::FloatRect(0, 0, 131, 12), "textures/relig.png");
    hdLabor = owt.createBackground(sf::FloatRect(0, 0, 131, 12), "textures/labor.png");
    hdName = new TextBox("No Holidays", sf::FloatRect(23, 220, 250, 30), 20, tF);
    crtNoteBtn = new PxButton(sf::FloatRect(20, 336, 200, 33), sf::Color::White, 148, "Create note                                     ", bF);
    crtNoteBtn->setSprite(owt.createBackground(crtNoteBtn->control, ""));
    crtNoteBtn->mainText.setFont(*hdName->resultBox.getFont());
    crtNoteBtn->brightBox = 1;
    crtNoteBtn->brightOver = 0.95;
    crtNoteBtn->brightPress = 0.9;
    crtNoteBtn->mainText.setFillColor(sf::Color(0, 0, 0, 182));
    crtNoteBtn->setColor(sf::Color::White);
    crtNoteBtn->update();
    crtCross = owt.createBackground(sf::FloatRect(42, 358, 11, 11), "textures/cross.png");
    ntName = new TextBox("No Notes", sf::FloatRect(23, 410, 250, 30), 20, tF);
    flSel = owt.createBackground(sf::FloatRect(299, 410, 59, 11), "textures/flag-selection.png");
    flSet = owt.createBackground(sf::FloatRect(297, 408, 13, 13), "textures/ring.png");
    noteScroll = new ScrollWindow(sf::FloatRect(0, 220 * USS, 220 * USS, 140 * USS), sf::FloatRect(0, 0, 220 * USS, 26000));
}

void selectMonthButton(size_t k)
{
    monthButtons[k]->brightBox = 0.8;
    monthButtons[k]->setColor(sf::Color::Red);
    monthButtons[k]->isEnabled = false;
}

void deselectMonthButton(size_t k)
{
    monthButtons[k]->brightBox = 0.7;
    monthButtons[k]->setColor(sf::Color::Red);
    monthButtons[k]->isEnabled = true;
}

void initMonthSelection()
{
    mcUpScroll = new ScrollWindow(sf::FloatRect(210 * USS, 0, YF_W + 20, 100), sf::FloatRect(0, 0, YF_W, 100));
    mcScroll = new ScrollWindow(sf::FloatRect(240 * USS, 0, YF_W - 40 * USS, 56), sf::FloatRect(0, 0, 56 * 12 * USS - 2, 56));
    mcLeft = new PxButton(sf::FloatRect(20, 20, 18, 18), sf::Color::Red, 1, "", bF);
    mcLeft->setSprite(owt.createBackground(tdLeft->control, "textures/left.png"));
    mcRight = new PxButton(sf::FloatRect(YF_W - 16, 20, 18, 18), sf::Color::Red, 1, "", bF);
    mcRight->setSprite(owt.createBackground(tdRight->control, "textures/right.png"));
    for (size_t i = 0; i < 12; ++i)
    {
        monthButtons[i] = new PxButton(sf::FloatRect(i * 56 * USS, 20, 54, 18), sf::Color::Red, 34, owt.MONTH_NAME[i], bF);
        monthButtons[i]->setSprite(owt.createBackground(monthButtons[i]->control, ""));
        monthButtons[i]->mainText.setFillColor(sf::Color::White);
        monthButtons[i]->mainText.setCharacterSize(12);
        sf::FloatRect mt = monthButtons[i]->mainText.getLocalBounds();
        monthButtons[i]->mainText.setOrigin(mt.left + mt.width / 2,
            mt.top + mt.height / 2);
    }
    selectMonthButton(0);
}

void selectMonth(int m)
{
    yfScroll->speed.x += (m - selMonth) * 28.08;
    deselectMonthButton(selMonth);
}

void initYearField()
{
    yfScroll = new ScrollWindow(sf::FloatRect(220 * USS, 76, YF_W, YF_H), sf::FloatRect(0, 0, ym_w * 12 * USS - 12 * YM_SPACE, YF_H));
    yfBack = owt.createBackground(sf::FloatRect(220 * USS, 76, YF_W / USS, YF_H / USS), "");
    yfBack->setColor(sf::Color(0, 0, 0, 30));
    for (size_t i = 0; i < 7; ++i)
    {
        weekLabels[i] = new TextBox(weekNames[i], sf::FloatRect(i * 82 + 36, 10, 50, 20), 25, tF);
        weekLabels[i]->resultBox.setFillColor(sf::Color(255, 255, 255, 255));
    }
}

void erase(size_t i)
{
    dateHeap[dateToday[i]->inArray] = NoteInf("BAM", 0, 0);
    dateToday.erase(dateToday.begin() + i);
    if(dateToday.size())
        ntName->text = "Notes(" + std::to_string(dateToday.size()) + "):";
    else ntName->text = "No Notes";
    ntName->updateCursor();
}

void add(NoteInf inf)
{
    dateHeap.push_back(inf);
    if (inf.date == dateNow)
    {
        Note * nte = new Note(inf, &owt, bF, tF);
        nte->inArray = dateHeap.size()-1;
        for (int i = 0; i < dateToday.size(); ++i)
            if (nte->note < dateToday[i]->note)
            {
                dateToday.insert(dateToday.begin() + i, nte);
                return;
            }
        dateToday.push_back(nte);
        ntName->text = "Notes(" + std::to_string(dateToday.size()) + "):";
        ntName->updateCursor();
    }
}

bool noteAddCmp(Note * a, Note * b)
{
    if (a->note.rank == -1 || b->note.rank == -1)
        return a->note.time < b->note.time;
    else return a->note.rank < b->note.rank;
}

void rankClear()
{
    for (int i = 0; i < dateToday.size(); ++i)
    {
        dateHeap[dateToday[i]->inArray].rank = i;
        delete dateToday[i];
    }
    dateToday.clear();
    noteStart = 0;
}

void holidayClear()
{
    for (int i = 0; i < holiToday.size(); ++i)
        delete holiToday[i];
    holiToday.clear();
    tdStateH = tdReligH = tdLaborH = 0;
}

void btsClear()
{
    for (int i = 0; i < monthButtons.size(); ++i)
        delete monthButtons[i];
    monthButtons.clear();
    for (int i = 0; i < weekLabels.size(); ++i)
        delete weekLabels[i];
    weekLabels.clear();
}

void fillFormDate(int64_t date)
{
    // notes //
    for (size_t i = 0; i < dateHeap.size(); ++i)
    {
        if (dateHeap[i].date == date && dateHeap[i].text != '\0')
        {
            Note * nte = new Note(dateHeap[i], &owt, bF, tF);
            nte->inArray = i;
            nte->note.rank = dateHeap[i].rank;
            dateToday.push_back(nte);
        }
    }
    sort(dateToday.begin(), dateToday.end(), noteAddCmp);
    tdStateH = 0, tdReligH = 0, tdLaborH = 0;
    // holidays //
    for (size_t i = 0; i < holiHeap.size(); ++i)
    {
        if (holiHeap[i].date == date)
        {
            sf::FloatRect rect(10, holiToday.size() * (HOLI_H + 2), HOLI_W, HOLI_H);
            TextBox * hld = new TextBox(holiHeap[i].text, rect, HOLIDAY_CHSZ, tF);
            hld->timer = holiHeap[i].time;
            tdStateH += (hld->timer == 1);
            tdReligH += (hld->timer == 2);
            tdLaborH += (hld->timer == 3);
            holiToday.push_back(hld);
        }
    }
}

void checkPos()
{
    while (noteStart + 1 < dateToday.size())
    {
        float y = dateToday[noteStart]->control.top;
        float h = dateToday[noteStart]->control.height;
        if (y + h >= noteScroll->mindPos.y) break;
        ++noteStart;
    }
    while (noteStart - 1 >= 0)
    {
        if (dateToday[noteStart]->control.top < noteScroll->mindPos.y) break;
        --noteStart;
    }
    if (dateToday.size() && noteStart >= dateToday.size())
        noteStart = dateToday.size()-1;
}

void updateTodayBar()
{
    nowDay = selD, nowMonth = selM;
    nowYear = owt.absY(parse(tdYear), !isTodayBC);
    dateNow = owt.dateHash(nowYear, nowMonth, nowDay, 0, 0);
    // TodayFields filling
    rankClear();
    holidayClear();
    fillFormDate(dateNow);
    // Bar filling //
    tdDayNum->text = std::to_string(nowDay);
    tdYearNum->text = tdYear->text;
    tdMonth->text = owt.MONTH_NAME[nowMonth - 1];
    // Upper bar filling //
    tdMShow->mainText.setString(tdMonth->text.substring(0, 3));
    if (tdDayNum->text.getSize() == 1)
        tdDShow->mainText.setString(" " + tdDayNum->text);
    else tdDShow->mainText.setString(tdDayNum->text);
    if (tdYear->text.getSize() > 5)
        tdMonth->text = tdMonth->text.substring(0, 3);
    int8_t wd = owt.weekDay(nowYear, nowMonth, nowDay);
    sf::String about = owt.weekDays[wd];
    if (tdStateH > 0)
        about += ", public holiday";
    else if (tdReligH > 0)
        about += ", religious holiday";
    else if (tdLaborH > 0)
        about += ", labour holiday";
    else if (wd >= 5)
        about += ", holiday";
    else about += ", working day";
    tdDayNum->updateCursor();
    tdMonth->updateCursor();
    tdYearNum->updateCursor();
    dayInf1->text = about;
    about = (nowDay < 10 ? "0" : "") + std::to_string(nowDay);
    about += (nowMonth < 10 ? ".0" : ".") + std::to_string(nowMonth);
    about += "." + tdYear->text;
    dayInf2->text = about;
    about = tdYear->text;
    if (nowYear <= 0)
    {
        dayInf4->text = "Before Christ";
        dayInf2->text += " B.C";
        about += " B.C";
    }
    else dayInf4->text = "";
    if (owt.isYearLeap(nowYear))
        about += " is a leap year";
    else about += " is a common year";
    dayInf3->text = about;
    dayInf1->updateCursor();
    dayInf2->updateCursor();
    dayInf3->updateCursor();
    dayInf4->updateCursor();
    setPos(dayInf3, 405, 180, 1, 0);
    setPos(dayInf4, 405, 140, 1, 0);
    setPos(tdYearNum, 405, 125, 1, 1);
    // Flag set //
    int16_t f = owt.flagMap[dateNow];
    flSet->setPosition(297 + f * 24, 408);
    // Holidays and notes set //
    if (tdStateH + tdReligH + tdLaborH)
        hdName->text = "Holidays(" + std::to_string(tdStateH + tdReligH + tdLaborH) + "):";
    else hdName->text = "No Holidays";
    hdName->updateCursor();
    if (dateToday.size())
        ntName->text = "Notes(" + std::to_string(dateToday.size()) + "):";
    else ntName->text = "No Notes";
    ntName->updateCursor();
}

void prepareHolidays(int64_t year)
{
	holiHeap.clear();
	holiHeap.push_back(NoteInf("Start era day", owt.dateHash(1, 1, 1, 0, 0), 1));
	holiHeap.push_back(NoteInf("Merry Christmas!", owt.dateHash(1, 1, 1, 0, 0), 2));
	holiHeap.push_back(NoteInf("Our day(example)", owt.dateHash(1, 1, 1, 0, 0), 3));
	if (year != 1)
		holiHeap.push_back(NoteInf("Happy new year!", owt.dateHash(year, 1, 1, 0, 0), 1));
	holiHeap.push_back(NoteInf("My birthday!", owt.dateHash(year, 8, 26, 0, 0), 3));
	holiHeap.push_back(NoteInf("May day", owt.dateHash(year, 5, 1, 0, 0), 3));
	holiHeap.push_back(NoteInf("Russia day", owt.dateHash(year, 6, 12, 0, 0), 1));
	holiHeap.push_back(NoteInf("Fatherland defender day", owt.dateHash(year, 2, 23, 0, 0), 1));
	holiHeap.push_back(NoteInf("International women day", owt.dateHash(year, 3, 8, 0, 0), 1));
	holiHeap.push_back(NoteInf("Constitution day", owt.dateHash(year, 12, 12, 0, 0), 1));
	holiHeap.push_back(NoteInf("National unity day", owt.dateHash(year, 11, 4, 0, 0), 1));

	holiHeap.push_back(NoteInf("Christmas", owt.dateHash(year, 1, 7, 0, 0), 2));
	holiHeap.push_back(NoteInf("Pressman day", owt.dateHash(year, 1, 13, 0, 0), 3));
	holiHeap.push_back(NoteInf("Science day", owt.dateHash(year, 2, 8, 0, 0), 3));
	holiHeap.push_back(NoteInf("Dentist day", owt.dateHash(year, 2, 8, 0, 0), 3));
	holiHeap.push_back(NoteInf("Diver day", owt.dateHash(year, 3, 19, 0, 0), 3));
	holiHeap.push_back(NoteInf("Cosmos day", owt.dateHash(year, 4, 12, 0, 0), 1));
	holiHeap.push_back(NoteInf("Fireman day", owt.dateHash(year, 4, 30, 0, 0), 3));
	holiHeap.push_back(NoteInf("Victory day", owt.dateHash(year, 5, 9, 0, 0), 1));
	holiHeap.push_back(NoteInf("Businessman day", owt.dateHash(year, 5, 26, 0, 0), 3));
	holiHeap.push_back(NoteInf("Pushkin day", owt.dateHash(year, 6, 6, 0, 0), 3));
	holiHeap.push_back(NoteInf("Medicine workers day", owt.dateHash(year, 6, 17, 0, 0), 3));
	holiHeap.push_back(NoteInf("VDV day", owt.dateHash(year, 8, 2, 0, 0), 3));
	holiHeap.push_back(NoteInf("Archaeologist day", owt.dateHash(year, 8, 15, 0, 0), 3));
	holiHeap.push_back(NoteInf("Russian cinema day", owt.dateHash(year, 8, 27, 0, 0), 3));
	holiHeap.push_back(NoteInf("Knowledge day", owt.dateHash(year, 9, 1, 0, 0), 3));
	holiHeap.push_back(NoteInf("Programmer day", owt.dateHash(year, 9, 13, 0, 0), 3));
	holiHeap.push_back(NoteInf("Tourist day", owt.dateHash(year, 9, 27, 0, 0), 3));
	holiHeap.push_back(NoteInf("Teacher day", owt.dateHash(year, 10, 5, 0, 0), 3));
	holiHeap.push_back(NoteInf("Manager day", owt.dateHash(year, 11, 1, 0, 0), 3));
	holiHeap.push_back(NoteInf("Greate revolution day", owt.dateHash(year, 11, 7, 0, 0), 3));
	holiHeap.push_back(NoteInf("Rocket soldier day", owt.dateHash(year, 11, 19, 0, 0), 3));
}

void setYearTable(int64_t year)
{
    size_t notes[12][32];
    memset(notes, 0, sizeof(notes));
    for (size_t i = 0; i < dateHeap.size(); ++i)
    {
        if (owt.getY(dateHeap[i].date) == year)
        {
            uint8_t m = owt.getM(dateHeap[i].date);
            uint8_t d = owt.getD(dateHeap[i].date);
            ++notes[m-1][d];
        }
    }
	prepareHolidays(year);
    bool vis = owt.isYearLeap(year);
    for (size_t m = 0; m < 12; ++m)
    {
        bool isdis = false, isoff = true;
        int day = -owt.weekDay(year, m+1, 1) + 1;
        for (size_t w = 0; w < 6; ++w)
        {
            for (size_t d = 0; d < 7; ++d, ++day)
            {
                size_t md = daysAtM[m + 1] + (vis && m == 1);
                if (w == 5 && day > md) isdis = true;
                if (day > 0 && day <= md)
                {
                    yearFlags[m][w][d] = day, isoff = false;
                    yearFlags[m][w][d] += notes[m][day] * 1000;
                }
                else if (day <= 0) 
                    yearFlags[m][w][d] = day + daysAtM[(m + 11) % 12 + 1] + (vis && m == 2), isoff = true;
                else yearFlags[m][w][d] = day - md, isoff = true;
                int64_t h = owt.dateHash(year, m+1, day, 0, 0);
                if (!isdis && !isoff)
                    yearFlags[m][w][d] += owt.flagMap[h] * 100;
                else yearFlags[m][w][d] += isdis ? 600 : 500;
            }
        }
    }
}

void createYearButtons()
{
    sf::FloatRect bnd0 = sf::FloatRect(0, 0, BW, BH);
    owt.dayBack = owt.createBackground(bnd0, "textures/date-filled.png");
    owt.dayOff = owt.createBackground(bnd0, "textures/date-empty.png");
    owt.dayNone = owt.createBackground(bnd0, "textures/empty.png");
    int x = YM_SPACE, y = YM_TOP;
    for (size_t w = 0; w < 6; ++w, y += YDH_SPACE + BH)
    {
        x = YM_SPACE;
        for (size_t d = 0; d < 7; ++d, x += (YDW_SPACE + BW))
        {
            sf::FloatRect bnds = sf::FloatRect(x * USS, y * USS, BW, BH);
            yearButtons[w][d] = new PxButton(bnds, sf::Color::Red, 18, "XX", bF);
            yearButtons[w][d]->mainFont.loadFromFile(owt.fontFamily);
            yearButtons[w][d]->mainText.setFont(yearButtons[w][d]->mainFont);
            yearButtons[w][d]->mainText.setFillColor(sf::Color::White);
            yearButtons[w][d]->setSprite(owt.dayNone);
            yearButtons[w][d]->hasSpecialSprite = true;
            moveButtonDay(0, d, w, 0);
        }
    }
}

void deleteYearButtons()
{
    delete owt.dayBack;
    delete owt.dayOff;
    delete owt.dayNone;
    for (size_t w = 0; w < 6; ++w)
        for (size_t d = 0; d < 7; ++d)
            delete yearButtons[w][d];
}

void drawYearButtons()
{
    for (size_t w = 0; w < 6; ++w)
        for (size_t d = 0; d < 7; ++d)
        {
            if (boxFlags[w][d]%10 == 6) continue;
            else if (boxFlags[w][d]%10 == 5)
                yearButtons[w][d]->setSprite(owt.dayOff);
            else yearButtons[w][d]->setSprite(owt.dayBack);
            yfScroll->draw(yearButtons[w][d]);
            if (boxFlags[w][d] % 10 > 0 && boxFlags[w][d] % 10 < 5)
            {
                sf::FloatRect bnds = yearButtons[w][d]->control;
                owt.noteFlags[boxFlags[w][d] % 10 - 1]->setPosition(bnds.left + 45,
                    bnds.top);
                yfScroll->draw(owt.noteFlags[boxFlags[w][d] % 10 - 1]);
            }
            if (boxFlags[w][d] / 10 > 0)
            {
                uint8_t k = std::min(boxFlags[w][d] / 10, (uint32_t)10) - 1;
                sf::FloatRect bnds = yearButtons[w][d]->control;
                owt.noteLabels[k]->setPosition(bnds.left, 
                    bnds.top + bnds.height * USS);
                yfScroll->draw(owt.noteLabels[k]);
            }
        }
}

void generateYear()
{
    setYearTable(nowYear);
    for (size_t w = 0; w < 6; ++w)
        for (size_t d = 0; d < 7; ++d)
            moveButtonDay(colMonth[d], d, w, 0);
    selD = 1;
    selM = 1;
    updateTodayBar();
}

void tdYearOffFocused()
{
    updTxb(tdYear, 1, owt.MAX_YEAR_ABS);
    int64_t y = parse(tdYear);
    tdLeft->isEnabled = (y > 1);
    tdRight->isEnabled = (y < owt.MAX_YEAR_ABS);
    nowYear = owt.absY(y, !isTodayBC);
    generateYear();
}

bool checkMonths()
{
    sf::FloatRect bnds = yearButtons[0][selCol]->control;
    if (yfScroll->speed.x > 0 && bnds.left + ym_w * USS < yfScroll->mindPos.x + yfScroll->control.width)
    {
        bnds = weekLabels[selCol]->bounds;
        weekLabels[selCol]->setPosition(bnds.left + ym_w * USS, bnds.top);
        colMonth[selCol] = (colMonth[selCol] + 1) % 12;
        for (size_t i = 0; i < 6; ++i)
            moveButtonDay(colMonth[selCol], selCol, i, 1);
        if (selCol == 3)
        {
            deselectMonthButton(selMonth);
            selMonth = (selMonth + 1) % 12;
            selectMonthButton(selMonth);
        }
        selCol = (selCol+1) % 7;
        return true;
    }
    bnds = yearButtons[0][(selCol+6)%7]->control;
    if (yfScroll->speed.x < 0 && bnds.left + bnds.width * USS > yfScroll->mindPos.x + ym_w * USS)
    {
        selCol = (selCol + 6) % 7;
        bnds = weekLabels[selCol]->bounds;
        weekLabels[selCol]->setPosition(bnds.left - ym_w * USS, bnds.top);
        colMonth[selCol] = (colMonth[selCol] + 11) % 12;
        for (size_t i = 0; i < 6; ++i)
            moveButtonDay(colMonth[selCol], selCol, i, -1);
        if (selCol == 3)
        {
            deselectMonthButton(selMonth);
            selMonth = (selMonth + 11) % 12;
            selectMonthButton(selMonth);
        }
        return true;
    }
    return false;
}

void init()
{
    window.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), PROGRAM_NAME);
    // Set main boxes //
    owt.fillNoteShapes();
    owt.loadNoteLabels();
    initTodayBar();
    initDial();
    initMonthSelection();
    initYearField();
}

int main()
{
// Prepare //
    bF.loadFromFile(fontFamily);
    tF.loadFromFile(textFontFamily);
    sf::Vector2f directStart(20, 0);
// Set begin //
	prepareHolidays(nowYear);
    windowBack = owt.createBackground(sf::FloatRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT), "textures/backGrounds/1.png");
    init();
    auto dateOne = owt.dateHash(2019, 2, 1, 0, 0);
    owt.flagMap[dateOne] = 1;
    owt.flagMap[dateOne + (1 << 16)] = 4;
    owt.flagMap[owt.dateHash(2019, 1, 31, 0, 0)] = 1;

    setYearTable(nowYear);
    createYearButtons();
    updateTodayBar();
// Get time //
    auto lastFrameTime = std::chrono::steady_clock::now();

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                delete windowBack;
                delete noteScroll;
                delete dialScroll;
                delete crtInput;
                delete crtBack;
                delete crtTb;
                delete crtPrev;
                delete crtClose;
                delete crtOK;
                delete crtLeft;
                delete crtRight;
                delete crtYb;
                delete crtBC;
                delete crtYear;
                delete crtMonth;
                delete crtMb;
                delete crtDay;
                delete crtDb;
                delete crtAPr;
                delete crtHb;
                delete crtMnb;
                delete tdScroll;
                delete tdYb;
                delete tdYear;
                delete tdBC;
                delete tdLeft;
                delete tdRight;
                delete tdMShow;
                delete tdDShow;
                delete tdDb;
                delete tdDayNum;
                delete tdMonth;
                delete tdYearNum;
                delete dayInf1;
                delete dayInf2;
                delete dayInf3;
                delete hdState;
                delete hdRelig;
                delete hdLabor;
                delete hdName;
                delete crtNoteBtn;
                delete crtCross;
                delete ntName;
                delete flSel;
                delete flSet;
                delete mcUpScroll;
                delete mcScroll;
                delete mcLeft;
                delete mcRight;
                delete yfBack;
                delete yfScroll;
                deleteYearButtons();
                owt.deleteNoteLabels();
                owt.clearNoteShapes();
                rankClear();
                holidayClear();
                btsClear();
                window.close();
                return 0;
            }
            else if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Button::Left)
                {   
                    if (tdYear->offFocused())
                        tdYearOffFocused();
                    if (crtInput->offFocused())
                        crtInputOffFocused();
                    if (crtYear->offFocused())
                        crtYearOffFocused();
                    if (crtMonth->offFocused())
                        crtMonthOffFocused();
                    if (crtDay->offFocused())
                        crtDayOffFocused();
                    crtHour->offFocused();
                    crtMin->offFocused();

                    tdYear->offFocused();
                    int X = event.mouseButton.x, Y = event.mouseButton.y;
                    if (windowMode == 2)
                    {
                        crtInput->checkPressed(dialScroll->control, dialScroll->mindPos,X, Y);
                        crtYear->checkPressed(dialScroll->control, dialScroll->mindPos, X, Y);
                        crtMonth->checkPressed(dialScroll->control, dialScroll->mindPos, X, Y);
                        crtDay->checkPressed(dialScroll->control, dialScroll->mindPos, X, Y);
                        crtHour->checkPressed(dialScroll->control, dialScroll->mindPos, X, Y);
                        crtMin->checkPressed(dialScroll->control, dialScroll->mindPos, X, Y);
                        if (crtBC->checkPressed(dialScroll->control, dialScroll->mindPos, X, Y))
                        {
                            if (isDialBC)
                                crtBC->mainText.setFillColor(sf::Color(255, 106, 0, 190));
                            else crtBC->mainText.setFillColor(sf::Color(0, 0, 0, 180));
                        }
                        crtLeft->checkPressed(dialScroll->control, dialScroll->mindPos, X, Y);
                        crtRight->checkPressed(dialScroll->control, dialScroll->mindPos, X, Y);
                        crtClose->checkPressed(dialScroll->control, dialScroll->mindPos, X, Y);
                        crtOK->checkPressed(dialScroll->control, dialScroll->mindPos, X, Y);
                        dialScroll->checkPressed(X, Y);
                    }
                    else if (windowMode == 1)
                    {
                        tdLeft->checkPressed(tdScroll->control, tdScroll->mindPos, X, Y);
                        tdRight->checkPressed(tdScroll->control, tdScroll->mindPos, X, Y);
                        crtNoteBtn->checkPressed(tdScroll->control, tdScroll->mindPos, X, Y);
                        mcLeft->checkPressed(mcUpScroll->control, mcUpScroll->mindPos, X, Y);
                        mcRight->checkPressed(mcUpScroll->control, mcUpScroll->mindPos, X, Y);
                        if (tdBC->checkPressed(tdScroll->control, tdScroll->mindPos, X, Y))
                        {
                            if (isTodayBC)
                                tdBC->mainText.setFillColor(sf::Color(255, 106, 0, 190));
                            else tdBC->mainText.setFillColor(sf::Color(0, 0, 0, 180));
                        }
                        tdYear->checkPressed(tdScroll->control, tdScroll->mindPos,
                            X, Y);
                        if (yfScroll->checkPressed(X, Y))
                        {
                            lastWas = yfScroll;
                            for (size_t w = 0; w < 6; ++w)
                                for (size_t d = 0; d < 7; ++d)
                                    yearButtons[w][d]->checkPressed(yfScroll->control, yfScroll->mindPos,
                                        X, Y);
                        }
                        hdScroll->checkPressed(X, Y);
                        if (noteScroll->checkPressed(X, Y))
                        {
                            lastWas = noteScroll;
                            int beg = std::max(noteStart - 10, 0);
                            int end = std::min(noteStart + 10, (int)dateToday.size());
                            for (int i = beg; i < end; ++i)
                            {
                                dateToday[i]->downBtn->checkPressed(noteScroll->control,
                                    noteScroll->mindPos, X, Y);
                                dateToday[i]->upBtn->checkPressed(noteScroll->control,
                                    noteScroll->mindPos, X, Y);
                                dateToday[i]->throwBtn->checkPressed(noteScroll->control,
                                    noteScroll->mindPos, X, Y);
                                dateToday[i]->deleteBtn->checkPressed(noteScroll->control,
                                    noteScroll->mindPos, X, Y);
                            }
                        }
                        if (mcScroll->checkPressed(X, Y))
                        {
                            lastWas = mcScroll;
                            for (int m = 0; m < 12; ++m)
                                monthButtons[m]->checkPressed(mcScroll->control,
                                    mcScroll->mindPos, X, Y);
                        }
                        if (flRect.contains(X, Y))
                        {
                            int16_t w = (owt.weekDay(nowYear, selM, 1) + selD - 1) / 7;
                            int16_t d1 = (owt.weekDay(nowYear, selM, selD)) % 7;
                            int f = (X - flRect.left) / 24;
                            yearFlags[selM - 1][w][d1] += (f - owt.flagMap[dateNow]) * 100;
                            owt.flagMap[dateNow] = f;
                            if(colMonth[d1] == selM - 1)
                                moveButtonDay(selM - 1, d1, w, 0);
                            flSet->setPosition(297 + f * 24, 408);
                        }
                    }
                }
                mouseX = event.mouseButton.x, mouseY = event.mouseButton.y;
            }
            else if (event.type == sf::Event::MouseButtonReleased)
            {
                if (event.mouseButton.button == sf::Mouse::Button::Left)
                {
                    int X = event.mouseButton.x, Y = event.mouseButton.y;
                    if (windowMode == 2)
                    {
                        if (crtBC->checkReleased(dialScroll->control, dialScroll->mindPos, X, Y))
                        {
                            sf::Color resC;
                            isDialBC ^= 1;
                            if (isDialBC)
                                resC = sf::Color(255, 106, 0, 130);
                            else resC = sf::Color(0, 0, 0, 120);
                            crtBC->mainText.setFillColor(resC);
                        }
                        else if (crtLeft->checkReleased(dialScroll->control, dialScroll->mindPos, X, Y))
                        {
                            int64_t y = parse(crtYear);
                            if (y == owt.MAX_YEAR_ABS)
                                crtRight->isEnabled = true;
                            else if (y == 2)
                                crtLeft->isEnabled = false;
                            y -= 1;
                            setTxt(crtYear, std::to_string(y));
                        }
                        else if (crtRight->checkReleased(dialScroll->control, dialScroll->mindPos, X, Y))
                        {
                            int64_t y = parse(crtYear);
                            if (y == 1)
                                crtLeft->isEnabled = true;
                            else if (y + 1 == owt.MAX_YEAR_ABS)
                                crtRight->isEnabled = false;
                            y += 1;
                            setTxt(crtYear, std::to_string(y));
                        }
                        else if (crtClose->checkReleased(dialScroll->control, dialScroll->mindPos, X, Y))
                        {
                            closeDial();
                        }
                        else if (crtOK->checkReleased(dialScroll->control, dialScroll->mindPos, X, Y))
                        {
                            int64_t y = parse(crtYear);
                            if (isDialBC) y = -y + 1;
                            int8_t m = parse(crtMonth);
                            int8_t d = parse(crtDay);
                            int16_t th = parse(crtHour);
                            int16_t tm = parse(crtMin);
                            int64_t dte = owt.dateHash(y, m, d, 0, 0);
                            NoteInf nte = NoteInf(crtInput->text, dte, th * 60 + tm);
                            if (y == nowYear)
                            {
                                int16_t w = (owt.weekDay(y, m, 1) + d - 1) / 7;
                                int16_t d1 = (owt.weekDay(y, m, d)) % 7;
                                yearFlags[m - 1][w][d1] += 1000;
                                if (colMonth[d1] == m-1)
                                    boxFlags[w][d1] += 10;
                            }
                            add(nte);
                            closeDial();
                        }
                        dialScroll->isPressed = false;
                    }
                    else if (windowMode == 1)
                    {
                        if (tdBC->checkReleased(tdScroll->control, tdScroll->mindPos,
                            X, Y))
                        {
                            sf::Color resC;
                            isTodayBC ^= 1;
                            nowYear = owt.absY(parse(tdYear), !isTodayBC);
                            generateYear();
                            if (isTodayBC)
                                resC = sf::Color(255, 106, 0, 130);
                            else resC = sf::Color(0, 0, 0, 120);
                            tdBC->mainText.setFillColor(resC);
                        }
                        else if (tdLeft->checkReleased(tdScroll->control, tdScroll->mindPos,
                            X, Y))
                        {
                            int64_t y = parse(tdYear);
                            if (y == owt.MAX_YEAR_ABS)
                                tdRight->isEnabled = true;
                            else if (y == 2)
                                tdLeft->isEnabled = false;
                            y -= 1;
                            setTxt(tdYear, std::to_string(y));
                            nowYear = owt.absY(y, !isTodayBC);
                            generateYear();
                        }
                        else if (tdRight->checkReleased(tdScroll->control, tdScroll->mindPos,
                            X, Y))
                        {
                            int64_t y = parse(tdYear);
                            if (y == 1)
                                tdLeft->isEnabled = true;
                            else if (y + 1 == owt.MAX_YEAR_ABS)
                                tdRight->isEnabled = false;
                            y += 1;
                            setTxt(tdYear, std::to_string(y));
                            nowYear = owt.absY(y, !isTodayBC);
                            generateYear();

                        }
                        else if (crtNoteBtn->checkReleased(tdScroll->control, tdScroll->mindPos,
                            X, Y))
                        {
                            openDial("", nowYear, nowMonth, nowDay, 10, 0);
                        }
                        else if (mcLeft->checkReleased(mcUpScroll->control, mcUpScroll->mindPos,
                            X, Y))
                        {
                            mcScroll->speed.x -= 4.5;
                            --selM;
                        }
                        else if (mcRight->checkReleased(mcUpScroll->control, mcUpScroll->mindPos,
                            X, Y))
                        {
                            mcScroll->speed.x += 4.5;
                            ++selM;
                        }
                        else if (mcScroll->isPressed)
                        {
                            for (int m = 0; m < 12; ++m)
                                if (monthButtons[m]->checkReleased(mcScroll->control,
                                    mcScroll->mindPos, X, Y))
                                    selectMonth(m);
                        }
                        else if (noteScroll->isPressed)
                        {
                            int beg = std::max(noteStart - 10, 0);
                            int end = std::min(noteStart + 10, (int)dateToday.size());
                            for (int i = beg; i < end; ++i)
                            {
                                if (!dateToday[i]->upped && dateToday[i]->downBtn->checkReleased(noteScroll->control,
                                    noteScroll->mindPos, X, Y))
                                {
                                    dateToday[i]->heightTo = dateToday[i]->txtHeight;
                                }
                                else if (dateToday[i]->upped && dateToday[i]->upBtn->checkReleased(noteScroll->control,
                                    noteScroll->mindPos, X, Y))
                                {
                                    dateToday[i]->heightTo = dateToday[i]->MIN_HEIGHT * USS;
                                }
                                else if (dateToday[i]->throwBtn->checkReleased(noteScroll->control,
                                    noteScroll->mindPos, X, Y))
                                {
                                    openDial(dateToday[i]->note.text, nowYear, nowMonth, nowDay, dateToday[i]->note.time / 60, 
                                        dateToday[i]->note.time % 60);
                                }
                                else if (dateToday[i]->deleteBtn->checkReleased(noteScroll->control,
                                    noteScroll->mindPos, X, Y))
                                {
                                    int16_t w = (owt.weekDay(nowYear, selM, 1) + selD - 1) / 7;
                                    int16_t d1 = (owt.weekDay(nowYear, selM, selD)) % 7;
                                    yearFlags[selM - 1][w][d1] -= 1000;
                                    if(colMonth[d1] == selM-1)
                                        boxFlags[w][d1] -= 10;
                                    dateToday[i]->isDeleted = true;
                                }
                            }
                        }
                        else if (yfScroll->isPressed)
                        {
                            for (size_t w = 0; w < 6; ++w)
                                for (size_t d = 0; d < 7; ++d)
                                    if (yearButtons[w][d]->checkReleased(yfScroll->control,
                                        yfScroll->mindPos, X, Y))
                                    {
                                        selD = yearFlags[colMonth[d]][w][d] % 100;
                                        selM = colMonth[d] + 1;
                                        updateTodayBar();
                                    }
                        }
                        noteScroll->isPressed = false;
                        mcScroll->isPressed = false;
                        yfScroll->isPressed = false;
                        hdScroll->isPressed = false;
                    }
                }
            }
            else if (event.type == sf::Event::MouseMoved)
            {
                if (windowMode == 2)
                {
                    dialScroll->checkMove(mouseX - event.mouseMove.x, mouseY - event.mouseMove.y);
                }
                else if(windowMode == 1)
                {
                    noteScroll->checkMove(mouseX - event.mouseMove.x, mouseY - event.mouseMove.y);
                    mcScroll->checkMove(mouseX - event.mouseMove.x, mouseY - event.mouseMove.y);
                    yfScroll->checkMove(mouseX - event.mouseMove.x, mouseY - event.mouseMove.y);
                    hdScroll->checkMove(mouseX - event.mouseMove.x, mouseY - event.mouseMove.y);
                }
                mouseX = event.mouseMove.x, mouseY = event.mouseMove.y;
            }
            else if (event.type == sf::Event::MouseWheelScrolled)
            {
                if (windowMode == 1 && lastWas)
                {
                    lastWas->speed.x -= event.mouseWheelScroll.delta * 6;
                    lastWas->speed.y -= event.mouseWheelScroll.delta * 6;
                }
            }
            else if (event.type == sf::Event::KeyPressed)
            {
                if (windowMode == 2)
                {
                    if (crtInput->checkKeyPressed(event.key))
                        crtInputOffFocused();
                    else if (crtYear->checkKeyPressed(event.key))
                        crtYearOffFocused();
                    else if (crtMonth->checkKeyPressed(event.key))
                        crtMonthOffFocused();
                    else if (crtDay->checkKeyPressed(event.key))
                        crtDayOffFocused();
                    else if (crtHour->checkKeyPressed(event.key))
                        updTxb(crtHour, 0, 23);
                    else if (crtMin->checkKeyPressed(event.key))
                        updTxb(crtMin, 0, 59);
                }
                else if (windowMode == 1)
                {
                    if (tdYear->checkKeyPressed(event.key))
                        tdYearOffFocused();
                }
                if (event.key.code == sf::Keyboard::F2)
                {
                    openDial("", 2019, 11, 27, 16, 15);
                }
            }
            else if (event.type == sf::Event::TextEntered)
            {
                if (windowMode == 2)
                {
                    crtInput->checkInput(event.text.unicode);
                    if (event.text.unicode >= '0' && event.text.unicode <= '9')
                    {
                        crtYear->checkInput(event.text.unicode);
                        crtMonth->checkInput(event.text.unicode);
                        crtDay->checkInput(event.text.unicode);
                        crtHour->checkInput(event.text.unicode);
                        crtMin->checkInput(event.text.unicode);
                    }
                }
                else if (windowMode == 1)
                {
                    if (event.text.unicode >= '0' && event.text.unicode <= '9')
                    {
                        tdYear->checkInput(event.text.unicode);
                    }
                }
            }
        }
        // Check new frame //
        auto timeInterval = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - lastFrameTime);
        if (timeInterval.count() > ProgramFrameTime)
        {
            lastFrameTime = std::chrono::steady_clock::now();
            ++frameTime;
        }
        else continue;
        // Update Over
        if (windowMode == 2)
        {
            crtLeft->checkOver(dialScroll->control, dialScroll->mindPos,
                mouseX, mouseY);
            crtRight->checkOver(dialScroll->control, dialScroll->mindPos,
                mouseX, mouseY);
            crtClose->checkOver(dialScroll->control, dialScroll->mindPos,
                mouseX, mouseY);
            crtOK->checkOver(dialScroll->control, dialScroll->mindPos,
                mouseX, mouseY);
        }
        else if (windowMode == 1)
        {
            tdLeft->checkOver(tdScroll->control, tdScroll->mindPos,
                mouseX, mouseY);
            tdRight->checkOver(tdScroll->control, tdScroll->mindPos,
                mouseX, mouseY);
            crtNoteBtn->checkOver(tdScroll->control, tdScroll->mindPos,
                mouseX, mouseY);
            mcLeft->checkOver(mcUpScroll->control, mcUpScroll->mindPos,
                mouseX, mouseY);
            mcRight->checkOver(mcUpScroll->control, mcUpScroll->mindPos,
                mouseX, mouseY);
        }
        if(frameTime / 100 % 10 == 0)
            windowBack->move(0, 0.1);
        else if (frameTime / 100 % 10 == 5) windowBack->move(0, -0.1);
        // Drawing boxes //
        window.clear(sf::Color(197, 132, 151, 255));
        window.draw(*windowBack);
        window.draw(*yfBack);
        noteScroll->update();
        tdYear->update();
        hdScroll->update();
        mcScroll->update();
        tdScroll->openDrawing();
        yfScroll->update();
        window.draw(*tdYb);
        tdScroll->draw(tdYb);
        tdScroll->draw(tdYear, 2);
        tdScroll->draw(tdBC);
        tdScroll->draw(tdLeft);
        tdScroll->draw(tdRight);
        tdScroll->draw(tdMShow);
        tdScroll->draw(tdDShow);
        tdScroll->draw(tdDb);
        tdScroll->draw(tdDayNum, 0);
        tdScroll->draw(tdMonth, 0);
        tdScroll->draw(tdYearNum, 0);
        tdScroll->draw(dayInf1, 0);
        tdScroll->draw(dayInf2, 0);
        tdScroll->draw(dayInf3, 0);
        tdScroll->draw(dayInf4, 0);
        tdScroll->draw(hdName, 0);
        tdScroll->draw(crtNoteBtn);
        tdScroll->draw(crtCross);
        tdScroll->draw(ntName, 0);
        tdScroll->draw(flSet);
        tdScroll->draw(flSel);
        tdScroll->uniteDrawing(&window);
        while (checkMonths());

        hdScroll->openDrawing();
        sf::Vector2f directPoint = directStart;
        directPoint.x += 3;
        for (size_t i = 0; i < holiToday.size(); ++i)
        {
            holiToday[i]->setPosition(directPoint.x + 4, directPoint.y + 2);
            sf::Sprite * box = hdState;
            if (holiToday[i]->timer == 1)
                box = hdState;
            else if (holiToday[i]->timer == 2)
                box = hdRelig;
            else if (holiToday[i]->timer == 3)
                box = hdLabor;
            box->setPosition(directPoint);
            hdScroll->draw(box);
            hdScroll->draw(holiToday[i], 0);
            directPoint.y += HOLI_H + 9;
        }
        hdScroll->uniteDrawing(&window);

        checkPos();
        noteScroll->openDrawing();
        int beg = std::max(noteStart - 10, 0);
        int end = std::min(noteStart + 10, (int)dateToday.size());
        directPoint = directStart;
        if (beg)
        {
            directPoint.y = dateToday[beg - 1]->control.top;
            directPoint.y += dateToday[beg - 1]->Height + 4;
        }
        for (size_t i = beg; i < end; ++i)
        {
            Note * nte = dateToday[i];
            if (nte->isDeleted && nte->Height == 0)
            {
                erase(i);
                continue;
            }
            else if (!nte->isDeleted)
            {
                nte->setPosition(directPoint.x, directPoint.y);
                nte->update();
                if(nte->Height / USS >= nte->MIN_HEIGHT)
                nte->backGround = owt.NoteShapes[nte->Height / USS];
                noteScroll->draw(nte);
            }
            else nte->Height--;
            directPoint.y += nte->Height + 4;
        }
        noteScroll->uniteDrawing(&window);
        mcUpScroll->openDrawing();
        mcUpScroll->draw(mcLeft);
        mcUpScroll->draw(mcRight);
        mcUpScroll->uniteDrawing(&window);
        mcScroll->openDrawing();
        for (size_t i = 0; i < 12; ++i)
        {
            mcScroll->draw(monthButtons[i]);
        }
        mcScroll->uniteDrawing(&window);
        yfScroll->openDrawing();
        for (size_t i = 0; i < 7; ++i)
            yfScroll->draw(weekLabels[i], 0);
        drawYearButtons();
        yfScroll->uniteDrawing(&window);
        if (windowMode == 2)
        {
            crtInput->update();
            crtYear->update();
            crtMonth->update();
            crtDay->update();
            crtHour->update();
            crtMin->update();
            dialScroll->update();
            dialScroll->openDrawing();
            dialScroll->draw(crtBack);
            dialScroll->draw(crtTb);
            dialScroll->draw(crtInput, 4);
            dialScroll->draw(crtPrev, 5);
            dialScroll->draw(crtClose);
            dialScroll->draw(crtOK);
            dialScroll->draw(crtLeft);
            dialScroll->draw(crtRight);
            dialScroll->draw(crtYb);
            dialScroll->draw(crtBC);
            dialScroll->draw(crtYear, 2);
            dialScroll->draw(crtMb);
            dialScroll->draw(crtMonth, 2);
            dialScroll->draw(crtDb);
            dialScroll->draw(crtDay, 2);
            dialScroll->draw(crtAPr, 2);
            dialScroll->draw(crtHb);
            dialScroll->draw(crtHour, 2);
            dialScroll->draw(crtMnb);
            dialScroll->draw(crtMin, 2);
            dialScroll->draw(crtAPm, 2);
            dialScroll->uniteDrawing(&window);
        }
        window.display();
    }
}