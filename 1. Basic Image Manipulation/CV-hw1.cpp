/**********************************************************
Name : Qun-Wei, Lin
Date : 2018/9/17
Project : Computer Vision HW1
**********************************************************/
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;

int main(int argc, char const *argv[]) {
	//讀圖，以灰階讀入
	Mat lena = imread("lena.BMP", CV_LOAD_IMAGE_GRAYSCALE);

	//複製圖片準備處理
	Mat upside_down, right_side_left, diagonally_mirrored;
	upside_down = lena.clone();
	right_side_left = lena.clone();
	diagonally_mirrored = lena.clone();

	//定義圖片寬度
	int widthLimit = lena.channels() * lena.cols;

	//上下顛倒
	uchar temp;
	for (int height = 0; height <= upside_down.rows / 2; height++) {
		//至多處理到高度的一半
		for (int width = 0; width < widthLimit; width++) {
			temp = upside_down.at<uchar>(height, width);
			upside_down.at<uchar>(height, width) = upside_down.at<uchar>(upside_down.rows - height - 1, width);
			upside_down.at<uchar>(upside_down.rows - height - 1, width) = temp;
			//利用暫時的變數temp進行像素的swap
		}
	}

	//左右顛倒
	temp = 0;
	for (int height = 0; height < right_side_left.rows; height++) {
		for (int width = 0; width <= widthLimit / 2; width++) {
			//至多處理到寬度的一半
			temp = right_side_left.at<uchar>(height, width);
			right_side_left.at<uchar>(height, width) = right_side_left.at<uchar>(height, right_side_left.cols - width - 1);
			right_side_left.at<uchar>(height, right_side_left.cols - width - 1) = temp;
			//利用暫時的變數temp進行像素的swap
		}
	}

	//對角線鏡射
	temp = 0;
	for (int height = 0; height < diagonally_mirrored.rows; height++) {
		for (int width = height; width < widthLimit; width++) {
			temp = diagonally_mirrored.at<uchar>(height, width);
			diagonally_mirrored.at<uchar>(height, width) = diagonally_mirrored.at<uchar>(width, height);
			diagonally_mirrored.at<uchar>(width, height) = temp;
			//利用暫時的變數temp進行像素的swap
		}
	}

	//存圖
	imwrite("upside_down.bmp", upside_down);
	imwrite("right_side_left.bmp", right_side_left);
	imwrite("diagonally_mirrored.bmp", diagonally_mirrored);

	exit(0);
}