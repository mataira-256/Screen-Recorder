#include "json.hpp"
#include "window.hpp"
#include "window_id.hpp"
#include "layout.hpp"

/* グローバル変数等々 */
HINSTANCE hInstance;
TCHAR class_main[] = TEXT("MainWindow");
TCHAR class_settings[] = TEXT("SettingsWindow");
HWND hwnd_main;
HWND hwnd_settings;
HWND hwnd_btn_recording;
HWND hwnd_btn_recstop;

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
        TEXT("〇Screen-Recorder"),
        WS_OVERLAPPED | WS_SYSMENU,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        BTN_WIDTH*2+MARGIN*3+MAGIC_WIDTH,
        BTN_HEIGHT+MARGIN*2+MAGIC_HEIGHT,
        NULL,
        NULL,
        hInstance,
        NULL);

    /* 録画ボタンを作る */
    hwnd_btn_recording = CreateWindow(
        TEXT("button"),
        TEXT("rec"),
        WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
        MARGIN,
        MARGIN,
        BTN_WIDTH,
        BTN_HEIGHT,
        hwnd_main,
        (HMENU)ID_BTN_RECORDING,
        hInstance,
        NULL);

    /* 停止ボタンを作る */
    hwnd_btn_recstop = CreateWindow(
        TEXT("button"),
        TEXT("stop"),
        WS_CHILD | BS_PUSHBUTTON,
        MARGIN,
        MARGIN,
        BTN_WIDTH,
        BTN_HEIGHT,
        hwnd_main,
        (HMENU)ID_BTN_RECSTOP,
        hInstance,
        NULL);

    /* 設定ボタンを作る */
    hwnd_btn_settings = CreateWindow(
        TEXT("button"),
        TEXT("settings"),
        WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
        BTN_WIDTH+MARGIN*2,
        MARGIN,
        BTN_WIDTH,
        BTN_HEIGHT,
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
    HWND hwnd_tb_device;
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
        TEXT("settings"),
        WS_POPUPWINDOW | WS_CAPTION,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        BOX_WIDHT + MARGIN * 2 + MAGIC_WIDTH + TEXT_WIDTH,
        BOX_HEIGHT * 5 + BTN_HEIGHT + MARGIN * 7 + MAGIC_HEIGHT,
        hwnd_main,
        NULL,
        hInstance,
        NULL);


    /* 保存先pathのテキストボックスの作成 */
    hwnd_tb_path = CreateWindow(
        TEXT("EDIT"),
        TEXT(""),
        WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT | ES_AUTOHSCROLL,
        MARGIN + TEXT_WIDTH,
        MARGIN,
        BOX_WIDHT,
        BOX_HEIGHT,
        hwnd_settings,
        (HMENU)ID_TB_PATH,
        hInstance,
        NULL);

    SendMessage(hwnd_tb_path, EM_LIMITTEXT, 256, 0);

    /* 録音デバイスのテキストボックスの作成 */
    hwnd_tb_device = CreateWindow(
        TEXT("EDIT"),
        TEXT(""),
        WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT | ES_AUTOHSCROLL,
        MARGIN + TEXT_WIDTH,
        BOX_HEIGHT + MARGIN * 2,
        BOX_WIDHT,
        BOX_HEIGHT,
        hwnd_settings,
        (HMENU)ID_TB_DEVICE,
        hInstance,
        NULL);

    SendMessage(hwnd_tb_device, EM_LIMITTEXT, 256, 0);


    /* 解像度のコンボボックスの作成 */
    hwnd_cb_resolution = CreateWindow(
        TEXT("COMBOBOX"),
        TEXT(""),
        WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | CBS_HASSTRINGS,
        MARGIN + TEXT_WIDTH,
        BOX_HEIGHT * 2 + MARGIN * 3,
        BOX_WIDHT,
        CB_HEIGHT,
        hwnd_settings,
        (HMENU)ID_CB_RESOLUTION,
        hInstance,
        NULL);

    /* 解像度のコンボボックスのリストを追加 */
    SendMessage(hwnd_cb_resolution, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)TEXT("1920x1080"));
    SendMessage(hwnd_cb_resolution, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)TEXT("1280x720"));
    SendMessage(hwnd_cb_resolution, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)TEXT("640x360"));
    SendMessage(hwnd_cb_resolution, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)TEXT("256x144"));


    /* フレームレートのコンボボックスの作成 */
    hwnd_cb_fps = CreateWindow(
        TEXT("COMBOBOX"),
        TEXT(""),
        WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | CBS_HASSTRINGS,
        MARGIN + TEXT_WIDTH,
        BOX_HEIGHT * 3 + MARGIN * 4,
        BOX_WIDHT,
        CB_HEIGHT,
        hwnd_settings,
        (HMENU)ID_CB_FPS,
        hInstance,
        NULL);

    /* フレームレートのコンボボックスのリストを追加 */
    SendMessage(hwnd_cb_fps, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)TEXT("60fps"));
    SendMessage(hwnd_cb_fps, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)TEXT("30fps"));
    SendMessage(hwnd_cb_fps, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)TEXT("15fps"));


    /* 音質のコンボボックスの作成 */
    hwnd_cb_sound = CreateWindow(
        TEXT("COMBOBOX"),
        TEXT(""),
        WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | CBS_HASSTRINGS,
        MARGIN + TEXT_WIDTH,
        BOX_HEIGHT * 4 + MARGIN * 5,
        BOX_WIDHT,
        CB_HEIGHT,
        hwnd_settings,
        (HMENU)ID_CB_SOUND,
        hInstance,
        NULL);

    /* 音質のコンボボックスのリストを追加 */
    SendMessage(hwnd_cb_sound, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)TEXT("48000Hz"));
    SendMessage(hwnd_cb_sound, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)TEXT("32000Hz"));
    SendMessage(hwnd_cb_sound, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)TEXT("16000Hz"));


    /* テキストボックスとコンボボックスに 初期値を与える */
    nlohmann::json json;
    std::ifstream file("settings.json");
    file >> json;

    std::string get_path = json["path"];
    std::string get_device = json["device"];
    int get_resolution = json["resolution"];
    int get_fps = json["fps"];
    int get_sound = json["sound"];

    std::wstring wstr_path(get_path.begin(), get_path.end());
    std::wstring wstr_device(get_device.begin(), get_device.end());

    SendMessage(hwnd_tb_path, WM_SETTEXT, 0, (LPARAM)wstr_path.c_str());
    SendMessage(hwnd_tb_device, WM_SETTEXT, 0, (LPARAM)wstr_device.c_str());
    SendMessage(hwnd_cb_resolution, CB_SETCURSEL, get_resolution, 0);
    SendMessage(hwnd_cb_fps, CB_SETCURSEL, get_fps, 0);
    SendMessage(hwnd_cb_sound, CB_SETCURSEL, get_sound, 0);


    /* 保存ボタンとキャンセルボタン */
    hwnd_btn_save_settings = CreateWindow(
        TEXT("button"),
        TEXT("save"),
        WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
        MARGIN + TEXT_WIDTH,
        BOX_HEIGHT * 5 + MARGIN * 6,
        BTN_WIDTH,
        BTN_HEIGHT,
        hwnd_settings,
        (HMENU)ID_BTN_SAVE_SETTINGS,
        hInstance,
        NULL);

    hwnd_btn_cancel_settings = CreateWindow(
        TEXT("button"),
        TEXT("cancel"),
        WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
        BTN_WIDTH + MARGIN + TEXT_WIDTH,
        BOX_HEIGHT * 5 + MARGIN * 6,
        BTN_WIDTH,
        BTN_HEIGHT,
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
    HWND hwnd_tb_device     = GetDlgItem(hwnd_settings, ID_TB_DEVICE);
    HWND hwnd_cb_resolution = GetDlgItem(hwnd_settings, ID_CB_RESOLUTION);
    HWND hwnd_cb_fps        = GetDlgItem(hwnd_settings, ID_CB_FPS);
    HWND hwnd_cb_sound      = GetDlgItem(hwnd_settings, ID_CB_SOUND);

    /* jsonを開く */
    nlohmann::json json;
    std::ofstream file("settings.json");

    /* 文字を変数に入れていく*/
    std::string set_path = GetTextFromEdit(hwnd_tb_path);
    std::string set_device = GetTextFromEdit(hwnd_tb_device);
    int set_resolution = SendMessage(hwnd_cb_resolution, CB_GETCURSEL, 0, 0);
    int set_fps = SendMessage(hwnd_cb_fps, CB_GETCURSEL, 0, 0);
    int set_sound= SendMessage(hwnd_cb_sound, CB_GETCURSEL, 0, 0);

    /* jsonに記入していく */
    json["path"]       = set_path;
    json["device"]     = set_device;
    json["resolution"] = set_resolution;
    json["fps"]        = set_fps;
    json["sound"]      = set_sound;

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
