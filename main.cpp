

   #include "TXLib.h"


struct Button
{
    int x;
    const char* name;
    string category;

    void draw()
    {
        txSetColor(TX_TRANSPARENT);
        txSetFillColor(TX_GRAY);
        Win32::RoundRect (txDC(), x+5, 35, x+145, 75, 30, 30);
        txSetColor(TX_BLACK, 2);
        txSetFillColor(TX_WHITE);
        Win32::RoundRect (txDC(), x, 30, x+140, 70, 30, 30);
        txSetColor(TX_BLACK);
        txSelectFont("Times New Roman", 24);
        txDrawText (x, 30, x+140, 70, name);
    }
    bool click()
    {
        return( txMouseButtons() == 1 &&
        txMouseX() >= x && txMouseX() <= x+140 &&
        txMouseY() >= 35 && txMouseY() <= 75);
    }


};

struct Picture
{
     int x;
     int y;
     HDC pic;
     int w_scr;
     int h_scr;
     int w;
     int h;
     bool visible;
     string category;

    void draw()
    {
       if(visible)
        {
            Win32::TransparentBlt (txDC(), x, y, w_scr, h_scr, pic, 0, 0, w, h, TX_WHITE);
        }
    }

    bool click()
    {
        return( txMouseButtons() == 1 &&
        txMouseX() >= 10 && txMouseX() <= 150 &&
        txMouseY() >= y && txMouseY() <= y+150);
    }



};



    int main()
    {
    txCreateWindow (1200, 700);
    txDisableAutoPause();
    txTextCursor (false);
    int count_btn = 4;
    int count_pic = 4;

    //кнопки масив
    Button btn[count_btn];
    btn[0] = {60, "персонажи", "персонажи"};
    btn[1] = {250, "растительность", "растительность"};
    btn[2] = {440, "шл€пы", "шл€пы"};
    btn[3] = {620, "оптика", "оптика"};

    //картинки меню выбора
    Picture menuPic[count_pic];
    menuPic[0] = {10, 100, txLoadImage("Pic/персонажи/shef.bmp"), 150, 150, 500, 500, false, "персонажи"};
    menuPic[1] = {10, 300, txLoadImage("Pic/персонажи/Stet.bmp"), 150, 150, 500, 500, false, "персонажи"};
    menuPic[2] = {10, 100, txLoadImage("Pic/–астительность/brov.bmp"), 150, 150, 500, 500, false, "–астительность"};
    menuPic[3] = {10, 300, txLoadImage("Pic/–астительность/usi.bmp"), 150, 150, 500, 500, false, "–астительность"};


    //картинки на физ. поле
    Picture centrPic[count_pic];
    centrPic[0] = {350, 100, menuPic[0].pic, 500, 500, menuPic[0].w, menuPic[0].h, false, "персонажи"};
    centrPic[1] = {350, 100, menuPic[1].pic, 500, 500, menuPic[1].w, menuPic[1].h, false, "персонажи"};
    centrPic[2] = {350, 100, menuPic[2].pic, 500, 500, menuPic[1].w, menuPic[1].h, false, "–астительность"};
    centrPic[3] = {350, 100, menuPic[3].pic, 500, 500, menuPic[1].w, menuPic[1].h, false, "–астительность"};

    while(!GetAsyncKeyState (VK_ESCAPE))
    {
        txBegin();
        txSetColor(TX_YELLOW);
        txSetFillColor(TX_YELLOW);
        txClear();
        //кнопки
        for(int i=0; i<4; i++)
        {
            btn[i].draw();
        }
        //рисование картинок в меню
        for(int i=0; i<count_pic; i++)
        {
            menuPic[i].draw();
        }
        //рисование картинок на поле
        for(int i=0; i<count_pic; i++)
        {
            centrPic[i].draw();
        }

        for(int ib=0; ib<count_btn; ib++)
        {
            if(btn[ib].click())
            {
                for(int ip=0; ip<count_pic; ip++)
                {
                    menuPic[ip].visible = false;
                    if(menuPic[ip].category == btn[ib].category)
                    {
                        menuPic[ip].visible = true;
                    }
                }
            }
        }

        if(btn[0].click())
        {
            menuPic[0].visible = true;
            menuPic[1].visible = true;
        }

        if(btn[1].click())
        {
            menuPic[2].visible = true;
        }

        if(menuPic[0].click())
        {
           centrPic[0].visible = true;
        }

        if(menuPic[1].click())
        {
           centrPic[1].visible = true;
        }

        if(menuPic[2].click())
        {
           centrPic[2].visible = true;
        }
        if(menuPic[3].click())
        {
           centrPic[3].visible = true;
        }

//Win32::TransparentBlt (txDC(), 350, 200, 500, 500, pictures, 0, 0, 500, 500, TX_WHITE);
        txEnd();
        txSleep(50);
    }

    for(int i=0; i<count_pic; i++)
 {
        txDeleteDC (centrPic[i].pic);
 }

    for(int i=0; i<count_pic; i++)
 {
        txDeleteDC (menuPic[i].pic);
 }


txTextCursor (false);
return 0;
}

