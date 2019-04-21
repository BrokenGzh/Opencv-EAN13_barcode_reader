
// ShowimageDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "Showimage.h"
#include "ShowimageDlg.h"
#include "afxdialogex.h"
#include"opencv2/opencv.hpp"
#include"opencv2/highgui/highgui_c.h"
using namespace std;
using namespace cv;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CShowimageDlg 对话框



CShowimageDlg::CShowimageDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SHOWIMAGE_DIALOG, pParent)
	, barcode(_T(""))
	, type(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CShowimageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, barcode);
	DDX_Text(pDX, IDC_EDIT2, type);
}

BEGIN_MESSAGE_MAP(CShowimageDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CShowimageDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &CShowimageDlg::OnBnClickedButton1)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CShowimageDlg 消息处理程序

BOOL CShowimageDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。


	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	GetClientRect(&m_rect);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CShowimageDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CShowimageDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CShowimageDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void ConvertMat2CImage(const cv::Mat& src_img, CImage& dst_img)
{
	int width = src_img.cols;           //获取输入图像的宽度  
	int height = src_img.rows;          //获取输入图像的高度  
	int channels = src_img.channels();  //获取输入图像的  
	int src_type = src_img.type();

	dst_img.Destroy();

	switch (src_type)
	{
	case CV_8UC1:
	{
		dst_img.Create(width, -1 * height, 8 * channels);
		unsigned char* dst_data = static_cast<unsigned char*>(dst_img.GetBits());
		int step_size = dst_img.GetPitch();     //获取位图行与行之间相差的字节数  
		unsigned char* src_data = nullptr;
		for (int i = 0; i < height; i++)
		{
			src_data = const_cast<unsigned char*>(src_img.ptr<unsigned char>(i));   //获取行指针  
			for (int j = 0; j < width; j++)
			{
				if (step_size > 0)
				{
					*(dst_data + step_size * i + j) = *src_data++;
				}   //像素的排列方式是自左上开始的  
				else
				{
					*(dst_data + step_size * i - j) = *src_data++;
				}
			}
		}
		break;
	}
	case CV_8UC3:
	{
		dst_img.Create(width, height, 8 * channels);
		unsigned char* dst_data = static_cast<unsigned char*>(dst_img.GetBits());
		int step_size = dst_img.GetPitch();     //获取位图行与行之间相差的字节数  
		unsigned char* src_data = nullptr;
		for (int i = 0; i < height; i++)
		{
			src_data = const_cast<unsigned char*>(src_img.ptr<unsigned char>(i));   //获取行指针  
			for (int j = 0; j < width; j++)
			{
				for (int k = 0; k < 3; k++)
				{
					*(dst_data + step_size * i + j * 3 + k) = src_data[3 * j + k];
				}
			}
		}
		break;
	}
	default:
		MessageBox(NULL, _T("输入的图像类型出错"), _T("提示"), MB_ICONINFORMATION | MB_OK);
		break;
	}
};

//定义全局变量
Mat scrImg;
CString strCodeStyle, strCodeNumber;
uchar ImageArray[500][500];

void CShowimageDlg::OnBnClickedOk()
{
	// 设置过滤器   
	TCHAR szFilter[] = _T("图像文件(*.bmp; *.jpg)|*.bmp; *.jpg|All Files (*.*)|*.*||");
	// 构造打开文件对话框   
	CFileDialog fileDlg(TRUE, _T("bmp"), NULL, 0, szFilter, this);
	CString strFilePath;

	// 显示打开文件对话框   
	if (IDOK == fileDlg.DoModal())
	{
		strFilePath= fileDlg.GetPathName();
		USES_CONVERSION;
		cv::String cvStr = W2A(strFilePath);   //转化string类型
		scrImg = imread(cvStr);
		Mat img;
		scrImg.convertTo(img,CV_8U);//将矩阵中数据转化为uchar类型，以便进行到cimage类转化

		CImage CI;
		ConvertMat2CImage(img, CI);
		/*
		int height, width;
		CRect rect;//定义矩形类
		CRect rect1;
		height = CI.GetHeight();
		width = CI.GetWidth(); 
		*/

		float cx, cy, dx, dy, k, t;//跟控件的宽和高以及图片宽和高有关的参数
		CRect   rect;//用于获取图片控件的宽和高
		cx =CI.GetWidth();
		cy = CI.GetHeight();//获取图片的宽 高
		k = cy / cx;//获得图片的宽高比

		CWnd *pWnd = NULL;
		pWnd = GetDlgItem(IDC_PIC);//获取控件句柄
		pWnd->GetClientRect(&rect);//获取Picture Control控件的客户区
		dx = rect.Width();
		dy = rect.Height();//获得控件的宽高比
		t = dy / dx;//获得控件的宽高比
		if (k >= t)
		{

			rect.right = floor(rect.bottom / k);
			rect.left = (dx - rect.right) / 2;
			rect.right = floor(rect.bottom / k) + (dx - rect.right) / 2;
		}
		else
		{
			rect.bottom = floor(k*rect.right);
			rect.top = (dy - rect.bottom) / 2;

			rect.bottom = floor(k*rect.right) + (dy - rect.bottom) / 2;
		}
		//相关的计算为了让图片在绘图区居中按比例显示，如果图片很宽但是不高，就上下留有空白区；如果图片很高而不宽就左右留有空白区，并且保持两边空白区一样大

		CDC *pDc = NULL;
		pDc = pWnd->GetDC();//获取图片控件的DC
		int ModeOld = SetStretchBltMode(pDc->m_hDC, STRETCH_HALFTONE);//设置指定设备环境中的位图拉伸模式

		GetDlgItem(IDC_PIC)->ShowWindow(FALSE);
		GetDlgItem(IDC_PIC)->ShowWindow(TRUE);
		CI.StretchBlt(pDc->m_hDC, rect, SRCCOPY);//显示函数
		SetStretchBltMode(pDc->m_hDC, ModeOld);
		ReleaseDC(pDc);//释放指针空间
	}
};

//条形码边缘检测函数，框选出条形码
Mat BarcodeScan(Mat &img)
{
	Mat greyimg;
	Mat resultImage;
	cvtColor(img,greyimg, CV_RGB2GRAY); //转化为灰度图
	img.convertTo(img, CV_8U);//深度变为8位，便于进行接下来运算
	Mat greyXimg, greyYimg;
	Sobel(greyimg, greyXimg, CV_32F, 1,0,-1);
	Sobel(greyimg, greyYimg, CV_32F, 0,1,-1); //xy方向的梯度
	Mat imgsobel,blurImage, thresholdImage, morphImage,angleMat;
	cartToPolar(greyXimg, greyYimg, imgsobel, angleMat);
	convertScaleAbs(imgsobel, imgsobel); //再转化为八位图像
	blur(imgsobel, blurImage, Size(5, 5));
	threshold(blurImage, thresholdImage, 160, 255, THRESH_BINARY); //二值化
	Mat kernel = getStructuringElement(MORPH_RECT, Size(21, 7));//寻找内核
	morphologyEx(thresholdImage, morphImage, MORPH_CLOSE, kernel);//闭运算
	erode(morphImage, morphImage, getStructuringElement(MORPH_RECT, Size(3, 3)), Point(-1, -1), 4);
	dilate(morphImage, morphImage, getStructuringElement(MORPH_RECT, Size(3, 3)), Point(-1, -1), 4);//腐蚀过后再膨胀
	vector<vector<Point2i>>contours;
	vector<float>contourArea;//创建二维浮点向量储存坐标
	findContours(morphImage, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE); //找到边缘四个点
	//计算轮廓的面积并且存放
	for (int i = 0; i < contours.size(); i++)
	{
		contourArea.push_back(cv::contourArea(contours[i]));
	}
	//找出面积最大的轮廓
	double maxValue; Point maxLoc;
	minMaxLoc(contourArea, NULL, &maxValue, NULL, &maxLoc);
	//计算面积最大的轮廓的最小的外包矩形
	RotatedRect minRect = minAreaRect(contours[maxLoc.x]);
	//为了防止找错,要检查这个矩形的偏斜角度不能超标
	//如果超标,那就是没找到
	if (minRect.angle < 2.0)
	{
		//找到了矩形的角度,但是这是一个旋转矩形,所以还要重新获得一个外包最小矩形
		Rect myRect = boundingRect(contours[maxLoc.x]);
		//把这个矩形在源图像中画出来
		rectangle(img,myRect,Scalar(0,255,255),3,LINE_AA);
		//看看显示效果,找的对不对
		//将扫描的图像裁剪下来,并保存为相应的结果,保留一些X方向的边界,所以对rect进行一定的扩张
		myRect.x = myRect.x - (myRect.width / 20);
		myRect.width = myRect.width*1.1;
		myRect.height = myRect.height*1.1;
		resultImage = Mat(img, myRect);
	}
	return resultImage;
};

int JudgNum(int w1, int w2, int w3, int w4, double mx)
{
	double a1, a2, a3;
	int ia1, ia2, ia3;
	a1 = (double)(w1 + w2) / mx;
	a2 = (double)(w2 + w3) / mx;
	a3 = (double)(w3 + w4) / mx;
	ia1 = (int)(a1 + 0.5);
	ia2 = (int)(a2 + 0.5);
	ia3 = (int)(a3 + 0.5);

	//判断该码值
	if ((ia1 == 5 && ia2 == 3 && ia3 == 2) || (ia1 == 2 && ia2 == 3 && ia3 == 5))
		return 0;

	if ((ia1 == 4 && ia2 == 4 && ia3 == 3) || (ia1 == 3 && ia2 == 4 && ia3 == 4))
	{
		if (ia1 == 4)
		{
			double dw2 = (double)w2 / mx;
			if (dw2 < 2.4)
				return 1;
			else if (dw2 > 2.6)
				return 7;
			else return -1;
		}

		if (ia1 == 3)
		{
			double dw3 = (double)w3 / mx;
			if (dw3 < 2.4)
				return 1;
			else if (dw3 > 2.6)
				return 7;
			else return -1;
		}
	}

	if ((ia1 == 3 && ia2 == 3 && ia3 == 4) || (ia1 == 4 && ia2 == 3 && ia3 == 3))
	{
		if (ia1 == 3)
		{
			double dw4 = (double)w4 / mx;
			if (dw4 < 2.4)
				return 2;
			else if (dw4 > 2.6)
				return 8;
			else return -1;
		}

		if (ia1 == 4)
		{
			double dw1 = (double)w1 / mx;
			if (dw1 < 2.4)
				return 2;
			else if (dw1 > 2.6)
				return 8;
			else return -1;
		}
	}

	if ((ia1 == 5 && ia2 == 5 && ia3 == 2) || (ia1 == 2 && ia2 == 5 && ia3 == 5))
		return 3;

	if ((ia1 == 2 && ia2 == 4 && ia3 == 5) || (ia1 == 5 && ia2 == 4 && ia3 == 2))
		return 4;

	if ((ia1 == 3 && ia2 == 5 && ia3 == 4) || (ia1 == 4 && ia2 == 5 && ia3 == 3))
		return 5;

	if ((ia1 == 2 && ia2 == 2 && ia3 == 5) || (ia1 == 5 && ia2 == 2 && ia3 == 2))
		return 6;

	if ((ia1 == 4 && ia2 == 2 && ia3 == 3) || (ia1 == 3 && ia2 == 2 && ia3 == 4))
		return 9;

	return false;
};

bool PreProcess(Mat &img)
{
	img.convertTo(img, CV_8U);//深度变为8位，便于进行接下来运算
	cvtColor(img, img, CV_RGB2GRAY); //转化为灰度图
	//二值化
	threshold(img, img, 160, 255, THRESH_BINARY);
	int i, j;
	int tempMax;
	int tempArray[1000];
	long ImageHeight = img.rows;
	long ImageWidth = img.cols;
	int arWidth[200];				//保存宽度序列：从最左边的黑条开始
	int arDifference[1000];			//差分
	bool arMark[1000];				//标记
	int ImageLeft;					//图像最左边的座标
	int ImageRight;					//图像最右边的座标
	int ImageTop;					//条形码顶部的座标
	int ImageBottom;				//条形码下部的座标
	int arPixelH[1000];				//为了水平方向直方图统计用
	int arPixelV[1000];				//为了垂直方向直方图统计用
	int arDelta[1000];
	int arLeftEdge1[1000];
	int arLeftEdge2[1000];
	int arLeftEdge[1000];
	//进行水平方向和垂直方向上的直方图统计
	for (i = 0; i < ImageHeight; i++)
	{
		for (j = 0; j < ImageWidth; j++)
		{
			if (img.at<uchar>(i, j) >= 180)
				ImageArray[i][j] = (BYTE)0;
			else
				ImageArray[i][j] = (BYTE)1;
		}
	}
	for (i = 0; i < ImageHeight; i++)
		arPixelV[i] = 0;
	for (i = 0; i < ImageWidth; i++)
		arPixelH[i] = 0;
	for (i = 0; i < ImageHeight; i++)
	{
		for (j = 0; j < ImageWidth; j++)
		{
			if (ImageArray[i][j] == 1)
			{
				arPixelV[i] += 1;
				arPixelH[j] += 1;
			}
		}
	}

	//寻找包含条形码的区域，
	//线寻找水平方向上黑象素最大的行

	tempMax = 0;
	for (i = 0; i < ImageHeight; i++)
	{
		if (arPixelV[i] > tempMax)
			tempMax = arPixelV[i];
		arMark[i] = false;
	}

	for (i = 0; i < ImageHeight - 1; i++)
	{
		//计算差分
		arDifference[i] = arPixelV[i + 1] - arPixelV[i];

		//如果该行像素足够多且变化不大，标记为true
		if ((abs(arDifference[i]) < 20) && (arPixelV[i] > (0.75*tempMax)))
			arMark[i] = true;
	}

	//确定包含条码的行
	int iLengthThrehold = 40;
	int iCount;
	for (i = 0; i < ImageHeight - iLengthThrehold; i++)
	{
		iCount = 0;
		for (j = 0; j < iLengthThrehold; j++)
		{
			if (arMark[i + j] == true)
				iCount++;
		}
		if (iCount >= 37)
		{
			ImageTop = i + 10;		//确定顶部
			break;
		}
	}

	for (i = ImageHeight - 1; i >= iLengthThrehold - 1; i--)
	{
		iCount = 0;
		for (j = 0; j < iLengthThrehold; j++)
		{
			if (arMark[i - j] == true)
				iCount++;
		}
		if (iCount >= 37)	//iLengthThrehold-3
		{
			ImageBottom = i - 10;		//确定底部
			break;
		}
	}

	//寻找左边缘，在已经确定的上下边界内全局搜索
	for (i = ImageTop; i <= ImageBottom; i++)
	{
		for (j = 2; j < ImageWidth; j++)
		{
			if ((ImageArray[i][j - 1] == 0) && (ImageArray[i][j] == 1))
			{
				arLeftEdge[i] = j;
				break;
			}
		}
	}

	//为消除噪声的干扰，下面确定准确的左边界
	tempMax = 0;
	int iMax = 0;
	for (i = ImageTop; i <= ImageBottom; i++)
	{
		if (arLeftEdge[i] > tempMax)
		{
			tempMax = arLeftEdge[i];
			iMax = i;
		}
	}

	//倾斜度不能大于1/10
	iCount = 0;
	for (i = ImageTop; i <= ImageBottom; i++)
	{
		if (abs(tempMax - arLeftEdge[i]) < abs(i - iMax) / 6 + 1)
		{
			iCount++;
		}
	}

	if ((iCount / (ImageBottom - ImageTop)) < 0.6)
		return false;

	//调整起点
	for (i = iMax; i > ImageTop; i--)
	{
		if (abs(arLeftEdge[i] - arLeftEdge[i - 1]) >= 2)
		{
			if (ImageArray[i - 1][arLeftEdge[i]] - ImageArray[i - 1][arLeftEdge[i] - 1] == 1)
				arLeftEdge[i - 1] = arLeftEdge[i];
			else if (ImageArray[i - 1][arLeftEdge[i] - 1] - ImageArray[i - 1][arLeftEdge[i] - 2] == 1)
				arLeftEdge[i - 1] = arLeftEdge[i] - 1;
			else if (ImageArray[i - 1][arLeftEdge[i] + 1] - ImageArray[i - 1][arLeftEdge[i]] == 1)
				arLeftEdge[i - 1] = arLeftEdge[i] + 1;
			else
				arLeftEdge[i - 1] = arLeftEdge[i];
		}
	}

	for (i = iMax; i < ImageBottom; i++)
	{
		if (i == ImageBottom)
			break;

		if (abs(arLeftEdge[i] - arLeftEdge[i + 1]) >= 2)
		{
			if (ImageArray[i + 1][arLeftEdge[i]] - ImageArray[i + 1][arLeftEdge[i] - 1] == 1)
				arLeftEdge[i + 1] = arLeftEdge[i];
			else if (ImageArray[i + 1][arLeftEdge[i] - 1] - ImageArray[i + 1][arLeftEdge[i] - 2] == 1)
				arLeftEdge[i + 1] = arLeftEdge[i] - 1;
			else if (ImageArray[i + 1][arLeftEdge[i] + 1] - ImageArray[i + 1][arLeftEdge[i]] == 1)
				arLeftEdge[i + 1] = arLeftEdge[i] + 1;
			else
				arLeftEdge[i + 1] = arLeftEdge[i];
		}
	}

	int n;
	//搜索出所有的宽度
	for (n = 0; n < 29; n++)
	{
		//搜索条的右边缘
		for (i = ImageTop; i <= ImageBottom; i++)
		{
			for (j = arLeftEdge[i] + 1; j < ImageWidth; j++)
			{
				if ((ImageArray[i][j - 1] == 1) && (ImageArray[i][j] == 0))
				{
					arLeftEdge1[i] = j;
					break;
				}
			}
			arDelta[i] = arLeftEdge1[i] - arLeftEdge[i];
		}

		//假定条和空的宽度最多为11
		//排序，可以认为最中间的5个宽度是平均宽度
		for (i = ImageTop; i < ImageBottom; i++)
			tempArray[i] = arDelta[i];

		for (i = ImageTop; i < ImageBottom; i++)
		{
			for (j = ImageBottom; j > i; j--)
			{
				int tempSwap;
				if (tempArray[j] < tempArray[j - 1])
				{
					tempSwap = tempArray[j];
					tempArray[j] = tempArray[j - 1];
					tempArray[j - 1] = tempSwap;
				}
			}
		}

		if (tempArray[ImageTop + (ImageBottom - ImageTop) / 2 + 2] - tempArray[ImageTop + (ImageBottom - ImageTop) / 2 - 2] > 1)
			return false;
		else
			arWidth[2 * n] = tempArray[ImageTop + (ImageBottom - ImageTop) / 2];

		//调整下一列边缘
		for (i = ImageTop; i <= ImageBottom; i++)
		{
			if (abs(arDelta[i] - arWidth[2 * n]) > 2)
				arLeftEdge1[i] = arLeftEdge[i] + arWidth[2 * n];
			arLeftEdge[i] = arLeftEdge1[i];
		}

		//搜索空的右边缘
		for (i = ImageTop; i <= ImageBottom; i++)
		{
			for (j = arLeftEdge[i] + 1; j < ImageWidth; j++)
			{
				if ((ImageArray[i][j - 1] == 0) && (ImageArray[i][j] == 1))
				{
					arLeftEdge1[i] = j;
					break;
				}
			}
			arDelta[i] = arLeftEdge1[i] - arLeftEdge[i];
		}

		//假定条和空的宽度最多为11
		//排序，可以认为最中间的5个宽度是平均宽度
		for (i = ImageTop; i < ImageBottom; i++)
			tempArray[i] = arDelta[i];

		for (i = ImageTop; i < ImageBottom; i++)
		{
			for (j = ImageBottom; j > i; j--)
			{
				int tempSwap;
				if (tempArray[j] < tempArray[j - 1])
				{
					tempSwap = tempArray[j];
					tempArray[j] = tempArray[j - 1];
					tempArray[j - 1] = tempSwap;
				}
			}
		}

		if (tempArray[ImageTop + (ImageBottom - ImageTop) / 2 + 2] - tempArray[ImageTop + (ImageBottom - ImageTop) / 2 - 2] > 1)
			return false;
		else
			arWidth[2 * n + 1] = tempArray[ImageTop + (ImageBottom - ImageTop) / 2];

		//调整下一列边缘
		for (i = ImageTop; i <= ImageBottom; i++)
		{
			if (abs(arDelta[i] - arWidth[2 * n + 1]) > 2)
				arLeftEdge1[i] = arLeftEdge[i] + arWidth[2 * n + 1];
			arLeftEdge[i] = arLeftEdge1[i];
		}
	}

	//搜索最后一个条的右边缘
	for (i = ImageTop; i <= ImageBottom; i++)
	{
		for (j = arLeftEdge[i] + 1; j < ImageWidth; j++)
		{
			if ((ImageArray[i][j - 1] == 1) && (ImageArray[i][j] == 0))
			{
				arLeftEdge1[i] = j;
				break;
			}
		}
		arDelta[i] = arLeftEdge1[i] - arLeftEdge[i];
	}

	//假定条和空的宽度最多为11
	//排序，可以认为最中间的5个宽度是平均宽度
	for (i = ImageTop; i < ImageBottom; i++)
		tempArray[i] = arDelta[i];

	for (i = ImageTop; i < ImageBottom; i++)
	{
		for (j = ImageBottom; j > i; j--)
		{
			int tempSwap;
			if (tempArray[j] < tempArray[j - 1])
			{
				tempSwap = tempArray[j];
				tempArray[j] = tempArray[j - 1];
				tempArray[j - 1] = tempSwap;
			}
		}
	}

	if (tempArray[ImageTop + (ImageBottom - ImageTop) / 2 + 2] - tempArray[ImageTop + (ImageBottom - ImageTop) / 2 - 2] > 1)
		return false;
	else
		arWidth[2 * n] = tempArray[ImageTop + (ImageBottom - ImageTop) / 2];

	//调整下一列边缘
	for (i = ImageTop; i <= ImageBottom; i++)
	{
		if (abs(arDelta[i] - arWidth[2 * n + 1]) > 2)
			arLeftEdge1[i] = arLeftEdge[i] + tempArray[ImageTop + (ImageBottom - ImageTop) / 2];
		arLeftEdge[i] = arLeftEdge1[i];
	}
	//总共有7×12＋3×2＋5＝ 95个单位宽度
	//有4×12＋3×2＋5＝59个宽度，
	int result[12];
	double mx = 0.0;	//平均宽度

	for (i = 0; i < 59; i++)
		mx += (double)arWidth[i];
	mx /= 95.0;

	//起始条文
	for (i = 0; i < 3; i++)
	{
		double dTemp = (double)arWidth[i] / mx;
		if (dTemp<0.6 || dTemp>1.4)
			break;
	}
	//起始码不符合要求
	//if(i<3)
	//	return false;

	//识别前6个
	for (i = 0; i < 6; i++)
	{
		result[i] = JudgNum(arWidth[i * 4 + 3], arWidth[i * 4 + 4], arWidth[i * 4 + 5], arWidth[i * 4 + 6], mx);
	}
	//识别后6个
	for (i = 6; i < 12; i++)
	{
		result[i] = JudgNum(arWidth[i * 4 + 8], arWidth[i * 4 + 9], arWidth[i * 4 + 10], arWidth[i * 4 + 11], mx);
	}

	//判断码制
	if (result[0] == 7 && result[1] == 7)
	{
		strCodeStyle = "ISSN";
	}
	else if (result[0] == 7 && result[1] == 8)
	{
		strCodeStyle = "ISBN";
	}
	else
		strCodeStyle = "Unknown!";

	//判断是否全部识别出来
	for (i = 0; i < 12; i++)
		if (result[i] == -1)
			return false;
	for (i = 0; i < 12; i++)
		if (result[i] == -1)
			return false;
	CString strTemp;
	for (i = 0; i < 12; i++)
	{
		strTemp.Format(_T("%d"), result[i]);
		strCodeNumber += strTemp;
	}
	return true;
};

void CShowimageDlg::OnBnClickedButton1()
{
	Mat img = BarcodeScan(scrImg);

	CImage CI;
	ConvertMat2CImage(scrImg, CI);
	/*
	int height, width;
	CRect rect;//定义矩形类
	CRect rect1;
	height = CI.GetHeight();
	width = CI.GetWidth();
	*/

	float cx, cy, dx, dy, k, t;//跟控件的宽和高以及图片宽和高有关的参数
	CRect   rect;//用于获取图片控件的宽和高
	cx = CI.GetWidth();
	cy = CI.GetHeight();//获取图片的宽 高
	k = cy / cx;//获得图片的宽高比

	CWnd *pWnd = NULL;
	pWnd = GetDlgItem(IDC_PIC);//获取控件句柄
	pWnd->GetClientRect(&rect);//获取Picture Control控件的客户区
	dx = rect.Width();
	dy = rect.Height();//获得控件的宽高比
	t = dy / dx;//获得控件的宽高比
	if (k >= t)
	{

		rect.right = floor(rect.bottom / k);
		rect.left = (dx - rect.right) / 2;
		rect.right = floor(rect.bottom / k) + (dx - rect.right) / 2;
	}
	else
	{
		rect.bottom = floor(k*rect.right);
		rect.top = (dy - rect.bottom) / 2;

		rect.bottom = floor(k*rect.right) + (dy - rect.bottom) / 2;
	}
	//相关的计算为了让图片在绘图区居中按比例显示，如果图片很宽但是不高，就上下留有空白区；如果图片很高而不宽就左右留有空白区，并且保持两边空白区一样大

	CDC *pDc = NULL;
	pDc = pWnd->GetDC();//获取图片控件的DC
	int ModeOld = SetStretchBltMode(pDc->m_hDC, STRETCH_HALFTONE);//设置指定设备环境中的位图拉伸模式

	GetDlgItem(IDC_PIC)->ShowWindow(FALSE);
	GetDlgItem(IDC_PIC)->ShowWindow(TRUE);
	CI.StretchBlt(pDc->m_hDC, rect, SRCCOPY);//显示函数
	SetStretchBltMode(pDc->m_hDC, ModeOld);
	ReleaseDC(pDc);//释放指针空间

	UpdateData(TRUE);
	if(PreProcess(img))
	{
		type = strCodeStyle;
		barcode = strCodeNumber;
	}
	UpdateData(FALSE);
	// TODO: 在此添加控件通知处理程序代码
};

void CShowimageDlg::ChangeSize(UINT nID, int x, int y) //nID为控件ID，x,y分别为对话框的当前长和宽
{


	CWnd *pWnd;
	pWnd = GetDlgItem(nID);
	if (pWnd != NULL) //判断是否为空，因为在窗口创建的时候也会调用OnSize函数，但是此时各个控件还没有创建，Pwnd为空
	{


		CRect rec;
		pWnd->GetWindowRect(&rec); //获取控件变化前的大小
		ScreenToClient(&rec); //将控件大小装换位在对话框中的区域坐标
		rec.left = rec.left*x / m_rect.Width(); //按照比例调整空间的新位置
		rec.top = rec.top*y / m_rect.Height();
		rec.bottom = rec.bottom*y / m_rect.Height();
		rec.right = rec.right*x / m_rect.Width();
		pWnd->MoveWindow(rec); //伸缩控件


	}
};
void CShowimageDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	if (nType != SIZE_MINIMIZED) //判断窗口是不是最小化了，因为窗口最小化之后 ，窗口的长和宽会变成0，当前一次变化的时就会出现除以0的错误操作
	{


		ChangeSize(IDC_STATIC, cx, cy); //对每一个控件依次做调整
		ChangeSize(IDC_PIC, cx, cy);
		ChangeSize(IDC_EDIT1, cx, cy);
		ChangeSize(IDC_EDIT2, cx, cy);
		ChangeSize(IDCANCEL, cx, cy);
		ChangeSize(IDC_BUTTON1, cx, cy);
		ChangeSize(IDOK, cx, cy);
		GetClientRect(&m_rect); //最后要更新对话框的大小，当做下一次变化的旧坐标
	}
}
