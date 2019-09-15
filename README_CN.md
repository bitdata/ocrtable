# OCR Table

## 简介

对于包含表格的图片，很多OCR软件识别文字时，只是单纯提取整页文字，丢弃的表格的结构。这给使用者带来不便。本工具在OCR时能够保留表格的结构。结果保存为Word文档。

本工具由一个dll和一个exe组成，均为64位。Dll对应的子目录为tableocr，由VC开发，实现核心功能，包括表格格式识别和文字识别；exe对应的子目录为ocrtable，由C\#开发，实现用户界面和内容输出。为体现GitHub精神，程序界面文字为英文，相信不会对中文开发者造成太大困扰。jpgs目录下面是示例图片。

如果觉得对您有用，麻烦点个赞。也欢迎提出改进建议和参与改进。除了提交issue外，也可通过邮箱31416\@msn.cn和我交流。

## 识别效果

[下载预编译组件](https://pan.baidu.com/s/1sr3HyL6QDFvMTEG5GmCr-w)

### 英文识别

纯英文识别时，不要勾选 "Recognize simplified Chinese characters"。识别效果如下：

原图：
<div align=center><img width="50%" src="pictures/1.png"/></div>

识别结果（注意表格没有按顺序显示）：
<div align=center><img src="results/1.png"/></div>

### 中文识别

中文识别时，需要勾选 "Recognize simplified Chinese characters"。识别效果如下：

原图：
<div align=center><img width="50%" src="pictures/3.jpeg"/></div>

识别结果：
<div align=center><img src="results/3.png"/></div>

中文识别依赖于Tesseract中文预训练包chi_sim.traineddata。预编译组件中提供的是官方的预训练包，仅支持宋体等少数字体。对不支持的字体，几乎都会识别为乱码。使用者可考虑自己训练Tesseract或改用其他的OCR技术。

## 开发环境

### dll开发环境

Windows 7 SP1 x64

Visual Studio Community 2017

OpenCV 3.4.3

Tesseract 4.0.0-beta.4
（git源代码编译，如何编译以及如何设置中文识别请搜索网上帖子）

为方便试用，该dll除了Debug和Release，还包含Debug EXE配置，编译结果为exe。该程序在图片中显示识别的表格结构，并通过OutputDebugString输出识别的文字。注意Tesseract识别时间较长，另外需通过键盘按键而不是鼠标关闭显示图片的窗口。

### exe开发环境

Windows 7 SP1 x64

Visual Studio Community 2017

DocX（Xceed.Words.NET.dll）（nuget下载）

## 修改历史

### 2018-09-30

1. 完成初版

### 2019-09-14

1. 修正dll中的BUG

2. exe增加国际化支持

3. 完善Readme
