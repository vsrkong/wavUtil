#include <iostream>
#include "AudioReader.h"
#include "AudioWriter.h"

int main() {
    std::cout << "Hello,Wav Tool!" << std::endl;

    AudioReader audioReader = AudioReader(nullptr);


    AudioWriter audioWriter = AudioWriter(nullptr, 0, 0, 0);


    return 0;
}
