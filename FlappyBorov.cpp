#include <TXLib.h>
#include "Buttons.h"

//-----------------------------------------------------------------------------

const int wWidth =  800;
const int wHeight = 800;

const int WALLS_MAX = 3;

const int HOLE_MIN_Y = 300;
const int HOLE_MAX_Y = 600;

//-----------------------------------------------------------------------------

double rnd (double from, double to);

//-----------------------------------------------------------------------------

struct Player

{

    double x_;
    double y_;

    double vy_;

    HDC image1_;
    HDC image2_;

    COLORREF alpha_;

    double time_;

    void draw ();

    void move ();

};

struct Wall

{

    double x_;
    double width_;

    double hole_y;
    double hole_height;

    double speed_;

    COLORREF color_;

    bool counted_;

    Wall ();

    Wall (int n, double speed, COLORREF color);

    void draw ();

    void move ();

};

Wall::Wall () :

    x_           (0),
    width_      (50),
    hole_y       (0),
    hole_height (50),
    speed_       (0),
    color_       (0),
    counted_ (false)

{}

Wall::Wall (int n, double speed, COLORREF color) :

    speed_   (speed),
    color_   (color),
    counted_ (false)

{

    x_ = wWidth + (wWidth / WALLS_MAX) * n;

    width_ = 50;

    hole_height = rnd (140, 200);

    hole_y = rnd (HOLE_MIN_Y, HOLE_MAX_Y - hole_height);

}

//-----------------------------------------------------------------------------

int checkWall (Wall wall, Player player);

int checkWalls (Wall * walls, Player player);

int run (int walls_speed, COLORREF bkcolor, COLORREF wall_color);

void drawScore (int score);

void addScore (int * score);

int gameOver (int score);

//-----------------------------------------------------------------------------

int main ()

{

    double walls_speed = 3;

    COLORREF bkcolor = TX_WHITE;
    COLORREF wall_color = RGB (50, 50, 50);

    txCreateWindow (wWidth, wHeight);
    txDisableAutoPause ();

    while (true)

    {

        int result = run (walls_speed, bkcolor, wall_color);

        if (result == 1) return 0;

    }

}

//-----------------------------------------------------------------------------

int run (int walls_speed, COLORREF bkcolor, COLORREF wall_color)

{

    HDC background = txLoadImage ("Background.bmp");

    int background_x = 0;

    int score = 0;

    Player player = {wWidth / 2, wHeight / 2, 0, txLoadImage ("Player1.bmp"), txLoadImage ("Player2.bmp"), TX_WHITE, 0};

    Wall walls[WALLS_MAX];

    for (int n = 0; n < WALLS_MAX; n++)

    {

        walls[n] = {n, walls_speed, wall_color};

    }

    bool space_pressed = false;

    while (true)

    {

        txBitBlt (txDC (), background_x, 0, 0, 0, background);
        txBitBlt (txDC (), txGetExtentX (background) + background_x, 0, 0, 0, background);

        if (GetAsyncKeyState (VK_SPACE))

        {

            if (!space_pressed)

            {

                player.vy_ = -5;

                space_pressed = true;

            }

        }

        else

        {

            space_pressed = false;

        }

        if (GetAsyncKeyState (VK_ESCAPE))

        {

            txDeleteDC (player.image1_);
            txDeleteDC (player.image2_);
            txDeleteDC (background);
            return 1;

        }

        for (int n = 0; n < WALLS_MAX; n++)

        {

            walls[n].draw ();

        }

        for (int n = 0; n < WALLS_MAX; n++)

        {

            walls[n].move ();

            if (walls[n].x_ + walls[n].width_ <= 0)

            {

                walls[n] = {n, walls_speed, wall_color};
                walls[n].x_ = wWidth;

            }

        }

        player.move ();
        player.draw ();

        int result = checkWalls (walls, player);

        switch (result)

        {

            case 1:

            {

                addScore (&score);
                break;

            }

            case 2:

            {

                txDeleteDC (player.image1_);
                txDeleteDC (player.image2_);
                txDeleteDC (background);

                int result = gameOver (score);

                if (result == 1) return 1;

                else return 0;

            }

        }

        drawScore (score);

        background_x -= 1;

        if (background_x <= -txGetExtentX (background))

        {

            background_x = 0;

        }

        txSleep (1);

    }

}

//-----------------------------------------------------------------------------

void Player::draw ()

{

    if (time_ <= 10)

    {

        txTransparentBlt (txDC (), x_, y_, 0, 0, image1_, 0, 0, alpha_);

    }

    else

    {

        txTransparentBlt (txDC (), x_, y_, 0, 0, image2_, 0, 0, alpha_);

    }

}

void Player::move ()

{

    if (y_ + vy_ > wHeight - 40) y_ = wHeight - 40;

    else if (y_ + vy_ < 0) y_ = 0;

    else y_ += vy_;

    if (vy_ < 5) vy_ += 0.2;

    time_ += 1;

    if (time_ > 20)

    {

        time_ = 0;

    }

}

void Wall::draw ()

{

    txSetColor (color_);
    txSetFillColor (color_);
    txRectangle (x_, 0, x_ + width_, hole_y);
    txRectangle (x_, hole_y + hole_height, x_ + width_, wHeight);

}

void Wall::move ()

{

    x_ -= speed_;

}

//-----------------------------------------------------------------------------

double rnd (double from, double to)

{

    return from + 1.0 * rand () / RAND_MAX * (to - from);

}

//-----------------------------------------------------------------------------

int checkWall (Wall wall, Player player)

{

    double px = player.x_, py = player.y_, px1 = px + txGetExtentX (player.image1_), py1 = py + txGetExtentY (player.image1_);

    double r1y = 0, r1y1 = wall.hole_y;

    double r2y = wall.hole_y + wall.hole_height, r2y1 = wHeight;

    double rx = wall.x_, rx1 = rx + wall.width_;

    if (px1 >= rx && px <= rx1)

    {

        if (py1 >= r1y && py <= r1y1)

        {

            return 1;

        }

        else if (py1 >= r2y && py <= r2y1)

        {

            return 1;

        }

        else

        {

            return 2;

        }

    }

    return -1;

}

//-----------------------------------------------------------------------------

int checkWalls (Wall * walls, Player player)

{

    for (int n = 0; n < WALLS_MAX; n++)

    {

        int result = checkWall (walls[n], player);

        switch (result)

        {

            case 2:

            {

                if (!walls[n].counted_)

                {

                    walls[n].counted_ = true;

                    return 1;

                }

                break;

            }

            case 1:

            {

                return 2;
                break;

            }

        }

    }

    return -1;

}

//-----------------------------------------------------------------------------

void drawScore (int score)

{

    txSetColor (TX_WHITE);
    txSelectFont ("Arial", 30);

    char text[100] = "";

    sprintf (text, "Score: %d", score);

    txTextOut (5, 5, text);

}

//-----------------------------------------------------------------------------

void addScore (int * score)

{

    * score += 1;

}

//-----------------------------------------------------------------------------

int gameOver (int score)

{

    Button buttons[] = {

        {wWidth / 2 - 125, wHeight / 2 - 25, 100, 30, "Restart", "Arial", 30, txDC (), RGB (60, 60, 60), TX_WHITE, TX_WHITE, false, 2},
        {wWidth / 2 +  25, wHeight / 2 - 25, 100, 30,    "Exit", "Arial", 30, txDC (), RGB (60, 60, 60), TX_WHITE, TX_WHITE, false, 2},
        EndButton

    };

    while (!GetAsyncKeyState (VK_ESCAPE))

    {

        txSetFillColor (RGB (45, 45, 45));
        txClear ();

        const char * text = "Game over";

        txSelectFont ("Segoe Script", 50);
        txSetColor (TX_WHITE);
        txTextOut (wWidth / 2 - txGetTextExtentX (text) / 2, wHeight / 2 - txGetTextExtentY (text) / 2 - 100, text);

        char scoreText[100] = "";

        sprintf (scoreText, "Score: %d", score);

        txSelectFont ("Arial", 30);
        txTextOut (wWidth / 2 - txGetTextExtentX (scoreText)/ 2, wHeight / 2 - txGetTextExtentY (scoreText) / 2 - 70, scoreText);

        int result = manageButtons (buttons);

        if (result != -1) return result;

        txSleep (1);

    }

    return 1;

}
