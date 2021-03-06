#include <iostream>
#include <opencv2/opencv.hpp>
#include <random>
#include <cmath>
#include <io.h>
#include <direct.h>
#include <vector>
#include <algorithm>
#include <fstream>

using namespace std;
using namespace cv;

#include "HW5.h"

Mat Gaussian_noise(Mat img, int amplitude) {

	Mat result(img.rows, img.cols, CV_8U, Scalar(0));

	// random device class instance, source of 'true' randomness for initializing random seed
	random_device rd;
	// Mersenne twister PRNG, initialized with seed from previous random device instance
	mt19937 gen(rd());
	// standard deviation affects the dispersion of generated values from the mean
	normal_distribution<float> d(0, 1);

	for (int i = 0; i < img.rows; i++) {
		for (int j = 0; j < img.cols; j++) {
			result.at<uchar>(i, j) = img.at<uchar>(i, j) + amplitude * d(gen);
		}
	}

	return result;
}

Mat salt_and_pepper_noise(Mat img, double threshold) {

	Mat result = img.clone();

	random_device rd;  //Will be used to obtain a seed for the random number engine
	mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
	uniform_real_distribution<> dis(0, 1);

	for (int i = 0; i < img.rows; i++) {
		for (int j = 0; j < img.cols; j++) {
			double d = dis(gen);
			if (d < threshold)
				result.at<uchar>(i, j) = 0;
			else if (d>1- threshold)
				result.at<uchar>(i, j) = 255;
		}
	}

	return result;

}

Mat box_filter(Mat img, int f_size) {
	Mat result(img.rows - f_size + 1, img.cols - f_size + 1, CV_8U, Scalar(0));

	for(int i = 0; i < result.rows; i++) {
		for (int j = 0; j < result.cols; j++) {
			Rect r1(j, i, f_size, f_size);
			result.at<uchar>(i,j) = mean(img(r1))[0];
		}
	}
	return result;
}


double medianMat(Mat Input) {

	vector<int> v;
	for (int i = 0; i < Input.rows; i++) {
		for (int j = 0; j < Input.cols; j++) {
			v.push_back(Input.at<uchar>(i, j));
		}
	}

	nth_element(v.begin(), v.begin() + v.size() / 2, v.end());
	return v[v.size() / 2];
}

Mat median_filter(Mat img, int f_size) {
	Mat result(img.rows - f_size + 1, img.cols - f_size + 1, CV_8U, Scalar(0));
	for (int i = 0; i < result.rows; i++) {
		for (int j = 0; j < result.cols; j++) {
			Rect r1(j, i, f_size, f_size);
			result.at<uchar>(i, j) = medianMat(img(r1));
		}
	}
	return result;
}

void SNR(string s) {
	IplImage *src1 = cvLoadImage("lena.bmp");
	IplImage *src2 = cvLoadImage(s.c_str());

	long long int sigma = 0;
	long long int squre = 0;
	double MSE = 0.0;
	double SNR = 0.0;
	double PSNR = 0.0;
	int frameSize = src1->height*src1->width * 3;
	int blue1 = 0, blue2 = 0;
	int green1 = 0, green2 = 0;
	int red1 = 0, red2 = 0;

	// width x height -> [height][width]
	for (int i = 0; i < src1->height; i++) {
		for (int j = 0; j < src1->widthStep; j = j + 3) {
			blue1 = (int)(uchar)src1->imageData[i*src1->widthStep + j];//Blue
			green1 = (int)(uchar)src1->imageData[i*src1->widthStep + j + 1];//Green
			red1 = (int)(uchar)src1->imageData[i*src1->widthStep + j + 2];//Red
			blue2 = (int)(uchar)src2->imageData[i*src2->widthStep + j];//Blue
			green2 = (int)(uchar)src2->imageData[i*src2->widthStep + j + 1];//Green
			red2 = (int)(uchar)src2->imageData[i*src2->widthStep + j + 2];//Red
			sigma += (blue1 - blue2)*(blue1 - blue2) +
				(green1 - green2)*(green1 - green2) +
				(red1 - red2)*(red1 - red2);
			squre += blue1 * blue1 + green1 * green1 + red1 * red1;
		}
	}
	MSE = sigma / (double)frameSize;
	PSNR = 10 * log10(255 * 255 / MSE);
	SNR = 10 * log10(squre / sigma);

	/*
	cout << s << endl;
	cout << "sigma: " << sigma << endl;
	cout << "MSE: " << MSE << endl;
	cout << "PSNR: " << PSNR << endl;
	cout << "SNR: " << SNR << endl;
	*/

	//一次輸出一行資料到txt檔上
	string writeFileName = "SNR.txt";
	ofstream out(writeFileName.c_str(),ios::app);
	out << s << endl;
	out << "sigma: " << sigma << endl;
	out << "MSE: " << MSE << endl;
	out << "PSNR: " << PSNR << endl;
	out << "SNR: " << SNR << endl;
	out.close();

}

int main()
{
	//讀圖
	Mat img = imread("lena.bmp", CV_LOAD_IMAGE_GRAYSCALE);
	int widthLimit = img.cols;
	int heightLimit = img.rows;

	Mat gauss10 = Gaussian_noise(img, 10);
	Mat gauss30 = Gaussian_noise(img, 30);
	Mat sap005=salt_and_pepper_noise(img, 0.05);
	Mat sap01 = salt_and_pepper_noise(img, 0.1);

	//box
	Mat box3_gauss10 = box_filter(gauss10, 3);
	Mat box5_gauss10 = box_filter(gauss10, 5);
	Mat box3_gauss30 = box_filter(gauss30, 3);
	Mat box5_gauss30 = box_filter(gauss30, 5);
	Mat box3_sap005 = box_filter(sap005, 3);
	Mat box5_sap005 = box_filter(sap005, 5);
	Mat box3_sap01 = box_filter(sap01, 3);
	Mat box5_sap01 = box_filter(sap01, 5);

	//median
	Mat med3_gauss10 = median_filter(gauss10, 3);
	Mat med5_gauss10 = median_filter(gauss10, 5);
	Mat med3_gauss30 = median_filter(gauss30, 3);
	Mat med5_gauss30 = median_filter(gauss30, 5);
	Mat med3_sap005 = median_filter(sap005, 3);
	Mat med5_sap005 = median_filter(sap005, 5);
	Mat med3_sap01 = median_filter(sap01, 3);
	Mat med5_sap01 = median_filter(sap01, 5);

	//kernel is a 3-5-5-5-3 octagon, where the orgin is at the center
	//[0]代表y與原點差距，[1]代表x與原點差距
	int kernel[21][2] = { {-2, -1}, {-2, 0}, {-2, 1},
					{-1, -2}, {-1, -1}, {-1, 0}, {-1, 1}, {-1, 2},
					{0, -2}, {0, -1}, {0, 0}, {0, 1}, {0, 2},
					{1, -2}, {1, -1}, {1, 0}, {1, 1}, {1, 2},
					{2, -1}, {2, 0}, {2, 1} };

	Mat gauss10_open_close = Closing(Opening(gauss10, kernel, gauss10.cols, gauss10.rows), kernel, gauss10.cols, gauss10.rows);
	Mat gauss10_close_open = Opening(Closing(gauss10, kernel, gauss10.cols, gauss10.rows), kernel, gauss10.cols, gauss10.rows);
	Mat gauss30_open_close = Closing(Opening(gauss30, kernel, gauss30.cols, gauss30.rows), kernel, gauss30.cols, gauss30.rows);
	Mat gauss30_close_open = Opening(Closing(gauss30, kernel, gauss30.cols, gauss30.rows), kernel, gauss30.cols, gauss30.rows);

	Mat sap005_open_close = Closing(Opening(sap005, kernel, sap005.cols, sap005.rows), kernel, sap005.cols, sap005.rows);
	Mat sap005_close_open = Opening(Closing(sap005, kernel, sap005.cols, sap005.rows), kernel, sap005.cols, sap005.rows);
	Mat sap01_open_close = Closing(Opening(sap01, kernel, sap01.cols, sap01.rows), kernel, sap01.cols, sap01.rows);
	Mat sap01_close_open = Opening(Closing(sap01, kernel, sap01.cols, sap01.rows), kernel, sap01.cols, sap01.rows);

	//創建資料夾
	string dir[] = { "./gauss","./sap","./box","./median","./open&close" };
	for(string d:dir)
	if (_access(d.c_str(), 0) == -1) {
		_mkdir(d.c_str());
	}


	imwrite("gauss\\gauss10.bmp", gauss10);
	imwrite("gauss\\gauss30.bmp", gauss30);
	imwrite("sap\\sap005.bmp", sap005);
	imwrite("sap\\sap01.bmp", sap01);

	imwrite("box\\box3_gauss10.bmp", box3_gauss10);
	imwrite("box\\box5_gauss10.bmp", box5_gauss10);
	imwrite("box\\box3_gauss30.bmp", box3_gauss30);
	imwrite("box\\box5_gauss30.bmp", box5_gauss30);
	imwrite("box\\box3_sap005.bmp", box3_sap005);
	imwrite("box\\box5_sap005.bmp", box5_sap005);
	imwrite("box\\box3_sap01.bmp", box3_sap01);
	imwrite("box\\box5_sap01.bmp", box5_sap01);

	imwrite("median\\med3_gauss10.bmp", med3_gauss10);
	imwrite("median\\med5_gauss10.bmp", med5_gauss10);
	imwrite("median\\med3_gauss30.bmp", med3_gauss30);
	imwrite("median\\med5_gauss30.bmp", med5_gauss30);
	imwrite("median\\med3_sap005.bmp", med3_sap005);
	imwrite("median\\med5_sap005.bmp", med5_sap005);
	imwrite("median\\med3_sap01.bmp", med3_sap01);
	imwrite("median\\med5_sap01.bmp", med5_sap01);

	imwrite("open&close\\gauss10_open_close.bmp", gauss10_open_close);
	imwrite("open&close\\gauss10_close_open.bmp", gauss10_close_open);
	imwrite("open&close\\gauss30_open_close.bmp", gauss30_open_close);
	imwrite("open&close\\gauss30_close_open.bmp", gauss30_close_open);

	imwrite("open&close\\sap005_open_close.bmp", sap005_open_close);
	imwrite("open&close\\sap005_close_open.bmp", sap005_close_open);
	imwrite("open&close\\sap01_open_close.bmp", sap01_open_close);
	imwrite("open&close\\sap01_close_open.bmp", sap01_close_open);


	//計算SNR
	ofstream out("SNR.txt", ios::trunc);
	out.close();
	SNR("gauss\\gauss10.bmp");
	SNR("gauss\\gauss30.bmp");
	SNR("sap\\sap005.bmp");
	SNR("sap\\sap01.bmp");

	return 0;
}
