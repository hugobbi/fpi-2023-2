#include <stdint.h>
#include <cstdio>

enum ImageType 
{
    JPG
};

struct Image
{
    uint8_t* data;
    int size;
    int w;
    int h;
    int channels;

    int tMin, tMax;

    Image(const char* fileName);
    Image(int w, int h, int channels);
    Image(const Image& img);
    ~Image();
    
    ImageType getFileType(const char* fileName);

    bool read(const char* fileName);
    bool write(const char* fileName);

    void printImageData();

    void mirrorImage(bool direction);
    void applyLuminance();
    void quantize(int n);
};
