#include <iostream>
#include <opencv2/opencv.hpp>

#define THREASHOLD 128

using namespace std;
using namespace cv;

//利用樣板來特化函式，以利二維陣列的傳入
template<typename T, size_t M, size_t N>
Mat Dilation(Mat img, T(&kernel)[M][N], int widthLimit, int heightLimit) {
	//創建新圖準備處理
	Mat result(heightLimit, widthLimit, CV_8U, Scalar(0));

	for (int height = 0; height < heightLimit; height++) {
		for (int width = 0; width < widthLimit; width++) {

			if (img.at<uchar>(height, width) > 0) {
				uchar max = 0;

				for (int i = 0; i < M; i += 1) {
					if (kernel[i][0] + height >= 0 && kernel[i][0] + height < heightLimit
						&& kernel[i][1] + width >= 0 && kernel[i][1] + width < widthLimit)
						max = img.at<uchar>(kernel[i][0] + height, kernel[i][1] + width) > max ? img.at<uchar>(kernel[i][0] + height, kernel[i][1] + width) : max;
				}

				for (int i = 0; i < M; i += 1) {
					if (kernel[i][0] + height >= 0 && kernel[i][0] + height < heightLimit
						&& kernel[i][1] + width >= 0 && kernel[i][1] + width < widthLimit)
						result.at<uchar>(kernel[i][0] + height, kernel[i][1] + width) = max;
				}

			}
		}
	}

	return result;
}


template<typename T, size_t M, size_t N>
Mat Erosion(Mat img, T(&kernel)[M][N], int widthLimit, int heightLimit) {
	//創建新圖準備處理
	Mat result(heightLimit, widthLimit, CV_8U, Scalar(0));

	for (int height = 0; height < heightLimit; height++) {
		for (int width = 0; width < widthLimit; width++) {

			bool exist;

			if (img.at<uchar>(height, width) > 0) {
				exist = true;
				uchar min = 255;

				for (int i = 0; i < M; i += 1) {
					if (kernel[i][0] + height < 0 || kernel[i][0] + height >= heightLimit
						|| kernel[i][1] + width < 0 || kernel[i][1] + width >= widthLimit
						|| img.at<uchar>(kernel[i][0] + height, kernel[i][1] + width) == 0) {
						exist = false;
						break;
					}
					min = img.at<uchar>(kernel[i][0] + height, kernel[i][1] + width) < min ? img.at<uchar>(kernel[i][0] + height, kernel[i][1] + width) : min;
				}

				for (int i = 0; i < M; i += 1) {
					if (exist)
						result.at<uchar>(kernel[i][0] + height, kernel[i][1] + width) = min;
				}
			}
		}
	}

	return result;
}

template<typename T, size_t M, size_t N>
Mat Opening(Mat img, T(&kernel)[M][N], int widthLimit, int heightLimit) {
	return Dilation(Erosion(img, kernel, widthLimit, heightLimit), kernel, widthLimit, heightLimit);
}

template<typename T, size_t M, size_t N>
Mat Closing(Mat img, T(&kernel)[M][N], int widthLimit, int heightLimit) {
	return Erosion(Dilation(img, kernel, widthLimit, heightLimit), kernel, widthLimit, heightLimit);
}