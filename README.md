# OCR Table

[中文文档](README_CN.md)

## Introduction

For scanning copies containing tables or forms, many OCR softwares recognize text in entire page as whole by discarding all tables. Sometimes it is inconvenient for users. This project retains table structures as well and save the recognizing result as a Microsoft Word document.

This project consists of a DLL and an EXE, both of which are 64-bit. The subdirectory corresponding to DLL is tableocr, developed by Visual C++. It implements core functions, including table structure recognition and text recognition. the subdirectory corresponding to EXE is ocrtable, developed by C\#, which provides user interface. Below the pictures directory are sample scanning copies.

Suggestions are welcome. In addition to submitting an issue, you can email me as well. My email address is 31416\@msn.cn.

## Recognition Performance

[Download pre-compiled components](https://pan.baidu.com/s/1DOST4gu8Gr87McebLEpfgQ 56j9)

### English Character Recognition Example

Please do not select "Recognize simplified Chinese characters".

Scanning copy:
<div align=center><img width="50%" src="https://github/bitdata/orctable/pictures/1.png"/></div>

Result (Note that tables are not displayed in order)：
<div align=center><img src="https://github/bitdata/orctable/results/1.png"/></div>

### Chinese Character Recognition Example

You need select "Recognize simplified Chinese characters".

Scanning copy：
<div align=center><img width="50%" src="https://github/bitdata/orctable/pictures/3.jpeg"></div>

Result:
<div align=center><img src="https://github/bitdata/orctable/results/3.png"></div>

Chinese character recognition relies on Tesseract official pre-training package, which supports only a few fonts. Users can consider training Tesseract mannually or using other OCR technologies instead.

## Development Environment

### DLL Development environment

Windows 7 SP1 x64

Visual Studio Community 2017

OpenCV 3.4.3

Tesseract 4.0.0-beta.4
（Compiled by Git source.  Please search online resources to learn how to setup Chinese character recognition.）

For the convenience of debugging, the DLL module includes Debug EXE configuration, which outputs EXE. The program displays table structures and outputs recognized text by OutputDebugString Windows API. Note that recognition process may take long time, and the popup window needs to be closed by keyboard instead of mouse.

### EXE Development environment

Windows 7 SP1 x64

Visual Studio Community 2017

DocX（Xceed.Words.NET.dll）（downloaded by nuget）

## Revision History

### 2018-09-30

1. Complete the first edition.

### 2019-09-14

1. Fix bugs in DLL.

2. Add international support in EXE.

3. Update this document.
