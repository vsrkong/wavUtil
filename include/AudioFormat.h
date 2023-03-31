#ifndef AUDIO_FORMAT_H
#define AUDIO_FORMAT_H

#define WAV_HEADER_SIZE (44)

enum AudioFormat{
	WAV,
	PCM,
};

typedef struct __attribute__((packed, aligned(1))) {
	char ChunkID[4];
	int ChunkSize;
	char Format[4];
	char Subchunk1ID[4];
	int Subchunk1Size;
	short AudioFormat;
	short NumChannels;
	int SampleRate;
	int ByteRate;
	short BlockAlign;
	short BitsPerSample;
}WavHeader;

typedef struct __attribute__((packed, aligned(1))) {
	char Subchunk2ID[4];
	int Subchunk2Size;
}WavSize;

#endif
