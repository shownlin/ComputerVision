#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>

using namespace cv;
using namespace std;

int main()
{
	//讀圖，以灰階讀入
	Mat lena = imread("lena.BMP", CV_LOAD_IMAGE_GRAYSCALE);

	//先將原圖調暗1/3
	for (int i = 0; i < lena.cols; i++) {
		uchar *data = lena.ptr<uchar>(i);
		for (int j = 0; j < lena.cols; j++) {
			data[j] = data[j] / 3;
		}
	}

	vector <int> v(256, 0);	//v = {0,0,...0} 用來記錄每個itensity的強度

	for (int i = 0; i < lena.rows; i++) {
		for (int j = 0; j < lena.cols; j++) {
			v[lena.at<uchar>(i, j)]++;
		}
	}

	vector<int> s_k(256, 0);
	//計算Histogram Equalization
	double n = lena.rows*lena.cols;
	double temp = 0;
	for (int k = 0; k <= 255; k++) {
		temp += v[k] / n;
		/*for (int j = 0; j <= k; j++) {
			double n_j = v[j];
			temp += n_j / n;
		}*/
		s_k[k] = temp * 255;
	}


	Mat he(lena.rows, lena.cols, CV_8U, Scalar(0));

	for (int height = 0; height < he.cols; height++) {
		uchar *ref = lena.ptr<uchar>(height);
		uchar *data= he.ptr<uchar>(height);
		for (int width = 0; width < he.cols; width++) {
			data[width] = s_k[ref[width]];
		}
	}

	//秀圖
	//namedWindow("Display window", WINDOW_AUTOSIZE);
	//imshow("Display window", he);
	imwrite("HE.bmp", he);
	return 0;
}