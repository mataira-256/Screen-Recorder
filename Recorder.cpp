#include "recorder.hpp"

extern HWND hwnd_main;
extern HWND hwnd_btn_recording;
extern HWND hwnd_btn_recstop;

STARTUPINFO si;
PROCESS_INFORMATION pi;

void Recording() {
    wchar_t commandLine[] = L"ffmpeg -video_size 1920x1080 -framerate 30 -f gdigrab -i desktop -s 256x144 -vcodec libx264 -pix_fmt yuv420p desktop.mp4";

    // 構造体の初期化
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si); // サイズの設定
    ZeroMemory(&pi, sizeof(pi));

    // ffmpegの起動
    CreateProcess(
        nullptr,
        commandLine,
        nullptr,
        nullptr,
        false,
        0,
        nullptr,
        nullptr,
        &si,
        &pi);

    /* 正常に終了された場合の処理 */
    std::thread([pi = std::move(pi)]() {    // 非同期処理のための別スレッド立て(詳しくは知らん)
        WaitForSingleObject(pi.hProcess, INFINITE);

        ShowWindow(hwnd_btn_recording, SW_NORMAL);
        ShowWindow(hwnd_btn_recstop, SW_HIDE);
        SetWindowText(hwnd_main, TEXT("〇Screen-Recorder"));

    }).detach();
}

void RecStop() {

    TerminateProcess(pi.hProcess, 0);

    // ハンドルを閉じる
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}