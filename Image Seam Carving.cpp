#include <opencv2/opencv.hpp>
#include <bits/stdc++.h>
using namespace std;
using namespace cv;
int m, n;

float **costArr = new float *[8000];
float **mat = new float *[8000];

float calculateEnergy(int i, int j, Mat &I)
{
  int ldx = j - 1;
  int rdx = j + 1;
  int udy = i - 1;
  int ldy = i + 1;
  if (ldx < 0)
    ldx = n - 1;
  if (rdx >= n)
    rdx = 0;
  if (udy < 0)
    udy = m - 1;
  if (ldy >= m)
    ldy = 0;
  Vec3b rx = I.at<Vec3b>(i, rdx);
  Vec3b lx = I.at<Vec3b>(i, ldx);
  int xr = rx[0] - lx[0];
  int xg = rx[1] - lx[1];
  int xb = rx[2] - lx[2];
  xr = (int)pow(xr, 2);
  xg = (int)pow(xg, 2);
  xb = (int)pow(xb, 2);
  int dx = xr + xg + xb;
  Vec3b uy = I.at<Vec3b>(udy, j);
  Vec3b ly = I.at<Vec3b>(ldy, j);
  int yr = ly[0] - uy[0];
  int yg = ly[1] - uy[1];
  int yb = ly[2] - uy[2];
  yr = (int)pow(yr, 2);
  yg = (int)pow(yg, 2);
  yb = (int)pow(yb, 2);
  int dy = yr + yg + yb;
  return sqrt(dx + dy);
}
void calculateEnergy(Mat &I)
{

  for (int i = 0; i < I.rows; i++)
  {
    for (int j = 0; j < I.cols; j++)
    {
      mat[i][j] = calculateEnergy(i, j, I);
    }
  }
}
int cx[3] = {1, 1, 1};
int cy[3] = {0, -1, 1};

void calculateCostY()
{
  for (int j = 0; j < n; j++)
  {
    costArr[m - 1][j] = mat[m - 1][j];
  }
  for (int i = m - 2; i >= 0; i--)
  {
    for (int j = 0; j < n; j++)
    {
      float minVal = (float)INT_MAX;
      for (int k = 0; k < 3; k++)
      {
        int x = i + cx[k];
        int y = j + cy[k];
        if (x >= 0 && x < m && y >= 0 && y < n)
        {
          if (costArr[x][y] < minVal)
          {
            minVal = costArr[x][y];
          }
        }
      }
      costArr[i][j] = mat[i][j] + minVal;
    }
  }
}
int dx[3] = {-1, 0, 1};
int dy[3] = {1, 1, 1};
void calculateCostX()
{
  for (int i = 0; i < m; i++)
  {
    costArr[i][n - 1] = mat[i][n - 1];
  }
  for (int j = n - 2; j >= 0; j--)
  {
    for (int i = 0; i < m; i++)
    {
      float minVal = (float)INT_MAX;
      for (int k = 0; k < 3; k++)
      {
        int x = i + dx[k];
        int y = j + dy[k];
        if (x >= 0 && x < m && y >= 0 && y < n)
        {
          if (costArr[x][y] < minVal)
          {
            minVal = costArr[x][y];
          }
        }
      }
      costArr[i][j] = mat[i][j] + minVal;
    }
  }
}

int seamPath[1000][2];
void findSeamY()
{
  float minVal = (float)INT_MAX;
  for (int j = 0; j < n; j++)
  {
    if (costArr[0][j] < minVal)
    {
      minVal = costArr[0][j];
      seamPath[0][0] = 0;
      seamPath[0][1] = j;
    }
  }
  for (int i = 1; i < m; i++)
  {
    minVal = (float)INT_MAX;
    for (int k = 0; k < 3; k++)
    {
      int x = seamPath[i - 1][0] + cx[k];
      int y = seamPath[i - 1][1] + cy[k];
      if (x >= 0 && x < m && y >= 0 && y < n)
      {
        if (costArr[x][y] < minVal)
        {
          minVal = costArr[x][y];
          seamPath[i][0] = x;
          seamPath[i][1] = y;
        }
      }
    }
  }
}
void findSeamX()
{
  float minVal = (float)INT_MAX;
  for (int i = 0; i < m; i++)
  {
    if (costArr[i][0] < minVal)
    {
      minVal = costArr[i][0];
      seamPath[0][0] = i;
      seamPath[0][1] = 0;
    }
  }
  for (int j = 1; j < n; j++)
  {
    minVal = (float)INT_MAX;
    for (int k = 0; k < 3; k++)
    {
      int x = seamPath[j - 1][0] + dx[k];
      int y = seamPath[j - 1][1] + dy[k];
      if (x >= 0 && x < m && y >= 0 && y < n)
      {
        if (costArr[x][y] < minVal)
        {
          minVal = costArr[x][y];
          seamPath[j][0] = x;
          seamPath[j][1] = y;
        }
      }
    }
  }
}
void ReduceMatY(Mat &I)
{
  for (int i = 0; i < m; i++)
  {
    int x = seamPath[i][0];
    int y = seamPath[i][1];
    while (y < n - 1)
    {
      I.at<cv::Vec3b>(x, y) = I.at<cv::Vec3b>(x, y + 1);
      mat[x][y] = mat[x][y + 1];

      y++;
    }
  }
}
void ReduceMatX(Mat &I)
{
  for (int j = 0; j < n; j++)
  {
    int x = seamPath[j][0];
    int y = seamPath[j][1];
    while (x < m - 1)
    {
      I.at<cv::Vec3b>(x, y) = I.at<cv::Vec3b>(x + 1, y);
      mat[x][y] = mat[x + 1][y];

      x++;
    }
  }
}

int main()
{
  string path;
  cout << "Enter the path to the file:" << endl;
  cin >> path;
  Mat I;
  I = imread(path, IMREAD_COLOR);
  cout << I.rows << " " << I.cols << endl;

  int rows = I.rows;
  int cols = I.cols;
  m = rows;
  n = cols;
  int h, w;
  cout<<"Enter height and width required for the resized image"<<endl;
  cin >> h >> w;

  for (int i = 0; i < 8000; i++)
  {
    costArr[i] = new float[8000];
    mat[i] = new float[8000];
  }

  // imshow("Original image", I);
  calculateEnergy(I);
  for (int p = 0; p < cols - w; p++)
  {

    calculateCostY();
    findSeamY();
    ReduceMatY(I);
    n--;
  }
  for (int p = 0; p < rows - h; p++)
  {

    calculateCostX();
    findSeamX();
    ReduceMatX(I);
    m--;
  }

  cv::Rect roi(0, 0, n, m);

  cv::Mat croppedImage = I(roi);
  imshow("Resized Image", croppedImage);
  int i;
  for (i = path.length() - 1; i >= 0; i--)
  {
    if (path[i] == '.')
      break;
  }
  string extension = path.substr(i);
  if (!imwrite("output" + extension, croppedImage))
    cout << "Could not save the image" << endl;
  else
    cout << "Successfully saved the resized image to output"+extension << endl;

  waitKey(0);
  destroyAllWindows();

  for (int i = 0; i < 8000; i++)
  {
    delete[] costArr[i];
    delete[] mat[i];
  }
  delete[] costArr;
  delete[] mat;
}
