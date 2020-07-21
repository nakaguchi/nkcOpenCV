#include "nkcOpenCV.h"
#include <Windows.h>

namespace nkc {
namespace ocv {

// �҂��Ȃ�waitKey()
int noWaitKey() {
	MSG msg;
	while (::PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) {
		if (::GetMessage(&msg, NULL, 0, 0) == 0) {
			// WM_QUIT
			::PostQuitMessage(msg.wParam);
			return 0;
		}

		if (msg.message == WM_CHAR) return (int)msg.wParam;

		::TranslateMessage(&msg);
		::DispatchMessage(&msg);
	}
	return -1;
}

// �����摜���c���ɘA���\��
void imShowMulti(
	cv::String winname, std::vector<cv::Mat>& imgs, // �S�Ă̕\���摜�i8bit 3ch or 8bit 1ch�̂�)
	unsigned int cols, // ���̘A����
	unsigned int rows, // �c�̘A����
	cv::Size imgsize, // �\������摜�T�C�Y
	unsigned int border) {
	if (imgs.size() < 1 || cols < 1 || rows < 1) return;

	unsigned int w = imgsize.width + border, h = imgsize.height + border;
	cv::Mat board(h * rows + border, w * cols + border, CV_8UC3, CV_RGB(128, 128, 128));
	for (unsigned int r = 0, i = 0; r < rows; r++) {
		for (unsigned int c = 0; c < cols; c++, i++) {
			cv::Rect roi_rect = cv::Rect(c * w + border, r * h + border, imgsize.width, imgsize.height);
			cv::Mat roi(board, roi_rect);
			if (i < imgs.size()) {
				if (imgs[i].type() == CV_8UC3) {
					resize(imgs[i], roi, imgsize);
				} else if (imgs[i].type() == CV_8UC1) {
					cv::Mat c3;
					cvtColor(imgs[i], c3, cv::COLOR_GRAY2BGR);
					resize(c3, roi, imgsize);
				} else {
					putText(roi, "Color mode not matched", cv::Point(20, 20), cv::FONT_HERSHEY_COMPLEX, 0.5, CV_RGB(0, 0, 0));
				}
			} else {
				putText(roi, "No image", cv::Point(20, 20), cv::FONT_HERSHEY_COMPLEX, 0.5, CV_RGB(0, 0, 0));
			}
		}
	}
	imshow(winname, board);
}

// �c�����ۂ������T�C�Y
cv::Mat KeepAspectResize(cv::Mat& img, int width, int height) {
	cv::Size outputSize;
	if (width > 0) {
		if (height > 0) {
			// �c���w��i���T�C�Y�Ɏ��܂�悤�ɂ���j
			outputSize = cv::Size(height * img.cols / img.rows, width * img.rows / img.cols);
			if (outputSize.width > width) outputSize.width = width;
			else outputSize.height = height;
		} else {
			// �����w��
			outputSize = cv::Size(width, width * img.rows / img.cols);
		}
	} else {
		if (height > 0) {
			// �c���w��
			outputSize = cv::Size(height * img.cols / img.rows, height);
		} else {
			// �c���ǂ�����w�薳���ꍇ�͓��͂̃R�s�[��Ԃ�
			return img.clone();
		}
	}
	cv::Mat resized;
	cv::resize(img, resized, outputSize);
	return resized;
}

// �E�C���h�E�n���h�����擾
HANDLE getWindowHandle(cv::String winname) {
	return ::FindWindowA(NULL, winname.c_str());
}


}; // namespace ocv
}; // namespace nkc
