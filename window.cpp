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
    WNDCLASS wc;
    HWND hwnd;
    MSG msg;

    /* wc(ウィンドウクラス)の属性を設定*/
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

    /* wcを登録(windowsに認識させる) */
    if (!RegisterClass(&wc)) return 0;

    /* hwnd(ウィンドウハンドル(識別子))の作成 */
    hwnd = CreateWindow(
        szAppName, TEXT("Screen-Recorder"),
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        CW_USEDEFAULT, CW_USEDEFAULT,
        NULL, NULL,
        hInstance, NULL);

    if (!hwnd) return 0;

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);     // ウィンドウを再描画(何のためかは分からん)

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