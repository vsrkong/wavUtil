//
// Created by VSRkong on 2017/8/7.
//
#ifndef AUDIO_AUDIOREADER_H
#define AUDIO_AUDIOREADER_H

#include <cstdio>

//#define DebugPrint 1

#define MAX_PATH_LEN (1024)

#pragma pack(push)
#pragma pack(1)
struct chunk{
    char id[4];
    int  size;
    char formType[4];
};

struct pcmData{
    short formatTag;//编码格式代码
    short channels;//声道个数
    int   sampleRate;//采样频率(Hz)
    int   byteRate;//音频的码率:声道数×采样频率×每样本的数据位数/8
    short blockAlign;//采样帧大小:声道数x量化位数/8
    short bitsPerSample;//音频sample的量化位数
};
#pragma pack(pop)


class AudioReader {

public:

    AudioReader(const char *inPath);

    ~AudioReader();


    int getFormatTag();

    int getChannels();

    int getSampleRate();

    int getByteRate();

    short getBlockAlign();

    short getBitsPerSample();

    int getSampleWidth();

    const char *getSource();

    int getSampleCounts();

    void printAudioInfo();

    int getSamples(void *buf, int num, int shift_num);
    int getSamples(void *buf, int num);

    void back(int nSamples);


private:
    int  formatTag;
    int  channels=-1;
    int  sampleRate=-1;
    int   byteRate=-1;
    short blockAlign;
    short bitsPerSample;

    int  sampleWidth;
    int  sampleCounts;

    char inPath[MAX_PATH_LEN];
    int  offset=0;

    FILE *fp;


    char *data;
    int dataLenth;

    char *fileBuffer;
    int fileSize;

    bool init();

    bool setSource(const char *inPath);

    char* findDataChunk(chunk *p_chunk);

    chunk* jumpChunk(chunk *p_chunk,int *jumpFlag);



};


#endif //AUDIO_AUDIOREADER_H
