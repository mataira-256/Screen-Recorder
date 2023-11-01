#include "recorder.hpp"

int Recorder() {
    avformat_network_init(); // �l�b�g���[�N�֘A�̏������i�I�v�V�����j

    AVFormatContext* input_ctx = NULL;
    AVFormatContext* output_ctx = NULL;

    // ���̓t�@�C�����J���i��F�J�����܂��̓X�N���[���L���v�`���j
    if (avformat_open_input(&input_ctx, "input_device", NULL, NULL) < 0) {
        return -1;
    }

    if (avformat_find_stream_info(input_ctx, NULL) < 0) {
        return -1;
    }

    // �o�̓t�@�C�����쐬�i��Foutput.mp4�j
    if (avformat_alloc_output_context2(&output_ctx, NULL, NULL, "output.mp4") < 0) {
        return -1;
    }

    AVCodec* video_codec = avcodec_find_encoder(AV_CODEC_ID_H264);
    AVCodecContext* video_enc_ctx = avcodec_alloc_context3(video_codec);

    // �r�f�I�G���R�[�_�̃p�����[�^��ݒ�
    video_enc_ctx->width = 1920; // �r�f�I�t���[���̕�
    video_enc_ctx->height = 1080; // �r�f�I�t���[���̍���
    video_enc_ctx->time_base = (AVRational){ 1, 30 }; // �t���[�����[�g�i30 fps�j
    video_enc_ctx->gop_size = 30; // GOP�T�C�Y
    video_enc_ctx->max_b_frames = 2; // �ő�B�t���[����

    if (avcodec_open2(video_enc_ctx, video_codec, NULL) < 0) {
        return -1;
    }

    // �o�̓t�@�C���Ƀr�f�I�X�g���[����ǉ�
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
            // �r�f�I�t���[�����G���R�[�h���ďo��
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