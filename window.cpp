#include <windows.h>
#include <iostream>
#include <fstream>
#include "json.hpp"

/* プロトタイプ宣言 */
LRESULT CALLBACK MainWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK SettingsWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void CreateMainWindow();
void CreateSettingsWindow();
void SaveSettings();
std::string GetTextFromEdit(HWND editBox);

/* グローバル変数等々 */
HINSTANCE hInstance;
TCHAR class_main[] = TEXT("MainWindow");
TCHAR class_settings[] = TEXT("SettingsWindow");
HWND hwnd_main;
HWND hwnd_settings;

#define ID_BTN_SETTINGS 01
#define ID_TB_PATH 11
#define ID_CB_RESOLUTION 12
#define ID_CB_FPS 13
#define ID_CB_SOUND 14
#define ID_BTN_SAVE_SETTINGS 15
#define ID_BTN_CANCEL_SETTINGS 16



/* WinMain(WINAPIのmainみたいなもの)の宣言 */
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow) {
    hInstance = hInst;
    MSG msg;

    CreateMainWindow();

    /* メッセージループ(イベントとかメッセージを受け取って、処理してくれるとこに送るのと、ウィンドウの維持) */
    while (GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}



/* メインウィンドウ系 */
void CreateMainWindow() {
    WNDCLASS wc_main;
    HWND hwnd_btn_settings;

    /* wc_main(ウィンドウクラス)の属性を設定 */
    wc_main.style = CS_HREDRAW | CS_VREDRAW;
    wc_main.lpfnWndProc = MainWndProc;
    wc_main.cbClsExtra = 0;
    wc_main.cbWndExtra = 0;
    wc_main.hInstance = hInstance;
    wc_main.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc_main.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc_main.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc_main.lpszMenuName = NULL;
    wc_main.lpszClassName = class_main;

    /* wc_mainを登録(windowsに認識させる) */
    RegisterClass(&wc_main);

    /* hwnd_main(ウィンドウハンドル(識別子))の作成 */
    hwnd_main = CreateWindow(
        class_main,
        TEXT("Screen-Recorder"),
        WS_OVERLAPPED | WS_SYSMENU,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        NULL,
        NULL,
        hInstance,
        NULL);

    /* 録画ボタンを作る */
    hwnd_btn_settings = CreateWindow(
        TEXT("button"),
        TEXT("設定"),
        WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
        100,
        100,
        100,
        50,
        hwnd_main,
        (HMENU)ID_BTN_SETTINGS,
        hInstance,
        NULL);

    ShowWindow(hwnd_main, SW_NORMAL);
    UpdateWindow(hwnd_main);
}



/* 設定用ウィンドウ系 */
void CreateSettingsWindow() {
    WNDCLASS wc_settings;
    HWND hwnd_tb_path;
    HWND hwnd_cb_resolution;
    HWND hwnd_cb_fps;
    HWND hwnd_cb_sound;
    HWND hwnd_btn_save_settings;
    HWND hwnd_btn_cancel_settings;

    /* wc_settings(ウィンドウクラス)の属性を設定 */
    wc_settings.style = CS_HREDRAW | CS_VREDRAW;
    wc_settings.lpfnWndProc = SettingsWndProc;
    wc_settings.cbClsExtra = 0;
    wc_settings.cbWndExtra = 0;
    wc_settings.hInstance = hInstance;
    wc_settings.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc_settings.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc_settings.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc_settings.lpszMenuName = NULL;
    wc_settings.lpszClassName = class_settings;

    /* wc_settingsを登録(windowsに認識させる) */
    RegisterClass(&wc_settings);

    /* hwnd_settings(ウィンドウハンドル(識別子))の作成 */
    hwnd_settings = CreateWindow(
        class_settings,
        TEXT("設定"),
        WS_POPUPWINDOW | WS_CAPTION,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        1080,
        720,
        hwnd_main,
        NULL,
        hInstance,
        NULL);


    /* 保存先pathのテキストボックスの作成 */
    hwnd_tb_path = CreateWindow(
        TEXT("EDIT"),
        TEXT(""),
        WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT,
        100,
        100,
        200,
        25,
        hwnd_settings,
        (HMENU)ID_TB_PATH,
        hInstance,
        NULL);


    /* 解像度のコンボボックスの作成 */
    hwnd_cb_resolution = CreateWindow(
        TEXT("COMBOBOX"),
        TEXT(""),
        WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | CBS_HASSTRINGS,
        100,
        150,
        200,
        200,
        hwnd_settings,
        (HMENU)ID_CB_RESOLUTION,
        hInstance,
        NULL);

    /* 解像度のコンボボックスの択を入れる */
    SendMessage(hwnd_cb_resolution, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)TEXT("1920x1080"));
    SendMessage(hwnd_cb_resolution, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)TEXT("1280x720"));
    SendMessage(hwnd_cb_resolution, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)TEXT("640x360"));
    SendMessage(hwnd_cb_resolution, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)TEXT("256x144"));



    /* フレームレートのコンボボックスの作成 */
    hwnd_cb_fps = CreateWindow(
        TEXT("COMBOBOX"),
        TEXT(""),
        WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | CBS_HASSTRINGS,
        100,
        200,
        200,
        200,
        hwnd_settings,
        (HMENU)ID_CB_FPS,
        hInstance,
        NULL);

    /* フレームレートのコンボボックスの択を入れる */
    SendMessage(hwnd_cb_fps, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)TEXT("60fps"));
    SendMessage(hwnd_cb_fps, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)TEXT("30fps"));
    SendMessage(hwnd_cb_fps, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)TEXT("15fps"));

    /* 音質のコンボボックスの作成 */
    hwnd_cb_sound = CreateWindow(
        TEXT("COMBOBOX"),
        TEXT(""),
        WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | CBS_HASSTRINGS,
        100,
        250,
        200,
        200,
        hwnd_settings,
        (HMENU)ID_CB_SOUND,
        hInstance,
        NULL);

    /* 音質のコンボボックスの択を入れる */
    SendMessage(hwnd_cb_sound, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)TEXT("48000Hz"));
    SendMessage(hwnd_cb_sound, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)TEXT("32000Hz"));
    SendMessage(hwnd_cb_sound, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)TEXT("16000Hz"));

    hwnd_btn_save_settings = CreateWindow(
        TEXT("button"),
        TEXT("保存"),
        WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
        400,
        400,
        50,
        25,
        hwnd_settings,
        (HMENU)ID_BTN_SAVE_SETTINGS,
        hInstance,
        NULL);

    hwnd_btn_cancel_settings = CreateWindow(
        TEXT("button"),
        TEXT("キャンセル"),
        WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
        450,
        400,
        100,
        25,
        hwnd_settings,
        (HMENU)ID_BTN_CANCEL_SETTINGS,
        hInstance,
        NULL);

    ShowWindow(hwnd_settings, SW_SHOW);
    UpdateWindow(hwnd_settings);

    EnableWindow(hwnd_main, FALSE); // メインウィンドウを使用不可に
}



/* 設定をjsonに保存する */
void SaveSettings() {
    /* ウィンドウハンドルを取得 */
    HWND hwnd_tb_path       = GetDlgItem(hwnd_settings, ID_TB_PATH);
    HWND hwnd_cb_resolution = GetDlgItem(hwnd_settings, ID_CB_RESOLUTION);
    HWND hwnd_cb_fps        = GetDlgItem(hwnd_settings, ID_CB_FPS);
    HWND hwnd_cb_sound      = GetDlgItem(hwnd_settings, ID_CB_SOUND);

    /* 文字を変数に入れていく*/
    std::string str_path = GetTextFromEdit(hwnd_tb_path);
    std::string str_resolution = GetTextFromEdit(hwnd_cb_resolution);
    std::string str_fps = GetTextFromEdit(hwnd_cb_fps);
    std::string str_sound = GetTextFromEdit(hwnd_cb_sound);

    /* jsonに入れてく */
    nlohmann::json json;
    std::ofstream file("settings.json");

    json["path"]       = str_path;
    json["resolution"] = str_resolution;
    json["fps"]        = str_fps;
    json["sound"]      = str_sound;

    file << json.dump(4);
    file.close();
}



// エディットボックスからテキストを取得してstd::stringに変換する関数
std::string GetTextFromEdit(HWND editBox) {
    int textLength = GetWindowTextLength(editBox);
    if (textLength == 0) {
        return ""; // エディットボックスが空の場合、空の文字列を返す
    }

    // テキストを格納するためのバッファを割り当てる
    wchar_t* buffer = new wchar_t[textLength + 1];

    // エディットボックスからテキストを取得
    GetWindowText(editBox, buffer, textLength + 1);

    // wchar_tをstd::wstringに変換
    std::wstring wstr(buffer);

    // std::wstringをstd::stringに変換
    std::string str(wstr.begin(), wstr.end());

    // メモリリークを防ぐためにバッファを解放
    delete[] buffer;

    return str;
}



/* MainWndProc(ウィンドウプロシージャ(受け取ったメッセージによって処理をする))の定義 */
LRESULT CALLBACK MainWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) { // uMsg(受け取ったメッセージが入ってる)が
    case WM_DESTROY: // WM_DESTROY(ウィンドウが破棄されたときに発生するメッセージ)なら
        PostQuitMessage(0); // 終了コードを0にする
        return 0;
    case WM_COMMAND: // WM_COMMAND(何らかのボタンが押されたときに発生するメッセージ)なら
        switch (LOWORD(wParam)) {
        case ID_BTN_SETTINGS:
            CreateSettingsWindow();
            break;
        }
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam); // 特別指定したやつ以外は既定のプロシージャにぶん投げる
}



/* SettingsWndProc(ウィンドウプロシージャ(受け取ったメッセージによって処理をする))の定義 */
LRESULT CALLBACK SettingsWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) { // uMsg(受け取ったメッセージが入ってる)が
    case WM_DESTROY: // WM_DESTROY(ウィンドウが破棄されたときに発生するメッセージ)なら
        return 0;
    case WM_CLOSE:
        EnableWindow(hwnd_main, TRUE);
        DestroyWindow(hwnd);
    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case ID_BTN_SAVE_SETTINGS:
            SaveSettings();
            EnableWindow(hwnd_main, TRUE);
            DestroyWindow(hwnd);
        case ID_BTN_CANCEL_SETTINGS:
            EnableWindow(hwnd_main, TRUE);
            DestroyWindow(hwnd);
        }
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam); // 特別指定したやつ以外は既定のプロシージャにぶん投げる
}
