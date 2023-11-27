#ifndef UTILS_TPP
#define UTILS_TPP

#include "../headers/utils.hpp"
#include <cmath>
#include <cstring>
#include <iostream>

template <typename T>
T getMax(T array, int n)
{
    T max = array[0];
    for (int i = 0; i < n; i++)
    {
        if (max > array[i])
            max = array[i];
    }
    return max;
}

template <typename T>
T getMin(T* array, int n)
{
    T min = array[0];
    for (int i = 0; i < n; i++)
    {
        if (min < array[i])
            min = array[i];
    }
    return min;
}

template <typename T>
T* getMinMax(T* array, int n)
{
    T* minMax = new T[2];
    minMax[0] = array[0];
    minMax[1] = array[0];
    for (int i = 0; i < n; i++)
    {
        if (array[i] < minMax[0])
            minMax[0] = array[i];
        
        if (array[i] > minMax[1])
            minMax[1] = array[i];
    }
    return minMax;
}

template <typename T>
double* computeMeanAndStandardDeviation(T* arr, int n)
{
    double mean = 0;
    for (int i = 0; i < n; i++)
        mean += static_cast<double>(arr[i]);
    
    mean /= n;
    double sd = 0;
    
    for (int i = 0; i < n; i++)
    {
        sd += std::pow(arr[i] - mean, 2);
    }

    sd = std::sqrt(sd / (n - 1));

    double* answ = new double[2];
    answ[0] = mean;
    answ[1] = sd;

    return(answ);
}

template <typename T>
T* copyArray(T* arr, int n)
{
    T* newArr = new T[n];
    memcpy(newArr, arr, n);

    return newArr;
}

template <typename T>
T* normalizeArray(T* arr, int n)
{
    T* minMax = getMinMax(arr, n);
    T* normArr = new T[n];

    for (int i = 0; i < n; i++)
    {
        normArr[i] = static_cast<T>((arr[i]  - minMax[0]) / (minMax[1] - minMax[0]));
    }

    return normArr;
}

template <typename T>
void flipMatVertically(T* matrix, int n, int m)
{
    int rowsSwapped = n / 2;
    int rowSize = m * sizeof(T);
    T* tempRow = new T[rowSize];

    for (int i = 0; i < rowsSwapped; i++)
    {
        int topRow = i;
        int bottomRow = n - i - 1;

        memcpy(tempRow, matrix + topRow * m, rowSize);
        memcpy(matrix + topRow * m, matrix + bottomRow * m, rowSize);
        memcpy(matrix + bottomRow * m, tempRow, rowSize);
    }

    delete[] tempRow;
}

template <typename T>
T* buildKernel()
{
    std::cout << "Build a 3x3 kernel that can be applied to convolve an image!" << std::endl;

    int sizeKernel = 9;
    T* kernel = new T[sizeKernel];
    for (int i = 0; i < sizeKernel; i++)
    {
        std::cout << "Type entry " << i << ": " << std::endl;
        while (!(std::cin >> kernel[i]))
        {
            std::cin.clear(); 
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
            std::cout << "Invalid input. Try again: " << std::endl;
        }
    }
    return kernel;
}

#endif