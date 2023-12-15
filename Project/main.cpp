
    #include "TXLib.h"
    #include "Button.cpp"
    #include "Picture.cpp"
    #include <fstream>
    #include <stdio.h>
    #include <dirent.h>

    using namespace std;

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

    string runFiledialog(bool isSave)
    {

            string Filename = "";
            OPENFILENAME ofn;
            TCHAR szFile[260] = {0};       //������� ���������

            // Initialize OPENFILENAME
            ZeroMemory(&ofn, sizeof(ofn));
            ofn.lStructSize = sizeof(ofn);
            ofn.hwndOwner = txWindow();
            ofn.lpstrFile = szFile;
            ofn.nMaxFile = sizeof(szFile);
            ofn.lpstrFilter = ("Text\0*.TXT\0");
            ofn.nFilterIndex = 1;
            ofn.lpstrFileTitle = NULL;
            ofn.nMaxFileTitle = 0;
            ofn.lpstrInitialDir = NULL;
            ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

            // Display the Open dialog box.
            if(isSave)
            {
                if (GetSaveFileName(&ofn)==TRUE)
                {
                    Filename = ofn.lpstrFile;
                    Filename = Filename + ".txt";
                }
            }
            else
            {
                if (GetOpenFileName(&ofn)==TRUE)
                {
                    Filename = ofn.lpstrFile;
                }
            }



        return Filename;
    }

    inline int GetFilePointer(HANDLE FileHandle)
    {
    return SetFilePointer(FileHandle, 0, 0, FILE_CURRENT);
}

    bool SaveBMPFile(char *filename, HBITMAP bitmap, HDC bitmapDC, int width, int height)
    {
        bool Success=0;
        HBITMAP OffscrBmp=NULL;
        HDC OffscrDC=NULL;
        LPBITMAPINFO lpbi=NULL;
        LPVOID lpvBits=NULL;
        HANDLE BmpFile=INVALID_HANDLE_VALUE;
        BITMAPFILEHEADER bmfh;
        if ((OffscrBmp = CreateCompatibleBitmap(bitmapDC, width, height)) == NULL)
            return 0;
        if ((OffscrDC = CreateCompatibleDC(bitmapDC)) == NULL)
            return 0;
        HBITMAP OldBmp = (HBITMAP)SelectObject(OffscrDC, OffscrBmp);
        BitBlt(OffscrDC, 0, 0, width, height, bitmapDC, 0, 0, SRCCOPY);
        if ((lpbi = (LPBITMAPINFO)(new char[sizeof(BITMAPINFOHEADER) + 256 * sizeof(RGBQUAD)])) == NULL)
            return 0;
        ZeroMemory(&lpbi->bmiHeader, sizeof(BITMAPINFOHEADER));
        lpbi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        SelectObject(OffscrDC, OldBmp);
        if (!GetDIBits(OffscrDC, OffscrBmp, 0, height, NULL, lpbi, DIB_RGB_COLORS))
            return 0;
        if ((lpvBits = new char[lpbi->bmiHeader.biSizeImage]) == NULL)
            return 0;
        if (!GetDIBits(OffscrDC, OffscrBmp, 0, height, lpvBits, lpbi, DIB_RGB_COLORS))
            return 0;
        if ((BmpFile = CreateFile(filename,
                            GENERIC_WRITE,
                            0, NULL,
                            CREATE_ALWAYS,
                            FILE_ATTRIBUTE_NORMAL,
                            NULL)) == INVALID_HANDLE_VALUE)
            return 0;
        DWORD Written;
        bmfh.bfType = 19778;
        bmfh.bfReserved1 = bmfh.bfReserved2 = 0;
        if (!WriteFile(BmpFile, &bmfh, sizeof(bmfh), &Written, NULL))
            return 0;
        if (Written < sizeof(bmfh))
            return 0;
        if (!WriteFile(BmpFile, &lpbi->bmiHeader, sizeof(BITMAPINFOHEADER), &Written, NULL))
            return 0;
        if (Written < sizeof(BITMAPINFOHEADER))
            return 0;
        int PalEntries;
        if (lpbi->bmiHeader.biCompression == BI_BITFIELDS)
            PalEntries = 3;
        else PalEntries = (lpbi->bmiHeader.biBitCount <= 8) ?
                          (int)(1 << lpbi->bmiHeader.biBitCount) : 0;
        if(lpbi->bmiHeader.biClrUsed)
        PalEntries = lpbi->bmiHeader.biClrUsed;
        if(PalEntries){
        if (!WriteFile(BmpFile, &lpbi->bmiColors, PalEntries * sizeof(RGBQUAD), &Written, NULL))
            return 0;
            if (Written < PalEntries * sizeof(RGBQUAD))
                return 0;
        }
        bmfh.bfOffBits = GetFilePointer(BmpFile);
        if (!WriteFile(BmpFile, lpvBits, lpbi->bmiHeader.biSizeImage, &Written, NULL))
            return 0;
        if (Written < lpbi->bmiHeader.biSizeImage)
            return 0;
        bmfh.bfSize = GetFilePointer(BmpFile);
        SetFilePointer(BmpFile, 0, 0, FILE_BEGIN);
        if (!WriteFile(BmpFile, &bmfh, sizeof(bmfh), &Written, NULL))
            return 0;
        if (Written < sizeof(bmfh))
            return 0;

        CloseHandle (BmpFile);

        delete [] (char*)lpvBits;
        delete [] lpbi;

        DeleteDC (OffscrDC);
        DeleteObject (OffscrBmp);


        return 1;
    }

    bool ScreenCapture(int x, int y, int width, int height, char *filename, HWND hwnd)
    {
    HDC hDC = GetDC(hwnd);
    HDC hDc = CreateCompatibleDC(hDC);

    HBITMAP hBmp = CreateCompatibleBitmap(hDC, width, height);

    HGDIOBJ old= SelectObject(hDc, hBmp);
    BitBlt(hDc, 0, 0, width, height, hDC, x, y, SRCCOPY);

    bool ret = SaveBMPFile(filename, hBmp, hDc, width, height);

    SelectObject(hDc, old);

    DeleteObject(hBmp);

    DeleteDC (hDc);
    ReleaseDC (hwnd, hDC);

    return ret;
}



    //���. ������
    const int count_btn = 12;
    //��������� ���������
    const int btn_prntsc = count_btn-6;
    //��������� � ���������
    const int btn_save = count_btn-5;
    const int btn_load = count_btn-4;
    //�������
    const int btn_help = count_btn-3;
    //�������
    const int btn_exit = count_btn-2;
    //�������
    const int btn_menu = count_btn-1;
    //����
    const int page_menu = 0;
    //��������
    const int page_redactor = 1;
    //�������
    const int page_help = 2;



    int main()
    {
    txCreateWindow (1300, 700);
    txDisableAutoPause();
    txTextCursor (false);
    int count_pic = 0;
    int nCentralPic = 0;

    int page = page_menu;

    //������ �����(��������� ����� +120, ������� +200)
    Button btn[count_btn];
    btn[0] = {60, 30, "���������", "���������"};
    btn[1] = {260, 30, "��������������", "��������������"};
    btn[2] = {460, 30,  "�����", "�����"};
    btn[3] = {660, 30,  "������", "������"};
    btn[4] = {860, 30,  "����������", "����������"};
    btn[5] = {1060, 30,  "������", "������"};
    btn[6] = {760, 650,  "������ ������", ""};
    btn[7] = {60, 650,  "���������", ""};
    btn[8] = {210, 650,  "���������", ""};
    btn[9] = {360, 650,  "�������", ""};
    btn[10] = {960, 650,  "�����", ""};
    btn[11] = {560, 270,  "�����", ""};

    //�������� ���� ������(�����)
    Picture menuPic[100];


    count_pic = ReadFromDir("Pic/���������/", menuPic, count_pic);
    count_pic = ReadFromDir("Pic/��������������/", menuPic, count_pic);
    count_pic = ReadFromDir("Pic/�����/", menuPic, count_pic);
    count_pic = ReadFromDir("Pic/������/", menuPic, count_pic);
    count_pic = ReadFromDir("Pic/����������/", menuPic, count_pic);
    count_pic = ReadFromDir("Pic/������/", menuPic, count_pic);

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

    //�������� �� ���. ����
    Picture centrPic[100];
    int vybor = -1;
    bool mouse_click = false;

    while(!btn[btn_exit].click())
    {
        txBegin();
        txSetColor(TX_WHITE);
        txSetFillColor(TX_WHITE);
        txClear();

        //����
        if(page == page_menu)
        {
            btn[btn_menu].draw();
            if(btn[btn_menu].click())
            {
                page = page_redactor;
                txSleep(100);
            }
        }


        //��������
        if(page == page_redactor)
        {
            txSetColor(TX_BLACK);
            txSetFillColor(TX_WHITE);
            txRectangle(250, 100, 950, 600);

            //������
            for(int i=0; i<count_btn-1; i++)
            {
                btn[i].draw();
            }
            //��������� �������� � ����
            for(int i=0; i<count_pic; i++)
            {
                menuPic[i].draw();
            }
            //��������� �������� �� ����
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
            //������������
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
            //����������
            if(btn[btn_save].click())
            {
                string FileName = runFiledialog(true);
                ofstream fileout;
                fileout.open(FileName);
                if (fileout.is_open())
                {
                    for(int i=0; i<nCentralPic; i++)
                    {
                        if(centrPic[i].visible)
                        {
                            fileout << centrPic[i].x << endl;
                            fileout << centrPic[i].y << endl;
                            fileout << centrPic[i].adress << endl;
                            fileout << centrPic[i].w_scr << endl;
                            fileout << centrPic[i].h_scr << endl;
                        }
                    }
                }
                fileout.close();
            }

            //��������
            if(btn[btn_load].click())
            {
                string FileName = runFiledialog(false);
                for(int i = 0; i<count_pic; i++)
                {
                    centrPic[i].visible = false;
                }
                char buff[50];
                ifstream filein(FileName); // �������� ���� ��� ������
                while (filein.good())
                {
                    filein.getline(buff, 50);
                    int x = atoi(buff);

                    filein.getline(buff, 50);
                    int y = atoi(buff);

                    filein.getline(buff, 50);
                    string adress = buff;

                    filein.getline(buff, 50);
                    int w_scr = atoi(buff);

                    filein.getline(buff, 50);
                    int h_scr = atoi(buff);

                    for(int i = 0; i<count_pic; i++)
                    {
                        if(menuPic[i].adress == adress)
                        {
                          centrPic[nCentralPic] = { x,
                                                    y,
                                                    menuPic[i].adress,
                                                    menuPic[i].pic,
                                                    w_scr,
                                                    h_scr,
                                                    menuPic[i].w,
                                                    menuPic[i].h,
                                                    true,
                                                    menuPic[i].category};
                          nCentralPic ++;
                        }

                    }



                }
                filein.close();
            }

            if(btn[btn_help].click())
            {
                page = page_help;
                btn[btn_help].name = "���������";
                txSleep(100);
            }




        }

        //�������
        else if(page == page_help)
        {
            txSetColor(TX_BLACK);
            txSelectFont("Times New Roman", 42);
            txDrawText(0, 0, 1200, 700,
                         "��������� �������� \n"
                         "� ������ ������ ��������������� ��������� ������������ ����, ������� �� ������ ������� � ������������� ��������\n"
                         "���������� ���������� ����� ���������: �. ���������, �. ������. \n"
                         "��������� ����� ��� ��������, ��� � ��������� �� ������ + � - ��������������\n"
                         "���������� � �������� ����� ��������� ���������� �� ��������� ������� � ������ ����� ���� \n"
                         "�� ������ ������� ������ ����� ���������, �������� �� � ���� � ����� ���������� ������. \n"
                         "����� �� ESC. \n");

            btn[btn_help].draw();
            if(btn[btn_help].click())
            {
                page = page_redactor;
                btn[btn_help].name = "�������";
                txSleep(100);
            }


        }

        if(btn[btn_prntsc].click())
        {
            ScreenCapture(250, 100, 700, 500, "1.bmp", txWindow());
            txMessageBox("��������� � 1.bmp");
        }



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

