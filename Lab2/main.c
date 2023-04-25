#include <windows.h>
#include <gl/gl.h>
#include <string.h>
#include "menu.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb-master/stb_image.h"
#include "texture.h"

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
void EnableOpenGL(HWND hwnd, HDC*, HGLRC*);
void DisableOpenGL(HWND, HDC, HGLRC);

int textureId;
char anim=0;
float TypeOfAnim=0;
float tick=0;

void InitMain()
{
    anim=0;
    Menu_Clear();
    Menu_AddButton("Begin",10,10,100,30,2);
    Menu_AddButton("Animations",130,10,120,30,2);
    Menu_AddButton("Quit",270,10,100,30,2);
}

void InitTest()
{
    tick=0;
    TypeOfAnim=0;
    anim =1;
    Menu_Clear();
    Menu_AddButton("Animation 1",10,10,130,30,2);
    Menu_AddButton("Animation 2",160,10,130,30,2);
    Menu_AddButton("Animation 3",310,10,130,30,2);
    Menu_AddButton("Back",460,10,100,30,2);
}

void MouseDown()
{
    int buttonId = Menu_MouseDown();
    if (buttonId<0) return;
    char *name = Menu_GetButtonName(buttonId);
    if (strcmp(name,"Quit")==0) PostQuitMessage(0);
    if (strcmp(name,"Animations")==0) InitTest();
    if (strcmp(name,"Back")==0) InitMain();
    if (strcmp(name,"Begin")==0) printf("%s\n",name);
    if (strcmp(name,"Animation 1")==0)
    {
        TypeOfAnim=0;
        tick=0;
    }
    if (strcmp(name,"Animation 2")==0)
    {
        TypeOfAnim=1;
        tick=0;
    }
    if (strcmp(name,"Animation 3")==0)
    {
        TypeOfAnim=2;
        tick=0;
    }
}

float TextureVertex[] = {40,100,200,100,200,340,40,340};
float TextureCord[] = {0,0,1,0,1,1,0,1};

void ShowTexture(float NumberOfSprite, float TypeOfAnim)
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,textureId);

    glColor3f(1,1,1);
    glPushMatrix();

glEnable(GL_ALPHA_TEST); // проверка на элементы α-канала
 //(не обязательно)
glAlphaFunc(GL_GREATER, 0.99); // задается тип уровня и его
 //числовая граница
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        static float spriteXsize=960; //переменные с размерами текстуры и отдельного кадра
        static float spriteYsize=360;
        static float charsizey=120;
        static float charsizex=120;
        float left=(charsizex*NumberOfSprite)/spriteXsize; //вычисление координат кадра на изображении от
        float right=left+(charsizex/spriteXsize); //номера кадра
        float top=(charsizey*TypeOfAnim)/spriteYsize;
        float bottom=top+(charsizey/spriteYsize);

        TextureCord[5]=TextureCord[7]=bottom;
        TextureCord[1]=TextureCord[3]=top;
        TextureCord[0]=TextureCord[6]=left;
        TextureCord[2]=TextureCord[4]=right;
        glVertexPointer(2,GL_FLOAT,0,TextureVertex);
        glTexCoordPointer(2,GL_FLOAT,0,TextureCord);
        glDrawArrays(GL_TRIANGLE_FAN,0,4);
        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        glDisable(GL_ALPHA_TEST);
    glPopMatrix();
}


int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
{
    WNDCLASSEX wcex;
    HWND hwnd;
    HDC hDC;
    HGLRC hRC;
    MSG msg;
    BOOL bQuit = FALSE;
    float theta = 0.0f;

    /* register window class */
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_OWNDC;
    wcex.lpfnWndProc = WindowProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = "GLSample";
    wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);;


    if (!RegisterClassEx(&wcex))
        return 0;

    /* create main window */
    hwnd = CreateWindowEx(0,
                          "GLSample",
                          "OpenGL Sample",
                          WS_OVERLAPPEDWINDOW,
                          CW_USEDEFAULT,
                          CW_USEDEFAULT,
                          800,
                          512,
                          NULL,
                          NULL,
                          hInstance,
                          NULL);

    ShowWindow(hwnd, nCmdShow);

    /* enable OpenGL for the window */
    EnableOpenGL(hwnd, &hDC, &hRC);

    Texture_LoadTexture("spriteList.png",&textureId,GL_REPEAT,GL_NEAREST, NULL);

    RECT rct;
    GetClientRect(hwnd, &rct);
    glOrtho(0, rct.right, rct.bottom, 0, 1,-1);

    InitMain();

    /* program main loop */
    while (!bQuit)
    {
        /* check for messages */
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            /* handle or dispatch messages */
            if (msg.message == WM_QUIT)
            {
                bQuit = TRUE;
            }
            else
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else
        {
            /* OpenGL animation code goes here */

            glClearColor(0.2f, 0.2f, 0.2f, 0.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            if (anim)
                ShowTexture(tick,TypeOfAnim);

            Menu_ShowMenu();

            SwapBuffers(hDC);
            tick++;
            if (tick>7)    tick=0;
            Sleep (100);
        }
    }

    /* shutdown OpenGL */
    DisableOpenGL(hwnd, hDC, hRC);

    /* destroy the window explicitly */
    DestroyWindow(hwnd);

    return msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_CLOSE:
            PostQuitMessage(0);
        break;

        case WM_MOUSEMOVE:
            Menu_MouseMove(LOWORD(lParam),HIWORD(lParam));
        break;

        case WM_LBUTTONDOWN:
            MouseDown();
        break;

        case WM_LBUTTONUP:
            Menu_MouseUp();
        break;

        case WM_DESTROY:
            return 0;

        case WM_KEYDOWN:
        {
            switch (wParam)
            {
                case VK_ESCAPE:
                    PostQuitMessage(0);
                break;
            }
        }
        break;

        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    return 0;
}

void EnableOpenGL(HWND hwnd, HDC* hDC, HGLRC* hRC)
{
    PIXELFORMATDESCRIPTOR pfd;

    int iFormat;

    /* get the device context (DC) */
    *hDC = GetDC(hwnd);

    /* set the pixel format for the DC */
    ZeroMemory(&pfd, sizeof(pfd));

    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW |
                  PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cDepthBits = 16;
    pfd.iLayerType = PFD_MAIN_PLANE;

    iFormat = ChoosePixelFormat(*hDC, &pfd);

    SetPixelFormat(*hDC, iFormat, &pfd);

    /* create and enable the render context (RC) */
    *hRC = wglCreateContext(*hDC);

    wglMakeCurrent(*hDC, *hRC);
}

void DisableOpenGL (HWND hwnd, HDC hDC, HGLRC hRC)
{
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(hRC);
    ReleaseDC(hwnd, hDC);
}
