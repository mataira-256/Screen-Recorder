#include <windows.h>

/* プロシージャのプロトタイプ宣言 ? */
LRESULT CALLBACK WndProc(
    HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

/* WinMain(WINAPIのmainみたいなもの)の宣言 */
int WINAPI WinMain(
    HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPSTR lpszCmdLine, int nCmdShow)
{
    /* 変数だか構造体だかの宣言 */
    TCHAR szAppName[] = TEXT("TestApp");
    WNDCLASS wc_main;
    HWND hwnd_main;
    MSG msg;

    HWND hwnd_button_rec;

    #define CHILD_ID_REC 1;

    /* wc(ウィンドウクラス)の属性を設定*/
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
    
    /* wcを登録(windowsに認識させる) */
    if (!RegisterClass(&wc_main)) return 0;

    /* hwnd(ウィンドウハンドル(識別子))の作成 */
    hwnd_main = CreateWindow(
        szAppName, TEXT("Screen-Recorder"),
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        CW_USEDEFAULT, CW_USEDEFAULT,
        NULL, NULL,
        hInstance, NULL);

    if (!hwnd_main) return 0;

    ShowWindow(hwnd_main, nCmdShow);
    UpdateWindow(hwnd_main);     // ウィンドウを再描画(何のためかは分からん)

    hwnd_button_rec = CreateWindow(
        "button_rec",
        "録画ボタン",
        WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
        100,
        100,
        100,
        50,
        hwnd_main,
        (HMENU)CHILD_ID_REC,
        hInstance,
        NULL);







    /* メッセージループ(イベントとかメッセージを受け取って、処理してくれるとこに送るのと、ウィンドウの維持) */
    while (GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}

/* WndProc(ウィンドウプロシージャ(受け取ったメッセージによって処理をする))の定義 */
LRESULT CALLBACK WndProc(
    HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg) {             // uMsg(受け取ったメッセージが入ってる)が
    case WM_DESTROY:            // WM_DESTROY(ウィンドウが破棄されたときに発生するメッセージ)なら
        PostQuitMessage(0);     // 終了コードを0にする
        return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);       // 特別指定したやつ以外は既定のプロシージャにぶん投げる
}