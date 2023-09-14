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
    WNDCLASS wc;
    HWND hwnd;
    MSG msg;

    /* wc(�E�B���h�E�N���X)�̑�����ݒ�*/
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = szAppName;

    /* wc��o�^(windows�ɔF��������) */
    if (!RegisterClass(&wc)) return 0;

    /* hwnd(�E�B���h�E�n���h��(���ʎq))�̍쐬 */
    hwnd = CreateWindow(
        szAppName, TEXT("Screen-Recorder"),
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        CW_USEDEFAULT, CW_USEDEFAULT,
        NULL, NULL,
        hInstance, NULL);

    if (!hwnd) return 0;

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);     // �E�B���h�E���ĕ`��(���̂��߂��͕������)

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