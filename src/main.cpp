#include <iostream>
#include "../headers/image.hpp"
#include "../headers/utils.hpp"

int main(int argc, char* argv[])
{
    gtk_init(&argc, &argv);

    // 1 HISTOGRAM
    /* Image img1("../images/kity.jpg");
    img1.computeLuminance();
    img1.applyLuminance();
    img1.computeHistogram();
    img1.displayHistogram(); */

    // 2 BRIGHTNESS
    /* Image img2("../images/kity.jpg");
    img2.adjustBrightness(50);
    img2.display(); */

    // 3 CONTRAST
    /* Image img3("../images/kity.jpg");
    img3.adjustContrast(5);
    img3.display(); */

    // 4 NEGATIVE
    /* Image img4("../images/kity.jpg");
    img4.applyNegative();
    img4.display(); */

    // 5 HISTOGRAM EQUALIZATION
    // Grayscale
    /* Image img5("../images/kity.jpg");
    img5.computeLuminance();
    img5.applyLuminance();
    img5.computeHistogram();
    img5.displayHistogram();
    img5.histogramEqualization();
    img5.computeHistogram();
    img5.displayHistogram();
    img5.display(); */

    // Color
    /* Image img5b("../images/kity.jpg");
    img5b.computeLuminance();
    img5b.computeHistogramColorUsingLuminance();
    img5b.displayHistogram();
    img5b.histogramEqualizationColorUsingLuminance();
    img5b.computeHistogramColorUsingLuminance();
    img5b.displayHistogram();
    img5b.display();  */

    // 6 HISTOGRAM MATCHING 
    
    /* Image img6("../images/kity.jpg");
    img6.computeLuminance();
    img6.applyLuminance();
    img6.computeHistogram();
    img6.displayHistogram();

    Image img6b("../images/shrek.jpg");
    img6b.computeLuminance();
    img6b.applyLuminance();
    img6b.computeHistogram();
    img6b.displayHistogram();

    img6.histogramMatching(img6b);
    img6.computeHistogram();
    img6.displayHistogram();
    img6.display(); */

    // 7 ZOOM OUT

    /* Image img7("../images/kity.jpg");
    img7.zoomOut(4, 4);
    img7.display(); */

    // 7 ZOOM IN
    /* Image img8("../images/kity.jpg");
    img8.zoomIn();
    img8.display();  */

    // 9 ROTATE
    /* Image img9("../images/kity.jpg");
    img9.rotateImage90Degrees(1); // clockwise
    img9.display();

    Image img9b("../images/kity.jpg");
    img9b.rotateImage90Degrees(0); // counterclockwise
    img9b.display(); */

    // 10 CONVOLUTIONs

    // i)
    /* Image img10("../images/kity.jpg");
    img10.display();
    float gaussian[] = {0.0625, 0.125, 0.0625, 0.125, 0.25, 0.125, 0.0625, 0.125, 0.0625};
    img10.applyConv2D(gaussian);
    img10.approximateToInterval();
    img10.display(); */

    // ii)
    /* Image img10("../images/kity.jpg");
    img10.computeLuminance();
    img10.applyLuminance();
    img10.display();
    int laplacian[] = {0, -1, 0, -1, 4, -1, 0, -1, 0};
    img10.applyConv2D(laplacian);
    img10.clamppingAndAproximate();
    img10.display(); */

    // iii)
    /* Image img10("../images/kity.jpg");
    img10.computeLuminance();
    img10.applyLuminance();
    img10.display();
    int genericHighPass[] = {-1, -1, -1, -1, 8, -1, -1, -1, -1};
    img10.applyConv2D(genericHighPass);
    img10.approximateToInterval();
    img10.display(); */ 

    //iv)
   /*  Image img10("../images/kity.jpg");
    img10.computeLuminance();
    img10.applyLuminance();
    img10.display();
    int prewittHx[] = {-1, 0, 1, -1, 0, 1, -1, 0, 1};
    img10.applyConv2D(prewittHx);
    img10.clamppingAndAproximate();
    img10.display(); */ 

    //v)
    /* Image img10("../images/kity.jpg");
    img10.computeLuminance();
    img10.applyLuminance();
    img10.display();
    int prewittHy[] = {-1, -1, -1, 0, 0, 0, 1, 1, 1};
    img10.applyConv2D(prewittHy);
    img10.clamppingAndAproximate();
    img10.display(); */ 

    //vi)
    /* Image img10("../images/kity.jpg");
    img10.computeLuminance();
    img10.applyLuminance();
    img10.display();
    int sobelHx[] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};
    img10.applyConv2D(sobelHx);
    img10.clamppingAndAproximate();
    img10.display(); */

    //vii)
    /* Image img10("../images/kity.jpg");
    img10.computeLuminance();
    img10.applyLuminance();
    img10.display();
    int sobelHy[] = {-1, -2, -1, 0, 0, 0, 1, 2, 1};
    img10.applyConv2D(sobelHy);
    img10.clamppingAndAproximate();
    img10.display(); */

    // Build custom kernel
    /* Image img10("../images/kity.jpg");
    img10.computeLuminance();
    img10.applyLuminance();
    img10.display();
    float* customKernel = buildKernel<float>();
    img10.applyConv2D(customKernel);
    img10.clamppingAndAproximate();
    img10.display(); */

    if (Image::getNumberWindows() > 0)
        gtk_main();

    return 0;
}