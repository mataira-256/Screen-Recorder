#pragma once
#include <Windows.h>
extern "C" {
#include <libavformat/avformat.h>
#include <libavutil/opt.h>
#include <libavcodec/avcodec.h>
}

int Recorder();