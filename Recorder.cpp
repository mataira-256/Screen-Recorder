#include "recorder.hpp"

int Recorder() {
    avformat_network_init(); // ネットワーク関連の初期化（オプション）

    AVFormatContext* input_ctx = NULL;
    AVFormatContext* output_ctx = NULL;

    // 入力ファイルを開く（例：カメラまたはスクリーンキャプチャ）
    if (avformat_open_input(&input_ctx, "input_device", NULL, NULL) < 0) {
        return -1;
    }

    if (avformat_find_stream_info(input_ctx, NULL) < 0) {
        return -1;
    }

    // 出力ファイルを作成（例：output.mp4）
    if (avformat_alloc_output_context2(&output_ctx, NULL, NULL, "output.mp4") < 0) {
        return -1;
    }

    AVCodec* video_codec = avcodec_find_encoder(AV_CODEC_ID_H264);
    AVCodecContext* video_enc_ctx = avcodec_alloc_context3(video_codec);

    // ビデオエンコーダのパラメータを設定
    video_enc_ctx->width = 1920; // ビデオフレームの幅
    video_enc_ctx->height = 1080; // ビデオフレームの高さ
    video_enc_ctx->time_base = (AVRational){ 1, 30 }; // フレームレート（30 fps）
    video_enc_ctx->gop_size = 30; // GOPサイズ
    video_enc_ctx->max_b_frames = 2; // 最大Bフレーム数

    if (avcodec_open2(video_enc_ctx, video_codec, NULL) < 0) {
        return -1;
    }

    // 出力ファイルにビデオストリームを追加
    AVStream* video_stream = avformat_new_stream(output_ctx, video_codec);
    avcodec_parameters_from_context(video_stream->codecpar, video_enc_ctx);

    if (avio_open(&output_ctx->pb, "output.mp4", AVIO_FLAG_WRITE) < 0) {
        return -1;
    }

    if (avformat_write_header(output_ctx, NULL) < 0) {
        return -1;
    }

    AVPacket packet;
    while (1) {
        if (av_read_frame(input_ctx, &packet) < 0) {
            break;
        }

        if (packet.stream_index == video_stream->index) {
            // ビデオフレームをエンコードして出力
            av_packet_rescale_ts(&packet, input_ctx->streams[0]->time_base, video_stream->time_base);
            packet.stream_index = video_stream->index;
            av_interleaved_write_frame(output_ctx, &packet);
        }

        av_packet_unref(&packet);
    }

    av_write_trailer(output_ctx);
    avio_close(output_ctx->pb);

    avformat_free_context(input_ctx);
    avformat_free_context(output_ctx);

    return 0;

}