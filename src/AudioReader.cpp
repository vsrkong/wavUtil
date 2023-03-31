//
// Created by VSRkong on 2017/8/7.
//
#include "../include/AudioReader.h"
#include <cstring>
#include <cstdlib>

AudioReader::AudioReader(const char *inPath) {
    setSource(inPath);
}

int AudioReader::getFormatTag(){
    return formatTag;
}

int AudioReader::getChannels() {
    return channels;
}

int AudioReader::getSampleRate() {
    return sampleRate;
}

int AudioReader::getByteRate(){
    return byteRate;
}

short AudioReader::getBlockAlign(){
    return blockAlign;
}

short AudioReader::getBitsPerSample(){
    return bitsPerSample;
}

int AudioReader::getSampleWidth() {
    return sampleWidth;
}

const char *AudioReader::getSource() {
    return inPath;
}

int AudioReader::getSampleCounts() {
    return sampleCounts;
}

bool AudioReader::setSource(const char *inPath) {
    int len = strlen(inPath);
    if (len+1 > MAX_PATH_LEN) {
        printf("%s is too long\n", inPath);
        return false;
    } else {
        strcpy(this->inPath, inPath);
        printf("AudioReader:Enter audio path:%s\n",inPath);
        if ((fp = fopen(inPath, "rb")) == NULL) {
            printf("read %s failed!\n", inPath);
        }
        return init();
    }
}

bool AudioReader::init() {
    fseek (fp , 0 , SEEK_END);
    fileSize = ftell (fp);
    rewind (fp);
#ifdef DebugPrint
    printf("filesize(Byte):%u\n",fileSize);
#endif
    fileBuffer = (char *) malloc(sizeof(char) * fileSize);
    if (NULL == fileBuffer) {
        perror("Memory allocation error");
        return false;
    }

    if (fread(fileBuffer, 1, fileSize, fp) < 1) {
        perror("Into memory failure");
        return false;
    }
    chunk *p_mChunk;
    p_mChunk = (chunk *) fileBuffer;

    data=findDataChunk(p_mChunk);
        return true;
}

char* AudioReader::findDataChunk(chunk *p_chunk) {
    int mJumbFlag=0;
    while(0==mJumbFlag)
    {
        p_chunk=jumpChunk(p_chunk,&mJumbFlag);
    }
    return (char *)p_chunk;
}

chunk* AudioReader::jumpChunk(chunk *p_chunk,int *jumpFlag) {

    if(strncmp(p_chunk->id,"RIFF",4)==0){
#ifdef DebugPrint
        printf("RIFF-id:%.4s\n",p_chunk->id);
        printf("RIFF-size(Byte):%u\n",p_chunk->size);
        printf("RIFF-formType:%.4s\n",p_chunk->formType);
#endif
        return (chunk*)((char *)p_chunk+sizeof(chunk));

    }else if(strncmp(p_chunk->id,"LIST",4)==0){

#ifdef DebugPrint
        printf("LIST-ID:%.4s\n",p_chunk->id);
        printf("LIST-size(Byte):%d\n",p_chunk->size);
        printf("LIST-formType:%s\n",p_chunk->formType);
#endif

        return (chunk*)(p_chunk->formType+p_chunk->size);

    }else if((strncmp(p_chunk->id,"fact",4)==0)){

#ifdef DebugPrint
        printf("fact-id:%.4s\n",p_chunk->id);
        printf("fact-size(Byte):%d\n",p_chunk->size);
#endif
        return (chunk*)(p_chunk->formType+p_chunk->size);

    }else if(strncmp(p_chunk->id,"fmt ",4)==0){

#ifdef DebugPrint
        printf("fmt?-id:%.4s\n",p_chunk->id);
        printf("fmt?-size(Byte):%d\n",p_chunk->size);
#endif
        pcmData* p_pcmData=(pcmData *)(p_chunk->formType);
#ifdef DebugPrint
        printf("pcmData-formatTag:%u\n",p_pcmData->formatTag);
        printf("pcmData-channels:%u\n",p_pcmData->channels);
        printf("pcmData-sampleRate(Hz):%u\n",p_pcmData->sampleRate);
        printf("pcmData-byteRate(kBps):%.2f\n",((float)p_pcmData->byteRate)/1000);
        printf("pcmData-bitRate(kbps):%.2f\n",((float)p_pcmData->byteRate)*8/1000);
        printf("pcmData-blockAlign(Byte):%u\n",p_pcmData->blockAlign);
        printf("pcmData-bitsPerSample(bit):%u\n",p_pcmData->bitsPerSample);
#endif
        formatTag=p_pcmData->formatTag;
        channels=p_pcmData->channels;
        sampleRate=p_pcmData->sampleRate;//Hz
        byteRate=p_pcmData->byteRate;//kBps
        blockAlign=p_pcmData->blockAlign;//Byte
        bitsPerSample=p_pcmData->bitsPerSample;//bit
        sampleWidth=p_pcmData->bitsPerSample/8;
        return (chunk*)(p_chunk->formType+p_chunk->size);

    }else if(strncmp(p_chunk->id,"data",4)==0){

#ifdef DebugPrint
        printf("data-id:%.4s\n",p_chunk->id);
        printf("data-size(Byte):%u\n",p_chunk->size);
#endif
        sampleCounts=p_chunk->size/sampleWidth/channels;
        *jumpFlag=1;
        dataLenth=p_chunk->size;
        //dataLenth = sampleCounts*channels*sampleWidth;
#ifdef DebugPrint
        printf("dataLenth(Byte):%u\n",dataLenth);
#endif
        return (chunk*)(p_chunk->formType);

    }else{
        printf("wav data not found OR read error");
        return NULL;
    }
}

int AudioReader::getSamples(void *buf, int num, int shift) {
    int tot_read = num * channels * sampleWidth;
    int shift_read = shift * channels * sampleWidth;
    if (offset + tot_read > dataLenth)
        return -1;

    memcpy(buf, data+offset, tot_read);
    offset += shift_read;
    return num;
}

int AudioReader::getSamples(void *buf, int num) {
    return getSamples(buf, num, num);
}

AudioReader::~AudioReader() {
    free(fileBuffer);
    if (fp) {
        fclose(fp);
    }
}

void AudioReader::printAudioInfo() {
    printf("------------------------------\n");
    printf("       ***Audio Info***       \n");
    printf("channels:%u\n",channels);
    printf("duration(s):%.1f\n",(float)dataLenth/(float)byteRate);
    printf("sampleRate(Hz):%u\n",sampleRate);
    printf("byteRate(kBps):%.2f\n",((float)byteRate)/1000);
    printf("bitRate(kbps):%.2f\n",((float)byteRate)*8/1000);
    printf("bitsPerSample(bit):%u\n",bitsPerSample);
    printf("sampleWidth(Byte):%d\nsampleCounts:%d\n",sampleWidth, sampleCounts);
    printf("------------------------------\n");
}

void AudioReader::back(int nSamples) {
    int t = nSamples * channels * sampleWidth;
    if (offset >= t) {
        offset -= t;
    }
}