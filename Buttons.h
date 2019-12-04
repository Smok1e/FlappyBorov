#include <TXLib.h>

//--Simple button--

struct Button

{

    int x_;
    int y_;
    int width_;
    int height_;

    const char * text_;
    const char * font_;
    int fontsize_;

    HDC dc_;

    COLORREF color_;
    COLORREF textcolor_;
    COLORREF strokecolor_;

    bool stroke_;

    int stroke_r_;

    void draw () const;

    bool check ();

    bool manage ();

    bool isEnd () const;

};

//-----------------------------------------------------------------------------

char text[10] = "text";

Button EndButton = {-1, -1, -1, -1, text, text};

//-----------------------------------------------------------------------------

void Button::draw () const

{

    if (stroke_)

    {

        txSetColor (strokecolor_, 1, dc_);
        txSetFillColor (strokecolor_, dc_);

        txRectangle (x_, y_, x_ + width_, y_ + height_, dc_);

        txSetColor (color_, 1, dc_);
        txSetFillColor (color_, dc_);

        txRectangle (x_ + stroke_r_ / 2, y_ + stroke_r_ / 2, x_ + width_ - stroke_r_ / 2, y_ + height_ - stroke_r_ / 2, dc_);

    }

    else

    {

        txSetColor (color_, 1, dc_);
        txSetFillColor (color_, dc_);

        txRectangle (x_, y_, x_ + width_, y_ + height_, dc_);

    }

    txSetColor (textcolor_, stroke_r_, dc_);

    txSelectFont (font_, fontsize_, -1, FW_DONTCARE, false, false, false, 0, dc_);
    txDrawText (x_, y_, x_ + width_, y_ + height_, text_, DT_CENTER, dc_);

}

//-----------------------------------------------------------------------------

bool Button::check ()

{

    POINT mPos = txMousePos ();

    if (mPos.x >= x_ && mPos.x < x_ + width_ && mPos.y >= y_ && mPos.y < y_ + height_)

    {

        stroke_ = true;

        return true;

    }

    stroke_ = false;

    return false;

}

//-----------------------------------------------------------------------------

bool Button::manage ()

{

    if (check ())

    {

        stroke_ = true;

    }

    else

    {

        stroke_ = false;

    }

    draw ();

    if (check () && txMouseButtons() == 1)

    {

        return true;

    }

    return false;

}

//-----------------------------------------------------------------------------

bool Button::isEnd () const

{

    if (x_ == -1 && y_ == -1 && width_ == -1 && height_ == -1)

    {

        return true;

    }

    return false;

}

//--Multiple buttons--

void drawButtons (Button buttons_[])

{

    for (int n = 0; !buttons_[n].isEnd (); n++)

    {

        buttons_[n].draw();

    }

}

//-----------------------------------------------------------------------------

int checkButtons (Button buttons_[])

{

    for (int n = 0; !buttons_[n].isEnd (); n++)

    {

        if (buttons_[n].check() && txMouseButtons () == 1)

        {

            return n;

        }

    }

    return -1;

}

//-----------------------------------------------------------------------------

int manageButtons (Button buttons_[])

{

    for (int n = 0; !buttons_[n].isEnd (); n++)

    {

        if (buttons_[n].check ())

        {

            buttons_[n].stroke_ = true;

        }

        else

        {

            buttons_[n].stroke_ = false;

        }

        buttons_[n].draw ();

        if (buttons_[n].check () && txMouseButtons() == 1)

        {

            return n;

        }

    }

    return -1;

}
