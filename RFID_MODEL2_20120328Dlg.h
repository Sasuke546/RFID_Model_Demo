// RFID_MODEL2_20120328Dlg.h : ͷ�ļ�
//
#ifndef RFID_MODEL2_20120328DLG_H
#define RFID_MODEL2_20120328DLG_H

#pragma once
#include"RFID_Position.h"

// CRFID_MODEL2_20120328Dlg �Ի���
class CRFID_MODEL2_20120328Dlg : public CDialog
{
// ����
public:
	CRFID_MODEL2_20120328Dlg(CWnd* pParent = NULL);	// ��׼���캯��

	~CRFID_MODEL2_20120328Dlg();

// �Ի�������
	enum { IDD = IDD_RFID_MODEL2_20120328_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	// ���ݻ���ID��ѯ����λ��
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
	// ��ǰ��ѯ�Ļ�����������
	int curRegion;
	// ��ǰ��ѯ�Ļ��������������е�λ�ñ��
	int curPosition;

	RFID_Position *rfid;
	Debug deg;
public:
	afx_msg void OnBnClickedRead();
	afx_msg void OnBnClickedClear();
};


#endif