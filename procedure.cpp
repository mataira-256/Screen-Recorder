#include "window.hpp"
#include "window_id.hpp"

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
        case ID_BTN_SETTINGS:
            CreateSettingsWindow();
            break;
        case ID_BTN_RECORDING:
            ShowWindow(hwnd_btn_recording, SW_HIDE);
            ShowWindow(hwnd_btn_recstop, SW_NORMAL);
            Recording();
            break;
        case ID_BTN_RECSTOP:
            ShowWindow(hwnd_btn_recording, SW_NORMAL);
            ShowWindow(hwnd_btn_recstop, SW_HIDE);
            RecStop();
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
