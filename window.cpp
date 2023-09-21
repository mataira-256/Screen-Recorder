#include <windows.h>
#include <iostream>
#include <fstream>
#include "json.hpp"

/* �v���g�^�C�v�錾 */
LRESULT CALLBACK MainWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK SettingsWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void CreateMainWindow();
void CreateSettingsWindow();
void SaveSettings();
std::string GetTextFromEdit(HWND editBox);

/* �O���[�o���ϐ����X */
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



/* WinMain(WINAPI��main�݂����Ȃ���)�̐錾 */
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow) {
    hInstance = hInst;
    MSG msg;

    CreateMainWindow();

    /* ���b�Z�[�W���[�v(�C�x���g�Ƃ����b�Z�[�W���󂯎���āA�������Ă����Ƃ��ɑ���̂ƁA�E�B���h�E�̈ێ�) */
    while (GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}



/* ���C���E�B���h�E�n */
void CreateMainWindow() {
    WNDCLASS wc_main;
    HWND hwnd_btn_settings;

    /* wc_main(�E�B���h�E�N���X)�̑�����ݒ� */
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

    /* wc_main��o�^(windows�ɔF��������) */
    RegisterClass(&wc_main);

    /* hwnd_main(�E�B���h�E�n���h��(���ʎq))�̍쐬 */
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

    /* �^��{�^������� */
    hwnd_btn_settings = CreateWindow(
        TEXT("button"),
        TEXT("�ݒ�"),
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



/* �ݒ�p�E�B���h�E�n */
void CreateSettingsWindow() {
    WNDCLASS wc_settings;
    HWND hwnd_tb_path;
    HWND hwnd_cb_resolution;
    HWND hwnd_cb_fps;
    HWND hwnd_cb_sound;
    HWND hwnd_btn_save_settings;
    HWND hwnd_btn_cancel_settings;

    /* wc_settings(�E�B���h�E�N���X)�̑�����ݒ� */
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

    /* wc_settings��o�^(windows�ɔF��������) */
    RegisterClass(&wc_settings);

    /* hwnd_settings(�E�B���h�E�n���h��(���ʎq))�̍쐬 */
    hwnd_settings = CreateWindow(
        class_settings,
        TEXT("�ݒ�"),
        WS_POPUPWINDOW | WS_CAPTION,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        1080,
        720,
        hwnd_main,
        NULL,
        hInstance,
        NULL);


    /* �ۑ���path�̃e�L�X�g�{�b�N�X�̍쐬 */
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


    /* �𑜓x�̃R���{�{�b�N�X�̍쐬 */
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

    /* �𑜓x�̃R���{�{�b�N�X�̑������� */
    SendMessage(hwnd_cb_resolution, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)TEXT("1920x1080"));
    SendMessage(hwnd_cb_resolution, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)TEXT("1280x720"));
    SendMessage(hwnd_cb_resolution, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)TEXT("640x360"));
    SendMessage(hwnd_cb_resolution, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)TEXT("256x144"));



    /* �t���[�����[�g�̃R���{�{�b�N�X�̍쐬 */
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

    /* �t���[�����[�g�̃R���{�{�b�N�X�̑������� */
    SendMessage(hwnd_cb_fps, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)TEXT("60fps"));
    SendMessage(hwnd_cb_fps, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)TEXT("30fps"));
    SendMessage(hwnd_cb_fps, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)TEXT("15fps"));

    /* �����̃R���{�{�b�N�X�̍쐬 */
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

    /* �����̃R���{�{�b�N�X�̑������� */
    SendMessage(hwnd_cb_sound, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)TEXT("48000Hz"));
    SendMessage(hwnd_cb_sound, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)TEXT("32000Hz"));
    SendMessage(hwnd_cb_sound, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)TEXT("16000Hz"));

    hwnd_btn_save_settings = CreateWindow(
        TEXT("button"),
        TEXT("�ۑ�"),
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
        TEXT("�L�����Z��"),
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

    EnableWindow(hwnd_main, FALSE); // ���C���E�B���h�E���g�p�s��
}



/* �ݒ��json�ɕۑ����� */
void SaveSettings() {
    /* �E�B���h�E�n���h�����擾 */
    HWND hwnd_tb_path       = GetDlgItem(hwnd_settings, ID_TB_PATH);
    HWND hwnd_cb_resolution = GetDlgItem(hwnd_settings, ID_CB_RESOLUTION);
    HWND hwnd_cb_fps        = GetDlgItem(hwnd_settings, ID_CB_FPS);
    HWND hwnd_cb_sound      = GetDlgItem(hwnd_settings, ID_CB_SOUND);

    /* ������ϐ��ɓ���Ă���*/
    std::string str_path = GetTextFromEdit(hwnd_tb_path);
    std::string str_resolution = GetTextFromEdit(hwnd_cb_resolution);
    std::string str_fps = GetTextFromEdit(hwnd_cb_fps);
    std::string str_sound = GetTextFromEdit(hwnd_cb_sound);

    /* json�ɓ���Ă� */
    nlohmann::json json;
    std::ofstream file("settings.json");

    json["path"]       = str_path;
    json["resolution"] = str_resolution;
    json["fps"]        = str_fps;
    json["sound"]      = str_sound;

    file << json.dump(4);
    file.close();
}



// �G�f�B�b�g�{�b�N�X����e�L�X�g���擾����std::string�ɕϊ�����֐�
std::string GetTextFromEdit(HWND editBox) {
    int textLength = GetWindowTextLength(editBox);
    if (textLength == 0) {
        return ""; // �G�f�B�b�g�{�b�N�X����̏ꍇ�A��̕������Ԃ�
    }

    // �e�L�X�g���i�[���邽�߂̃o�b�t�@�����蓖�Ă�
    wchar_t* buffer = new wchar_t[textLength + 1];

    // �G�f�B�b�g�{�b�N�X����e�L�X�g���擾
    GetWindowText(editBox, buffer, textLength + 1);

    // wchar_t��std::wstring�ɕϊ�
    std::wstring wstr(buffer);

    // std::wstring��std::string�ɕϊ�
    std::string str(wstr.begin(), wstr.end());

    // ���������[�N��h�����߂Ƀo�b�t�@�����
    delete[] buffer;

    return str;
}



/* MainWndProc(�E�B���h�E�v���V�[�W��(�󂯎�������b�Z�[�W�ɂ���ď���������))�̒�` */
LRESULT CALLBACK MainWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) { // uMsg(�󂯎�������b�Z�[�W�������Ă�)��
    case WM_DESTROY: // WM_DESTROY(�E�B���h�E���j�����ꂽ�Ƃ��ɔ������郁�b�Z�[�W)�Ȃ�
        PostQuitMessage(0); // �I���R�[�h��0�ɂ���
        return 0;
    case WM_COMMAND: // WM_COMMAND(���炩�̃{�^���������ꂽ�Ƃ��ɔ������郁�b�Z�[�W)�Ȃ�
        switch (LOWORD(wParam)) {
        case ID_BTN_SETTINGS:
            CreateSettingsWindow();
            break;
        }
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam); // ���ʎw�肵����ȊO�͊���̃v���V�[�W���ɂԂ񓊂���
}



/* SettingsWndProc(�E�B���h�E�v���V�[�W��(�󂯎�������b�Z�[�W�ɂ���ď���������))�̒�` */
LRESULT CALLBACK SettingsWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) { // uMsg(�󂯎�������b�Z�[�W�������Ă�)��
    case WM_DESTROY: // WM_DESTROY(�E�B���h�E���j�����ꂽ�Ƃ��ɔ������郁�b�Z�[�W)�Ȃ�
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

    return DefWindowProc(hwnd, uMsg, wParam, lParam); // ���ʎw�肵����ȊO�͊���̃v���V�[�W���ɂԂ񓊂���
}
