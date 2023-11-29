#include "window.hpp"
#include "window_id.hpp"

extern HWND hwnd_main;
extern HWND hwnd_settings;

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
