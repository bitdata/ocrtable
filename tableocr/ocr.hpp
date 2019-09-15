#ifndef OCR_HPP
#define OCR_HPP
#include <tesseract/baseapi.h>
#include <opencv2/opencv.hpp>

using namespace tesseract;
using namespace cv;

class Ocr
{
public:
	Ocr()
	{
		s = NULL;
		api.Init(NULL, NULL);
	}
	explicit Ocr(const char* language)
	{
		s = NULL;
		api.Init(NULL, language);
	}
	~Ocr()
	{
		if (s != NULL)
		{
			delete[] s;
		}
		api.End();
	}

	const char* GetImageText(Mat mat)
	{
		api.SetImage((unsigned char*)mat.data, mat.cols, mat.rows, mat.channels(), (int)mat.step1());
		return GetText();
	}
	const char* GetImageText(Mat mat, Rect rect)
	{
		api.TesseractRect(mat.data, mat.channels(), (int)mat.step1(), rect.x, rect.y, rect.width, rect.height);
		return GetText();
	}

private:
	TessBaseAPI api;
	char* s;

	const char* GetText()
	{
		if (s != NULL)
		{
			delete[] s;
			s = NULL;
		}
		char* p = api.GetUTF8Text();
		if (p != NULL)
		{
			s = U2G(p);
			delete[] p;
		}
		return s;
	}

	static char* U2G(const char* utf8)
	{
		int len = MultiByteToWideChar(CP_UTF8, 0, utf8, -1, NULL, 0);
		wchar_t* wstr = new wchar_t[len + 1];
		memset(wstr, 0, len + 1);
		MultiByteToWideChar(CP_UTF8, 0, utf8, -1, wstr, len);
		len = WideCharToMultiByte(CP_ACP, 0, wstr, -1, NULL, 0, NULL, NULL);
		char* str = new char[len + 1];
		memset(str, 0, len + 1);
		WideCharToMultiByte(CP_ACP, 0, wstr, -1, str, len, NULL, NULL);
		delete[] wstr;
		return str;
	}
};

#endif