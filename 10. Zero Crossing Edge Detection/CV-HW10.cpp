#include <iostream>
#include <opencv2/opencv.hpp>
#include <cmath>

using namespace std;
using namespace cv;

Mat convolve(Mat a, Mat b) {

	Mat ans(a.rows - b.rows, a.cols - b.cols, CV_64F, Scalar(0));
	for (int i = 0; i < ans.rows; i++) {
		for (int j = 0; j < ans.cols; j++) {
			double result = 0;
			for (int k = 0; k < b.rows; k++) {
				for (int z = 0; z < b.cols; z++) {
					result += b.at<double>(b.rows - k - 1, b.cols - z - 1) * a.at<uchar>(i + k, j + z);
				}
			}
			ans.at<double>(i, j) = result;
		}
	}
	return ans;
}

Mat binary(Mat img, double t) {

	for (int height = 0; height < img.rows; height++) {
		double *data = img.ptr<double>(height);
		for (int width = 0; width < img.cols; width++) {
			data[width] = data[width] < t ? 255 : 0;
		}
	}

	return img;
}

Mat LaplaceMask1(Mat img) {
	double A[3][3] = { {0,1,0},
						{1,-4,1},
						{0,1,0} };

	Mat mask(3, 3, CV_64F, Scalar(0), A);
	Mat result = binary(convolve(img, mask), 15);
	return result;
}

Mat LaplaceMask2(Mat img) {
	double A[3][3] = { {1. / 3,1. / 3,1. / 3},
					{1. / 3,-8. / 3,1. / 3},
					{1. / 3,1. / 3,1. / 3} };
	Mat mask(3, 3, CV_64F, Scalar(0), A);
	Mat result = binary(convolve(img, mask), 15);
	return result;
}

Mat MinimumVarianceLaplacian(Mat img) {
	double m[11][11] = { {2, -1, 2},
						{-1, -4, -1},
						{2, -1, 2} };
	Mat mask(3, 3, CV_64F, Scalar(0),m);
	Mat result = binary(convolve(img, mask), 20);
	return result;
}

Mat LaplaceOfGaussian(Mat img) {
	
	double m[11][11] = { {0, 0, 0, -1, -1, -2, -1, -1, 0, 0, 0},
		{0, 0, -2, -4, -8, -9, -8, -4, -2, 0, 0},
		{0, -2, -7, -15, -22, -23, -22, -15, -7, -2, 0},
		{-1, -4, -15, -24, -14, -1, -14, -24, -15, -4, -1},
		{-1, -8, -22, -14, 52, 103, 52, -14, -22, -8, -1},
		{-2, -9, -23, -1, 103, 178, 103, -1, -23, -9, -2},
		{-1, -8, -22, -14, 52, 103, 52, -14, -22, -8, -1},
		{-1, -4, -15, -24, -14, -1, -14, -24, -15, -4, -1},
		{0, -2, -7, -15, -22, -23, -22, -15, -7, -2, 0},
		{0, 0, -2, -4, -8, -9, -8, -4, -2, 0, 0},
		{0, 0, 0, -1, -1, -2, -1, -1, 0, 0, 0} };

	Mat mask(11, 11, CV_64F, Scalar(0), m);
	Mat result = binary(convolve(img, mask), 3000);
	return result;

}

Mat DifferenceOfGaussian(Mat img) {
	double m[11][11] = { {-1, -3, -4, -6, -7, -8, -7, -6, -4, -3, -1},
		{-3, -5, -8, -11, -13, -13, -13, -11, -8, -5, -3},
		{-4, -8, -12, -16, -17, -17, -17, -16, -12, -8, -4},
		{-6, -11, -16, -16, 0, 15, 0, -16, -16, -11, -6},
		{-7, -13, -17, 0, 85, 160, 85, 0, -17, -13, -7},
		{-8, -13, -17, 15, 160, 283, 160, 15, -17, -13, -8},
		{-7, -13, -17, 0, 85, 160, 85, 0, -17, -13, -7},
		{-6, -11, -16, -16, 0, 15, 0, -16, -16, -11, -6},
		{-4, -8, -12, -16, -17, -17, -17, -16, -12, -8, -4},
		{-3, -5, -8, -11, -13, -13, -13, -11, -8, -5, -3},
		{-1, -3, -4, -6, -7, -8, -7, -6, -4, -3, -1}, };

	Mat mask(11, 11, CV_64F, Scalar(0), m);
	Mat result = convolve(img, mask);

	for (int height = 0; height < result.rows; height++) {
		double *data = result.ptr<double>(height);
		for (int width = 0; width < result.cols; width++) {
			data[width] = data[width] < 1 ? 0 : 255;
		}
	}

	return result;
}


int main()
{
	//讀圖
	Mat img = imread("lena.bmp", CV_LOAD_IMAGE_GRAYSCALE);

	//Mat laplaceMask1 = LaplaceMask1(img);
	//Mat laplaceMask2 = LaplaceMask2(img);
	//Mat minimumVarianceLaplacian = MinimumVarianceLaplacian(img);
	//Mat laplaceOfGaussian = LaplaceOfGaussian(img);
	Mat differenceOfGaussian = DifferenceOfGaussian(img);

	//imwrite("LaplaceMask1.bmp", laplaceMask1);
	//imwrite("LaplaceMask2.bmp", laplaceMask2);
	//imwrite("MinimumVarianceLaplacian.bmp", minimumVarianceLaplacian);
	//imwrite("LaplaceOfGaussian.bmp", laplaceOfGaussian);
	imwrite("DifferenceOfGaussian.bmp", differenceOfGaussian);

}