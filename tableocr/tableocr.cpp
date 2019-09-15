//#define WIN32_LEAN_AND_MEAN	
#define _WIN32_WINNT 0x0500
#include <windows.h>
#include <tchar.h>
#define TABLEOCR_API extern "C"
#include "tableocr.h"
#include "table.hpp"
#include "ocr.hpp"

class TableOcr;

class TableEx : public Table
{
public:
	TableEx(TableOcr* pOwner, const Rect& rect);

	TableOcr* pOwner;
	Rect rect;
};

class TableOcr
{
public:
	TableOcr(const char* pszImagePath, const char* pszLanguage, const TOOPTION* pOption)
		:ocr(pszLanguage)
	{
		pExtractor = NULL;
		option = *pOption;
		Mat src = imread(pszImagePath);
		if (src.data != NULL)
		{
			pExtractor = new TableExtractor(src);
			pExtractor->prepare(option.minSize);
			pExtractor->locateTables(option.minSize*option.minSize, 3, option.delta);
			for (size_t i = 0; i < pExtractor->rects.size(); i++)
			{
				Rect rect = pExtractor->rects[i];
				TableEx* pTable = new TableEx(this, rect);
				pTable->prepare(option.minSize, option.threshold, option.delta);
				if (option.isMerged)
				{
					pTable->AdjustTable(option.delta);
				}
				tables.push_back(pTable);
			}
		}
	}
	~TableOcr()
	{
		for (size_t i = 0; i < tables.size(); i++)
		{
			delete tables[i];
		}
		tables.clear();
		if (pExtractor != NULL)
		{
			delete pExtractor;
		}
	}

	TOOPTION option;
	TableExtractor* pExtractor;
	std::vector<TableEx*> tables;
	Ocr ocr;
};

TableEx::TableEx(TableOcr* pOwner, const Rect& rect)
	:Table(pOwner->pExtractor->horizontal(rect), pOwner->pExtractor->vertical(rect))
{
	this->pOwner = pOwner;
	this->rect = rect;
}

TABLEOCR_API void* WINAPI TOLoadImage(const char* pszImagePath, const char* pszLanguage, const TOOPTION* pOption, LPSIZE pSize)
{
	TableOcr* p = new TableOcr(pszImagePath, pszLanguage, pOption);
	if (p != NULL)
	{
		if (p->pExtractor != NULL)
		{
			if (pSize != NULL)
			{
				pSize->cx = p->pExtractor->gray.cols;
				pSize->cy = p->pExtractor->gray.rows;
			}
			return (void*)p;
		}
		delete p;
	}
	return NULL;
}

TABLEOCR_API void WINAPI TOFree(void* hTO)
{
	delete (TableOcr*)hTO;
}

TABLEOCR_API int WINAPI TOGetTableCount(void* hTO)
{
	TableOcr* p = (TableOcr*)hTO;
	return (int)p->tables.size();
}

TABLEOCR_API void* WINAPI TOGetTable(void* hTO, int index, LPRECT pRect)
{
	TableOcr* p = (TableOcr*)hTO;
	if (pRect != NULL)
	{
		Rect rect = p->pExtractor->rects[index];
		pRect->left = rect.x;
		pRect->top = rect.y;
		pRect->right = rect.x + rect.width;
		pRect->bottom = rect.y + rect.height;
	}
	return (void*)p->tables[index];
}

TABLEOCR_API int WINAPI TOGetRowCount(void* hTable)
{
	TableEx* p = (TableEx*)hTable;
	return (int)p->getRowCount();
}

TABLEOCR_API int WINAPI TOGetColumnCount(void* hTable)
{
	TableEx* p = (TableEx*)hTable;
	return (int)p->getColumnCount();
}

TABLEOCR_API int WINAPI TOGetRowHeight(void* hTable, int iRow)
{
	TableEx* p = (TableEx*)hTable;
	return (int)p->getRowHeight(iRow);
}

TABLEOCR_API int WINAPI TOGetColumnWidth(void* hTable, int iCol)
{
	TableEx* p = (TableEx*)hTable;
	return (int)p->getColumnWidth(iCol);
}

TABLEOCR_API BOOL WINAPI TOGetCell(void* hTable, int iRow, int iCol, LPRECT pRect)
{
	TableEx* p = (TableEx*)hTable;
	Rect rect;
	BOOL fRet;
	if (!p->pOwner->option.isMerged)
	{
		rect = p->toRect(iRow, iCol);
		fRet = TRUE;
	}
	else
	{
		fRet = p->getRect(iRow, iCol, rect);
	}
	rect.x += p->rect.x;
	rect.y += p->rect.y;
	pRect->left = rect.x;
	pRect->top = rect.y;
	pRect->right = rect.x + rect.width;
	pRect->bottom = rect.y + rect.height;
	return fRet;
}

TABLEOCR_API void WINAPI TODrawTable(void* hTable, const char* winname, int ratio)
{
	TableEx* p = (TableEx*)hTable;
	Mat img = p->pOwner->pExtractor->gray(p->rect);
	p->drawLines(img, Scalar(0), 5);
	resize(img, img, Size(img.cols / ratio, img.rows / ratio));
	imshow(winname, img);
}

TABLEOCR_API const char* WINAPI TOOcr(void* hTO, const RECT* pRect)
{
	Rect cell(pRect->left, pRect->top, pRect->right - pRect->left, pRect->bottom - pRect->top);
	TableOcr* p = (TableOcr*)hTO;
	return p->ocr.GetImageText(p->pExtractor->gray, cell);
}


#if defined(_USRDLL)

BOOL WINAPI DllMain(
	HINSTANCE hinstDLL,  // handle to DLL module
	DWORD fdwReason,     // reason for calling function
	LPVOID)  // reserved
{
	switch (fdwReason)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hinstDLL);
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

#else

int APIENTRY _tWinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPTSTR    lpCmdLine,
	int       nCmdShow)
{
	TCHAR szFile[MAX_PATH];
	if (!*lpCmdLine)
	{
		*szFile = 0; 
		OPENFILENAME ofn = { sizeof(OPENFILENAME) };
		ofn.lpstrFile = szFile; 
		ofn.nMaxFile = MAX_PATH;
		ofn.Flags = OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_NONETWORKBUTTON;
		if (!GetOpenFileName(&ofn))
			return GetLastError();
		lpCmdLine = szFile;
	}

	TOOPTION option;
	option.isMerged = TRUE;
	option.minSize = 20;
	option.threshold = 10;
	option.delta = 3;
	void* hTO = TOLoadImage(lpCmdLine, "chi_sim", &option,NULL);
	if (NULL == hTO)
		return -1;
	int nTables = TOGetTableCount(hTO);
	for (int k = 0; k < nTables; k++)
	{
		void* hTable = TOGetTable(hTO, k, NULL);
		TODrawTable(hTable, "lines", 2);
		int nRows = TOGetRowCount(hTable);
		int nCols = TOGetColumnCount(hTable);
		for (int i = 0; i < nRows; i++)
		{
			for (int j = 0; j < nCols; j++)
			{
				RECT rect;
				if (TOGetCell(hTable, i, j, &rect))
				{
					const char* s = TOOcr(hTO, &rect);
					OutputDebugStringA(s);
					OutputDebugStringA("\r\n");
				}
			}
		}
	}
	waitKey(0);
	TOFree(hTO);
	return 0;
}

#endif 