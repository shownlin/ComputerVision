#include <iostream>
#include <opencv2/opencv.hpp>

#define THREASHOLD 128

using namespace std;
using namespace cv;

template<typename T, size_t M, size_t N> Mat Dilation(Mat img, T(&kernel)[M][N], int widthLimit, int heightLimit);
template<typename T, size_t M, size_t N> Mat Erosion(Mat img, T(&kernel)[M][N], int widthLimit, int heightLimit);
template<typename T, size_t M, size_t N>Mat Opening(Mat img, T(&kernel)[M][N], int widthLimit, int heightLimit);
template<typename T, size_t M, size_t N>Mat Closing(Mat img, T(&kernel)[M][N], int widthLimit, int heightLimit);
int main()
{
	//讀圖
	Mat img = imread("lena.bmp", CV_LOAD_IMAGE_GRAYSCALE);

	int widthLimit = img.cols;	//圖片寬
	int heightLimit = img.rows;	//圖片高

	//設定閾值
	uchar t = THREASHOLD;

	//kernel is a 3-5-5-5-3 octagon, where the orgin is at the center
	//[0]代表y與原點差距，[1]代表x與原點差距
	int kernel[21][2] = { {-2, -1}, {-2, 0}, {-2, 1},
					{-1, -2}, {-1, -1}, {-1, 0}, {-1, 1}, {-1, 2},
					{0, -2}, {0, -1}, {0, 0}, {0, 1}, {0, 2},
					{1, -2}, {1, -1}, {1, 0}, {1, 1}, {1, 2},
					{2, -1}, {2, 0}, {2, 1} };

	//進行Dilation
	Mat dilation = Dilation(img, kernel, widthLimit, heightLimit);

	//進行Erosion
	Mat erosion = Erosion(img, kernel, widthLimit, heightLimit);

	//進行Opening
	Mat opening = Opening(img, kernel, widthLimit, heightLimit);

	//進行Opening
	Mat closing = Closing(img, kernel, widthLimit, heightLimit);

	//秀圖
	/*namedWindow("binDisplay", WINDOW_AUTOSIZE);
	imshow("binDisplay", closing);
	waitKey(0);
	destroyAllWindows();*/

	//存圖
	imwrite("dilation.bmp", dilation);
	imwrite("erosion.bmp", erosion);
	imwrite("opening.bmp", opening);
	imwrite("closing.bmp", closing);

	return 0;

}

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