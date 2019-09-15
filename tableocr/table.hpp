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

	void prepare(int minSize, int threshold, int delta)
	{
		calcLines(threshold);
		mergeLines(minSize, delta);
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

	bool mergeLines(int minSize, int delta)
	{
		bool isMerged = false;
		sort(hLines.begin(), hLines.end(), compareHorizontal);
		for (size_t i = 0; i < hLines.size(); i++)
		{
			for (size_t j = i + 1; j < hLines.size(); j++)
			{
				if (ypos(hLines[i]) + delta < ypos(hLines[j]))
					break;
				Vec4i& line1 = hLines[left(hLines[i]) <= left(hLines[j]) ? i : j];
				Vec4i& line2 = hLines[left(hLines[i]) <= left(hLines[j]) ? j : i];
				if (left(line2) > right(line1))
				{
					if (left(line2) - right(line1) < delta ||
						(left(line2) - right(line1) < minSize && right(line2) - left(line1) > horizontal.cols / 2))
					{
						line2[1] = line2[3] = (line1[1] + line2[1]) / 2;
						line2[0] = line1[0];
						line1[2] = line1[0] = -1; //will be removed afterward
						isMerged = true;
					}
				}
			}
		}
		sort(vLines.begin(), vLines.end(), compareVertical);
		for (size_t i = 0; i < vLines.size(); i++)
		{
			for (size_t j = i + 1; j < vLines.size(); j++)
			{
				if (xpos(vLines[i]) + delta < xpos(vLines[j]))
					break;
				Vec4i& line1 = vLines[top(vLines[i]) <= top(vLines[j]) ? i : j];
				Vec4i& line2 = vLines[top(vLines[i]) <= top(vLines[j]) ? j : i];
				if (top(line2) > bottom(line1))
				{
					if (top(line2) - bottom(line1) < delta ||
						(top(line2) - bottom(line1) < minSize && bottom(line2) - top(line1) > horizontal.rows / 2))
					{
						line2[0] = line2[2] = (line1[0] + line2[0]) / 2;
						line2[1] = line1[1];
						line1[3] = line1[1] = -1; //will be removed afterward
						isMerged = true;
					}
				}
			}
		}
		return isMerged;
	}

	bool removeIsolatedLines(int delta)
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
		bool isRemoved = (hLines.size() != hSet.size() || vLines.size() != vSet.size());
		update(hLines, hSet);
		update(vLines, vSet);
		return isRemoved;
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
	// should be called when isMerged = false
	Rect toRect(int row, int col) const
	{
		return Rect(xMarks[col], yMarks[row], xMarks[col + 1] - xMarks[col], yMarks[row + 1] - yMarks[row]);
	}

	// should be called when isMerged = true
	void AdjustTable(int delta)
	{
		AdjustPosition();

		AdjustLength(delta, TRUE, FALSE);
		AdjustLength(delta, FALSE, TRUE);

		calcMarks(delta);
		AdjustByCells();
	}
	void AdjustPosition()
	{
		for (size_t i = 0; i < hLines.size(); i++)
		{
			Vec4i& hLine = hLines[i];
			for (size_t j = 1; j < yMarks.size(); j++)
			{
				if (yMarks[j] >= hLine[1])
				{
					if (hLine[1] > (yMarks[j - 1] + yMarks[j]) / 2)
					{
						hLine[1] = hLine[3] = yMarks[j];
					}
					else
					{
						hLine[1] = hLine[3] = yMarks[j - 1];
					}
					break;
				}
			}
		}
		for (size_t j = 0; j < vLines.size(); j++)
		{
			Vec4i& vLine = vLines[j];
			for (size_t i = 1; i < xMarks.size(); i++)
			{
				if (xMarks[i] >= vLine[0])
				{
					if (vLine[0] > (xMarks[i - 1] + xMarks[i]) / 2)
					{
						vLine[0] = vLine[2] = xMarks[i];
					}
					else
					{
						vLine[0] = vLine[2] = xMarks[i - 1];
					}
					break;
				}
			}
		}
	}
	void AdjustLength(int delta, BOOL increase, BOOL decrease)
	{
		sort(hLines.begin(), hLines.end(), compareHorizontal);
		sort(vLines.begin(), vLines.end(), compareVertical);

		for (size_t i = 0; i < hLines.size(); i++)
		{
			Vec4i& hLine = hLines[i];
			int x = hLine[0];
			for (size_t j = 0; j < vLines.size(); j++)
			{
				Vec4i& vLine = vLines[j];
				if (isCrossed(hLine, vLine, delta))
				{
					x = xpos(vLine);
					continue;
				}
				if (xpos(vLine) > right(hLine) && top(vLine) <= ypos(hLine) && bottom(vLine) >= ypos(hLine))
				{
					if (increase)
					{
						if (right(hLine) > (x + xpos(vLine)) / 2)
						{
							hLine[2] = xpos(vLine);
						}
					}
					if (decrease)
					{
						if (right(hLine) <= (x + xpos(vLine)) / 2)
						{
							hLine[2] = x;
						}
					}
					break;
				}
			}
			x = right(hLine);
			for (int j = (int)vLines.size() - 1; j >= 0; j--)
			{
				Vec4i& vLine = vLines[j];
				if (isCrossed(hLine, vLine, delta))
				{
					x = xpos(vLine);
					continue;
				}
				if (xpos(vLine) < left(hLine) && top(vLine) <= ypos(hLine) && bottom(vLine) >= ypos(hLine))
				{
					if (increase)
					{
						if (left(hLine) < (x + xpos(vLine)) / 2)
						{
							hLine[0] = xpos(vLine);
						}
					}
					if (decrease)
					{
						if (left(hLine) >= (x + xpos(vLine)) / 2)
						{
							hLine[0] = x;
						}
					}
					break;
				}
			}
		}
		for (size_t j = 0; j < vLines.size(); j++)
		{
			Vec4i& vLine = vLines[j];
			int y = top(vLine);
			for (size_t i = 0; i < hLines.size(); i++)
			{
				Vec4i& hLine = hLines[i];
				if (isCrossed(hLine, vLine, delta))
				{
					y = ypos(hLine);
					continue;
				}
				if (ypos(hLine) > bottom(vLine) && left(hLine) <= xpos(vLine) && right(hLine) >= xpos(vLine))
				{
					if (increase)
					{
						if (bottom(vLine) > (y + ypos(hLine)) / 2)
						{
							vLine[3] = ypos(hLine);
						}
					}
					if (decrease)
					{
						if (bottom(vLine) <= (y + ypos(hLine)) / 2)
						{
							vLine[3] = y;
						}
					}
					break;
				}
			}
			y = bottom(vLine);
			for (int i = (int)hLines.size() - 1; i >= 0; i--)
			{
				Vec4i& hLine = hLines[i];
				if (isCrossed(hLine, vLine, delta))
				{
					y = ypos(hLine);
					continue;
				}
				if (ypos(hLine) < top(vLine) && left(hLine) <= xpos(vLine) && right(hLine) >= xpos(vLine))
				{
					if (increase)
					{
						if (top(vLine) < (y + ypos(hLine)) / 2)
						{
							vLine[1] = ypos(hLine);
						}
					}
					if (decrease)
					{
						if (top(vLine) >= (y + ypos(hLine)) / 2)
						{
							vLine[1] = y;
						}
					}
					break;
				}
			}
		}
	}
	bool AdjustByCells()
	{
		bool isDirty = false;
		for (int i = 0; i < (int)getRowCount(); i++)
		{
			for (int j = 0; j < (int)getColumnCount(); j++)
			{
				Rect rect;
				getRect(i, j, rect);
				int x1 = rect.x;
				int x2 = rect.x + rect.width;
				int y1 = rect.y;
				int y2 = rect.y + rect.height;
				if (y1<yMarks[i] || y2 >yMarks[i + 1])
				{
					for (size_t k = 0; k < hLines.size(); k++)
					{
						Vec4i& hLine = hLines[k];
						if (ypos(hLine) > y1 && ypos(hLine) < y2)
						{
							if (left(hLine) < x2 && right(hLine) > x1)
							{
								isDirty = true;
								if (left(hLine) < x1)
								{
									hLine[2] = x1;
								}
								else if (right(hLine) > x2)
								{
									hLine[0] = x2;
								}
								else
								{
									hLine[0] = hLine[2] = -1;
								}
							}
						}
					}
				}
				if (x1<xMarks[j] || x2>xMarks[j + 1])
				{
					for (size_t k = 0; k < vLines.size(); k++)
					{
						Vec4i& vLine = vLines[k];
						if (xpos(vLine) > x1 && xpos(vLine) < x2)
						{
							if (top(vLine) < y2 && bottom(vLine) > y1)
							{
								isDirty = true;
								if (top(vLine) < y1)
								{
									vLine[3] = y1;
								}
								else if (bottom(vLine) > y2)
								{
									vLine[1] = y2;
								}
								else
								{
									vLine[1] = vLine[3] = -1;
								}
							}
						}
					}
				}
			}
		}
		return isDirty;
	}
	// should be called when isMerged = true
	bool getRect(int row, int col, Rect& rect) const
	{
		int x1 = xMarks[col];
		int x2 = xMarks[col + 1];
		int y1 = yMarks[row];
		int y2 = yMarks[row + 1];
		for (int i = (int)vLines.size() - ((int)getColumnCount() - col); i >= 0; i--)
		{
			const Vec4i& vLine = vLines[i];
			if (xpos(vLine) <= x1 && top(vLine) <= y1 && bottom(vLine) >= y2)
			{
				rect.x = xpos(vLine);
				break;
			}
		}
		for (int i = col; i < (int)vLines.size(); i++)
		{
			const Vec4i& vLine = vLines[i];
			if (xpos(vLine) >= x2 && top(vLine) <= y1 && bottom(vLine) >= y2)
			{
				rect.width = xpos(vLine) - rect.x;
				break;
			}
		}
		for (int i = (int)hLines.size() - ((int)getRowCount() - row); i >= 0; i--)
		{
			const Vec4i& hLine = hLines[i];
			if (ypos(hLine) <= y1 && left(hLine) <= x1 && right(hLine) >= x2)
			{
				rect.y = ypos(hLine);
				break;
			}
		}
		for (int i = row; i < (int)hLines.size(); i++)
		{
			const Vec4i& hLine = hLines[i];
			if (ypos(hLine) >= y2 && left(hLine) <= x1 && right(hLine) >= x2)
			{
				rect.height = ypos(hLine) - rect.y;
				break;
			}
		}
		return (rect.x == x1 && rect.y == y1);
	}
	int getEndRow(const Rect& rect, int row) const
	{
		for (int i = row + 1; i < yMarks.size(); i++)
		{
			if (yMarks[i] >= rect.y + rect.height)
				return i - 1;
		}
		return -1;
	}
	int getEndColumn(const Rect& rect, int col) const
	{
		for (int j = col + 1; j < xMarks.size(); j++)
		{
			if (xMarks[j] >= rect.x + rect.width)
				return j - 1;
		}
		return -1;
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
			if (v[i] - v[i - 1] <= delta)
			{
				v[i] = (v[i] + v[i - 1]) / 2;
				v[i - 1] = -1;
			}
		}
		std::vector<int> temp;
		temp.swap(v);
		for (size_t i = 0; i < temp.size(); i++)
		{
			if (temp[i] >= 0)
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

	static int left(const Vec4i& hLine)
	{
		return hLine[0];
	}
	static int right(const Vec4i& hLine)
	{
		return hLine[2];
	}
	static int ypos(const Vec4i& hLine)
	{
		return hLine[1];
	}
	static int top(const Vec4i& vLine)
	{
		return vLine[1];
	}
	static int bottom(const Vec4i& vLine)
	{
		return vLine[3];
	}
	static int xpos(const Vec4i& vLine)
	{
		return vLine[0];
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
			if (boundRect[i].x - delta >= 0 && boundRect[i].y - delta >= 0 &&
				boundRect[i].x + boundRect[i].width + delta <= bw.cols && boundRect[i].y + boundRect[i].height + delta <= bw.rows)
			{
				boundRect[i].x -= delta;
				boundRect[i].y -= delta;
				boundRect[i].width += delta * 2;
				boundRect[i].height += delta * 2;
			}
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