﻿
// ShowimageDlg.h: 头文件
//

#pragma once

// CShowimageDlg 对话框
class CShowimageDlg : public CDialogEx
{
// 构造
public:
	CRect rect1;
	CRect m_rect;

	CShowimageDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SHOWIMAGE_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg
		void OnBnClickedOk();
	afx_msg void OnBnClickedButton1();
	void ChangeSize(UINT nID, int x, int y);
	CString barcode;
	CString type;
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
