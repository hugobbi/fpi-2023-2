#ifndef UTILS_HPP
#define UTILS_HPP

#include "../src/utils.tpp"

template <typename T>
T getMax(T* array, int n);

template <typename T>
T getMin(T* array, int n);

template <typename T>
T* getMinMax(T* array, int n);

template <typename T>
double* computeMeanAndStandardDeviation(T* arr, int n);

template <typename T>
T* copyArray(T* arr, int size);

template <typename T>
T* normalizeArray(T* arr, int n);

#endif