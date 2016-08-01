/**
 * @author Robin Duda 19931019
 * @date 2016-07-22
 *
 * Application window setup.
 */


#include <iostream>

using namespace std;

#include <Windows.h>
#include "Game.h"

const char *TITLE = "THE LABYRINTH GAME";
const int WIDTH = 332;
const int HEIGHT = 524;
const int ID_QUIT = 1;
const int ID_START = 6;
const int ID_BUTTON_EAST = 2;
const int ID_BUTTON_NORTH = 3;
const int ID_BUTTON_SOUTH = 4;
const int ID_BUTTON_WEST = 5;

const char *PLAYER_CHARACTER = {"[]"};
Game *game = new Game();

/**
 * Handles messages related to the menu and the buttons.
 */
void HandleWindowMessage(HWND hwnd, WPARAM wParam) {
    switch (LOWORD(wParam)) {
        case ID_BUTTON_NORTH:
            game->MovePlayer(NORTH);
            break;
        case ID_BUTTON_WEST:
            game->MovePlayer(WEST);
            break;
        case ID_BUTTON_SOUTH:
            game->MovePlayer(SOUTH);
            break;
        case ID_BUTTON_EAST:
            game->MovePlayer(EAST);
            break;
        case ID_START:
            game->Reset();
            break;
        case ID_QUIT:
            PostQuitMessage(0);
            break;
    }

    if (game->IsFinished()) {
        MessageBox(hwnd, game->GetPlayerSteps(), "Finished!", MB_OK);
        game->Reset();
    }
    InvalidateRect(hwnd, 0, TRUE);
}

/**
 * Handles paint messages by redrawing the map, player and info.
 */
void HandlePaintMessage(HWND hwnd) {
    PAINTSTRUCT ps = {0};
    HDC hdc = BeginPaint(hwnd, &ps);

    for (int i = 0; i < MAP_SIZE; i++) {
        for (int k = 0; k < MAP_SIZE; k++) {
            TextOut(hdc, i * 16, k * 16, game->GetTile(i, k), 1);
        }
    }

    TextOut(hdc, game->GetPlayerX() * 16, game->GetPlayerY() * 16, PLAYER_CHARACTER, strlen(PLAYER_CHARACTER));
    TextOut(hdc, 24, HEIGHT - 175, game->GetPlayerCoords(), strlen(game->GetPlayerCoords()));
    TextOut(hdc, 24, HEIGHT - 150, game->GetPlayerSteps(), strlen(game->GetPlayerSteps()));

    EndPaint(hwnd, &ps);
}

/**
 * Forwards window messages to handlers.
 */
LRESULT CALLBACK WinProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_PAINT:
            HandlePaintMessage(hwnd);
            break;
        case WM_CLOSE:
            PostQuitMessage(0);
            return 0;
        case WM_COMMAND:
            HandleWindowMessage(hwnd, wParam);
            break;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

/**
 * Pops messages from the application event queue.
 */
void HandleEventLoop() {
    MSG msg = {0};
    while (GetMessage(&msg, 0, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

/**
 * Centers the application window using form with and screen dimension.
 */
void CenterWindow(HWND hwnd) {
    RECT rect;
    GetWindowRect(hwnd, &rect);
    SetWindowPos(hwnd, 0, (GetSystemMetrics(SM_CXSCREEN) - rect.right) / 2,
                 (GetSystemMetrics(SM_CYSCREEN) - rect.bottom) / 2, WIDTH, HEIGHT, 0);

}

/**
 * Creates the main menu with options to restart the game or quit.
 */
HMENU CreateMainMenu() {
    HMENU menu = CreateMenu();

    HMENU quit = CreateMenu();
    AppendMenu(quit, MF_STRING, ID_START, "Start");
    AppendMenu(quit, MF_STRING, ID_QUIT, "Quit");

    AppendMenu(menu, MF_POPUP, (UINT_PTR) quit, "Menu");

    return menu;
}

/**
 * Creates the button components used for game input.
 */
void SetupControllers(HWND hwnd, HINSTANCE hInstance) {
    CreateWindow("BUTTON", "NORTH", WS_CHILD | WS_VISIBLE, WIDTH - 148, HEIGHT - 202, 64, 64, hwnd,
                 (HMENU) ID_BUTTON_NORTH,
                 hInstance, 0);
    CreateWindow("BUTTON", "WEST", WS_CHILD | WS_VISIBLE, WIDTH - 214, HEIGHT - 164, 64, 64, hwnd,
                 (HMENU) ID_BUTTON_WEST,
                 hInstance, 0);
    CreateWindow("BUTTON", "EAST", WS_CHILD | WS_VISIBLE, WIDTH - 84, HEIGHT - 164, 64, 64, hwnd,
                 (HMENU) ID_BUTTON_EAST,
                 hInstance, 0);
    CreateWindow("BUTTON", "SOUTH", WS_CHILD | WS_VISIBLE, WIDTH - 148, HEIGHT - 126, 64, 64, hwnd,
                 (HMENU) ID_BUTTON_SOUTH,
                 hInstance, 0);

}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
    WNDCLASSEX window = {0};
    char className[] = {"GameWindowClass"};

    window.cbSize = sizeof(window);
    window.style = 0;
    window.lpfnWndProc = WinProc;
    window.hInstance = hInstance;
    window.hIcon = (HICON) LoadIcon(0, IDI_ASTERISK);
    window.hIconSm = 0;
    window.hCursor = (HCURSOR) LoadCursor(0, IDC_ARROW);
    window.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
    window.lpszMenuName = 0;
    window.lpszClassName = className;

    RegisterClassEx(&window);

    HWND hwnd = CreateWindow(window.lpszClassName,
                             TITLE, WS_OVERLAPPEDWINDOW | WS_VISIBLE, 0, 0,
                             WIDTH,
                             HEIGHT,
                             0, CreateMainMenu(), hInstance, 0);

    SetupControllers(hwnd, hInstance);
    CenterWindow(hwnd);
    HandleEventLoop();
    return (hwnd == 0) ? -1 : 0;
}