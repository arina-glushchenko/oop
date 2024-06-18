#include "help.h"
#include "converter.h"


bool sstream::strCompr(const char* f, const char* s)
{
    for (int i = 0; i < 4; i++)
        if (f[i] != s[i])
            return false;
    return true;
}

void sstream::read(std::string filename) //считываем аудиофайл
{
    std::ifstream in(filename, std::ios::binary | std::ios::in); // открываем файл в бинарном режиме для чтения
    if (in.fail()) 
        throw std::exception("File not found"); 

    in.read((char*)&header, sizeof(WAVHEADER)); // Читаем заголовок WAV файла

    // Производим множественные проверки параметров файла (формат, частота дискретизации и другие)
    if ((!strCompr(header.chunkId, "RIFF")) || (!strCompr(header.format, "WAVE")) || (!strCompr(header.subchunk1Id, "fmt ")) || (header.subchunk1Size != 16) ||
        (header.audioFormat != 1) || (header.numChannels != 1) || (header.sampleRate != 44100) || (header.byteRate != 88200) || (header.blockAlign != 2) ||
        (header.bitsPerSample != 16)) //куча мучительных проверок на формат файла
        throw std::exception("File is not supported");

    if (strCompr(header.subchunk2Id, "LIST"))
    {
        int skip;
        in.read((char*)&skip, 4);
        for (int i = 0; i < skip; i++)
            in.read((char*)&skip, 1);
        in.read(header.subchunk2Id, 4); 
    }
    if (!strCompr(header.subchunk2Id, "data"))
        throw std::exception("File is not supported");

    //length = 1.f * header.subchunk2Size / (header.bitsPerSample / 8) / header.numChannels / header.sampleRate; // Рассчитываем длительность аудиофайла в секундах

    signals.clear();
    int data_cnt = header.subchunk2Size / (header.bitsPerSample / 8) / header.numChannels;
    for (int i = 0; i < data_cnt; i++)
    {
        signals.push_back(0);
        in.read((char*)&(signals.back()), 2);
    }
    in.close();
}


void sstream::save(std::string filename)
{
    std::ofstream out(filename, std::ios::binary | std::ios::out);
    out.write((char*)&header, sizeof(WAVHEADER));
    for (short i : signals)
        out.write((char*)&i, 2);
    out.close();
}