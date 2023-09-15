#include <windows.h>

/* �v���V�[�W���̃v���g�^�C�v�錾 ? */
LRESULT CALLBACK WndProc(
    HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

/* WinMain(WINAPI��main�݂����Ȃ���)�̐錾 */
int WINAPI WinMain(
    HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPSTR lpszCmdLine, int nCmdShow)
{
    /* �ϐ������\���̂����̐錾 */
    TCHAR szAppName[] = TEXT("TestApp");
    WNDCLASS wc_main;
    HWND hwnd_main;
    MSG msg;

    HWND hwnd_button_rec;

    #define CHILD_ID_REC 1;

    /* wc(�E�B���h�E�N���X)�̑�����ݒ�*/
    wc_main.style = CS_HREDRAW | CS_VREDRAW;
    wc_main.lpfnWndProc = WndProc;
    wc_main.cbClsExtra = 0;
    wc_main.cbWndExtra = 0;
    wc_main.hInstance = hInstance;
    wc_main.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc_main.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc_main.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc_main.lpszMenuName = NULL;
    wc_main.lpszClassName = szAppName;
    
    /* wc��o�^(windows�ɔF��������) */
    if (!RegisterClass(&wc_main)) return 0;

    /* hwnd(�E�B���h�E�n���h��(���ʎq))�̍쐬 */
    hwnd_main = CreateWindow(
        szAppName, TEXT("Screen-Recorder"),
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        CW_USEDEFAULT, CW_USEDEFAULT,
        NULL, NULL,
        hInstance, NULL);

    if (!hwnd_main) return 0;

    ShowWindow(hwnd_main, nCmdShow);
    UpdateWindow(hwnd_main);     // �E�B���h�E���ĕ`��(���̂��߂��͕������)

    hwnd_button_rec = CreateWindow(
        "button_rec",
        "�^��{�^��",
        WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
        100,
        100,
        100,
        50,
        hwnd_main,
        (HMENU)CHILD_ID_REC,
        hInstance,
        NULL);







    /* ���b�Z�[�W���[�v(�C�x���g�Ƃ����b�Z�[�W���󂯎���āA�������Ă����Ƃ��ɑ���̂ƁA�E�B���h�E�̈ێ�) */
    while (GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}

/* WndProc(�E�B���h�E�v���V�[�W��(�󂯎�������b�Z�[�W�ɂ���ď���������))�̒�` */
LRESULT CALLBACK WndProc(
    HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg) {             // uMsg(�󂯎�������b�Z�[�W�������Ă�)��
    case WM_DESTROY:            // WM_DESTROY(�E�B���h�E���j�����ꂽ�Ƃ��ɔ������郁�b�Z�[�W)�Ȃ�
        PostQuitMessage(0);     // �I���R�[�h��0�ɂ���
        return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);       // ���ʎw�肵����ȊO�͊���̃v���V�[�W���ɂԂ񓊂���
}