#include <SFML/Graphics.hpp>
#include <time.h>
using namespace sf;

const int M = 20;
const int N = 20;

int field[M][N] = { 0 };

struct Point
{
    int x, y;
} a[4], b[4], nextTetromino[4];

int figures[7][4] =
{
    1,3,5,7, // I
    2,4,5,7, // Z
    3,5,4,6, // S
    3,5,4,7, // T
    2,3,5,7, // L
    3,5,7,6, // J
    2,3,4,5, // O
};

int score = 0;
bool gameover = false;

bool check()
{
    for (int i = 0; i < 4; i++)
        if (a[i].x < 0 || a[i].x >= N || a[i].y >= M)
            return false;
        else if (field[a[i].y][a[i].x])
            return false;

    return true;
};

bool checkGameover()
{
    for (int i = 0; i < 4; i++)
    {
        if (a[i].y >= M || field[a[i].y][a[i].x] != 0)
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

void getNextTetromino()
{
    for (int i = 0; i < 4; i++)
    {
        a[i].x = nextTetromino[i].x + N / 2 - 1;
        a[i].y = nextTetromino[i].y;
    }
    generateNextTetromino();
}

int main()
{
    srand(time(0));

    RenderWindow window(VideoMode(800, 610), "The Game!");

    Texture t1, t2, t3;
    t1.loadFromFile("images/tiles1.png");
    t2.loadFromFile("images/background.png");
    t3.loadFromFile("images/frame.png");

    Font font;
    font.loadFromFile("images/Font.ttf"); // Replace "arial.ttf" with the path to your font file

    Sprite s(t1), background(t2), frame(t3);

    int dx = 0;
    bool rotate = false;
    int colorNum = 1, nextColorNum = 1;
    float timer = 0, delay = 0.5;

    Clock clock;

    generateNextTetromino();
    getNextTetromino();

    while (window.isOpen())
    {
        float time = clock.restart().asSeconds();
        timer += time;

        Event e;
        while (window.pollEvent(e))
        {
            if (e.type == Event::Closed)
                window.close();

            if (e.type == Event::KeyPressed)
            {
                if (e.key.code == Keyboard::Up)
                    rotate = true;
                else if (e.key.code == Keyboard::Left)
                    dx = -1;
                else if (e.key.code == Keyboard::Right)
                    dx = 1;
                else if (e.key.code == Keyboard::Down)
                    delay = 0.05;
            }
        }

        if (!gameover)
        {
            //// <- Move -> ///
            for (int i = 0; i < 4; i++)
            {
                b[i] = a[i];
                a[i].x += dx;
            }
            if (!check())
            {
                for (int i = 0; i < 4; i++)
                    a[i] = b[i];
            }

            //////Rotate//////
            if (rotate)
            {
                Point p = a[1];
                for (int i = 0; i < 4; i++)
                {
                    int x = a[i].y - p.y;
                    int y = a[i].x - p.x;
                    a[i].x = p.x - x;
                    a[i].y = p.y + y;
                }
                if (!check())
                {
                    for (int i = 0; i < 4; i++)
                        a[i] = b[i];
                }
            }

            ///////Tick//////
            if (timer > delay)
            {
                for (int i = 0; i < 4; i++)
                {
                    b[i] = a[i];
                    a[i].y += 1;
                }

                if (!check())
                {
                    for (int i = 0; i < 4; i++)
                        field[b[i].y][b[i].x] = colorNum;

                    getNextTetromino();
                }

                timer = 0;
            }

            ///////check lines//////////
            int k = M - 1;
            for (int i = M - 1; i > 0; i--)
            {
                int count = 0;
                for (int j = 0; j < N; j++)
                {
                    if (field[i][j])
                        count++;
                    field[k][j] = field[i][j];
                }
                if (count < N)
                    k--;
                else
                    score += 10;
            }

            if (checkGameover())
            {
                gameover = true;
                window.clear();
                sf::Text sc1("Game over", font, 30);
                sc1.setPosition(200, 390);
                sc1.setFillColor(sf::Color::Red);
                window.draw(sc1);
                window.display();
            }

            dx = 0;
            rotate = false;
            delay = 0.3;
        }

        /////////draw//////////
        window.clear(Color::White);
        // Draw the background
        window.draw(background);

        // Draw the main playing field
        for (int i = 0; i < M; i++)
            for (int j = 0; j < N; j++)
            {
                if (field[i][j] == 0)
                    continue;
                s.setTextureRect(IntRect(field[i][j] * 28, 0, 28, 28));
                s.setPosition(j * 28, i * 28);
                s.move(25, 50); // offset
                window.draw(s);
            }

        // Draw the current tetromino
        for (int i = 0; i < 4; i++)
        {
            s.setTextureRect(IntRect(colorNum * 28, 0, 28, 28));
            s.setPosition(a[i].x * 28, a[i].y * 28);
            s.move(25, 50); // offset
            window.draw(s);
        }

        // Draw the upcoming tetromino
        for (int i = 0; i < 4; i++)
        {
            s.setTextureRect(IntRect(nextColorNum * 28, 0, 28, 28));
            s.setPosition(nextTetromino[i].x * 28 + 700, nextTetromino[i].y * 28 + 51);
            window.draw(s);
        }

        sf::Text sc(std::to_string(score), font, 30);
        sc.setPosition(200, 390);
        sc.setFillColor(sf::Color::Red);
        window.draw(sc);

        window.display();
    }

    return 0;
}
