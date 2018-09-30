# ocrtable

对于包含表格的图片，很多OCR软件识别文字时，只是单纯提取整页文字，丢弃的表格的结构。这给使用者带来不便。本工具在OCR时能够保留表格的结构。结果保存为Word文档。

本工具由一个dll和一个exe组成，均为64位。Dll对应的子目录为tableocr，由VC开发，实现核心功能，包括表格格式识别和文字识别；exe对应的子目录为ocrtable，由C\#开发，实现用户界面和内容输出。jpgs目录下面是示例图片。

为方便试用，该dll除了Debug和Release，还包含Debug
EXE配置，编译结果为exe。该程序在图片中显示识别的表格结构，并通过OutputDebugString输出识别的文字。注意Tesseract识别时间较长，另外需通过键盘按键关闭显示图片的窗口。

如果觉得对您有用，麻烦点个赞。也欢迎提出改进建议和参与改进。除了提交issue外，也可通过邮箱31416\@msn.cn和我交流。

# dll开发环境

Windows 7 SP1 x64

Visual Studio Community 2017

OpenCV 3.4.3

Tesseract 4.0.0-beta.4
（git源代码编译，如何编译以及如何设置中文识别请搜索网上帖子）

# exe开发环境

Windows 7 SP1 x64

Visual Studio Community 2017

DocX（Xceed.Words.NET.dll）（nuget下载）
