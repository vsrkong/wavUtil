//
// Created by VSRkong on 2017/8/8.
//

#include <cstdio>
#include <cstring>
#include "../include/AudioWriter.h"


AudioWriter::AudioWriter(const char *outPath,unsigned sampleRate,unsigned channels,unsigned sampleWidth) {
    if(channels>MAX_CHANNEL_NUM){
        printf("channels must be less than or equal to 32");
    }
    setPath(outPath);
    this->sampleRate= sampleRate;
    this->sampleRate =sampleRate;
    this->channels = channels;
    this->sampleWidth = sampleWidth;
}

bool AudioWriter::setPath(const char *outPath) {

    int len = strlen(outPath);
    if (len+1 > MAX_PATH_LEN) {
        printf("%s is too long\n", outPath);
        return false;
    } else {
        strcpy(this->outPath, outPath);
        printf("AudioWriter:Output audio path:%s\n",outPath);
        if ((fp = fopen(outPath, "wb")) == NULL) {
            printf("creat %s failed!\n",outPath);
            return false;
        }
    }
    wavHeader header;
    if (fwrite(&header, sizeof(wavHeader), 1, fp) < 1) {
        return false;
    };
    return true;
}

void AudioWriter::writeSample(const void *pSample) {
    fwrite(pSample, sampleWidth, 1, fp);
}

void AudioWriter::finish() {
        int fileLen = ftell(fp);
        fseek(fp, 0, SEEK_SET);
        wavHeader header;
        header.RIFFid[0] = 'R';
        header.RIFFid[1] = 'I';
        header.RIFFid[2] = 'F';
        header.RIFFid[3] = 'F';
        header.chunkSize = fileLen - 8;
        header.formType[0] = 'W';
        header.formType[1] = 'A';
        header.formType[2] = 'V';
        header.formType[3] = 'E';
        header.formatId[0] = 'f';
        header.formatId[1] = 'm';
        header.formatId[2] = 't';
        header.formatId[3] = ' ';
        header.formatSize = 16;
        header.formatTag = 1;
        header.Channel= channels;
        header.sampleRate = sampleRate;
        header.byteRate = sampleRate * channels * sampleWidth;
        header.blockAlign = channels * sampleWidth;
        header.bitsPerSample = sampleWidth * 8;
        header.dataId[0] = 'd';
        header.dataId[1] = 'a';
        header.dataId[2] = 't';
        header.dataId[3] = 'a';
        header.dataSize = fileLen - sizeof(wavHeader);
        fwrite(&header, sizeof(wavHeader), 1, fp);
        printf("AudioWriter:wav head update complete!\n");
        fclose(fp);

}

