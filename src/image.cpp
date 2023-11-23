/*
Henrique Uhlmann Gobbi
November 2023
FPI INF01046 2023/2

Code to handle image processing using stb_image to decompress JPG file.
Inspired by https://github.com/Code-Break0/Image-Processing/
stb_image: https://github.com/nothings/stb
*/

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../headers/image.h"
#include "../headers/stb_image.h"
#include "../headers/stb_image_write.h"
#include <iostream>

// Checks number os images displayed
int Image::numberWindows = 0;

int Image::getNumberWindows() { return numberWindows; }
void Image::incNumberWindows() { numberWindows++; }

// Constructor that reads image given filename
Image::Image(const char* fileName)
{
    if (read(fileName))
    {
        std::cout << "Image " << fileName << " read! :D" << std::endl;
        size = w*h*channels;
    }
    else
    {
        std::cerr << "Image " << fileName << " could not be loaded. :(" << std::endl;
    }
}

// Constructor that creates a blank image given width, heigth and channels
Image::Image(int w, int h, int channels) : w(w), h(h), channels(channels) 
{
	size = w * h * channels;
	data = new uint8_t[size]();
}

// Constructor that copies an image
Image::Image(const Image& img) : Image(img.w, img.h, img.channels) 
{
	memcpy(data, img.data, size);
}

// Deletes image data
Image::~Image()
{
    delete[] data;
}

// Gets image extension from filename, currently only supports JPG
ImageType Image::getFileType(const char* fileName)
{
    const char* ext = strrchr(fileName, '.');
    if (!ext)
        std::cout << "Warning, file " << fileName << " wihtout extension!" << std::endl;
    else
    {
        ext += 1;
        if (strcmp(ext, "jpg") == 0)
            return JPG;
        else
            std::cout << "Unsuported image type, using JPG" << std::endl;
    }

    return JPG;
}

// Reads image given filename
bool Image::read(const char* fileName)
{
    data = stbi_load(fileName, &w, &h, &channels, 0);
    return data != NULL;
}

// Writes image given filename, currently only supports JPG
bool Image::write(const char* fileName)
{
    ImageType type = getFileType(fileName);
    int success = 0;
    switch (type)
    {
    case JPG:
        success = stbi_write_jpg(fileName, w, h, channels, data, 100); // Using 100% quality
        break;
    }

    if (!success)
        std::cerr << "Error writing " << fileName << std::endl;
    else
        std::cout << "Successfully written " << fileName << "!" << std::endl;
    
    return success != 0;
}

// Prints data of image
void Image::printImageData()
{
    printf("Width: %d px, Height: %d px, Channels: %d, Size: %d\n", w, h, channels, size);
}

// Mirrors data of image: direction= true (vertically) | false (horizontally)
void Image::mirrorImage(bool direction)
{
    if (direction)
    {
        int numRowsSwapped = 0;
        if (h % 2 == 0)
            numRowsSwapped = h / 2;
        else
            numRowsSwapped = (h - 1) / 2;
        
        const size_t row_size = w * channels;
        uint8_t* temp_row = static_cast<uint8_t*>(malloc(row_size));

        int input_offset, output_offset;

        for (int i = 0; i < numRowsSwapped; i++)
        {
            input_offset = i * row_size;
            output_offset = (h - i - 1) * row_size;

            memcpy(temp_row, data + input_offset, row_size);
            memcpy(data + input_offset, data + output_offset, row_size);
            memcpy(data + output_offset, temp_row, row_size);
        }
        
        free(temp_row);
    }
    else
    {
        int numColsSwapped = 0;
        if (w % 2 == 0)
            numColsSwapped = w / 2;
        else
            numColsSwapped = (w - 1) / 2;

        uint8_t* temp = static_cast<uint8_t*>(malloc(channels));
        
        for (int i = 0; i < h; i++)
        {
            for (int j = 0; j < numColsSwapped; j++)
            {
                // for each color channel
                for (int c = 0; c < channels; c++) 
                {
                    int index1 = (i * w + j) * channels + c;
                    int index2 = (i * w + (w - j - 1)) * channels + c;

                    uint8_t temp = data[index1];
                    data[index1] = data[index2];
                    data[index2] = temp;
                }
            }
        }

        free(temp);
    }
}

// Applies luminance to image
void Image::applyLuminance()
{
    for(int i = 0; i < h; i++)
    {
        for (int j = 0; j < w; j++)
        {
            int idx_r = (i * w + j) * channels + 0;
            int idx_g = (i * w + j) * channels + 1;
            int idx_b = (i * w + j) * channels + 2;

            //L = 0.299*R + 0.587*G + 0.114*B
            uint8_t L = 0.299 * data[idx_r] + 0.587 * data[idx_g] + 0.114 * data[idx_b];
            data[idx_r] = L;
            data[idx_g] = L;
            data[idx_b] = L;

            // Gets min and max luminance for quantization funcion
            if (i == 0 && j == 0)
            {
                tMin = L;
                tMax = L;
            }
            else
            {
                if (L < tMin)
                    tMin = L;
                
                if (L > tMax)
                    tMax = L;
            }
        }
    }
}

// Quantizes the image given an integer n
void Image::quantize(int n)
{
    applyLuminance();

    int tam_int = tMax - tMin + 1;

    if (n < tam_int)
    {
        if (n == 0)
        {
            std::cerr << "Error, n cannot be 0." << std::endl;        
        }
        else
        {
            float tb = tam_int / n;
            for (int i = 0; i < h; i++)
            {
                for (int j = 0; j < w; j++)
                {
                    int L = data[(i * w + j) * channels];
                    int b = static_cast<int>((L - tMin) / tb);
                    float b1 = tMin - 0.5 + b*tb;
                    float b2 = tMin - 0.5 + (b+1)*tb;
                    int centerBin = static_cast<int>(((b2 - b1) / 2) + b1);
                    data[(i * w + j) * channels + 0] = centerBin;
                    data[(i * w + j) * channels + 1] = centerBin;
                    data[(i * w + j) * channels + 2] = centerBin;
                }
            }
        }
    }
}

void Image::displayImage()
{
    Image::incNumberWindows();

    int bytesPerPixel = channels;
    int cols = w;
    int rows = h;

    // Generate image pixel buffer
    GdkPixbuf *pb = gdk_pixbuf_new_from_data(
        data,
        GDK_COLORSPACE_RGB,     // colorspace (must be RGB)
        0,                      // has_alpha (0 for no alpha)
        8,                      // bits-per-sample (must be 8)
        cols, rows,             // cols, rows
        cols * bytesPerPixel,   // rowstride
        NULL, NULL              // destroy_fn, destroy_fn_data
    );
    GtkWidget *image = gtk_image_new_from_pixbuf(pb);
    
    // Create GTK window
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Window settings
    gtk_window_set_title(GTK_WINDOW(window), "Image");
    gtk_window_set_default_size(GTK_WINDOW(window), cols, rows);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);

    // Display window
    gtk_container_add(GTK_CONTAINER(window), image);
    gtk_widget_show_all(window);

    // Save some memory
    g_object_unref(pb);

    std::cout << "Image displayed" << std::endl;
}
