// RFID_MODEL2_20120328Dlg.h : 头文件
//
#ifndef RFID_MODEL2_20120328DLG_H
#define RFID_MODEL2_20120328DLG_H

#pragma once
#include"RFID_Position.h"

// CRFID_MODEL2_20120328Dlg 对话框
class CRFID_MODEL2_20120328Dlg : public CDialog
{
// 构造
public:
	CRFID_MODEL2_20120328Dlg(CWnd* pParent = NULL);	// 标准构造函数

	~CRFID_MODEL2_20120328Dlg();

// 对话框数据
	enum { IDD = IDD_RFID_MODEL2_20120328_DIALOG };

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
	// 根据货物ID查询货物位置
	bool query(const CString & goodsID, CString & ans, int & region, int & position);
public:
	afx_msg void OnBnClickedButtonQuery();
protected:
	CString m_input1;
	CString m_input2;
public:
	CString m_output;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	// 当前查询的货物所在区域
	int curRegion;
	// 当前查询的货物在所在区域中的位置编号
	int curPosition;

	RFID_Position *rfid;
	Debug deg;
public:
	afx_msg void OnBnClickedRead();
	afx_msg void OnBnClickedClear();
};


#endif