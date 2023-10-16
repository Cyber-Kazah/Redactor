

    #include "TXLib.h"
    #include "Button.cpp"
    #include "Picture.cpp"

    int main()
    {
    txCreateWindow (1200, 700);
    txDisableAutoPause();
    txTextCursor (false);
    int count_btn = 5;
    int count_pic = 9;
    int nCentralPic = 0;

    //������ �����(��������� ����� +120, ������� +200)
    Button btn[count_btn];
    btn[0] = {60, "���������", "���������"};
    btn[1] = {260, "��������������", "��������������"};
    btn[2] = {460, "�����", "�����"};
    btn[3] = {660, "������", "������"};
    btn[4] = {860, "����������", "����������"};

    //�������� ���� ������
    Picture menuPic[count_pic];
    menuPic[0] = {10, 100, txLoadImage("Pic/���������/shef.bmp"), 150, 150, 500, 500, false, "���������"};
    menuPic[1] = {10, 300, txLoadImage("Pic/���������/Stet.bmp"), 150, 150, 500, 500, false, "���������"};
    menuPic[2] = {10, 500, txLoadImage("Pic/���������/Ryan.bmp"), 150, 150, 500, 500, false, "���������"};
    menuPic[3] = {10, 100, txLoadImage("Pic/��������������/brov.bmp"), 150, 150, 500, 500, false, "��������������"};
    menuPic[4] = {10, 300, txLoadImage("Pic/��������������/usi.bmp"), 150, 150, 500, 500, false, "��������������"};
    menuPic[5] = {10, 500, txLoadImage("Pic/��������������/boroda.bmp"), 150, 150, 500, 500, false, "��������������"};
    menuPic[6] = {10, 100, txLoadImage("Pic/�����/Fur.bmp"), 150, 150, 500, 500, false, "�����"};
    menuPic[7] = {10, 300, txLoadImage("Pic/�����/cow.bmp"), 150, 150, 500, 500, false, "�����"};
    menuPic[8] = {10, 500, txLoadImage("Pic/�����/clown.bmp"), 150, 150, 500, 500, false, "�����"};


    //�������� �� ���. ����
    Picture centrPic[100];
    int vybor = -1;
    bool mouse_click = false;

    while(!GetAsyncKeyState (VK_ESCAPE))
    {
        txBegin();
        txSetColor(TX_YELLOW);
        txSetFillColor(TX_YELLOW);
        txClear();
        //������
        for(int i=0; i<count_btn; i++)
        {
            btn[i].draw();
        }
        //��������� �������� � ����
        for(int i=0; i<count_pic; i++)
        {
            menuPic[i].draw();
        }
        //��������� �������� �� ����
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

        //�������������(���� ���)
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
        //���� �� ��������
        for(int npic=0; npic<count_pic; npic++)
        {
            if(menuPic[npic].click() && menuPic[npic].visible)
            {
                centrPic[npic].visible = true;
            }


        }

        for(int i=0; i<count_pic; i++)
        {
          if(centrPic[i].click() && centrPic[i].visible)
          {
                vybor = i;
                mouse_click = false;
          }


        }

        char str[10];
        sprintf(str, "������ = %d", vybor);
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

