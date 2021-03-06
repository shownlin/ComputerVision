#include <iostream>
#include <opencv2/opencv.hpp>

#define THREASHOLD 128

using namespace std;
using namespace cv;

Mat binary(Mat img, uchar t, int widthLimit, int heightLimit) {

	for (int height = 0; height < heightLimit; height++) {
		uchar *data = img.ptr<uchar>(height);
		for (int width = 0; width < widthLimit; width++) {
			data[width] = data[width] < t ? 0 : 1;
		}
	}

	return img;
}

char h(bool b, bool c, bool d, bool e) {
	if (b == c && (d != b || e != b))
		return 'q';
	
	if (b == c && (d == b && e == b))
		return 'r';
	
	return 's';
}

int main()
{
	Mat img = imread("lena.bmp", CV_LOAD_IMAGE_GRAYSCALE);
	int widthLimit = img.cols;
	int heightLimit = img.rows;


	//先進行二值化
	Mat b_img = binary(img, THREASHOLD, widthLimit, heightLimit);

	//downsample lena image from 512x512 to 64x64 by using 8x8 blocks as a unit and take the topmost-left pixel as the downsampled data
	Mat lena_down(64, 64, CV_8U, Scalar(0));
	for (int height = 0; height < 64; height++) {
		for (int width = 0; width < 64; width++) {
			lena_down.at<uchar>(height, width) = b_img.at<uchar>(8 * height, 8 * width);
		}
	}

	//定義x0~x8
	bool x0, x1, x2, x3, x4, x5, x6, x7, x8;
	x0 = x1 = x2 = x3 = x4 = x5 = x6 = x7 = x8 = 0;

	for (int height = 0; height < lena_down.rows; height++) {
		for (int width = 0; width < lena_down.cols; width++) {
			if (lena_down.at<uchar>(height, width) > 0) {//代表是foreground的像素

				if (height == 0) {

					if (width == 0) {
						//top-left
						x7 = x2 = x6 = 0;
						x3 = 0; x0 = lena_down.at<uchar>(height, width); x1 = lena_down.at<uchar>(height, width + 1);
						x8 = 0; x4 = lena_down.at<uchar>(height + 1, width); x5 = lena_down.at<uchar>(height + 1, width + 1);
					}
					else if (width == lena_down.cols - 1) {
						//top-right
						x7 = x2 = x6 = 0;
						x3 = lena_down.at<uchar>(height, width - 1); x0 = lena_down.at<uchar>(height, width); x1 = 0;
						x8 = lena_down.at<uchar>(height + 1, width - 1); x4 = lena_down.at<uchar>(height + 1, width); x5 = 0;
					}

					else {
						//第一列其他元素
						x7 = x2 = x6 = 0;
						x3 = lena_down.at<uchar>(height, width - 1); x0 = lena_down.at<uchar>(height, width); x1 = lena_down.at<uchar>(height, width + 1);
						x8 = lena_down.at<uchar>(height + 1, width - 1); x4 = lena_down.at<uchar>(height + 1, width); x5 = lena_down.at<uchar>(height + 1, width + 1);
					}
				}

				else if (height == lena_down.rows - 1) {

					if (width == 0) {
						//bottom-left
						x7 = 0; x2 = lena_down.at<uchar>(height - 1, width); x6 = lena_down.at<uchar>(height - 1, width + 1);
						x3 = 0; x0 = lena_down.at<uchar>(height, width); x1 = lena_down.at<uchar>(height, width + 1);
						x8 = x4 = x5 = 0;
					}
					else if (width == lena_down.cols - 1) {
						//bottom-right
						x7 = lena_down.at<uchar>(height - 1, width - 1); x2 = lena_down.at<uchar>(height - 1, width); x6 = 0;
						x3 = lena_down.at<uchar>(height, width - 1); x0 = lena_down.at<uchar>(height, width); x1 = 0;
						x8 = x4 = x5 = 0;
					}

					else {
						//最後一列其他元素
						x7 = lena_down.at<uchar>(height - 1, width - 1); x2 = lena_down.at<uchar>(height - 1, width); x6 = lena_down.at<uchar>(height - 1, width + 1);
						x3 = lena_down.at<uchar>(height, width - 1); x0 = lena_down.at<uchar>(height, width); x1 = lena_down.at<uchar>(height, width + 1);
						x8 = x4 = x5 = 0;
					}
				}

				else {

					if (width == 0) {
						//中間最左
						x7 = 0; x2 = lena_down.at<uchar>(height - 1, width); x6 = lena_down.at<uchar>(height - 1, width + 1);
						x3 = 0; x0 = lena_down.at<uchar>(height, width); x1 = lena_down.at<uchar>(height, width + 1);
						x8 = 0; x4 = lena_down.at<uchar>(height + 1, width); x5 = lena_down.at<uchar>(height + 1, width + 1);
					}
					else if (width == lena_down.cols - 1) {
						//中間最右，臣又來了
						x7 = lena_down.at<uchar>(height - 1, width - 1); x2 = lena_down.at<uchar>(height - 1, width); x6 = 0;
						x3 = lena_down.at<uchar>(height, width - 1); x0 = lena_down.at<uchar>(height, width); x1 = 0;
						x8 = lena_down.at<uchar>(height + 1, width - 1); x4 = lena_down.at<uchar>(height + 1, width); x5 = 0;
					}

					else {
						//都有在範圍內
						x7 = lena_down.at<uchar>(height - 1, width - 1); x2 = lena_down.at<uchar>(height - 1, width); x6 = lena_down.at<uchar>(height - 1, width + 1);
						x3 = lena_down.at<uchar>(height, width - 1); x0 = lena_down.at<uchar>(height, width); x1 = lena_down.at<uchar>(height, width + 1);
						x8 = lena_down.at<uchar>(height + 1, width - 1); x4 = lena_down.at<uchar>(height + 1, width); x5 = lena_down.at<uchar>(height + 1, width + 1);
					}
				}

				char a[5];
				a[1] = h(x0, x1, x6, x2);
				a[2] = h(x0, x2, x7, x3);
				a[3] = h(x0, x3, x8, x4);
				a[4] = h(x0, x4, x5, x1);

				int yokoi;

				if (a[1] == 'r' && a[2] == 'r' && a[3] == 'r' && a[4] == 'r')
					yokoi = 5;
				else {
					yokoi = 0;
					for (int i = 1; i < 5; i++) {
						if (a[i] == 'q')
							yokoi++;
					}
				}
				cout << yokoi << ' ';
			}
			else
				cout << "  ";
			if (width == lena_down.cols - 1)
				cout << endl;
		}
	}
	
}