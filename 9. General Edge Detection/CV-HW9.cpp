#include <iostream>
#include <opencv2/opencv.hpp>
#include <cmath>

using namespace std;
using namespace cv;

int convolve(Mat a, Mat b) {
	
	int result = 0;

	for (int i = 0; i < a.rows; i++) {
		for (int j = 0; j < a.cols; j++) {
			result += int(a.at<uchar>(i, j)) * int(b.at<char>(b.rows - i - 1, b.cols - j - 1));
		}
	}
	return result;
}

double convolve2(Mat a, Mat b) {

	double result = 0;

	for (int i = 0; i < a.rows; i++) {
		for (int j = 0; j < a.cols; j++) {
			result += double(a.at<uchar>(i, j)) * b.at<double>(b.rows - i - 1, b.cols - j - 1);
		}
	}
	return result;
}

Mat Roberts(Mat img) {

	Mat mask1(2, 2, CV_8S, Scalar(0));
	Mat mask2(2, 2, CV_8S, Scalar(0));

	mask1.at<char>(0, 0) = 1;
	mask1.at<char>(1, 1) = -1;

	mask2.at<char>(0, 1) = 1;
	mask2.at<char>(1, 0) = -1;

	Mat Gx(img.rows - 1, img.cols - 1, CV_32S, Scalar(0));
	Mat Gy(img.rows - 1, img.cols - 1, CV_32S, Scalar(0));

	for (int i = 0; i < img.rows - 1; i++) {
		for (int j = 0; j < img.rows - 1; j++) {
			Rect r(j, i, 2, 2);
			Gx.at<int>(i, j) = convolve(img(r), mask1);
			Gy.at<int>(i, j) = convolve(img(r), mask2);
		}
	}
	Mat G1(img.rows - 1, img.cols - 1, CV_32S, Scalar(0));
	Mat G2(img.rows - 1, img.cols - 1, CV_32S, Scalar(0));
	pow(Gx, 2, G1);
	pow(Gy, 2, G2);
	Mat G(img.rows - 1, img.cols - 1, CV_32S, Scalar(0));
	G = G1 + G2;
	for (int i = 0; i < img.rows - 1; i++) {
		for (int j = 0; j < img.rows - 1; j++) {
			G.at<int>(i, j) = sqrt(G.at<int>(i, j));
			if (G.at<int>(i, j) > 12) G.at<int>(i, j) = 0;
			else G.at<int>(i, j) = 255;
		}
	}
	return G;
}

Mat Prewitt(Mat img) {
	Mat mask1(3, 3, CV_8S, Scalar(0));
	Mat mask2(3, 3, CV_8S, Scalar(0));

	mask1.at<char>(0, 0) = -1;
	mask1.at<char>(1, 0) = -1;
	mask1.at<char>(2, 0) = -1;
	mask1.at<char>(0, 2) = 1;
	mask1.at<char>(1, 2) = 1;
	mask1.at<char>(2, 2) = 1;

	mask2.at<char>(0, 0) = -1;
	mask2.at<char>(0, 1) = -1;
	mask2.at<char>(0, 2) = -1;
	mask2.at<char>(2, 0) = 1;
	mask2.at<char>(2, 1) = 1;
	mask2.at<char>(2, 2) = 1;

	Mat Gx(img.rows - 2, img.cols - 2, CV_32S, Scalar(0));
	Mat Gy(img.rows - 2, img.cols - 2, CV_32S, Scalar(0));

	for (int i = 0; i < img.rows - 2; i++) {
		for (int j = 0; j < img.rows - 2; j++) {
			Rect r(j, i, 3, 3);
			Gx.at<int>(i, j) = convolve(img(r), mask1);
			Gy.at<int>(i, j) = convolve(img(r), mask2);
		}
	}
	Mat G1(img.rows - 2, img.cols - 2, CV_32S, Scalar(0));
	Mat G2(img.rows - 2, img.cols - 2, CV_32S, Scalar(0));
	pow(Gx, 2, G1);
	pow(Gy, 2, G2);
	Mat G(img.rows - 2, img.cols - 2, CV_32S, Scalar(0));
	G = G1 + G2;
	for (int i = 0; i < img.rows - 2; i++) {
		for (int j = 0; j < img.rows - 2; j++) {
			G.at<int>(i, j) = sqrt(G.at<int>(i, j));
			if (G.at<int>(i, j) > 24) G.at<int>(i, j) = 0;
			else G.at<int>(i, j) = 255;
		}
	}
	return G;
}


Mat Sobel(Mat img) {
	Mat mask1(3, 3, CV_8S, Scalar(0));
	Mat mask2(3, 3, CV_8S, Scalar(0));

	mask1.at<char>(0, 0) = -1;
	mask1.at<char>(1, 0) = -2;
	mask1.at<char>(2, 0) = -1;
	mask1.at<char>(0, 2) = 1;
	mask1.at<char>(1, 2) = 2;
	mask1.at<char>(2, 2) = 1;

	mask2.at<char>(0, 0) = -1;
	mask2.at<char>(0, 1) = -2;
	mask2.at<char>(0, 2) = -1;
	mask2.at<char>(2, 0) = 1;
	mask2.at<char>(2, 1) = 2;
	mask2.at<char>(2, 2) = 1;

	Mat Gx(img.rows - 2, img.cols - 2, CV_32S, Scalar(0));
	Mat Gy(img.rows - 2, img.cols - 2, CV_32S, Scalar(0));

	for (int i = 0; i < img.rows - 2; i++) {
		for (int j = 0; j < img.rows - 2; j++) {
			Rect r(j, i, 3, 3);
			Gx.at<int>(i, j) = convolve(img(r), mask1);
			Gy.at<int>(i, j) = convolve(img(r), mask2);
		}
	}
	Mat G1(img.rows - 2, img.cols - 2, CV_32S, Scalar(0));
	Mat G2(img.rows - 2, img.cols - 2, CV_32S, Scalar(0));
	pow(Gx, 2, G1);
	pow(Gy, 2, G2);
	Mat G(img.rows - 2, img.cols - 2, CV_32S, Scalar(0));
	G = G1 + G2;
	for (int i = 0; i < img.rows - 2; i++) {
		for (int j = 0; j < img.rows - 2; j++) {
			G.at<int>(i, j) = sqrt(G.at<int>(i, j));
			if (G.at<int>(i, j) > 38) G.at<int>(i, j) = 0;
			else G.at<int>(i, j) = 255;
		}
	}
	return G;
}

Mat Frei(Mat img) {
	Mat mask1(3, 3, CV_64F, Scalar(0));
	Mat mask2(3, 3, CV_64F, Scalar(0));

	mask1.at<double>(0, 0) = -1;
	mask1.at<double>(0, 1) = -sqrt(2);
	mask1.at<double>(0, 2) = -1;
	mask1.at<double>(2, 0) = 1;
	mask1.at<double>(2, 1) = sqrt(2);
	mask1.at<double>(2, 2) = 1;

	mask2.at<double>(0, 0) = -1;
	mask2.at<double>(0, 1) = -0;
	mask2.at<double>(0, 2) = 1;
	mask2.at<double>(1, 0) = -sqrt(2);
	mask2.at<double>(1, 2) = sqrt(2);
	mask2.at<double>(2, 0) = -1;
	mask2.at<double>(2, 1) = 0;
	mask2.at<double>(2, 2) = 1;

	Mat Gx(img.rows - 2, img.cols - 2, CV_64F, Scalar(0));
	Mat Gy(img.rows - 2, img.cols - 2, CV_64F, Scalar(0));

	for (int i = 0; i < img.rows - 2; i++) {
		for (int j = 0; j < img.rows - 2; j++) {
			Rect r(j, i, 3, 3);
			Gx.at<double>(i, j) = convolve2(img(r), mask1);
			Gy.at<double>(i, j) = convolve2(img(r), mask2);
		}
	}
	Mat G1(img.rows - 2, img.cols - 2, CV_64F, Scalar(0));
	Mat G2(img.rows - 2, img.cols - 2, CV_64F, Scalar(0));
	pow(Gx, 2, G1);
	pow(Gy, 2, G2);
	Mat G(img.rows - 2, img.cols - 2, CV_64F, Scalar(0));
	G = G1 + G2;
	for (int i = 0; i < img.rows - 2; i++) {
		for (int j = 0; j < img.rows - 2; j++) {
			G.at<double>(i, j) = sqrt(G.at<double>(i, j));
			if (G.at<double>(i, j) > 30) G.at<double>(i, j) = 0;
			else G.at<double>(i, j) = 255;
		}
	}
	return G;
}

Mat Kirsch(Mat img) {
	Mat k0(3, 3, CV_8S, Scalar(0));
	Mat k1(3, 3, CV_8S, Scalar(0));
	Mat k2(3, 3, CV_8S, Scalar(0));
	Mat k3(3, 3, CV_8S, Scalar(0));
	Mat k4(3, 3, CV_8S, Scalar(0));
	Mat k5(3, 3, CV_8S, Scalar(0));
	Mat k6(3, 3, CV_8S, Scalar(0));
	Mat k7(3, 3, CV_8S, Scalar(0));

	k0.at<char>(0, 0) = -3; k0.at<char>(0, 1) = -3; k0.at<char>(0, 2) = 5;
	k0.at<char>(1, 0) = -3; k0.at<char>(1, 1) = 0; k0.at<char>(1, 2) = 5;
	k0.at<char>(2, 0) = -3; k0.at<char>(2, 1) = -3; k0.at<char>(2, 2) = 5;

	k1.at<char>(0, 0) = -3; k1.at<char>(0, 1) = 5; k1.at<char>(0, 2) = 5;
	k1.at<char>(1, 0) = -3; k1.at<char>(1, 1) = 0; k1.at<char>(1, 2) = 5;
	k1.at<char>(2, 0) = -3; k1.at<char>(2, 1) = -3; k1.at<char>(2, 2) = -3;

	k2.at<char>(0, 0) = 5; k2.at<char>(0, 1) = 5; k2.at<char>(0, 2) = 5;
	k2.at<char>(1, 0) = -3; k2.at<char>(1, 1) = 0; k2.at<char>(1, 2) = -3;
	k2.at<char>(2, 0) = -3; k2.at<char>(2, 1) = -3; k2.at<char>(2, 2) = -3;

	k3.at<char>(0, 0) = 5; k3.at<char>(0, 1) = 5; k3.at<char>(0, 2) = -3;
	k3.at<char>(1, 0) = 5; k3.at<char>(1, 1) = 0; k3.at<char>(1, 2) = -3;
	k3.at<char>(2, 0) = -3; k3.at<char>(2, 1) = -3; k3.at<char>(2, 2) = -3;

	k4.at<char>(0, 0) = 5; k4.at<char>(0, 1) = -3; k4.at<char>(0, 2) = -3;
	k4.at<char>(1, 0) = 5; k4.at<char>(1, 1) = 0; k4.at<char>(1, 2) = -3;
	k4.at<char>(2, 0) = 5; k4.at<char>(2, 1) = -3; k4.at<char>(2, 2) = -3;

	k5.at<char>(0, 0) = -3; k5.at<char>(0, 1) = -3; k5.at<char>(0, 2) = -3;
	k5.at<char>(1, 0) = 5; k5.at<char>(1, 1) = 0; k5.at<char>(1, 2) = -3;
	k5.at<char>(2, 0) = 5; k5.at<char>(2, 1) = 5; k5.at<char>(2, 2) = -3;

	k6.at<char>(0, 0) = -3; k6.at<char>(0, 1) = -3; k6.at<char>(0, 2) = -3;
	k6.at<char>(1, 0) = -3; k6.at<char>(1, 1) = 0; k6.at<char>(1, 2) = -3;
	k6.at<char>(2, 0) = 5; k6.at<char>(2, 1) = 5; k6.at<char>(2, 2) = 5;

	k7.at<char>(0, 0) = -3; k7.at<char>(0, 1) = -3; k7.at<char>(0, 2) = -3;
	k7.at<char>(1, 0) = -3; k7.at<char>(1, 1) = 0; k7.at<char>(1, 2) = 5;
	k7.at<char>(2, 0) = -3; k7.at<char>(2, 1) = 5; k7.at<char>(2, 2) = 5;

	Mat G(img.rows - 2, img.cols - 2, CV_32S, Scalar(0));

	for (int i = 0; i < img.rows - 2; i++) {
		for (int j = 0; j < img.rows - 2; j++) {
			Rect r(j, i, 3, 3);
			int g[7];
			g[0] = convolve(img(r), k0);
			g[1] = convolve(img(r), k1);
			g[2] = convolve(img(r), k2);
			g[3] = convolve(img(r), k3);
			g[4] = convolve(img(r), k4);
			g[5] = convolve(img(r), k5);
			g[6] = convolve(img(r), k6);
			int _max = convolve(img(r), k7);

			for (int k = 1; k < 7; k++) {
				_max = max(_max,g[k]);
			}

			G.at<int>(i, j) = (_max > 135) ? 0 : 255;
		}
	}

	return G;
}

Mat Robinson(Mat img) {
	Mat k0(3, 3, CV_8S, Scalar(0));
	Mat k1(3, 3, CV_8S, Scalar(0));
	Mat k2(3, 3, CV_8S, Scalar(0));
	Mat k3(3, 3, CV_8S, Scalar(0));
	Mat k4(3, 3, CV_8S, Scalar(0));
	Mat k5(3, 3, CV_8S, Scalar(0));
	Mat k6(3, 3, CV_8S, Scalar(0));
	Mat k7(3, 3, CV_8S, Scalar(0));

	k0.at<char>(0, 0) = -1; k0.at<char>(0, 1) = 0; k0.at<char>(0, 2) = 1;
	k0.at<char>(1, 0) = -2; k0.at<char>(1, 1) = 0; k0.at<char>(1, 2) = 2;
	k0.at<char>(2, 0) = -1; k0.at<char>(2, 1) = 0; k0.at<char>(2, 2) = 1;

	k1.at<char>(0, 0) = 0; k1.at<char>(0, 1) = 1; k1.at<char>(0, 2) = 2;
	k1.at<char>(1, 0) = -1; k1.at<char>(1, 1) = 0; k1.at<char>(1, 2) = 1;
	k1.at<char>(2, 0) = -2; k1.at<char>(2, 1) = -1; k1.at<char>(2, 2) = 0;

	k2.at<char>(0, 0) = 1; k2.at<char>(0, 1) = 2; k2.at<char>(0, 2) = 1;
	k2.at<char>(1, 0) = 0; k2.at<char>(1, 1) = 0; k2.at<char>(1, 2) = 0;
	k2.at<char>(2, 0) = -1; k2.at<char>(2, 1) = -2; k2.at<char>(2, 2) = -1;

	k3.at<char>(0, 0) = 2; k3.at<char>(0, 1) = 1; k3.at<char>(0, 2) = 0;
	k3.at<char>(1, 0) = 1; k3.at<char>(1, 1) = 0; k3.at<char>(1, 2) = -1;
	k3.at<char>(2, 0) = 0; k3.at<char>(2, 1) = -1; k3.at<char>(2, 2) = -2;

	k4.at<char>(0, 0) = 1; k4.at<char>(0, 1) = 0; k4.at<char>(0, 2) = -1;
	k4.at<char>(1, 0) = 2; k4.at<char>(1, 1) = 0; k4.at<char>(1, 2) = -2;
	k4.at<char>(2, 0) = 1; k4.at<char>(2, 1) = 0; k4.at<char>(2, 2) = -1;

	k5.at<char>(0, 0) = 0; k5.at<char>(0, 1) = -1; k5.at<char>(0, 2) = -2;
	k5.at<char>(1, 0) = 1; k5.at<char>(1, 1) = 0; k5.at<char>(1, 2) = -1;
	k5.at<char>(2, 0) = 2; k5.at<char>(2, 1) = 1; k5.at<char>(2, 2) = 0;

	k6.at<char>(0, 0) = -1; k6.at<char>(0, 1) = -2; k6.at<char>(0, 2) = -1;
	k6.at<char>(1, 0) = 0; k6.at<char>(1, 1) = 0; k6.at<char>(1, 2) = 0;
	k6.at<char>(2, 0) = 1; k6.at<char>(2, 1) = 2; k6.at<char>(2, 2) = 1;

	k7.at<char>(0, 0) = -2; k7.at<char>(0, 1) = -1; k7.at<char>(0, 2) = 0;
	k7.at<char>(1, 0) = -1; k7.at<char>(1, 1) = 0; k7.at<char>(1, 2) = 1;
	k7.at<char>(2, 0) = 0; k7.at<char>(2, 1) = 1; k7.at<char>(2, 2) = 2;

	Mat G(img.rows - 2, img.cols - 2, CV_32S, Scalar(0));

	for (int i = 0; i < img.rows - 2; i++) {
		for (int j = 0; j < img.rows - 2; j++) {
			Rect r(j, i, 3, 3);
			int g[7];
			g[0] = convolve(img(r), k0);
			g[1] = convolve(img(r), k1);
			g[2] = convolve(img(r), k2);
			g[3] = convolve(img(r), k3);
			g[4] = convolve(img(r), k4);
			g[5] = convolve(img(r), k5);
			g[6] = convolve(img(r), k6);
			int _max = convolve(img(r), k7);

			for (int k = 1; k < 7; k++) {
				_max = max(_max, g[k]);
			}

			G.at<int>(i, j) = (_max > 43) ? 0 : 255;
		}
	}

	return G;
}

Mat Nevatia(Mat img) {
	Mat k0(5, 5, CV_8S, Scalar(0));
	Mat k1(5, 5, CV_8S, Scalar(0));
	Mat k2(5, 5, CV_8S, Scalar(0));
	Mat k3(5, 5, CV_8S, Scalar(0));
	Mat k4(5, 5, CV_8S, Scalar(0));
	Mat k5(5, 5, CV_8S, Scalar(0));

	k0.at<char>(0, 0) = 100; k0.at<char>(0, 1) = 100; k0.at<char>(0, 2) = 100; k0.at<char>(0, 3) = 100; k0.at<char>(0, 4) = 100;
	k0.at<char>(1, 0) = 100; k0.at<char>(1, 1) = 100; k0.at<char>(1, 2) = 100; k0.at<char>(1, 3) = 100; k0.at<char>(1, 4) = 100;
	k0.at<char>(2, 0) = 0; k0.at<char>(2, 1) = 0; k0.at<char>(2, 2) = 0; k0.at<char>(2, 3) = 0; k0.at<char>(2, 4) = 0;
	k0.at<char>(3, 0) = -100; k0.at<char>(3, 1) = -100; k0.at<char>(3, 2) = -100; k0.at<char>(3, 3) = -100; k0.at<char>(3, 4) = -100;
	k0.at<char>(4, 0) = -100; k0.at<char>(4, 1) = -100; k0.at<char>(4, 2) = -100; k0.at<char>(4, 3) = -100; k0.at<char>(4, 4) = -100;

	k1.at<char>(0, 0) = 100; k1.at<char>(0, 1) = 100; k1.at<char>(0, 2) = 100; k1.at<char>(0, 3) = 100; k1.at<char>(0, 4) = 100;
	k1.at<char>(1, 0) = 100; k1.at<char>(1, 1) = 100; k1.at<char>(1, 2) = 100; k1.at<char>(1, 3) = 78; k1.at<char>(1, 4) = -32;
	k1.at<char>(2, 0) = 100; k1.at<char>(2, 1) = 92; k1.at<char>(2, 2) = 0; k1.at<char>(2, 3) = -92; k1.at<char>(2, 4) = -100;
	k1.at<char>(3, 0) = 32; k1.at<char>(3, 1) = -78; k1.at<char>(3, 2) = -100; k1.at<char>(3, 3) = -100; k1.at<char>(3, 4) = -100;
	k1.at<char>(4, 0) = -100; k1.at<char>(4, 1) = -100; k1.at<char>(4, 2) = -100; k1.at<char>(4, 3) = -100; k1.at<char>(4, 4) = -100;

	k2.at<char>(0, 0) = 100; k2.at<char>(0, 1) = 100; k2.at<char>(0, 2) = 100; k2.at<char>(0, 3) = 32; k2.at<char>(0, 4) = -100;
	k2.at<char>(1, 0) = 100; k2.at<char>(1, 1) = 100; k2.at<char>(1, 2) = 92; k2.at<char>(1, 3) = -78; k2.at<char>(1, 4) = -100;
	k2.at<char>(2, 0) = 100; k2.at<char>(2, 1) = 100; k2.at<char>(2, 2) = 0; k2.at<char>(2, 3) = -100; k2.at<char>(2, 4) = -100;
	k2.at<char>(3, 0) = 100; k2.at<char>(3, 1) = 78; k2.at<char>(3, 2) = -92; k2.at<char>(3, 3) = -100; k2.at<char>(3, 4) = -100;
	k2.at<char>(4, 0) = 100; k2.at<char>(4, 1) = -32; k2.at<char>(4, 2) = -100; k2.at<char>(4, 3) = -100; k2.at<char>(4, 4) = -100;

	k3.at<char>(0, 0) = -100; k3.at<char>(0, 1) = -100; k3.at<char>(0, 2) = 0; k3.at<char>(0, 3) = 100; k3.at<char>(0, 4) = 100;
	k3.at<char>(1, 0) = -100; k3.at<char>(1, 1) = -100; k3.at<char>(1, 2) = 0; k3.at<char>(1, 3) = 100; k3.at<char>(1, 4) = 100;
	k3.at<char>(2, 0) = -100; k3.at<char>(2, 1) = -100; k3.at<char>(2, 2) = 0; k3.at<char>(2, 3) = 100; k3.at<char>(2, 4) = 100;
	k3.at<char>(3, 0) = -100; k3.at<char>(3, 1) = -100; k3.at<char>(3, 2) = 0; k3.at<char>(3, 3) = 100; k3.at<char>(3, 4) = 100;
	k3.at<char>(4, 0) = -100; k3.at<char>(4, 1) = -100; k3.at<char>(4, 2) = 0; k3.at<char>(4, 3) = 100; k3.at<char>(4, 4) = 100;

	k4.at<char>(0, 0) = -100; k4.at<char>(0, 1) = 32; k4.at<char>(0, 2) = 100; k4.at<char>(0, 3) = 100; k4.at<char>(0, 4) = 100;
	k4.at<char>(1, 0) = -100; k4.at<char>(1, 1) = -78; k4.at<char>(1, 2) = 92; k4.at<char>(1, 3) = 100; k4.at<char>(1, 4) = 100;
	k4.at<char>(2, 0) = -100; k4.at<char>(2, 1) = -100; k4.at<char>(2, 2) = 0; k4.at<char>(2, 3) = 100; k4.at<char>(2, 4) = 100;
	k4.at<char>(3, 0) = -100; k4.at<char>(3, 1) = -100; k4.at<char>(3, 2) = -92; k4.at<char>(3, 3) = 78; k4.at<char>(3, 4) = 100;
	k4.at<char>(4, 0) = -100; k4.at<char>(4, 1) = -100; k4.at<char>(4, 2) = -100; k4.at<char>(4, 3) = -32; k4.at<char>(4, 4) = 100;

	k5.at<char>(0, 0) = 100; k5.at<char>(0, 1) = 100; k5.at<char>(0, 2) = 100; k5.at<char>(0, 3) = 100; k5.at<char>(0, 4) = 100;
	k5.at<char>(1, 0) = -32; k5.at<char>(1, 1) = 78; k5.at<char>(1, 2) = 100; k5.at<char>(1, 3) = 100; k5.at<char>(1, 4) = 100;
	k5.at<char>(2, 0) = -100; k5.at<char>(2, 1) = -92; k5.at<char>(2, 2) = 0; k5.at<char>(2, 3) = 92; k5.at<char>(2, 4) = 100;
	k5.at<char>(3, 0) = -100; k5.at<char>(3, 1) = -100; k5.at<char>(3, 2) = -100; k5.at<char>(3, 3) = -78; k5.at<char>(3, 4) = 32;
	k5.at<char>(4, 0) = -100; k5.at<char>(4, 1) = -100; k5.at<char>(4, 2) = -100; k5.at<char>(4, 3) = -100; k5.at<char>(4, 4) = -100;

	Mat G(img.rows - 4, img.cols - 4, CV_32S, Scalar(0));

	for (int i = 0; i < img.rows - 4; i++) {
		for (int j = 0; j < img.rows - 4; j++) {
			Rect r(j, i, 5, 5);
			int g[5];
			g[0] = convolve(img(r), k0);
			g[1] = convolve(img(r), k1);
			g[2] = convolve(img(r), k2);
			g[3] = convolve(img(r), k3);
			g[4] = convolve(img(r), k4);
			int _max = convolve(img(r), k5);

			for (int k = 1; k < 5; k++) {
				_max = max(_max, g[k]);
			}

			G.at<int>(i, j) = (_max > 12500) ? 0 : 255;
		}
	}

	return G;
}

int main()
{
	//讀圖
	Mat img = imread("lena.bmp", CV_LOAD_IMAGE_GRAYSCALE);
	int widthLimit = img.cols;
	int heightLimit = img.rows;

	//Mat Roberts_12 = Roberts(img);
	//Mat Prewitt_24 = Prewitt(img);
	//Mat Sobel_38 = Sobel(img);
	//Mat Frei_30 = Frei(img);
	//Mat Kirsch_135 = Kirsch(img);
	//Mat Robinson_43 = Robinson(img);
	Mat Nevatia_12500 = Nevatia(img);

	//imwrite("Roberts_12.bmp", Roberts_12);
	//imwrite("Prewitt_24.bmp", Prewitt_24);
	//imwrite("Sobel_38.bmp", Sobel_38);
	//imwrite("Robinson_43.bmp", Robinson_43);
	imwrite("Nevatia_12500.bmp", Nevatia_12500);

}
