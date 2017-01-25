// Render -- displays the hivolts user interface. (Implementation)
//
// By Jaldhar h. Vyas <jaldhar@braincells.com>
// Copyright (C) 2017, Consolidated Braincells Inc.  All rights reserved.
// "Do What Thou Wilt." shall be the whole of the license.

#include <clocale>
#include <csignal>
#include <cstdlib>
#include <map>
#include <set>
#include <vector>
#include <curses.h>
// These ncurses macros name clash with c++ symbols on old versions of ncurses
#if NCURSES_MAJOR_VERSION < 5 || (NCURSES_MAJOR_VERSION == 5 && NCURSES_MINOR_VERSION < 9)
#undef box
#undef clear
#undef erase
#undef move
#undef refresh
#endif
#include "render.h"
#include "world.h"
using namespace std;


using APPEARANCEMAP = std::map<APPEARANCE, chtype>;

static const int TILEHEIGHT = 1;
static const int TILEWIDTH = 1;

struct Render::RenderImpl {
    RenderImpl();
    ~RenderImpl() = default;

    void    drawStatus();
    void    drawTitle();
    bool    playAgain(VICTORY victory);
    void    playWarningSound();
    void    refresh();
    void    reallyEnd();
    void    resize();
    void    setStatusWin(WINDOW*& win);
    void    setTitleWin(WINDOW*& win);

    static int  createStatusWin(WINDOW*, int);
    static int  createTitleWin(WINDOW*, int);
    static void endSig(int);

    WINDOW*       _status;
    WINDOW*       _title;
    WINDOW*       _viewport;
    int           _lines;
    int           _cols;
    string        _statusText;
    string        _titleText;
    APPEARANCEMAP _appearance;
} Render::_impl;

Render::RenderImpl::RenderImpl() : _status{nullptr},
_title{nullptr}, _viewport{nullptr}, _lines{0}, _cols{0}, _statusText(""),
_titleText(""), _appearance {
    { APPEARANCE::EMPTY,      ' ' | COLOR_PAIR(1) },
    { APPEARANCE::FENCE,      '#' | COLOR_PAIR(3) },
    { APPEARANCE::MHO,        'O' | COLOR_PAIR(4) },
    { APPEARANCE::PLAYER,     '@' | COLOR_PAIR(5) },
    { APPEARANCE::EXPLOSION1, 'O' | COLOR_PAIR(5) },
    { APPEARANCE::EXPLOSION2, 'o' | COLOR_PAIR(5) },
    { APPEARANCE::EXPLOSION3, '*' | COLOR_PAIR(6) },
}{
}

void Render::RenderImpl::drawStatus() {
    wclear(_status);
    mvwaddstr(_status, 0, 1, _statusText.c_str());
    wnoutrefresh(_status);
    doupdate();
}

void Render::RenderImpl::drawTitle() {
    wclear(_title);
    const int len = _titleText.length();
    mvwaddstr(_title, 0, (_cols - len)/2, _titleText.c_str());
    wnoutrefresh(_title);
}

// If the game is over or the player wants to quit, this function will get
// called.  It returns true if he wishes to play again or false if the game
// should end.
bool Render::RenderImpl::playAgain(VICTORY victory) {
    string message;

    switch(victory) {
    case VICTORY::WIN:
        message = "You've won! ";
        break;
    case VICTORY::LOSE:
        message = "You've lost!";
        break;
    case VICTORY::DRAW:
        message = "It's a draw!";
        break;
    default:
        break;
    }

    message += " Play again? (Y/N)";

    for(;;) {
        _statusText = message;
        drawStatus();

        int c = getch();

        if (c == 'Y' || c == 'y') {
            _statusText = "";
            drawStatus();
            return true;
        } else if (c == 'N' || c == 'n') {
            return false;
        } else {
            playWarningSound();
        }
    }
}

// A sound that will get played if an error occurs.
void Render::RenderImpl::playWarningSound() {
    beep();
}

// Restores the cursor, shuts down the display and exits the program.
void Render::RenderImpl::reallyEnd() {
    curs_set(1);
    if (_status != nullptr) {
        delwin(_status);
    }
    if (_title != nullptr) {
        delwin(_title);
    }
    if (_viewport != nullptr) {
        delwin(_viewport);
    }
    endwin();
    clear();
    exit(EXIT_SUCCESS);
}

// Refreshes the screen if it has gotten corrupted.
void Render::RenderImpl::refresh() {
    wnoutrefresh(_title);
    wnoutrefresh(_viewport);
    wnoutrefresh(_status);
    doupdate();
}

// Called once during initialization to set subwindow sizes and if the screen
// has been resized (if it is running in an xterm for instance.)
void Render::RenderImpl::resize() {
    getmaxyx(stdscr, _lines, _cols);

    if (_viewport == nullptr) {
        cbreak();
        noecho();
        nonl();
        _viewport = subwin(stdscr, _lines, _cols, 0, 0);
    } else {
        touchwin(stdscr);
        wresize(_viewport, _lines, _cols);
        wbkgd(_viewport, _appearance[APPEARANCE::EMPTY]);
    }

    wresize(_status, 1 , _cols);
    wbkgd(_status, ' ' | COLOR_PAIR(2));
    drawStatus();

    wresize(_title, 1 , _cols);
    wbkgd(_title, ' ' | COLOR_PAIR(2));
    drawTitle();
}

// These next four methods are only necessary because the status and title
// windows have to be created via static methods with a specific signature.

void Render::RenderImpl::setStatusWin(WINDOW*& win) {
    _status = win;
}

void Render::RenderImpl::setTitleWin(WINDOW*& win) {
    _title = win;
}

int Render::RenderImpl::createStatusWin(WINDOW* win, int /*ncols*/){
    _impl.setStatusWin(win);

    return 0;
}

int Render::RenderImpl::createTitleWin(WINDOW* win, int /*ncols*/) {
    _impl.setTitleWin(win);

    return 0;
}

// This is the interrupt handler for SIGINT and SIGSEGV so it has to be a static
// method that will call the actual end method.
void Render::RenderImpl::endSig(int /* sig */) {
    Render render;
    render._impl.reallyEnd();
}

Render::Render() {
}

Render::~Render() = default;

// Sets up the display.  Called once at the beginning of the game.
void Render::init(const string titleText) {
    setlocale(LC_ALL, "POSIX");

    struct sigaction act;
    act.sa_handler = Render::RenderImpl::endSig;
    sigemptyset (&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGINT, &act, NULL);
    sigaction(SIGSEGV, &act, NULL);

    ripoffline(1, Render::RenderImpl::createTitleWin);
    ripoffline(-1, Render::RenderImpl::createStatusWin);
    initscr();
    keypad(stdscr, TRUE);
    intrflush(stdscr, FALSE);
    nodelay(_impl._viewport, FALSE);

    if (has_colors()) {
        start_color();
        init_pair(1, COLOR_WHITE, COLOR_BLACK);
        init_pair(2, COLOR_BLACK, COLOR_WHITE);
        init_pair(3, COLOR_YELLOW, COLOR_BLACK);
        init_pair(4, COLOR_GREEN, COLOR_BLACK);
        init_pair(5, COLOR_RED, COLOR_BLACK);
        init_pair(6, COLOR_MAGENTA, COLOR_BLACK);
    }

    curs_set(0);
    _impl.resize();
    _impl._statusText = "";
    _impl.drawStatus();
    _impl._titleText = titleText;
    _impl.drawTitle();
}

// Called every time the screen has to be redrawn.  There are three special
// cases:
// 1. If the refresh key (CTRL-R) has been pressed in which case the screen is
//    refreshed and redrawn.
// 2. If the screen is resized in which case the screen is redrawn.
// 3. If an invalid key was pressed in which case the warning sound is played
//    and the screen is not redrawn.
// Otherwise the screen is redrawn.
void Render::update(World& world) {
    bool finished = false;

    world.foreach_entity([&world, &finished, this](Entity& e, int i) -> bool {
        if (e.has(COMPONENT_INPUT)) {
            COMMAND command = world._inputs[i]._command;
            if (command == COMMAND::REFRESH) {
                this->_impl.refresh();
            }
            else if (command == COMMAND::RESIZE) {
                this->_impl.resize();
            }
            else if (command == COMMAND::BAD) {
                this->_impl.playWarningSound();
                finished = true;
            }
            return false;
        }
        return true;
    });
    if (finished) {
        return;
    }

    vector<vector<APPEARANCE>> board;
    board.resize(world._SZ, vector<APPEARANCE>(world._SZ, APPEARANCE::EMPTY));

    multimap<Position, int> positions;

    world.foreach_entity([&world, &positions](Entity& e, int i) -> bool {
        if (e.has(COMPONENT_POSITION)) {
            Position& p = world._positions[i];
            if (p._row > -1 && p._column > -1) {
                positions.emplace(p, i);
            }
        }
        return true;
    });

    set<Position> keys; // list of unique positions
    for (auto pos: positions) {
        keys.emplace(pos.first);
    }

    // For each position select for display the entity on it with the highest
    // z-order.
    for (auto key: keys) {
        Position& p = key;
        auto values = positions.equal_range(key);
        int z = -1;
        int e = 0;
        for (auto i = values.first; i != values.second; ++i) {
            if (i->first._zorder > z) {
                z = i->first._zorder;
                p = i->first;
                e = i->second;
            }
        }
        board[p._row][p._column] = world._appearances[e]._is;
    }

    curs_set(0);
    wclear(_impl._viewport);

    wbkgd(_impl._viewport, _impl._appearance[APPEARANCE::EMPTY]);

    // Center the board in the screen.
    int top = ((_impl._lines - world._SZ) / 2) - 1;
    int left = ((_impl._cols - world._SZ) / 2) - 1;

    for (auto i = 0; i < world._SZ; ++i) {
        for (auto j = 0; j < world._SZ; ++j) {
        mvwaddch(_impl._viewport, top + i + TILEHEIGHT, left + j + TILEWIDTH,
            _impl._appearance[board[i][j]]);
        }
    }

    wnoutrefresh(_impl._viewport);
    doupdate();
}

// Called when a game is over.  playAgain() is run to give the player a chance
// to start a new game or to finish the program altogether.  Returns false if
// another game is to be played or true if the program should really end.
bool Render::end(VICTORY victory) {
    if (_impl.playAgain(victory)) {
        return false;
    }
    _impl.reallyEnd();
    return true;
}
