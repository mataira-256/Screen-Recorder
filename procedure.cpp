#include "window.hpp"
#include "window_id.hpp"
#include "layout.hpp"


extern HWND hwnd_main;
extern HWND hwnd_settings;
extern HWND hwnd_btn_recording;
extern HWND hwnd_btn_recstop;

/* MainWndProc(ウィンドウプロシージャ(受け取ったメッセージによって処理をする))の定義 */
LRESULT CALLBACK MainWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) { // uMsg(受け取ったメッセージが入ってる)が
    case WM_DESTROY: // WM_DESTROY(ウィンドウが破棄されたときに発生するメッセージ)なら
        PostQuitMessage(0); // 終了コードを0にする
        return 0;
    case WM_COMMAND: // WM_COMMAND(何らかのボタンが押されたときに発生するメッセージ)なら
        switch (LOWORD(wParam)) {
        case ID_BTN_SETTINGS:   // 設定ウィンドウボタンが押されたとき
            CreateSettingsWindow();
            break;
        case ID_BTN_RECORDING:  // 録画ボタンなら
            ShowWindow(hwnd_btn_recording, SW_HIDE);
            ShowWindow(hwnd_btn_recstop, SW_NORMAL);
            SetWindowText(hwnd_main, TEXT("●Screen-Recorder"));
            Recording();
            break;
        case ID_BTN_RECSTOP:    // 停止ボタンなら
            int id; // メッセージボックスで警告してから消す
            id = MessageBox(
                hwnd_main,
                TEXT("非常停止しますか？した場合、今回の録画は正常終了しません\n(正常終了する場合はFFmpegのウィンドウでCtrl+Cをしてください)"),
                TEXT(""),
                MB_OKCANCEL | MB_ICONEXCLAMATION);
            if (id == IDOK) {   // 緊急停止を選んだ場合
                ShowWindow(hwnd_btn_recording, SW_NORMAL);
                ShowWindow(hwnd_btn_recstop, SW_HIDE);
                SetWindowText(hwnd_main, TEXT("〇Screen-Recorder"));
                RecStop();
            }
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
    case WM_PAINT:  // テキストの描画
        // 描画コンテキストの取得
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        // テキストの描画
        TextOut(hdc, MARGIN, MARGIN, L"path", 4);
        TextOut(hdc, MARGIN, BOX_HEIGHT + MARGIN * 2, L"device[beta]", 12);
        TextOut(hdc, MARGIN, BOX_HEIGHT * 2 + MARGIN * 3, L"resolution", 10);
        TextOut(hdc, MARGIN, BOX_HEIGHT * 3 + MARGIN * 4, L"fps", 3);
        TextOut(hdc, MARGIN, BOX_HEIGHT * 4 + MARGIN * 5, L"sound", 5);

        // 描画コンテキストの解放
        EndPaint(hwnd, &ps);
        break;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam); // 特別指定したやつ以外は既定のプロシージャにぶん投げる
}
