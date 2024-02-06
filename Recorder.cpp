#include "recorder.hpp"
#include "json.hpp"

extern HWND hwnd_main;
extern HWND hwnd_btn_recording;
extern HWND hwnd_btn_recstop;

STARTUPINFO si;
PROCESS_INFORMATION pi;

void Recording() {
    /* コマンドの作成 */   //この形にする -> ffmpeg -f gdigrab -framerate 60 -video_size 1920x1080 -i desktop -f dshow -i audio="[device]" -s [resolution] -r [fps] -ac 2 -ar 48000 -vcodec libx264 -pix_fmt yuv420p -acodec aac -strict experimental -b:a [sound] -threads 0 -y [path] \\ [time].mp4
    std::string FFmpegCommand;
    std::string path;
    std::string device;
    std::string resolution;
    std::string fps;
    std::string sound;
    std::string time;

    nlohmann::json json;
    std::ifstream file("settings.json");
    file >> json;

    std::string get_path = json["path"];
    std::string get_device = json["device"];
    int get_resolution = json["resolution"];
    int get_fps = json["fps"];
    int get_sound = json["sound"];

    path = get_path;
    device = get_device;
    switch (get_resolution){
    case 0:
        resolution = "1920x1080";
        break;
    case 1:
        resolution = "1280x720";
        break;
    case 2:
        resolution = "640x360";
        break;
    case 3:
        resolution = "256x144";
        break;
    }
    switch (get_fps) {
    case 0:
        fps = "60";
        break;
    case 1:
        fps = "30";
        break;
    case 2:
        fps = "15";
        break;
    }
    switch (get_sound) {
    case 0:
        sound = "480k";
        break;
    case 1:
        sound = "320k";
        break;
    case 2:
        sound = "160k";
        break;
    }
    time = getCurrentDateTimeString();

    FFmpegCommand = "ffmpeg -f gdigrab -framerate 60 -video_size 1920x1080 -i desktop -f dshow -i audio=\"" + device + "\" -s " + resolution + " -r " + fps + " -ac 2 -ar 48000 -vcodec libx264 -pix_fmt yuv420p -acodec aac -strict experimental -b:a " + sound + " -threads 0 -y " + path + "\\" + time + ".mp4";

    /* stringからLPWSTRの変換 */
    std::wstring wstr(FFmpegCommand.begin(), FFmpegCommand.end());
    LPWSTR commandLine = const_cast<LPWSTR>(wstr.c_str());

    /* 構造体の初期化 */
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si); // サイズの設定
    ZeroMemory(&pi, sizeof(pi));
    
    /* ffmpegの起動 */
    CreateProcessW(
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

std::string getCurrentDateTimeString() { // 日時を取得する関数
    // 現在の日時を取得
    auto now = std::chrono::system_clock::now();

    // 現在の日時をstd::tm構造体に変換
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    std::tm now_tm;
    localtime_s(&now_tm, &now_c);

    // 年-月-日-時-分-秒の形式で文字列に保存
    std::ostringstream oss;
    oss << std::put_time(&now_tm, "%Y-%m-%d-%H-%M-%S");
    return oss.str();
}