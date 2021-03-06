#include <iostream>
#include <opencv2/opencv.hpp>

#define THREASHOLD 128

using namespace std;
using namespace cv;

Mat Binary(Mat img, uchar t, int widthLimit, int heightLimit);
template<typename T, size_t M, size_t N> Mat BinaryDilation(Mat img, T(&kernel)[M][N], int widthLimit, int heightLimit);
template<typename T, size_t M, size_t N> Mat BinaryErosion(Mat img, T(&kernel)[M][N], int widthLimit, int heightLimit);
template<typename T, size_t M, size_t N>Mat BinaryOpening(Mat img, T(&kernel)[M][N], int widthLimit, int heightLimit);
template<typename T, size_t M, size_t N>Mat BinaryClosing(Mat img, T(&kernel)[M][N], int widthLimit, int heightLimit);
template<typename T, size_t M, size_t N>Mat BinaryHitAndMiss(Mat img, T(&J_kernel)[M][N], T(&K_kernel)[M][N], int widthLimit, int heightLimit);

int main()
{
	//讀圖
	Mat img = imread("lena.bmp", CV_LOAD_IMAGE_GRAYSCALE);

	int widthLimit = img.cols;	//圖片寬
	int heightLimit = img.rows;	//圖片高

	//設定閾值
	uchar t = THREASHOLD;

	//以Threashold進行二值化
	Mat b_img = Binary(img, t, widthLimit, heightLimit);

	//kernel is a 3-5-5-5-3 octagon, where the orgin is at the center
	//[0]代表y與原點差距，[1]代表x與原點差距
	int kernel[21][2] = { {-2, -1}, {-2, 0}, {-2, 1},
					{-1, -2}, {-1, -1}, {-1, 0}, {-1, 1}, {-1, 2},
					{0, -2}, {0, -1}, {0, 0}, {0, 1}, {0, 2},
					{1, -2}, {1, -1}, {1, 0}, {1, 1}, {1, 2},
					{2, -1}, {2, 0}, {2, 1} };

	//進行Binary Dilation
	Mat b_dilation = BinaryDilation(b_img, kernel, widthLimit, heightLimit);

	//進行Binary Erosion
	Mat b_erosion = BinaryErosion(b_img, kernel, widthLimit, heightLimit);

	//進行Binary Opening
	Mat b_opening = BinaryOpening(b_img, kernel, widthLimit, heightLimit);

	//進行Binary Opening
	Mat b_closing = BinaryClosing(b_img, kernel, widthLimit, heightLimit);

	// use the "L" shaped kernel (same as the text book) to detect the upper-right corner for hit-and-miss transform.
	int J_kernel[3][2] = { {0, -1}, {0, 0}, {1, 0} };
	int K_kernel[3][2] = { {-1, 0}, {-1, 1}, {0, 1} };

	//進行Hit and Miss transform
	Mat b_HaM = BinaryHitAndMiss(b_img, J_kernel, K_kernel, widthLimit, heightLimit);

	//秀圖
	/*namedWindow("binDisplay", WINDOW_AUTOSIZE);
	imshow("binDisplay", b_HaM);
	waitKey(0);
	destroyAllWindows();*/

	//存圖
	imwrite("dilation.bmp", b_dilation);
	imwrite("erosion.bmp", b_erosion);
	imwrite("opening.bmp", b_opening);
	imwrite("closing.bmp", b_closing);
	imwrite("HaM.bmp", b_HaM);

	return 0;

}

Mat Binary(Mat img, uchar t, int widthLimit, int heightLimit) {
	
	for (int height = 0; height < heightLimit; height++) {
		uchar *data = img.ptr<uchar>(height);
		for (int width = 0; width < widthLimit; width++) {
			//超過theashod設255，其餘設0
			data[width] = data[width] < t ? 0 : 255;
		}
	}

	return img;
}

//利用樣板來特化函式，以利二維陣列的傳入
template<typename T, size_t M, size_t N>
Mat BinaryDilation(Mat img, T (&kernel)[M][N], int widthLimit, int heightLimit) {
	//創建新圖準備處理
	Mat result(heightLimit, widthLimit, CV_8U, Scalar(0));

	for (int height = 0; height < heightLimit; height++) {
		for (int width = 0; width < widthLimit; width++) {

			if (img.at<uchar>(height,width) > 0) {
				for (int i = 0; i < M; i += 1) {
					if (kernel[i][0] + height >= 0 && kernel[i][0] + height < heightLimit
						&& kernel[i][1] + width >= 0 && kernel[i][1] + width < widthLimit)
						result.at<uchar>(kernel[i][0] + height, kernel[i][1] + width) = 255;
				}
			}
		}
	}

	return result;
}


template<typename T, size_t M, size_t N>
Mat BinaryErosion(Mat img, T(&kernel)[M][N], int widthLimit, int heightLimit) {
	//創建新圖準備處理
	Mat result(heightLimit, widthLimit, CV_8U, Scalar(0));

	for (int height = 0; height < heightLimit; height++) {
		for (int width = 0; width < widthLimit; width++) {

			bool exist;

			if (img.at<uchar>(height, width) > 0) {
				exist = true;

				for (int i = 0; i < M; i += 1) {
					if (kernel[i][0] + height < 0 || kernel[i][0] + height >= heightLimit
						|| kernel[i][1] + width < 0 || kernel[i][1] + width >= widthLimit
						|| img.at<uchar>(kernel[i][0] + height, kernel[i][1] + width) == 0) {
						exist = false;
						break;
					}
				}

				if (exist)
					result.at<uchar>(height, width) = 255;
			} 
		}
	}

	return result;
}

template<typename T, size_t M, size_t N>
Mat BinaryOpening(Mat img, T(&kernel)[M][N], int widthLimit, int heightLimit) {
	return BinaryDilation(BinaryErosion(img, kernel, widthLimit, heightLimit), kernel, widthLimit, heightLimit);
}

template<typename T, size_t M, size_t N>
Mat BinaryClosing(Mat img, T(&kernel)[M][N], int widthLimit, int heightLimit) {
	return BinaryErosion(BinaryDilation(img, kernel, widthLimit, heightLimit), kernel, widthLimit, heightLimit);
}

template<typename T, size_t M, size_t N>
Mat BinaryHitAndMiss(Mat img, T(&J_kernel)[M][N], T(&K_kernel)[M][N], int widthLimit, int heightLimit) {
	//創建新圖準備處理
	Mat result(heightLimit, widthLimit, CV_8U, Scalar(255));
	
	//算出補圖
	Mat complement(heightLimit, widthLimit, CV_8U, Scalar(0));

	for (int height = 0; height < heightLimit; height++)
		for (int width = 0; width < widthLimit; width++)
			complement.at<uchar>(height, width) = 255 - img.at<uchar>(height, width);

	//進行Binary Dilation
	Mat img_erosion = BinaryErosion(img, J_kernel, widthLimit, heightLimit);

	//進行Binary Erosion
	Mat com_erosion = BinaryErosion(complement, K_kernel, widthLimit, heightLimit);

	for (int height = 0; height < heightLimit; height++)
		for (int width = 0; width < widthLimit; width++) {
			if (img_erosion.at<uchar>(height, width) == 0 && com_erosion.at<uchar>(height, width) == 0)
				result.at<uchar>(height, width) = 0;
		}

	return result;
}