using System;
using System.Collections.Generic;
using System.Drawing;
using System.Runtime.InteropServices;

[StructLayoutAttribute(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
public struct RECT
{
    public int left;
    public int top;
    public int right;
    public int bottom;
}

class HTable
{
    public HTable(IntPtr hTable)
    {
        this.hTable = hTable;
        RowCount = TOGetRowCount(hTable);
        ColumnCount = TOGetColumnCount(hTable);
    }

    public int RowCount { get; set; }

    public int ColumnCount { get; set; }

    public int GetRowHeight(int rowIndex)
    {
        return TOGetRowHeight(hTable, rowIndex);
    }

    public int GetColumnWidth(int columnIndex)
    {
        return TOGetColumnWidth(hTable, columnIndex);
    }

    public bool GetCell(int rowIndex, int columnIndex, out Rectangle rectangle)
    {
        RECT rect;
        bool hasCell = (TOGetCell(hTable, rowIndex, columnIndex, out rect) != 0);
        rectangle = new Rectangle();
        rectangle.X = rect.left;
        rectangle.Y = rect.top;
        rectangle.Width = rect.right - rect.left;
        rectangle.Height = rect.bottom - rect.top;
        return hasCell;
    }

    private IntPtr hTable;

    [DllImport("tableocr.dll")]
    static extern int TOGetRowCount(IntPtr hTable);

    [DllImport("tableocr.dll")]
    static extern int TOGetColumnCount(IntPtr hTable);

    [DllImport("tableocr.dll")]
    static extern int TOGetRowHeight(IntPtr hTable, int rowIndex);

    [DllImport("tableocr.dll")]
    static extern int TOGetColumnWidth(IntPtr hTable, int columnIndex);

    [DllImport("tableocr.dll")]
    static extern int TOGetCell(IntPtr hTable, int rowIndex, int columnIndex, out RECT rect);
}

class HTO : IDisposable
{
    [StructLayoutAttribute(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
    public struct TOOPTION
    {
        public int isMerged;
        public int minSize;
        public int threshold;
        public int delta;
    }

    [StructLayoutAttribute(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
    public struct SIZE
    {
        public int cx;
        public int cy;
    }

    public HTO(string imagePath, bool isMerged, int minSize, int threshold = 5, int delta = 3)
    {
        var option = new TOOPTION();
        option.isMerged = (isMerged ? 1 : 0);
        option.minSize = minSize;
        option.threshold = threshold;
        option.delta = delta;
        hTO = TOLoadImage(imagePath, option, out size);
    }

    public void Dispose()
    {
        TOFree(hTO);
    }

    public int GetTableCount()
    {
        return TOGetTableCount(hTO);
    }

    public HTable GetTable(int index)
    {
        RECT rect;
        IntPtr hTable = TOGetTable(hTO, index, out rect);
        return new HTable(hTable);
    }
    public HTable GetTable(int index, out Rectangle rectangle)
    {
        RECT rect;
        IntPtr hTable = TOGetTable(hTO, index, out rect);
        rectangle = new Rectangle();
        rectangle.X = rect.left;
        rectangle.Y = rect.top;
        rectangle.Width = rect.right - rect.left;
        rectangle.Height = rect.bottom - rect.top;
        return new HTable(hTable);
    }

    public string Ocr(Rectangle rectangle)
    {
        RECT rect;
        rect.left = rectangle.Left;
        rect.top = rectangle.Top;
        rect.right = rectangle.Right;
        rect.bottom = rectangle.Bottom;
        IntPtr ptr = TOOcr(hTO, rect);
        return Marshal.PtrToStringAnsi(ptr);
    }

    public string OcrParagraph(int index)
    {
        int y1, y2;
        if (index == 0)
        {
            y1 = 0;
        }
        else
        {
            RECT rect;
            TOGetTable(hTO, index - 1, out rect);
            y1 = rect.bottom;
        }
        if (index == GetTableCount())
        {
            y2 = size.cy;
        }
        else
        {
            RECT rect;
            TOGetTable(hTO, index, out rect);
            y2 = rect.top;
        }
        if (y1 >= y2)
            return null;
        var rectangle = new Rectangle(0, y1, size.cx, y2 - y1);
        return Ocr(rectangle);
    }

    private IntPtr hTO;
    private SIZE size;

    [DllImport("tableocr.dll", CharSet = CharSet.Ansi)]
    static extern IntPtr TOLoadImage(string imagePath, TOOPTION option, out SIZE size);

    [DllImport("tableocr.dll")]
    static extern void TOFree(IntPtr hTO);

    [DllImport("tableocr.dll")]
    static extern int TOGetTableCount(IntPtr hTO);

    [DllImport("tableocr.dll")]
    static extern IntPtr TOGetTable(IntPtr hTO, int index, out RECT rect);

    [DllImport("tableocr.dll", CharSet = CharSet.Ansi)]
    static extern IntPtr TOOcr(IntPtr hTO, RECT rect);
}