#pragma once
#ifndef TABLEOCR_API
	#define TABLEOCR_API extern "C" __declspec(dllimport)
#endif

typedef struct _TOOPTION
{
	BOOL isMerged;	
	int minSize;	
	int threshold;	
	int delta;		
}TOOPTION;

TABLEOCR_API void* WINAPI TOLoadImage(const char* pszImagePath, const char* pszLanguage, const TOOPTION* pOption, LPSIZE pSize);

TABLEOCR_API void WINAPI TOFree(void* hTO);

TABLEOCR_API int WINAPI TOGetTableCount(void* hTO);

TABLEOCR_API void* WINAPI TOGetTable(void* hTO, int index, LPRECT pRect);

TABLEOCR_API int WINAPI TOGetRowCount(void* hTable);

TABLEOCR_API int WINAPI TOGetColumnCount(void* hTable);

TABLEOCR_API int WINAPI TOGetRowHeight(void* hTable, int iRow);

TABLEOCR_API int WINAPI TOGetColumnWidth(void* hTable, int iCol);

TABLEOCR_API BOOL WINAPI TOGetCell(void* hTable, int iRow, int iCol, LPRECT pRect);

TABLEOCR_API void WINAPI TODrawTable(void* hTable, const char* winname, int ratio);

TABLEOCR_API const char* WINAPI TOOcr(void* hTO, const RECT* pRect);