#include "recorder.hpp"

STARTUPINFO si;
PROCESS_INFORMATION pi;

void Recording() {
    wchar_t commandLine[] = L"ffmpeg -video_size 1920x1080 -framerate 30 -f gdigrab -i desktop -s 256x144 -vcodec libx264 -pix_fmt yuv420p desktop.mp4";

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

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
}

void RecStop() {

    TerminateProcess(pi.hProcess, 0);

    // ハンドルを閉じる
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}