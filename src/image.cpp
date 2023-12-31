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
#include "../headers/image.hpp"
#include "../headers/utils.hpp"
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
    delete[] dataLuminance;
    delete[] histogram;
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
    memcpy(data, dataLuminance, size);
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

uint8_t* Image::copyData()
{
    uint8_t* dataDisplay = new uint8_t[size];
    memcpy(dataDisplay, data, size);

    return dataDisplay;
}

void Image::display()
{
    Image::incNumberWindows();

    int bytesPerPixel = channels;
    int cols = w;
    int rows = h;

    // Creates image pixel buffer
    uint8_t* dataDisplay = copyData();
    GdkPixbuf *pb = gdk_pixbuf_new_from_data(
        dataDisplay,
        GDK_COLORSPACE_RGB,  
        0,                    
        8,                    
        cols, rows,             
        cols * bytesPerPixel,  
        NULL, NULL              
    );
    GtkWidget *image = gtk_image_new_from_pixbuf(pb);
    
    // Creates GTK window
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Window settings
    gtk_window_set_title(GTK_WINDOW(window), "Image");
    gtk_window_set_default_size(GTK_WINDOW(window), cols, rows);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);

    // Adds image to window
    gtk_container_add(GTK_CONTAINER(window), image);
    gtk_widget_show_all(window);

    // Save some memory
    g_object_unref(pb);

    std::cout << "Image displayed!" << std::endl;
}

void Image::computeLuminance()
{
    if (dataLuminance != nullptr) 
    {
        delete[] dataLuminance;
    }

    dataLuminance = new uint8_t[size];
    for (int i = 0; i < size; i += channels)
    {
        int ix = i;
        uint8_t L = static_cast<uint8_t>(0.299 * data[ix] + 0.587 * data[ix+1] + 0.114 * data[ix+2]);

        for (int j = 0; j < channels; j++)
        {
            dataLuminance[ix+j] = L;
        }

        // Gets min and max luminance for quantization funcion
        if (i == 0)
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

void Image::computeHistogram()
{
    if (histogram != nullptr) 
    {
        delete[] histogram;
    }

    int n = 256;
    histogram = new int[size]();

    for (int i = 0; i < size; i+=channels) // Assuming 3-channel luminance data
    {
        histogram[data[i]]++;
    }
}

void Image::drawHistogram(cairo_t* cr, int* data, int width, int height) {
    cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size(cr, 20.0);

    cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);

    // Drawing numbers
    double x = 0.015 * width;
    double y = (0.88 * height) + 20;

    const char* textZero = "0";
    cairo_move_to(cr, x, y);
    cairo_show_text(cr, textZero);

    x = 0.935 * width;
    y = (0.88 * height) + 20;

    const char* text255 = "255";
    cairo_move_to(cr, x, y);
    cairo_show_text(cr, text255);
    
    // Drawing bars
    cairo_set_line_width(cr, 2);

    int n = 256; // Number of bars
    int* minMax = getMinMax(data, n); // Gets minimum and maximum of data

    // Set the width and height of each bar
    double bar_width = 5;
    double bar_spacing = 2;

    x = 0.02 * width;
    y = 0.88 * height;

    // Set the color for the bars
    cairo_set_source_rgb(cr, 1, 0.6, 0.0);

    for (int i = 0; i < n; i++) 
    {
        double dataNorm = (static_cast<double>(data[i]) - static_cast<double>(minMax[0])) / (static_cast<double>(minMax[1]) - static_cast<double>(minMax[0]));
        double barHeight = 0.78*height * dataNorm;
        if (barHeight < 0)
            barHeight = 0;
        cairo_rectangle(cr, x, y, bar_width, -barHeight);
        cairo_fill(cr);

        x += bar_width + bar_spacing;
    }
}

gboolean Image::onDraw(GtkWidget* widget, cairo_t* cr, gpointer data) 
{
    CallbackData* cbData = static_cast<CallbackData*>(data);
    drawHistogram(cr, cbData->data, cbData->width, cbData->height);

    return FALSE;
}

void Image::displayHistogram()
{
    Image::incNumberWindows();

    int width = 1920;
    int height = 700;
    int n = 256;

    GtkWidget* window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Histogram");
    gtk_window_set_default_size(GTK_WINDOW(window), width, height);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget* drawing_area = gtk_drawing_area_new();
    gtk_container_add(GTK_CONTAINER(window), drawing_area);

    int* histogramDisplay = new int[n];
    for (int i = 0; i < n; i++)
    {
        histogramDisplay[i] = histogram[i];
    }
    CallbackData* callbackData = new CallbackData{histogramDisplay, width, height};
    g_signal_connect(G_OBJECT(drawing_area), "draw", G_CALLBACK(onDraw), callbackData);

    gtk_widget_show_all(window);
}

void Image::adjustBrightness(float b)
{
    if (b >= -255 && b <= 255)
    {
        for (int i = 0; i < size; i++)
        {
            float temp = static_cast<float>(data[i]) + b;
            if (temp < 0)
                data[i] = static_cast<uint8_t>(0);
            else
                if (temp > 255)
                    data[i] = static_cast<uint8_t>(255);
                else
                    data[i] = static_cast<uint8_t>(temp);
        }
    }
    else
    {
        std::cerr << "Brightness increase must be in [-255, 255]" << std::endl;
    }
}

void Image::adjustContrast(float c)
{
    if (c > 0 && c <= 255)
    {
        for (int i = 0; i < size; i++)
        {
            float temp = static_cast<float>(data[i]) * c;
            if (temp < 0)
                data[i] = static_cast<uint8_t>(0);
            else
                if (temp > 255)
                    data[i] = static_cast<uint8_t>(255);
                else
                    data[i] = static_cast<uint8_t>(temp);
        }
    }
    else
    {
        std::cerr << "Constrast increase must be in (0, 255]" << std::endl;
    }
}

void Image::applyNegative()
{
    for (int i = 0; i < size; i++)
    {
        data[i] = static_cast<u_int8_t>(255) - data[i];
    }
}

void Image::histogramEqualization()
{
    int n = 256;
    float alpha = static_cast<float>(n-1) / (static_cast<float>(w) * static_cast<float>(h));
        
    int* histCum = new int[n];
    histCum[0] = histogram[0];
    for (int i = 1; i < n; i++)
    {
        histCum[i] = histCum[i-1] + histogram[i];
    }
    
    uint8_t* newData = new uint8_t[size];
    for (int i = 0; i < size; i+=channels)
    {
        for (int j = 0; j < channels; j++)
        {
            newData[i+j] = static_cast<uint8_t>(alpha * histCum[data[i+j]]);
        }
    }

    memcpy(data, newData, size);
    delete[] newData;
}

void Image::computeHistogramColorUsingLuminance()
{
    if (histogram != nullptr) 
    {
        delete[] histogram;
    }

    computeLuminance(); // Computes lumminance channel for RGB image

    int n = 256;
    histogram = new int[size]();

    for (int i = 0; i < size; i+=channels) // Assuming 3-channel luminance data
    {
        histogram[dataLuminance[i]]++;
    }
}

void Image::histogramEqualizationColorUsingLuminance()
{
    int n = 256;
    float alpha = static_cast<float>(n-1) / (static_cast<float>(w) * static_cast<float>(h));
        
    int* histCum = new int[n];
    histCum[0] = histogram[0];
    for (int i = 1; i < n; i++)
    {
        histCum[i] = histCum[i-1] + histogram[i];
    }
    
    uint8_t* newData = new uint8_t[size];
    for (int i = 0; i < size; i+=channels)
    {
        for (int j = 0; j < channels; j++)
        {
            newData[i+j] = static_cast<uint8_t>(alpha * histCum[data[i+j]]);
        }
    }

    memcpy(data, newData, size);
    delete[] newData;
}

uint8_t findClosestTargetShade(float* sourceHistCum, float* tgtHistCum, int shade, int n)
{    
    uint8_t closestShade;
    float minDifference = static_cast<float>(INFINITY);
    for (int j = 0; j < n; j++)
    {
        if (sourceHistCum[shade] == tgtHistCum[j])
            return static_cast<uint8_t>(j);
        else if (abs(sourceHistCum[shade] - tgtHistCum[j]) < minDifference)
        {
            minDifference = abs(sourceHistCum[shade] - tgtHistCum[j]);
            closestShade = static_cast<uint8_t>(j);
        }
    }
        
    return closestShade;
}

// Applies HM to grayscale images
void Image::histogramMatching(Image& target)
{
    // Compute histograms
    this->computeHistogram();
    target.computeHistogram();

    // Compute cumulative histograms
    int n = 256;
    float* sourceHistCum = new float[n];
    float* targetHistCum = new float[n];
    sourceHistCum[0] = this->histogram[0];
    targetHistCum[0] = target.histogram[0];
    for (int i = 1; i < n; i++)
    {
        sourceHistCum[i] = sourceHistCum[i-1] + this->histogram[i];
        targetHistCum[i] = targetHistCum[i-1] + target.histogram[i];
    }

    // Normalize cumulative histograms
    sourceHistCum = normalizeArray(sourceHistCum, n);
    targetHistCum = normalizeArray(targetHistCum, n);

    // Compute HM function
    uint8_t* HM = new uint8_t[n]();
    for (int i = 0; i < n; i++)
        if(this->histogram[i] == 0)
            continue;
        else
        {
            HM[i] = findClosestTargetShade(sourceHistCum, targetHistCum, i, n);
        }

    // Translate HM function to original image
    uint8_t* newData = new uint8_t[size];
    for (int i = 0; i < size; i+=channels)
    {
        for (int j = 0; j < channels; j++)
        {
           newData[i+j] = HM[data[i+j]];
        }
    }

    memcpy(data, newData, size);
    delete[] newData, sourceHistCum, targetHistCum, HM;
}

// Rotate image: true clockwise, false counterclockwise
void Image::rotateImage90Degrees(bool direction)
{
    int rotatedHeight = w;
    int rotatedWidth = h;
    uint8_t* newData = new uint8_t[size];
    for (int i = 0; i < h; i++) 
    {
        for (int j = 0; j < w; j++) 
        {
            int rotatedJ = i;
            int rotatedI = rotatedHeight - 1 - j;

            for (int c = 0; c < channels; ++c) 
            {
                if (direction)
                    newData[((rotatedHeight - rotatedI) * rotatedWidth + (rotatedWidth - 1 - rotatedJ)) * channels + c] = data[(i * w + j) * channels + c];
                else
                    newData[(rotatedI * rotatedWidth + (rotatedWidth - 1 - rotatedJ)) * channels + c] = data[(i * w + j) * channels + c];                 
            }
        }
    }

    memcpy(data, newData, size);
    h = rotatedHeight;
    w = rotatedWidth;
    delete[] newData;
}

void Image::zoomOut(int sx, int sy)
{
    if (sx <= 0 || sy <= 0)
    {
        std::cerr << "Zoom cannot be negative or zero" << std::endl;
    }
    else
    {

        int newH = h / sy;
        int newW = w / sx;
        int newSize = newH * newW * channels;
        uint8_t* newData = new uint8_t[newSize];

        for (int i = 0; i < newH; i++) 
        {
            for (int j = 0; j < newW; j++) 
            {
                int sumR = 0, sumG = 0, sumB = 0;

                for (int y = 0; y < sy; y++) 
                {
                    for (int x = 0; x < sx; x++) 
                    {
                        int sourceY = i * sy + y;
                        int sourceX = j * sx + x;

                        if (sourceX < w && sourceY < h) 
                        {
                            int pixelIndex = (sourceY * w + sourceX) * channels;
                            sumR += data[pixelIndex];
                            sumG += data[pixelIndex + 1];
                            sumB += data[pixelIndex + 2];
                        }
                    }
                }

                int kernelSize = sx * sy;
                int pixelIndex = (i * newW + j) * channels;
                newData[pixelIndex] = sumR / kernelSize;
                newData[pixelIndex + 1] = sumG / kernelSize;
                newData[pixelIndex + 2] = sumB / kernelSize;
            }
        }

        w = newW;
        h = newH;
        size = newSize;
        memcpy(data, newData, size);
        delete[] newData;
    }
}

void Image::zoomIn()
{
    int newWidth = w * 2;
    int newHeight = h * 2;
    int newSize = newWidth * newHeight * channels;

    uint8_t* newData = new uint8_t[newSize]();

    // Part I: Build new matrix
    for (int i = 0; i < h; i++)
    {
        for (int j = 0; j < w; j++)
        {
            int newIndex = (i * 2 * newWidth + j * 2) * channels;
            int oldIndex = (i * w + j) * channels;

            for (int c = 0; c < channels; c++) 
            {
                newData[newIndex + c] = data[oldIndex + c];
            }
        }
    }

    // Part II: Linear interpolation
    for (int i = 0; i < newHeight; i+=2)
    {
        for (int j = 0; j < newWidth; j+=2)
        {
            for (int c = 0; c < channels; c++)
            {
                if (j == newWidth - 1)
                {
                    newData[((newHeight-1) * newWidth + j) * channels + c] = newData[((newHeight-2) * newWidth + j) * channels + c];
                }
                newData[(i * newWidth + (j + 1)) * channels + c] = 
                    static_cast<uint8_t>((newData[(i * newWidth + j) * channels + c] + newData[(i * newWidth + (j + 2)) * channels + c]) / 2);
            }
        }
    }

    for (int i = 0; i < newHeight; i += 2) 
    {
        for (int j = 0; j < newWidth; j++) 
        {
            for (int c = 0; c < channels; c++) 
            {
                if (i == newHeight - 1)
                {
                    newData[(i * newWidth + (newWidth-1)) * channels + c] = newData[(i * newWidth + (newWidth-2)) * channels + c];
                }
                else
                {
                    newData[((i + 1) * newWidth + j) * channels + c] = 
                        static_cast<uint8_t>((newData[(i * newWidth + j) * channels + c] + newData[((i + 2) * newWidth + j) * channels + c]) / 2);
                }
            }
        }
    }

    w = newWidth;
    h = newHeight;
    size = newSize;
    delete[] data;
    data = new uint8_t[size];
    memcpy(data, newData, size);
    delete[] newData;
}

void Image::applyConv2D(int* kernel)
{
    uint8_t* newData = new uint8_t[size];
    memcpy(newData, data, size);
    int kernelRadius = 1;
    int kernelSize = 3; 
    flipMatVertically(kernel, kernelSize, kernelSize);
    for (int i = kernelRadius; i < h - kernelRadius; i++)
    {
        for (int j = kernelRadius; j < w - kernelRadius; j++)
        {
            for (int c = 0; c < channels; c++)
            {
                int valuePixelKernelSum = 0;
                for (int y = 0; y < kernelSize; y++)
                {
                    for (int x = 0; x < kernelSize; x++)
                    {
                        int valuePixel = data[((i - kernelRadius + y) * w + (j - kernelRadius + x)) * channels + c];
                        valuePixelKernelSum += valuePixel * kernel[y * kernelSize + x];
                    }
                }
                
                if (valuePixelKernelSum > 255)
                    valuePixelKernelSum = 255;
                else
                    if(valuePixelKernelSum < 0)
                        valuePixelKernelSum = 0;

                int pixelIndex = (i * w + j) * channels + c;                
                newData[pixelIndex] = static_cast<uint8_t>(valuePixelKernelSum);
            }
        }
    }

    memcpy(data, newData, size);
    delete[] newData;
}

void Image::applyConv2D(float* kernel)
{
    uint8_t* newData = new uint8_t[size];
    memcpy(newData, data, size);
    int kernelRadius = 1;
    int kernelSize = 3; 
    flipMatVertically(kernel, kernelSize, kernelSize);
    for (int i = kernelRadius; i < h - kernelRadius; i++)
    {
        for (int j = kernelRadius; j < w - kernelRadius; j++)
        {
            for (int c = 0; c < channels; c++)
            {
                float valuePixelKernelSum = 0;
                for (int y = 0; y < kernelSize; y++)
                {
                    for (int x = 0; x < kernelSize; x++)
                    {
                        float valuePixel = static_cast<float>(data[((i - kernelRadius + y) * w + (j - kernelRadius + x)) * channels + c]);
                        valuePixelKernelSum += static_cast<float>(valuePixel * kernel[y * kernelSize + x]);
                    }
                }
                
                if (valuePixelKernelSum > 255)
                    valuePixelKernelSum = 255;
                else
                    if(valuePixelKernelSum < 0)
                        valuePixelKernelSum = 0;

                int pixelIndex = (i * w + j) * channels + c;                
                newData[pixelIndex] = static_cast<uint8_t>(valuePixelKernelSum);
            }
        }
    }

    memcpy(data, newData, size);
    delete[] newData;
}

void Image::approximateToInterval()
{
    for (int i = 0; i < size; i++)
    {
        if (data[i] > 255)
            data[i] = 255;
        else
            if(data[i] < 0)
                data[i] = 0;
    }
}

void Image::clamppingAndAproximate()
{
    adjustBrightness(127);
}