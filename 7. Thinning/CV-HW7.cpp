#include <iostream>
#include <opencv2/opencv.hpp>

#define THREASHOLD 128
using namespace std;
using namespace cv;

Mat binary(Mat img, uchar t, int widthLimit, int heightLimit) {

	for (int height = 0; height < heightLimit; height++) {
		uchar *data = img.ptr<uchar>(height);
		for (int width = 0; width < widthLimit; width++) {
			data[width] = data[width] < t ? 0 : 255;
		}
	}

	return img;
}

char h1(char c, char d) {
	if (c == d)
		return c;
	return 'b';
}

Mat Matmark_interior_border(Mat img, int widthLimit, int heightLimit) {
	Mat result(heightLimit, widthLimit, CV_8U, Scalar(0));
	int x1, x2, x3, x4;
	for (int i = 0; i < heightLimit; i++) {
		for (int j = 0; j < widthLimit; j++) {
			if (img.at<uchar>(i, j) > 0) {
				x1 = x2 = x3 = x4 = 0;
				if (i == 0) {
					if (j == 0) {
						x1 = img.at<uchar>(i, j + 1);
						x4 = img.at<uchar>(i + 1, j);
					}
					else if (j == widthLimit - 1) {
						x3 = img.at<uchar>(i, j - 1);
						x4 = img.at<uchar>(i + 1, j);
					}
					else {
						x1 = img.at<uchar>(i, j + 1);
						x3 = img.at<uchar>(i, j - 1);
						x4 = img.at<uchar>(i + 1, j);
					}
				}
				else if (i == heightLimit - 1) {
					if (j == 0) {
						x1 = img.at<uchar>(i, j + 1);
						x2 = img.at<uchar>(i - 1, j);
					}
					else if (j == widthLimit - 1) {
						x2 = img.at<uchar>(i - 1, j);
						x3 = img.at<uchar>(i, j - 1);
					}
					else {
						x1 = img.at<uchar>(i, j + 1);
						x2 = img.at<uchar>(i - 1, j);
						x3 = img.at<uchar>(i, j - 1);
					}
				}
				else {
					if (j == 0) {
						x1 = img.at<uchar>(i, j + 1);
						x2 = img.at<uchar>(i - 1, j);
						x4 = img.at<uchar>(i + 1, j);
					}
					else if (j == widthLimit - 1) {
						x2 = img.at<uchar>(i - 1, j);
						x3 = img.at<uchar>(i, j - 1);
						x4 = img.at<uchar>(i + 1, j);
					}
					else {
						x1 = img.at<uchar>(i, j + 1);
						x2 = img.at<uchar>(i - 1, j);
						x3 = img.at<uchar>(i, j - 1);
						x4 = img.at<uchar>(i + 1, j);
					}

					x1 /= 255;
					x2 /= 255;
					x3 /= 255;
					x4 /= 255;

					char a1 = h1('1', x1 + '0');
					char a2 = h1(a1, x2 + '0');
					char a3 = h1(a2, x3 + '0');
					char a4 = h1(a3, x4 + '0');
					
					if (a4 == 'b')
						result.at<uchar>(i, j) = 2;
					else
						result.at<uchar>(i, j) = 1;
				}
			}
		}
	}
	return result;
}

int h2(int a, int m) {
	if (a == m)
		return 1;
	return 0;
}

Mat mark_pair_relationship(Mat img, int widthLimit, int heightLimit) {
	Mat pair(heightLimit, widthLimit, CV_8U, Scalar(0));
	int x1, x2, x3, x4;
	for (int i = 0; i < heightLimit; i++) {
		for (int j = 0; j < widthLimit; j++) {
			if (img.at<uchar>(i, j) > 0) {
				x1 = x2 = x3 = x4 = 0;
				if (i == 0) {
					if (j == 0) {
						x1 = img.at<uchar>(i, j + 1);
						x4 = img.at<uchar>(i + 1, j);
					}
					else if (j == widthLimit - 1) {
						x3 = img.at<uchar>(i, j - 1);
						x4 = img.at<uchar>(i + 1, j);
					}
					else {
						x1 = img.at<uchar>(i, j + 1);
						x3 = img.at<uchar>(i, j - 1);
						x4 = img.at<uchar>(i + 1, j);
					}
				}
				else if (i == heightLimit - 1) {
					if (j == 0) {
						x1 = img.at<uchar>(i, j + 1);
						x2 = img.at<uchar>(i - 1, j);
					}
					else if (j == widthLimit - 1) {
						x2 = img.at<uchar>(i - 1, j);
						x3 = img.at<uchar>(i, j - 1);
					}
					else {
						x1 = img.at<uchar>(i, j + 1);
						x2 = img.at<uchar>(i - 1, j);
						x3 = img.at<uchar>(i, j - 1);
					}
				}
				else {
					if (j == 0) {
						x1 = img.at<uchar>(i, j + 1);
						x2 = img.at<uchar>(i - 1, j);
						x4 = img.at<uchar>(i + 1, j);
					}
					else if (j == widthLimit - 1) {
						x2 = img.at<uchar>(i - 1, j);
						x3 = img.at<uchar>(i, j - 1);
						x4 = img.at<uchar>(i + 1, j);
					}
					else {
						x1 = img.at<uchar>(i, j + 1);
						x2 = img.at<uchar>(i - 1, j);
						x3 = img.at<uchar>(i, j - 1);
						x4 = img.at<uchar>(i + 1, j);
					}
				}

				if (h2(x1, 1) + h2(x2, 1) + h2(x3, 1) + h2(x4, 1) >= 1 && img.at<uchar>(i, j) == 2)
					pair.at<uchar>(i, j) = 1;
				else
					pair.at<uchar>(i, j) = 2;
			}
		}
	}
	return pair;
}

char h(bool b, bool c, bool d, bool e) {
	if (b == c && (d != b || e != b))
		return 'q';

	if (b == c && (d == b && e == b))
		return 'r';

	return 's';
}

Mat step3(Mat img) {
	Mat result(img.rows, img.cols, CV_8U, Scalar(0));

	//定義x0~x8
	bool x0, x1, x2, x3, x4, x5, x6, x7, x8;
	x0 = x1 = x2 = x3 = x4 = x5 = x6 = x7 = x8 = 0;

	for (int height = 0; height < img.rows; height++) {
		for (int width = 0; width < img.cols; width++) {
			if (img.at<uchar>(height, width) > 0) {//代表是foreground的像素

				if (height == 0) {

					if (width == 0) {
						//top-left
						x7 = x2 = x6 = 0;
						x3 = 0; x0 = img.at<uchar>(height, width); x1 = img.at<uchar>(height, width + 1);
						x8 = 0; x4 = img.at<uchar>(height + 1, width); x5 = img.at<uchar>(height + 1, width + 1);
					}
					else if (width == img.cols - 1) {
						//top-right
						x7 = x2 = x6 = 0;
						x3 = img.at<uchar>(height, width - 1); x0 = img.at<uchar>(height, width); x1 = 0;
						x8 = img.at<uchar>(height + 1, width - 1); x4 = img.at<uchar>(height + 1, width); x5 = 0;
					}

					else {
						//第一列其他元素
						x7 = x2 = x6 = 0;
						x3 = img.at<uchar>(height, width - 1); x0 = img.at<uchar>(height, width); x1 = img.at<uchar>(height, width + 1);
						x8 = img.at<uchar>(height + 1, width - 1); x4 = img.at<uchar>(height + 1, width); x5 = img.at<uchar>(height + 1, width + 1);
					}
				}

				else if (height == img.rows - 1) {

					if (width == 0) {
						//bottom-left
						x7 = 0; x2 = img.at<uchar>(height - 1, width); x6 = img.at<uchar>(height - 1, width + 1);
						x3 = 0; x0 = img.at<uchar>(height, width); x1 = img.at<uchar>(height, width + 1);
						x8 = x4 = x5 = 0;
					}
					else if (width == img.cols - 1) {
						//bottom-right
						x7 = img.at<uchar>(height - 1, width - 1); x2 = img.at<uchar>(height - 1, width); x6 = 0;
						x3 = img.at<uchar>(height, width - 1); x0 = img.at<uchar>(height, width); x1 = 0;
						x8 = x4 = x5 = 0;
					}

					else {
						//最後一列其他元素
						x7 = img.at<uchar>(height - 1, width - 1); x2 = img.at<uchar>(height - 1, width); x6 = img.at<uchar>(height - 1, width + 1);
						x3 = img.at<uchar>(height, width - 1); x0 = img.at<uchar>(height, width); x1 = img.at<uchar>(height, width + 1);
						x8 = x4 = x5 = 0;
					}
				}

				else {

					if (width == 0) {
						//中間最左
						x7 = 0; x2 = img.at<uchar>(height - 1, width); x6 = img.at<uchar>(height - 1, width + 1);
						x3 = 0; x0 = img.at<uchar>(height, width); x1 = img.at<uchar>(height, width + 1);
						x8 = 0; x4 = img.at<uchar>(height + 1, width); x5 = img.at<uchar>(height + 1, width + 1);
					}
					else if (width == img.cols - 1) {
						//中間最右，臣又來了
						x7 = img.at<uchar>(height - 1, width - 1); x2 = img.at<uchar>(height - 1, width); x6 = 0;
						x3 = img.at<uchar>(height, width - 1); x0 = img.at<uchar>(height, width); x1 = 0;
						x8 = img.at<uchar>(height + 1, width - 1); x4 = img.at<uchar>(height + 1, width); x5 = 0;
					}

					else {
						//都有在範圍內
						x7 = img.at<uchar>(height - 1, width - 1); x2 = img.at<uchar>(height - 1, width); x6 = img.at<uchar>(height - 1, width + 1);
						x3 = img.at<uchar>(height, width - 1); x0 = img.at<uchar>(height, width); x1 = img.at<uchar>(height, width + 1);
						x8 = img.at<uchar>(height + 1, width - 1); x4 = img.at<uchar>(height + 1, width); x5 = img.at<uchar>(height + 1, width + 1);
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
				result.at<uchar>(height, width) = yokoi;
			}
		}
	}
	return result;
}

int main()
{
	//讀圖
	Mat img = imread("lena.bmp", CV_LOAD_IMAGE_GRAYSCALE);
	int widthLimit = img.cols;
	int heightLimit = img.rows;

	//二值化
	Mat b_img = binary(img, THREASHOLD, widthLimit, heightLimit);

	//downsample lena image from 512x512 to 64x64 by using 8x8 blocks as a unit and take the topmost-left pixel as the downsampled data
	Mat lena_down(64, 64, CV_8U, Scalar(0));
	for (int height = 0; height < 64; height++) {
		for (int width = 0; width < 64; width++) {
			lena_down.at<uchar>(height, width) = b_img.at<uchar>(8 * height, 8 * width);
		}
	}

	//複製圖片準備處理
	Mat img_thin = lena_down.clone();

	widthLimit = lena_down.cols;
	heightLimit = lena_down.rows;


	bool flag = true;

	while (flag) {

		flag = false;

		Mat old = img_thin.clone();

		//step 1: mark the interior/border pixels
		Mat s1 = Matmark_interior_border(img_thin, widthLimit, heightLimit);

		/*for (int i = 0; i < heightLimit; i++) {
			for (int j = 0; j < widthLimit; j++) {
				cout << (int)s1.at<uchar>(i, j);
			}
			cout << endl;
		}*/

		//step 2: pair relationship operator
		Mat s2 = mark_pair_relationship(s1, widthLimit, heightLimit);

		//step 3: check and delete the deletable pixels
		Mat s3 = step3(s1);

		for (int i = 0; i < heightLimit; i++) {
			for (int j = 0; j < widthLimit; j++) {
				if (s3.at<uchar>(i, j) == 1 && s2.at<uchar>(i, j) == 1)
					img_thin.at<uchar>(i, j) = 0;
			}
		}

		for (int i = 0; i < heightLimit; i++) {
			for (int j = 0; j < widthLimit; j++) {
				if (img_thin.at<uchar>(i, j) != old.at<uchar>(i, j))
					flag = true;
			}
		}

	}

	imwrite("thin.bmp", img_thin);
}