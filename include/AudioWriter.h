//
// Created by VSRkong on 2017/8/8.
//

#ifndef AUDIO_AUDIOWRITER_H
#define AUDIO_AUDIOWRITER_H

#include <stdio.h>

#define MAX_PATH_LEN (1024)
#define MAX_CHANNEL_NUM (32)

#pragma pack(push)
#pragma pack(1)
struct wavHeader{
    char  RIFFid[4];
    int   chunkSize;
    char  formType[4];
    char  formatId[4];
    int   formatSize;
    short formatTag;
    short Channel;
    int   sampleRate;
    int   byteRate;
    short blockAlign;
    short bitsPerSample;
    char  dataId[4];
    int   dataSize;
};
#pragma pack(pop)

class AudioWriter {
public:
    AudioWriter(const char *outPath,unsigned sampleRate,unsigned channels,unsigned sampleWidth);

    void writeSample(const void *pSample);

    void finish();

private:
    int sampleRate;
    int channels;
    int sampleWidth;

    char outPath[MAX_PATH_LEN];
    int sampleCounts = 0;

    FILE *fp;

    bool setPath(const char *outPath);
};

#endif //AUDIO_AUDIOWRITER_H
