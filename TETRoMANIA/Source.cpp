#include <SFML/Graphics.hpp>
#include<SFML/Audio.hpp>
#include<fstream>
#include <iostream>
#include <time.h>
#include<Windows.h>

sf::Texture tetris,intro, Back1, Back2,about, playTexture, historyTexture, exitTexture, over1, over2,easy,medium,hard,insane,win1,win2;
sf::Font font;
sf::Music music;

struct Point{
    int x, y;
} a[4], b[4], nextTetromino[4];

int figures[7][4] = {
    1,3,5,7, // I
    2,4,5,7, // Z
    3,5,4,6, // S
    3,5,4,7, // T
    2,3,5,7, // L
    3,5,7,6, // J
    2,3,4,5, // O
}, colorNum = 5, nextColorNum = 8,score,hscore;
std::string ch;

////////////////////////////////////////// Utility functions \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\/

bool check(int Large_field[20][20], int M, int N)
{
    for (int i = 0; i < 4; i++)
        if (a[i].x < 0 || a[i].x >= N || a[i].y >= M)
            return false;
        else if (Large_field[a[i].y][a[i].x])
            return false;

    return true;
}
bool checkGameover(int Large_field[20][20], int M, int N)
{
    for (int i = 0; i < 4; i++)
    {
        if (a[i].y >= M || Large_field[a[i].y][a[i].x] != 0)
            return true;
    }
    return false;
}
void generateNextTetromino()
{
    int n = rand() % 7;
    for (int i = 0; i < 4; i++)
    {
        nextTetromino[i].x = figures[n][i] % 2;
        nextTetromino[i].y = figures[n][i] / 2;
    }
}
void getNextTetromino(int N)
{
    colorNum = 1 + rand() % 8;
    for (int i = 0; i < 4; i++)
    {
        a[i].x = nextTetromino[i].x + N / 2 - 1;
        a[i].y = nextTetromino[i].y;
    }
    generateNextTetromino();
}

////////////////////////////////////////// Over  \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\/

bool Game_over(sf::RenderWindow& window) {
    sf::Sprite ov1(over1), ov2(over2);
    int currentOption = 0;
    bool eval = false;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                music.stop();
                window.close(); exit(0);
            }
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Down || event.key.code == sf::Keyboard::Up) {
                    currentOption = currentOption ^ 1;
                }
                else if (event.key.code == sf::Keyboard::Return) {
                    if (currentOption == 0) {
                        eval = true;
                    }
                    else if (currentOption == 1) {
                        music.stop();
                        window.close();
                        exit(0);
                    }
                }
            }
        }
        window.clear();
        // Draw the current selected option
        if (currentOption == 0)
            window.draw(ov1);
        else if (currentOption == 1)
            window.draw(ov2);

        sf::Text sc(std::to_string(score), font, 82);
        sc.setPosition(275, 302);
        sc.setFillColor(sf::Color::Magenta);
        window.draw(sc);

        window.display();
        if (eval == true)
            break;
    }
    return true;
}
bool win_over(sf::RenderWindow& window, int mode) {
    sf::Sprite w1(win1), w2(win2);
    int currentOption = 0;
    bool eval = false;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                music.stop();
                window.close(); exit(0);
            }
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Down || event.key.code == sf::Keyboard::Up) {
                    currentOption = currentOption ^ 1;
                }
                else if (event.key.code == sf::Keyboard::Up) {
                    currentOption = (currentOption + 2) % 2;
                }
                else if (event.key.code == sf::Keyboard::Return) {
                    if (currentOption == 1) {
                        eval = true;
                    }
                    else if (currentOption == 0) {
                        music.stop();
                        window.close();
                        exit(0);
                    }
                }
            }
        }
        window.clear();
        // Draw the current selected option
        if (currentOption == 1)
            window.draw(w1);
        else if (currentOption == 0)
            window.draw(w2);
        //mode 
        std::string str;
        if (mode == 2) { 
            str = "HARD"; 
        }
        else if (mode == 3) {
            str = "INSANE"; 
        }
        sf::Text mod(str, font, 70);
        mod.setPosition(380, 188);
        mod.setFillColor(sf::Color::Magenta);

        if (mode == 2) {
            mod.setPosition(588, 196);
        }
        else if (mode == 3) {
            mod.setPosition(578, 196);
        }
        mod.setFillColor(sf::Color::Red);
        window.draw(mod);
        //score 
        sf::Text sc(std::to_string(score), font, 82);
        sc.setPosition(302, 188);
        sc.setFillColor(sf::Color::Magenta);
        window.draw(sc);
        window.display();
        if (eval == true)
            break;
    }
    return true;
}

////////////////////////////////////////// Playing Levels \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\/

void medium_lvl(int Area[][20]) {
    for (int i = 0; i <= 2; i++) {
        Area[19][i] = 9;
        Area[13][i] = 9;
        Area[7][i] = 9;
    }
    for (int i = 9; i >= 7; i--) {
        Area[16][i] = 9;
        Area[10][i] = 9;
    }
}
void hard_lvl(int Area[][20]) {
    for (int i = 16; i <= 19; i++) {
        Area[i][9] = 9;
        Area[i][11] = 9;
    }
    for (int i = 15; i <= 19; i++) {
        Area[i][10] = 9;
    }
    for (int i = 17; i <= 19; i++) {
        Area[i][8] = 9;
        Area[i][12] = 9;
    }
    for (int i = 18; i <= 19; i++) {
        Area[i][7] = 9;
        Area[i][13] = 9;
    }
    Area[19][6] = 9;
    Area[19][14] = 9;
    for (int i = 0; i <= 5; i++) {
        Area[11][i] = 9;

    }
    for (int i = 19; i >= 14; i--) {
        Area[11][i] = 9;
    }
}
void insane_lvl(int Area[][20]) {
    for (int i = 17; i <= 19; i++) {
        Area[i][8] = 9;
        Area[i][12] = 9;
        Area[i][0] = 9;
        Area[i][19] = 9;
    }
    Area[6][0] = 9;
    Area[6][19] = 9;
    for (int i = 18; i <= 19; i++) {
        Area[i][7] = 9;
        Area[i][13] = 9;
    }
    Area[11][16] = 9;
    Area[11][3] = 9;
    for (int i = 17; i <= 18; i++) {
        Area[i][1] = 9;
        Area[i][18] = 9;
    }
    Area[19][6] = 9;
    Area[19][14] = 9;
    for (int i = 6; i <= 7; i++) {
        Area[i][1] = 9;
        Area[i][18] = 9;
    }
    for (int i = 8; i <= 12; i++) {
        Area[10][i] = 9;
    }
    for (int i = 0; i <= 7; i++) {
        if (i % 2 == 0) {
            Area[2][i] = 9;
        }
        else {
            Area[3][i] = 9;
        }
    }
    for (int j = 12; j <= 19; j++) {
        if (j % 2 == 0) {
            Area[3][j] = 9;
        }
        else {
            Area[2][j] = 9;
        }
    }
}

////////////////////////////////////////// Play function \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\/

bool play(sf::RenderWindow& window, int mode, int Area[][20], int code,float delay) {
    sf::Sprite block(tetris), background;
    std::ifstream rdr("images/hscore.txt");
    rdr >> ch;
    rdr.close();
    hscore = std::stoi(ch);
    if (code == 1) {
        background.setTexture(Back2);
    }
    else {
        background.setTexture(Back1);
    }
    int dx = 0;
    bool rotate = false, eval = false;
    float timer = 0;
    score = 0;
    sf::Clock clock;
    const int M = 20;
    int val,win = 10000;
    if (code == 1) {
        val = 10;
    }
    else {
        val = 20;
    }
    const int N = val;
    if (mode == 1) {
        medium_lvl(Area);
    }
    else if (mode == 2) {
        hard_lvl(Area);
        win = 10;
    }
    else if (mode == 3) {
        insane_lvl(Area);
        win = 10;
    }
    // falling and upcoming tetramino
    generateNextTetromino();
    getNextTetromino(N);

    while (window.isOpen()){
        float time = clock.restart().asSeconds();
        timer += time;
        sf::Event e;
        while (window.pollEvent(e))
        {
            if (e.type == sf::Event::Closed) {
                music.stop();
                window.close(); exit(0);
            }
            if (e.type == sf::Event::KeyPressed)
            {
                if (e.key.code == sf::Keyboard::Up)
                    rotate = true;
                else if (e.key.code == sf::Keyboard::Left)
                    dx = -1;
                else if (e.key.code == sf::Keyboard::Right)
                    dx = 1;
                else if (e.key.code == sf::Keyboard::Down)
                    delay = 0.05;
            }
        }

        //// <- Move -> ///
        for (int i = 0; i < 4; i++){
            b[i] = a[i];
            a[i].x += dx;
        }
        if (!check(Area,M,N)){
            for (int i = 0; i < 4; i++)
                a[i] = b[i];
        }

        ////// Rotate //////
        if (rotate){
            Point p = a[1];
            for (int i = 0; i < 4; i++){
                int x = a[i].y - p.y;
                int y = a[i].x - p.x;
                a[i].x = p.x - x;
                a[i].y = p.y + y;
            }
            if (!check(Area,M,N)){
                for (int i = 0; i < 4; i++)
                    a[i] = b[i];
            }
        }

        /////// Falling Tetramino //////
        if (timer > delay){
            for (int i = 0; i < 4; i++){
                b[i] = a[i];
                a[i].y += 1;
            }

            if (!check(Area,M,N)){
                for (int i = 0; i < 4; i++)
                    Area[b[i].y][b[i].x] = colorNum;

                getNextTetromino(N);
            }
            timer = 0;
        }
        /////// Whole Grid Lines //////////
        int k = M - 1;
        for (int i = M - 1; i > 0; i--){
            int count = 0;
            for (int j = 0; j < N; j++){
                if (Area[i][j])
                    count++;
                Area[k][j] = Area[i][j];
            }
            if (count < N) {
                k--;
            }
            //////////////////////  score \\\\\\\\\\\\\\\\\\\\\\\\\\\\\/
            else {
                score += 10;
                if (hscore < score) {
                    std::ofstream wrt("images/hscore.txt");
                    wrt << score;
                    wrt.close();
                }
            }
        }

        if (checkGameover(Area,M,N)){
            eval = Game_over(window);
        }
        if (mode == 2 || mode == 3) {
            if (score == win) {
                eval = win_over(window, mode);
            }
        }
        dx = 0;
        rotate = false;
        delay = 0.3;

        /////////draw//////////
        window.clear();
        window.draw(background);
        // Playing Field
        for (int i = 0; i < M; i++) {
            for (int j = 0; j < N; j++) {
                if (Area[i][j] == 0)   continue;
                block.setTextureRect(sf::IntRect(Area[i][j] * 28, 0, 28, 28));
                block.setPosition(j * 28, i * 28);
                if (code == 1) {
                    block.move(162, 50);
                }
                else {
                    block.move(22, 50);
                }
                window.draw(block);
            }
        }
        // Falling tetramino
        for (int i = 0; i < 4; i++){
            block.setTextureRect(sf::IntRect(colorNum * 28, 0, 28, 28));
            block.setPosition(a[i].x * 28, a[i].y * 28);
            if (code == 1) {
                block.move(162, 50);
            }
            else {
                block.move(22, 50);
            }
            window.draw(block);
        }
        // Next Block of Tetramino
        for (int i = 0; i < 4; i++){
            block.setTextureRect(sf::IntRect(nextColorNum * 28, 0, 28, 28));
            block.setPosition(nextTetromino[i].x * 28 + 656, nextTetromino[i].y * 28 + 101);
            window.draw(block);
        }
        sf::Text sc(std::to_string(score), font, 50);
        sc.setPosition(674, 289);
        sc.setFillColor(sf::Color::Magenta);
        window.draw(sc);
        // mode display
        std::string str1;
        if (mode == 2) {
            str1 = "HARD";
        }
        else if (mode == 3) {
            str1 = "INSANE";
        }
        else if (mode == 0) {
            str1 = "EASY";
        }
        else {
            str1 = "MEDIUM";
        }
        sf::Text mod1(str1, font, 50);
        if (mode == 2 || mode == 0) {
            mod1.setPosition(659, 409);
        }
        else if (mode == 3 || mode == 1) {
            mod1.setPosition(637, 409);
        }
        mod1.setFillColor(sf::Color::Red);
        window.draw(mod1);
        // highest score
        if (mode == 0 || mode == 1) {
            std::string hsco = std::to_string(hscore);
            sf::Text h_score(hsco, font, 50);
            h_score.setPosition(672, 526);
            h_score.setFillColor(sf::Color::Cyan);
            window.draw(h_score);
        }
        // Target score
        if (mode == 2 || mode == 3) {
            std::string str;
            if (mode == 2) str = "100";
            else str = "250";
            sf::Text win(str, font, 50);
            win.setPosition(672, 526);
            win.setFillColor(sf::Color::Cyan);
            window.draw(win);
        }

        window.display();
        if (eval == true) {
            break;
        }
    }
    return true;
}

////////////////////////////////////////// Menu Lvls \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\/

bool levels(sf::RenderWindow& window, int Large_field[][20]) {
    sf::Sprite e(easy), m(medium), h(hard), i(insane);
    int currentOption = 0;
    bool eval = false;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed){
                music.stop();
                window.close(); exit(0);
            }

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Down) {
                    currentOption = (currentOption + 1) % 4;
                }
                else if (event.key.code == sf::Keyboard::Up) {
                    currentOption = (currentOption + 3) % 4;
                }
                else if (event.key.code == sf::Keyboard::Return) {
                    if (currentOption == 0) {
                        eval = play(window, 0, Large_field, 1,2);
                    }
                    else if (currentOption == 1) {
                        eval = play(window, 1, Large_field, 1,1.5);
                    }
                    else if (currentOption == 2) {
                        eval = play(window, 2, Large_field, 2,0.05);
                    }
                    else if (currentOption == 3) {
                        eval = play(window, 3, Large_field, 2, 0.05);
                    }
                }
            }
        }
        window.clear();
        // Draw the current selected option
        if (currentOption == 0)
            window.draw(e);
        else if (currentOption == 1)
            window.draw(m);
        else if (currentOption == 2)
            window.draw(h);
        else if (currentOption == 3)
            window.draw(i);

        window.display();
        if (eval == true) {
            break;
        }
    }
    return true;
}

////////////////////////////////////////// About \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\/

bool about_(sf::RenderWindow& window) {
    sf::Sprite abt(about);
    bool eval = false;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                music.stop();
                window.close(); exit(0);
            }

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Return) {
                    eval = true;
                }
            }
        }
        window.clear();
        window.draw(abt);

        window.display();
        if (eval == true) {
            break;
        }
    }
    return true;
}

////////////////////////////////////////// Int Main \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\/

int main()
{
    HWND hWnd = GetConsoleWindow();
    ShowWindow(hWnd, SW_HIDE);
    sf::RenderWindow window(sf::VideoMode(800, 610), "TETRoMANIA");
    // load all files
    intro.loadFromFile("images/intro.png");
    tetris.loadFromFile("images/tiles3.png");
    Back1.loadFromFile("images/background.png");
    Back2.loadFromFile("images/easy_back1.png");
    over1.loadFromFile("images/gameover.png");
    over2.loadFromFile("images/gameover2.png");
    playTexture.loadFromFile("images/main1.png");
    historyTexture.loadFromFile("images/main2.png");
    exitTexture.loadFromFile("images/main3.png");
    easy.loadFromFile("images/easy.png");
    medium.loadFromFile("images/medium.png");
    hard.loadFromFile("images/hard.png");
    insane.loadFromFile("images/insane.png");
    win1.loadFromFile("images/win12.png");
    win2.loadFromFile("images/win13.png");
    about.loadFromFile("images/about.png");
    sf::Sprite playSprite(playTexture), historySprite(historyTexture), exitSprite(exitTexture),intr(intro);
    //music and font
    music.openFromFile("images/back_music.wav");
    music.setLoop(true);
    music.setVolume(40);

    window.draw(intr);
    window.display();
    sf::sleep(sf::seconds(2.2));

    music.play();
    font.loadFromFile("images/Font.ttf");
    int currentOption = 0;
    bool game = true;

    // game flow
    do {
        int Area[20][20] = { 0 };
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                music.stop();
                window.close(); exit(0);
            }

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Down) {
                    currentOption = (currentOption + 1) % 3;
                }
                else if (event.key.code == sf::Keyboard::Up) {
                    currentOption = (currentOption + 2) % 3;
                }
                else if (event.key.code == sf::Keyboard::Return) {
                    if (currentOption == 0) {
                        game = levels(window, Area);
                        if (game == true) {
                            break;
                        }
                    }
                    else if (currentOption == 1) {
                        game = about_(window);
                        if (game == true) {
                            break;
                        }
                    }
                    else if (currentOption == 2) {
                        music.stop();
                        window.close();
                        exit(0);
                    }
                }
            }
        }
        window.clear();
        // Current selected option
        if (currentOption == 0)
            window.draw(playSprite);
        else if (currentOption == 1)
            window.draw(historySprite);
        else if (currentOption == 2)
            window.draw(exitSprite);

        window.display();
    } while (game == true);
    return 0;
}