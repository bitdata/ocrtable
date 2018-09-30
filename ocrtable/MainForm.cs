using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.IO;
using System.Windows.Forms;
using System.Diagnostics;
using Xceed.Words.NET;

namespace ocrtable
{
    public partial class MainForm : Form
    {

        public MainForm()
        {
            InitializeComponent();
        }

        private void MainForm_Load(object sender, EventArgs e)
        {

        }

        private void browseOpenButton_Click(object sender, EventArgs e)
        {
            if (openFileDialog.ShowDialog() == DialogResult.OK)
            {
                imgTextBox.Text = openFileDialog.FileName;
                if (docTextBox.Text == "")
                {
                    docTextBox.Text = Path.ChangeExtension(openFileDialog.FileName, ".docx");
                }
            }
        }

        private void browseSaveButton_Click(object sender, EventArgs e)
        {
            if (saveFileDialog.ShowDialog() == DialogResult.OK)
            {
                docTextBox.Text = saveFileDialog.FileName;
            }
        }

        private void goButton_Click(object sender, EventArgs e)
        {
            if (!File.Exists(imgTextBox.Text))
            {
                MessageBox.Show("请指定待识别的图片文件");
                return;
            }
            goButton.Enabled = false;
            progressBar.Value = 0;
            backgroundWorker.RunWorkerAsync();
        }

        private void exitButton_Click(object sender, EventArgs e)
        {
            Close();
        }

        private void backgroundWorker_DoWork(object sender, DoWorkEventArgs e)
        {
            try
            {
                using (var hto = new HTO(imgTextBox.Text, isMergedCheckBox.Checked, (int)upDown.Value))
                {
                    using (var doc = DocX.Create(docTextBox.Text))
                    {
                        string text;
                        if (allOcrCheckBox.Checked)
                        {
                            text = hto.OcrParagraph(0);
                            if (!String.IsNullOrEmpty(text))
                            {
                                doc.InsertParagraph().Append(text);
                            }
                        }
                        for (int k = 0; k < hto.GetTableCount(); k++)
                        {
                            int percentProcess = (k * 2 + 1) * 100 / (hto.GetTableCount() * 2 + 1);
                            backgroundWorker.ReportProgress(percentProcess);
                            var htable = hto.GetTable(k);
                            var table = doc.AddTable(htable.RowCount, htable.ColumnCount);
                            for (int i = 0; i < htable.RowCount; i++)
                            {
                                var row = table.Rows[i];
                                for (int j = 0; j < htable.ColumnCount; j++)
                                {
                                    backgroundWorker.ReportProgress(percentProcess +
                                        (i * htable.ColumnCount + j) * 100
                                        / (htable.RowCount * htable.ColumnCount * (hto.GetTableCount() * 2 + 1)));
                                    Rectangle rect;
                                    if (htable.GetCell(i, j, out rect))
                                    {
                                        text = hto.Ocr(rect);
                                        if (!multilineCheckBox.Checked)
                                        {
                                            text = text.Replace("\r", "").Replace("\n", "");
                                        }
                                        else
                                        {
                                            text = text.TrimEnd('\r', '\n');
                                        }
                                        row.Cells[j].Paragraphs[0].Append(text);
                                    }
                                }
                            }
                            if (isMergedCheckBox.Checked)
                            {
                                MergeCells(htable, table);
                            }
                            doc.InsertTable(table);
                            doc.InsertParagraph().Append("");
                            if (allOcrCheckBox.Checked)
                            {
                                text = hto.OcrParagraph(k + 1);
                                if (!String.IsNullOrEmpty(text))
                                {
                                    doc.InsertParagraph().Append(text);
                                }
                            }
                        }
                        doc.Save();
                    }
                }
            }
            catch (Exception exception)
            {
                e.Result = exception;
            }
        }

        private void backgroundWorker_ProgressChanged(object sender, ProgressChangedEventArgs e)
        {
            progressBar.Value = e.ProgressPercentage;
        }

        private void backgroundWorker_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
        {
            if (e.Result != null)            {
                var exception = (Exception)e.Result;
                MessageBox.Show(exception.Message);
            }
            else
            {
                progressBar.Value = 100;
            }
            goButton.Enabled = true;
        }

        private void MergeCells(HTable htable, Table table)
        {
            for (int j = htable.ColumnCount - 1; j >= 0; j--)
            {
                for (int i = htable.RowCount - 1; i >= 0; i--)
                {
                    TryMergeCells(htable, table, i, j);
                }
            }
        }

        private static bool TryMergeCells(HTable htable, Table table, int rowIndex, int columnIndex)
        {
            Rectangle rect;
            if (!htable.GetCell(rowIndex, columnIndex, out rect))
                return false;
            int width = htable.GetColumnWidth(columnIndex);
            int height = htable.GetRowHeight(rowIndex);
            if (rect.Width == width && rect.Height == height)
                return false;
            int endColumn = columnIndex;
            if (width < rect.Width)
            {
                for (int j = columnIndex + 1; j < htable.ColumnCount; j++)
                {
                    width += htable.GetColumnWidth(j);
                    if (width >= rect.Width)
                    {
                        endColumn = j;
                        break;
                    }
                }
            }
            int endRow = rowIndex;
            if (height < rect.Height)
            {
                for (int i = rowIndex + 1; i < htable.RowCount; i++)
                {
                    height += htable.GetRowHeight(i);
                    if (height >= rect.Height)
                    {
                        endRow = i;
                        break;
                    }
                }
            }
            Debug.WriteLine("Merge cells: (" + rowIndex.ToString() + "," + columnIndex.ToString() +
                "),(" + endRow.ToString() + "," + endColumn.ToString() + ")");
            MergeCells(table, rowIndex, columnIndex, endRow, endColumn);
            return true;
        }

        private static void MergeCells(Table table, int startRow, int startColumn, int endRow, int endColumn)
        {
            if (startColumn != endColumn)
            {
                for (int i = endRow; i >= startRow; i--)
                {
                    table.Rows[i].MergeCells(startColumn, endColumn);
                }
            }
            if (startRow != endRow)
            {
                table.MergeCellsInColumn(startColumn, startRow, endRow);
            }
        }
    }
}
