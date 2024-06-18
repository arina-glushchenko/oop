#pragma once
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>

struct WAVHEADER
{
    char chunkId[4];//riff
    unsigned long chunkSize;
    char format[4];//wave
    char subchunk1Id[4];//fmt
    unsigned long subchunk1Size;
    unsigned short audioFormat;
    unsigned short numChannels;
    unsigned long sampleRate;
    unsigned long byteRate;
    unsigned short blockAlign;
    unsigned short bitsPerSample;
    char subchunk2Id[4];//data
    unsigned long subchunk2Size;
};
class sstream
{
private:
    bool strCompr(const char*, const char*);
public:
    std::vector<short int> signals;
    void read(std::string);
    float length = 0;
    WAVHEADER header = {};
    void save(std::string);
};
