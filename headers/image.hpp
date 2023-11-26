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
        static gboolean onDraw(GtkWidget *widget, cairo_t *cr, gpointer data);
        static void drawHistogram(cairo_t* cr, int* data, int width, int height);

    public:
        uint8_t* data;
        int size;
        int w;
        int h;
        int channels;

        int tMin, tMax;

        int* histogram;

        // Used to transfer information in callback
        struct CallbackData
        {
            int* data;
            int width;
            int height;
        };

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
        void displayHistogram();

        uint8_t* copyData();

        static int getNumberWindows();
        static void incNumberWindows();
};
