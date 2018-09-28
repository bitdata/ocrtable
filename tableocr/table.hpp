#ifndef TABLE_HPP
#define TABLE_HPP

#include <vector>
#include <queue>
#include <opencv2/opencv.hpp>

constexpr auto PI = 3.14;

using namespace cv;

class Table
{
public:

	Table(Mat horizontal, Mat vertical)
	{
		this->horizontal = horizontal;
		this->vertical = vertical;
	}

	void prepare(int threshold, int delta)
	{
		calcLines(threshold);
		mergeLines(delta);
		removeIsolatedLines(delta);
		calcMarks(delta);
	}

	void calcLines(int threshold)
	{
		HoughLinesP(horizontal, hLines, 1, PI / 180, threshold, 0, 10);
		HoughLinesP(vertical, vLines, 1, PI / 180, threshold, 0, 10);
		for (size_t i = 0; i < hLines.size(); i++)
		{
			Vec4i& hLine = hLines[i];
			if (hLine[0] > hLine[2])
			{
				int x = hLine[0];
				hLine[0] = hLine[2];
				hLine[2] = x;
			}
			if (hLine[1] != hLine[3])
			{
				hLine[1] = hLine[3] = (hLine[1] + hLine[3]) / 2;
			}
		}
		for (size_t i = 0; i < vLines.size(); i++)
		{
			Vec4i& vLine = vLines[i];
			if (vLine[1] > vLine[3])
			{
				int y = vLine[1];
				vLine[1] = vLine[3];
				vLine[3] = y;
			}
			if (vLine[0] != vLine[0])
			{
				vLine[0] = vLine[2] = (vLine[0] + vLine[2]) / 2;
			}
		}
	}

	void mergeLines(int delta)
	{
		sort(hLines.begin(), hLines.end(), compareHorizontal);
		for (size_t i = 0; i < hLines.size(); i++)
		{
			Vec4i& line1 = hLines[i];
			for (size_t j = i + 1; j < hLines.size(); j++)
			{
				Vec4i& line2 = hLines[j];
				if (line1[1] + delta < line2[1])
					break;
				if (line2[0] > line1[2] && line2[0] < line1[2] + delta)
				{
					line2[1] = line2[3] = (line1[1] + line2[1]) / 2;
					line2[0] = line1[0];
					line1[2] = line1[0] = -1; //will be removed afterward
				}
				else if (line1[0] > line2[2] && line1[0] < line2[2] + delta)
				{
					line2[1] = line2[3] = (line1[1] + line2[1]) / 2;
					line2[2] = line1[2];
					line1[0] = line1[2] = -1; //will be removed afterward
				}
			}
		}
		sort(vLines.begin(), vLines.end(), compareVertical);
		for (size_t i = 0; i < vLines.size(); i++)
		{
			Vec4i& line1 = vLines[i];
			for (size_t j = i + 1; j < vLines.size(); j++)
			{
				Vec4i& line2 = vLines[j];
				if (line1[0] + delta < line2[0])
					break;
				if (line2[1] > line1[3] && line2[1] < line1[3] + delta)
				{
					line2[0] = line2[2] = (line1[0] + line2[0]) / 2;
					line2[1] = line1[1];
					line1[3] = line1[1] = -1; //will be removed afterward
				}
				else if (line1[1] > line2[3] && line1[1] < line2[3] + delta)
				{
					line2[0] = line2[2] = (line1[0] + line2[0]) / 2;
					line2[3] = line1[3];
					line1[1] = line1[3] = -1; //will be removed afterward
				}
			}
		}
	}

	void removeIsolatedLines(int delta)
	{
		std::set<int> hSet, vSet;
		std::queue<int> hQueue, vQueue;
		int k = -1;
		int maxLength = 0;
		for (int i = 0; i < hLines.size(); i++)
		{
			int length = hLines[i][2] - hLines[i][0];
			if (length > maxLength)
			{
				maxLength = length;
				k = i;
			}
		}
		hSet.insert(k);
		hQueue.push(k);
		while (!hQueue.empty() || !vQueue.empty())
		{
			while (!hQueue.empty())
			{
				Vec4i line1 = hLines[hQueue.front()];
				hQueue.pop();
				for (int i = 0; i < vLines.size(); i++)
				{
					Vec4i line2 = vLines[i];
					if (vSet.count(i) == 0)
					{
						if (isCrossed(line1, line2, delta))
						{
							vSet.insert(i);
							vQueue.push(i);
						}
					}
				}
			}
			while (!vQueue.empty())
			{
				Vec4i line1 = vLines[vQueue.front()];
				vQueue.pop();
				for (int i = 0; i < hLines.size(); i++)
				{
					Vec4i line2 = hLines[i];
					if (hSet.count(i) == 0)
					{
						if (isCrossed(line2, line1, delta))
						{
							hSet.insert(i);
							hQueue.push(i);
						}
					}
				}
			}
		}
		update(hLines, hSet);
		update(vLines, vSet);
	}

	void calcMarks(int delta)
	{
		for (size_t i = 0; i < hLines.size(); i++)
		{
			yMarks.push_back(hLines[i][1]);
		}
		reduce(yMarks, delta);
		for (size_t i = 0; i < vLines.size(); i++)
		{
			xMarks.push_back(vLines[i][0]);
		}
		reduce(xMarks, delta);
	}

	size_t getRowCount() const
	{
		return yMarks.size() - 1;
	}
	size_t getColumnCount() const
	{
		return xMarks.size() - 1;
	}
	int getRowHeight(int row) const
	{
		return yMarks[row + 1] - yMarks[row];
	}
	int getColumnWidth(int col) const
	{
		return xMarks[col + 1] - xMarks[col];
	}
	// 针对规则的表格
	Rect toRect(int row, int col) const
	{
		return Rect(xMarks[col], yMarks[row], xMarks[col + 1] - xMarks[col], yMarks[row + 1] - yMarks[row]);
	}

	void AdjustLines(int delta)
	{
		for (size_t i = 0; i < hLines.size(); i++)
		{
			Vec4i& hLine = hLines[i];
			for (size_t j = 0; j < yMarks.size(); j++)
			{
				int y = yMarks[j];
				if (hLine[1] >= y - delta && hLine[3] <= y + delta)
				{
					hLine[1] = y;
					hLine[3] = y;
				}
			}
			for (size_t j = 1; j < xMarks.size(); j++)
			{
				if (hLine[0] <= xMarks[j])
				{
					if (hLine[0] < (xMarks[j - 1] + xMarks[j]) / 2)
					{
						hLine[0] = xMarks[j - 1];
					}
					else
					{
						hLine[0] = xMarks[j];
					}
					break;
				}
			}
			for (size_t j = xMarks.size() - 2; j >= 0; j--)
			{
				if (hLine[2] >= xMarks[j])
				{
					if (hLine[2] > (xMarks[j] + xMarks[j + 1]) / 2)
					{
						hLine[2] = xMarks[j + 1];
					}
					else
					{
						hLine[2] = xMarks[j];
					}
					break;
				}
			}
		}
		for (size_t i = 0; i < vLines.size(); i++)
		{
			Vec4i& vLine = vLines[i];
			for (size_t j = 0; j < xMarks.size(); j++)
			{
				int y = xMarks[j];
				if (vLine[0] >= y - delta && vLine[2] <= y + delta)
				{
					vLine[0] = y;
					vLine[2] = y;
				}
			}
			for (size_t j = 1; j < yMarks.size(); j++)
			{
				if (vLine[1] <= yMarks[j])
				{
					if (vLine[1] < (yMarks[j - 1] + yMarks[j]) / 2)
					{
						vLine[1] = yMarks[j - 1];
					}
					else
					{
						vLine[1] = yMarks[j];
					}
					break;
				}
			}
			for (size_t j = yMarks.size() - 2; j >= 0; j--)
			{
				if (vLine[3] >= yMarks[j])
				{
					if (vLine[3] > (yMarks[j] + yMarks[j + 1]) / 2)
					{
						vLine[3] = yMarks[j + 1];
					}
					else
					{
						vLine[3] = yMarks[j];
					}
					break;
				}
			}
		}
		sort(hLines.begin(), hLines.end(), compareHorizontal);
		sort(vLines.begin(), vLines.end(), compareVertical);
	}
	// 针对不规则的表格
	bool getRect(int row, int col, Rect& rect) const
	{
		int x1 = xMarks[col];
		int x2 = xMarks[col + 1];
		int y1 = yMarks[row];
		int y2 = yMarks[row + 1];
		for (int i = (int)vLines.size() - ((int)getColumnCount() - col); i >= 0; i--)
		{
			const Vec4i& vLine = vLines[i];
			if (vLine[0] <= x1 && vLine[1] <= y1 && vLine[3] >= y2)
			{
				rect.x = vLine[0];
				break;
			}
		}
		for (int i = col; i < (int)vLines.size(); i++)
		{
			const Vec4i& vLine = vLines[i];
			if (vLine[0] >= x2 && vLine[1] <= y1 && vLine[3] >= y2)
			{
				rect.width = vLine[0] - rect.x;
				break;
			}
		}
		for (int i = (int)hLines.size() - ((int)getRowCount() - row); i >= 0; i--)
		{
			const Vec4i& hLine = hLines[i];
			if (hLine[1] <= y1 && hLine[0] <= x1 && hLine[2] >= x2)
			{
				rect.y = hLine[1];
				break;
			}
		}
		for (int i = row; i < (int)hLines.size(); i++)
		{
			const Vec4i& hLine = hLines[i];
			if (hLine[1] >= y2 && hLine[0] <= x1 && hLine[2] >= x2)
			{
				rect.height = hLine[1] - rect.y;
				break;
			}
		}
		return (rect.x == x1 && rect.y == y1);
	}

	void drawLines(Mat& img, const Scalar& color, int thickness)
	{
		for (size_t i = 0; i < hLines.size(); i++)
		{
			Vec4i& hLine = hLines[i];
			line(img, Point(hLine[0], hLine[1]), Point(hLine[2], hLine[3]), color, thickness);
		}
		for (size_t i = 0; i < vLines.size(); i++)
		{
			Vec4i& vLine = vLines[i];
			line(img, Point(vLine[0], vLine[1]), Point(vLine[2], vLine[3]), color, thickness);
		}
	}

private:
	Mat horizontal;
	Mat vertical;

	std::vector<Vec4i> hLines;
	std::vector<Vec4i> vLines;

	std::vector<int> xMarks;
	std::vector<int> yMarks;

	static void update(std::vector<Vec4i>& lines, std::set<int> indexes)
	{
		std::vector<Vec4i> temp;
		temp.swap(lines);
		for (std::set<int>::iterator it = indexes.begin(); it != indexes.end(); it++)
		{
			lines.push_back(temp[*it]);
		}
	}
	static bool isCrossed(Vec4i hLine, Vec4i vLine, int delta)
	{
		int left = hLine[0] - delta;
		int right = hLine[2] + delta;
		int top = vLine[1] - delta;
		int bottom = vLine[3] + delta;
		return (left <= vLine[0] && right >= vLine[2] &&
			top <= hLine[1] && bottom >= hLine[3]);
	}

	static void reduce(std::vector<int>& v, int delta)
	{
		sort(v.begin(), v.end());
		for (size_t i = 1; i < v.size(); i++)
		{
			if (v[i] - v[i - 1] < delta)
			{
				v[i] = (v[i] + v[i - 1]) / 2;
				v[i - 1] = 0;
			}
		}
		std::vector<int> temp;
		temp.swap(v);
		for (size_t i = 0; i < temp.size(); i++)
		{
			if (temp[i] != 0)
			{
				v.push_back(temp[i]);
			}
		}
	}

	static bool compareHorizontal(const Vec4i& a, const Vec4i& b)
	{
		if (a[1] < b[1])
			return true;
		if (a[1] == b[1] && a[0] < b[0])
			return true;
		return false;
	}
	static bool compareVertical(const Vec4i& a, const Vec4i& b)
	{
		if (a[0] < b[0])
			return true;
		if (a[0] == b[0] && a[1] < b[1])
			return true;
		return false;
	}
};

class TableExtractor
{
public:
	Mat src;
	Mat gray;
	Mat bw;
	Mat horizontal;
	Mat vertical;

	std::vector<Rect> rects;

	TableExtractor(Mat src)
	{
		this->src = src;
	}

	void prepare(int minSize)
	{
		gray = toGray(src);
		bw = toBinary(gray);
		horizontal = toLines(bw, getStructuringElement(MORPH_RECT, Size(minSize, 1)));
		vertical = toLines(bw, getStructuringElement(MORPH_RECT, Size(1, minSize)));
	}

	// Find external contours from the mask, which most probably will belong to tables or to images
	void locateTables(double minArea, double epsilon, int delta)
	{
		Mat mask = toMask(horizontal, vertical);
		Mat joints = toJoints(horizontal, vertical);
		std::vector<Vec4i> hierarchy;
		std::vector<std::vector<Point> > contours;
		findContours(mask, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
		std::vector<std::vector<Point> > contours_poly(contours.size());
		std::vector<Rect> boundRect(contours.size());

		for (size_t i = 0; i < contours.size(); i++)
		{
			// find the area of each contour
			double area = contourArea(contours[i]);
			//        // filter individual lines of blobs that might exist and they do not represent a table
			if (area < minArea) // value is randomly chosen, you will need to find that by yourself with trial and error procedure
				continue;
			approxPolyDP(Mat(contours[i]), contours_poly[i], epsilon, true);
			boundRect[i] = boundingRect(Mat(contours_poly[i]));
			// find the number of joints that each table has
			Mat roi = joints(boundRect[i]);
			std::vector<std::vector<Point> > joints_contours;
			findContours(roi, joints_contours, RETR_CCOMP, CHAIN_APPROX_SIMPLE);
			// if the number is not more than 6 then most likely it not a table
			if (joints_contours.size() < 6)
				continue;
			boundRect[i].x -= delta;
			boundRect[i].y -= delta;
			boundRect[i].width += delta * 2;
			boundRect[i].height += delta * 2;
			rects.push_back(boundRect[i]);
		}
	}

	// Transform source image to gray if it is not
	static Mat toGray(Mat src)
	{
		Mat gray;
		if (src.channels() != 3)
		{
			gray = src;
		}
		else
		{
			cvtColor(src, gray, CV_BGR2GRAY);
		}
		return gray;
	}
	// Apply adaptiveThreshold at the bitwise_not of gray, notice the ~ symbol
	static Mat toBinary(Mat gray)
	{
		Mat bw;
		adaptiveThreshold(~gray, bw, 255, CV_ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 15, -2);
		return bw;
	}
	// Create the images that will use to extract the horizonta and vertical lines
	static Mat toLines(Mat bw, Mat structure)
	{
		Mat mat = bw.clone();
		// Apply morphology operations
		erode(mat, mat, structure, Point(-1, -1));
		dilate(mat, mat, structure, Point(-1, -1));
		//    dilate(vertical, vertical, verticalStructure, Point(-1, -1)); // expand vertical lines
		return mat;
	}
	// create a mask which includes the tables
	static Mat toMask(Mat horizontal, Mat vertical)
	{
		return horizontal + vertical;
	}
	// find the joints between the lines of the tables, we will use this information in order to descriminate tables from pictures (tables will contain more than 4 joints while a picture only 4 (i.e. at the corners))
	static Mat toJoints(Mat horizontal, Mat vertical)
	{
		Mat joints;
		bitwise_and(horizontal, vertical, joints);
		return joints;
	}
};

#endif