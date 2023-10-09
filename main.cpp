

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
    int count_btn = 5;
    int count_pic = 5;

    //кнопки масив
    Button btn[count_btn];
    btn[0] = {60, "персонажи", "персонажи"};
    btn[1] = {250, "растительность", "растительность"};
    btn[2] = {440, "шляпы", "шляпы"};
    btn[3] = {630, "оптика", "оптика"};
    btn[4] = {800, "аксессуары", "аксессуары"};

    //картинки меню выбора
    Picture menuPic[count_pic];
    menuPic[0] = {10, 100, txLoadImage("Pic/персонажи/shef.bmp"), 150, 150, 500, 500, false, "персонажи"};
    menuPic[1] = {10, 300, txLoadImage("Pic/персонажи/Stet.bmp"), 150, 150, 500, 500, false, "персонажи"};
    menuPic[2] = {10, 500, txLoadImage("Pic/персонажи/Ryan.bmp"), 150, 150, 500, 500, false, "персонажи"};
    menuPic[3] = {10, 100, txLoadImage("Pic/Растительность/brov.bmp"), 150, 150, 500, 500, false, "растительность"};
    menuPic[4] = {10, 300, txLoadImage("Pic/Растительность/usi.bmp"), 150, 150, 500, 500, false, "растительность"};



    //картинки на физ. поле
    Picture centrPic[count_pic];
    centrPic[0] = {350, 100, menuPic[0].pic, 500, 500, menuPic[0].w, menuPic[0].h, false, "персонажи"};
    centrPic[1] = {350, 100, menuPic[1].pic, 500, 500, menuPic[1].w, menuPic[1].h, false, "персонажи"};
    centrPic[2] = {350, 100, menuPic[2].pic, 500, 500, menuPic[1].w, menuPic[1].h, false, "персонажи"};
    centrPic[3] = {350, 100, menuPic[3].pic, 500, 500, menuPic[1].w, menuPic[1].h, false, "растительность"};
    centrPic[4] = {350, 100, menuPic[4].pic, 500, 500, menuPic[1].w, menuPic[1].h, false, "растительность"};


    int vybor = -1;
    bool mouse_click = false;

    while(!GetAsyncKeyState (VK_ESCAPE))
    {
        txBegin();
        txSetColor(TX_YELLOW);
        txSetFillColor(TX_YELLOW);
        txClear();
        //кнопки
        for(int i=0; i<count_btn; i++)
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

        //ликвидировать
        for(int npic=0; npic<count_pic; npic++)
        {
            if(menuPic[npic].click() && menuPic[npic].visible)
            {
                for(int npic1=0; npic1<count_pic; npic1++)
                {
                    if(centrPic[npic1].category == centrPic[npic].category)
                    {
                        centrPic[npic1].visible = false;
                    }

                }
                centrPic[npic].visible = !centrPic[npic].visible;
                txSleep(10);
            }
         }
        //клик по картинке
        for(int npic=0; npic<count_pic; npic++)
        {
            if(menuPic[npic].click() && menuPic[npic].visible)
            {
                centrPic[npic].visible = true;
            }


        }

        for(int i=0; i<count_pic; i++)
        {
          if(txMouseButtons() == 1 &&
          txMouseX() >= centrPic[i].x &&
          txMouseX()<= centrPic[i].x + centrPic[i].w_scr &&
          txMouseY() >= centrPic[i].y &&
          txMouseY()<= centrPic[i].y + centrPic[i].h_scr &&
          centrPic[i].visible)
          {
            vybor = i;
            mouse_click = true;
          }


        }

        char str[10];
        sprintf(str, "Индекс = %d", vybor);
        txTextOut(10, 650, str);

        if(vybor>=0)
        {
        if(GetAsyncKeyState (VK_LEFT))
         {
            centrPic[vybor].x -= 5;
         }
        if(GetAsyncKeyState (VK_RIGHT))
         {
            centrPic[vybor].x += 5;
         }
        if(GetAsyncKeyState (VK_DOWN))
         {
            centrPic[vybor].y += 5;
         }
        if(GetAsyncKeyState (VK_UP))
         {
            centrPic[vybor].y -= 5;
         }
        if(GetAsyncKeyState (VK_OEM_PLUS) || GetAsyncKeyState (VK_ADD))
         {
            centrPic[vybor].w_scr = centrPic[vybor].w_scr * 1.1;
            centrPic[vybor].h_scr = centrPic[vybor].h_scr * 1.1;
         }
        if(GetAsyncKeyState (VK_OEM_MINUS) || GetAsyncKeyState (VK_SUBTRACT))
         {
            centrPic[vybor].w_scr = centrPic[vybor].w_scr * 0.9;
            centrPic[vybor].h_scr = centrPic[vybor].h_scr * 0.9;
         }



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

