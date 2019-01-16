#ifndef HW5_H_INCLUDED
#define  HW5_H_INCLUDED

template<typename T, size_t M, size_t N> Mat Dilation(Mat img, T(&kernel)[M][N], int widthLimit, int heightLimit);
template<typename T, size_t M, size_t N> Mat Erosion(Mat img, T(&kernel)[M][N], int widthLimit, int heightLimit);
template<typename T, size_t M, size_t N>Mat Opening(Mat img, T(&kernel)[M][N], int widthLimit, int heightLimit);
template<typename T, size_t M, size_t N>Mat Closing(Mat img, T(&kernel)[M][N], int widthLimit, int heightLimit);

#include "HW5.cpp"

#endif //HW5_H_INCLUDED