#include <../headers/utils.h>

template <typename T>
T getMax(T &array, int n)
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
T getMin(T &array, int n)
{
    T min = array[0];
    for (int i = 0; i < n; i++)
    {
        if (min < array[i])
            min = array[i];
    }
    return min;
}