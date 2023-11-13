

    #include "TXLib.h"
    #include "Button.cpp"
    #include "Picture.cpp"
    #include <fstream>
    #include <stdio.h>
    #include <dirent.h>

int get_w(string adress)
{

    FILE *f1 = fopen(adress.c_str(), "rb");
    unsigned char headerinfo[54];
    fread(headerinfo, sizeof(unsigned char), 54, f1);
    int w = *(int *)&headerinfo[18];
    return w;
}

int get_h(string adress)
{

    FILE *f1 = fopen(adress.c_str(), "rb");
    unsigned char headerinfo[54];
    fread(headerinfo, sizeof(unsigned char), 54, f1);
    int h = *(int *)&headerinfo[22];
    return h;
}



    int ReadFromDir(string adress, Picture menuPic[], int count_pic)
    {
     DIR *dir;
     struct dirent *ent;
     int X = 10;
     int Y = 100;

     if ((dir = opendir (adress.c_str())) != NULL)
     {
            while ((ent = readdir (dir)) != NULL)
            {
                if((string)ent->d_name != "." && (string)ent->d_name != "..")
                {
                    if(Y<=500 && X == 20)
                    {
                        X = 20;
                    }
                    else if (Y>600)
                    {
                        X = 120;
                        Y = 100;
                    }

                    menuPic[count_pic].y = Y;
                    menuPic[count_pic].x = X;

                    menuPic[count_pic].adress = adress + (string)ent->d_name;
                    count_pic++;
                    Y += 200;
                }
            }
            closedir (dir);

     }



      return count_pic;
    }

    int main()
    {
    txCreateWindow (1200, 700);
    txDisableAutoPause();
    txTextCursor (false);
    int count_btn = 6;
    int count_pic = 0;
    int nCentralPic = 0;

    //кнопки масив(маленькие слова +120, большие +200)
    Button btn[count_btn];
    btn[0] = {60, "персонажи", "персонажи"};
    btn[1] = {260, "растительность", "растительность"};
    btn[2] = {460, "шл€пы", "шл€пы"};
    btn[3] = {660, "оптика", "оптика"};
    btn[4] = {860, "аксессуары", "аксессуары"};
    btn[5] = {860, "друзь€", "друзь€"};

    //картинки меню выбора(масив)
    Picture menuPic[100];


    count_pic = ReadFromDir("Pic/персонажи/", menuPic, count_pic);
    count_pic = ReadFromDir("Pic/растительность/", menuPic, count_pic);
    count_pic = ReadFromDir("Pic/шл€пы/", menuPic, count_pic);
    count_pic = ReadFromDir("Pic/оптика/", menuPic, count_pic);
    count_pic = ReadFromDir("Pic/аксессуары/", menuPic, count_pic);

    for(int i=0; i<count_pic; i++)
    {

        menuPic[i].pic = txLoadImage(menuPic[i].adress.c_str());

        menuPic[i].w = get_w(menuPic[i].adress);
        menuPic[i].h = get_h(menuPic[i].adress);

        menuPic[i].w_scr = menuPic[i].w/5;
        menuPic[i].h_scr = menuPic[i].w/5;

        menuPic[i].visible = false;
        string str = menuPic[i].adress;
        int pos1 = str.find("/");
        int pos2 = str.find("/", pos1+1);
        menuPic[i].category = str.substr(pos1+1, pos2-pos1-1);

    }

    //картинки на физ. поле
    Picture centrPic[100];
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
        for(int i=0; i<nCentralPic; i++)
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

        for(int npic=0; npic<count_pic; npic++)
        {
            if(menuPic[npic].click() && menuPic[npic].visible)
            {
                 while(txMouseButtons() == 1)
                 {
                    txSleep(10);
                 }
                 centrPic[nCentralPic] = {  500,
                                            100,
                                            menuPic[npic].adress,
                                            menuPic[npic].pic,
                                            menuPic[npic].w,
                                            menuPic[npic].h,
                                            menuPic[npic].w,
                                            menuPic[npic].h,
                                            menuPic[npic].visible,
                                            menuPic[npic].category};
                 nCentralPic ++;
            }


         }

        for(int i=0; i<nCentralPic; i++)
        {
              if(centrPic[i].click() && centrPic[i].visible)
              {
                    vybor = i;
                    mouse_click = false;
              }
        }

        if(vybor>=0 && GetAsyncKeyState (VK_DELETE))
        {
                centrPic[vybor] = centrPic[nCentralPic-1];
                nCentralPic--;
                vybor = -1;
                mouse_click = true;
        }



        char str[10];
        sprintf(str, "Ќомер картинки = %d  оличество = %d", vybor, count_pic/*nCentralPic*/);
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

        if(vybor>=0)
        {
            if(txMouseButtons() == 1 && !mouse_click)
            {
                centrPic[vybor].x = txMouseX() - centrPic[vybor].w_scr/2;
                centrPic[vybor].y = txMouseY() - centrPic[vybor].h_scr/2;
            }
            else
            {
                if(txMouseButtons() != 1)
                {
                    mouse_click = true;
                }
            }

        }



//Win32::TransparentBlt (txDC(), 350, 200, 500, 500, pictures, 0, 0, 500, 500, TX_WHITE);
        txEnd();
        txSleep(50);
    }

    for(int i=0; i<nCentralPic; i++)
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

