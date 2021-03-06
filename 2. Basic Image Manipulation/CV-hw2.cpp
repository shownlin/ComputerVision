#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>

#define THREASHOLD 128

using namespace std;
using namespace cv;

Mat binary(Mat img, uchar t, int widthLimit, int heightLimit);
void drawHistImg(Mat &dst, vector <int>&v);
Mat histogram(Mat img, vector <int>&v, int widthLimit, int heightLimit);
Mat connected(Mat img, int widthLimit, int heightLimit);

int main(int argc, char const *argv[])
{
	//讀圖
	Mat img = imread("lll.bmp", CV_LOAD_IMAGE_GRAYSCALE);
	int widthLimit = img.cols;
	int heightLimit = img.rows;
	//複製圖片準備處理
	Mat b_img= img.clone();
	
	//設定閾值
	uchar t = THREASHOLD;

	//二值化
	b_img = binary(b_img,t, widthLimit, heightLimit);

	
	//直方圖
	vector <int>v(256,0);	//v = {0,0,...0}
	Mat his = histogram(img, v, widthLimit, heightLimit);
	
	//bounding box
	Mat c_img = connected(b_img, widthLimit, heightLimit);
	vector<int> labelSet;
	int max = 1;//找出最大的標籤幾號
	for (int height = 0; height < heightLimit; height++) {
		for (int width = 0; width < widthLimit; width++) {
			max = max < c_img.at<int>(height, width) ? c_img.at<int>(height, width) : max;
			labelSet.push_back(c_img.at<int>(height, width));
		}
	}
	sort(labelSet.begin(), labelSet.end());
	labelSet.erase(unique(labelSet.begin(), labelSet.end()), labelSet.end());
	cout << "最大值是" << max << endl;

	//開始畫邊界
	img = imread("lena.bmp", CV_LOAD_IMAGE_COLOR);
	int com = 0;
	int rec[4] = {heightLimit, widthLimit, 0, 0};	//[0]:左 [1]:上 [2]:右 [3]:下

	while(!labelSet.empty()) {
		int label = labelSet.back();
		labelSet.pop_back();
		for (int height = 0; height < heightLimit; height++) {
			for (int width = 0; width < widthLimit; width++) {
				if (c_img.at<int>(height, width) == label) {
					com++;
					if (width < rec[0])rec[0] = width;
					if (height < rec[1])rec[1] = height;
					if (width > rec[2])rec[2] = width;
					if (height > rec[3])rec[3] = height;
				}
			}
		}
		if (com > 500)rectangle(img, Point(rec[0], rec[1]), Point(rec[2], rec[3]), Scalar(0, 0, 255), 1);
		com = 0;
		rec[0] = heightLimit;
		rec[1] = widthLimit;
		rec[2] = 0;
		rec[3] = 0;
	}
	
	
	namedWindow("Display window", WINDOW_AUTOSIZE);
	namedWindow("Display window2", WINDOW_AUTOSIZE);
	imshow("Display window", img);
	imshow("Display window2", his);
	
	//imwrite("boundingBox.bmp", img);
	//imwrite("Histogram.bmp", his);
	imwrite("Binary.bmp", b_img);
	waitKey(0);

	return 0;
}

Mat binary(Mat img, uchar t, int widthLimit, int heightLimit) {

	for (int height = 0; height < heightLimit; height++) {
		uchar *data = img.ptr<uchar>(height);
		for (int width = 0; width < widthLimit; width++) {
			data[width] = data[width] < t ? 0 : 255;
		}
	}

	return img;
}

Mat histogram(Mat img, vector <int>&v, int widthLimit, int heightLimit) {

	int grayscale;

	for (int height = 0; height < heightLimit; height++) {
		uchar *data = img.ptr<uchar>(height);
		for (int width = 0; width < widthLimit; width++) {
			grayscale = data[width];
			v.at(grayscale)++;
		}
	}

	Mat showHistImg(256, 256, CV_8UC1, Scalar(255));  //把直方圖秀在一個256*256大的影像上
	drawHistImg(showHistImg,v);
	//imshow("window1", img);
	//imshow("window2", showHistImg);
	return showHistImg;
	//waitKey(0);
}

Mat connected(Mat img, int widthLimit, int heightLimit) {
	Mat labImg(heightLimit, widthLimit, CV_32S, Scalar(0));

	//initialization of each forebackground pixel to 1
	int i = 1;
	for (int height = 0; height < heightLimit; height++) {
		for (int width = 0; width < widthLimit; width++) {
			//labImg.at<int>(height, width) = img.at<uchar>(height, width) == 255 ? 0 : 1;
			labImg.at<int>(height, width) = img.at<uchar>(height, width) == 255 ? 0 : i++;
		}
	}

	//repeat until flag "change" is false
	bool change;

	do {
		change = false;
		//Top-down
		for (int height = 0; height < heightLimit; height++) {
			for (int width = 0; width < widthLimit; width++) {

				int temp = labImg.at<int>(height, width);
				if (temp == 0)continue;

				//若是最靠近左邊的pixel則不跟左邊比
				//int left = labImg.at<int>(height, width - 1);
				if ((width > 0) && (temp > labImg.at<int>(height, width - 1)) && (labImg.at<int>(height, width - 1) > 0)) {
					temp = labImg.at<int>(height, width - 1);
				}

				//int left_up = labImg.at<int>(height - 1, width - 1);
				if ((width > 0) && (height > 0) && (temp > labImg.at<int>(height - 1, width - 1)) && (labImg.at<int>(height - 1, width - 1) > 0)) {
					temp = labImg.at<int>(height - 1, width - 1);
				}

				//跟上面比
				//int up = labImg.at<int>(height - 1, width);
				if ((height > 0) && (temp > labImg.at<int>(height - 1, width)) && (labImg.at<int>(height - 1, width) > 0))
					temp = labImg.at<int>(height - 1, width);

				//跟右上比
				//int up_right = labImg.at<int>(height - 1, width + 1);
				if ((height > 0) && (width < widthLimit - 1) && (temp > labImg.at<int>(height - 1, width + 1)) && (labImg.at<int>(height - 1, width + 1) > 0)) {
					temp = labImg.at<int>(height - 1, width + 1);
				}
				if (labImg.at<int>(height, width) != temp) {
					labImg.at<int>(height, width) = temp;
					change = true;
				}
			}
		}

		//Buttom-up
				for (int height = heightLimit - 1; height >= 0; height--) {
			for (int width = widthLimit - 1; width >=0; width--) {

				int temp = labImg.at<int>(height, width);
				if (temp == 0)continue;

				//若是最靠近右邊的pixel則不跟右邊比
				//int compare_right = labImg.at<int>(height, width + 1);
				if ((width < widthLimit - 1) && (temp > labImg.at<int>(height, width + 1)) && (labImg.at<int>(height, width + 1) > 0)) {
					temp = labImg.at<int>(height, width + 1);
				}

				//int compare_right_down = labImg.at<int>(height + 1, width + 1);
				//跟右下比
				if ((width < widthLimit - 1) && (height < heightLimit - 1) && (temp > labImg.at<int>(height + 1, width + 1)) && (labImg.at<int>(height + 1, width + 1) > 0)) {
					temp = labImg.at<int>(height + 1, width + 1);
				}

				//int compare_down = labImg.at<int>(height + 1, width);
				//跟下比
				if ((height < heightLimit - 1)&&(temp > labImg.at<int>(height + 1, width))&&(labImg.at<int>(height + 1, width) > 0)) {
					temp = labImg.at<int>(height + 1, width);
				}

				if (width > 0) {	//跟左下比
					//int compare_down_left = labImg.at<int>(height + 1, width-1);
					if ((width > 0) && (height < heightLimit - 1) && (temp > labImg.at<int>(height + 1, width - 1)) && (labImg.at<int>(height + 1, width - 1) > 0)) {
						temp = labImg.at<int>(height + 1, width - 1);
					}
				}

				if (labImg.at<int>(height, width) != temp) {
					labImg.at<int>(height, width) = temp;
					change = true;
				}
			}
		}
	} while (change);

	cout << "完成！" << endl;
	return labImg;
}


void drawHistImg(Mat &dst, vector <int>&v) {
	int histSize = 256;
	float histMaxValue = 0;

	for (int i = 0; i < histSize; i++) {
		float tempValue = v.at(i);
		if (histMaxValue < tempValue) {
			histMaxValue = tempValue;
		}
	}

	float scale = (0.9 * 256) / histMaxValue;
	for (int i = 0; i < histSize; i++) {
		int intensity = static_cast<int>((float)v.at(i)*scale);
		line(dst, Point(i, 255), Point(i, 255 - intensity), Scalar(0));
	}
}