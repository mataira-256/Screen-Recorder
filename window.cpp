#include <windows.h>

/* �v���g�^�C�v�錾 */
LRESULT CALLBACK MainWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK SettingsWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void CreateMainWindow();
void CreateSettingsWindow();

/* �O���[�o���ϐ����X */
HINSTANCE hInstance;
TCHAR class_main[] = TEXT("MainWindow");
TCHAR class_settings[] = TEXT("SettingsWindow");
HWND hwnd_main;
HWND hwnd_settings;

#define ID_BTN_SETTINGS 1

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

    /* wc_main(�E�B���h�E�N���X)�̑�����ݒ�*/
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

    /* wc_settings(�E�B���h�E�N���X)�̑�����ݒ�*/
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
        200,
        200,
        200,
        200,
        hwnd_main,
        NULL,
        hInstance,
        NULL);

    ShowWindow(hwnd_settings, SW_SHOW);
    UpdateWindow(hwnd_settings);

    EnableWindow(hwnd_main, FALSE);     // ���C���E�B���h�E���g�p�s��

}

/* MainWndProc(�E�B���h�E�v���V�[�W��(�󂯎�������b�Z�[�W�ɂ���ď���������))�̒�` */
LRESULT CALLBACK MainWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) { // uMsg(�󂯎�������b�Z�[�W�������Ă�)��
    case WM_DESTROY: // WM_DESTROY(�E�B���h�E���j�����ꂽ�Ƃ��ɔ������郁�b�Z�[�W)�Ȃ�
        PostQuitMessage(0); // �I���R�[�h��0�ɂ���
        return 0;
    case WM_COMMAND: // WM_COMMAND(���炩�̃{�^���������ꂽ�Ƃ��ɔ������郁�b�Z�[�W)�Ȃ�
        switch LOWORD(wParam) {
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
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam); // ���ʎw�肵����ȊO�͊���̃v���V�[�W���ɂԂ񓊂���
}
