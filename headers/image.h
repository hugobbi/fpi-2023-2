#include <cstdio>
#include <gtkmm.h>
#include <gdkmm/pixbuf.h>
#include <thread>
#include <mutex>

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

        std::thread displayImageThread();

        static int getNumberWindows();
        static void incNumberWindows();
};
