#ifndef WAV_LOADER_H
#define WAV_LOADER_H

// A Header only wav loader
// #define NL_WAV_LOADER for implementation 
// The implementation uses fstream

#pragma pack(push, 1)
struct WAVFileHeader
{
    union
    {
        char RIFF[4];
        struct
        {
            char R, I, F1, F2;
        };
    };
    int  fileSize;
    union
    {
        struct
        {
            char W, A, V, E;
        };
        char WAVE[4];
    };
    union
    {
        char fmt[4];
        struct
        {
            char f, m, t, space;
        };
    };
    int  headerSize;
    
    short format;
    short channels;
    int sampleRate;
    int bytesPerSecond;
    short bytesPerSample;
    short bitsPerSample;
    
    union
    {
        char dataHeader[4];
        struct
        {
            char D, A1, T, A2;
        };
    };
    
    int dataSize;
};
static_assert(sizeof(WAVFileHeader) == 44, "We expect the wav header to be 44 bytes long");
#pragma pack(pop)

char* LoadWAVFile(const char* filename, int* dataSize = nullptr, short* format = nullptr, short* channels = nullptr, int* sampleRate = nullptr);

#endif //WAV_LOADER_H

#if defined NL_WAV_LOADER

#include <fstream>
#include <iostream>

char* 
LoadWAVFile(const char* filename, int* dataSize, short* format, short* channels, int* sampleRate)
{
    std::ifstream wav(filename, std::ios::binary | std::ios::ate);
    
    if (!wav.is_open())
    {
        std::cout << "Unable to open " << filename << std::endl;
        return nullptr;
    }
    
    if (wav.tellg() < sizeof(WAVFileHeader))
    {
        std::cout << filename << " was less than 44 btyes and is most likely not a wav file" << std::endl;
        return nullptr;
    }
    
    wav.seekg(0);
    
    WAVFileHeader LoadedWAV = {};
    wav.read(reinterpret_cast<char*>(&LoadedWAV), sizeof(WAVFileHeader));
    
    // Check every byte of the header to make sure it is valid
    if (LoadedWAV.R != 'R' || LoadedWAV.I != 'I' || LoadedWAV.F1 != 'F' || LoadedWAV.F2 != 'F' || LoadedWAV.W != 'W' || LoadedWAV.A != 'A' || LoadedWAV.V != 'V' || LoadedWAV.E != 'E' || LoadedWAV.f != 'f' || LoadedWAV.m != 'm' || LoadedWAV.t != 't' || LoadedWAV.headerSize != 16 || LoadedWAV.D != 'd' || LoadedWAV.A1 != 'a' || LoadedWAV.T != 't' || LoadedWAV.A2 != 'a')
    {
        std::cout << filename << " was not a valid wav file" << std::endl;
        return nullptr;
    }
    
    char *data = new char[LoadedWAV.dataSize];
    wav.read(data, LoadedWAV.dataSize);
    
    if (wav.eof())
    {
        std::cout << filename << " was read past the end of file" << std::endl;
    }
    
    if (wav.good())
    {
        std::cout << filename << " was loaded successfully" << std::endl;
    }
    
    if (dataSize != nullptr)
        *dataSize = LoadedWAV.dataSize;
    
    if (format != nullptr)
        *format = LoadedWAV.format;
    
    if (channels != nullptr)
        *channels = LoadedWAV.channels;
    
    if (sampleRate != nullptr)
        *sampleRate = LoadedWAV.sampleRate;
    
    return data;
}
#endif
