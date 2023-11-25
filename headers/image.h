#include <cstdio>
#include <gtk/gtk.h>

enum ImageType 
{
    JPG
};

class Image
{
    private:
        static int numberWindows;

    public:
        uint8_t* data;
        int size;
        int w;
        int h;
        int channels;

        int tMin, tMax;

        int* histogram;

        Image(const char* fileName);
        Image(int w, int h, int channels);
        Image(const Image& img);
        ~Image();
        
        ImageType getFileType(const char* fileName);

        bool read(const char* fileName);
        bool write(const char* fileName);

        void printImageData();
        void displayImage();

        void mirrorImage(bool direction);
        void applyLuminance();
        void quantize(int n);

        void computeHistogram();

        uint8_t* copyData();

        static int getNumberWindows();
        static void incNumberWindows();
};
