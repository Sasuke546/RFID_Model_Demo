// RFID_MODEL2_20120328.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CRFID_MODEL2_20120328App:
// �йش����ʵ�֣������ RFID_MODEL2_20120328.cpp
//

class CRFID_MODEL2_20120328App : public CWinApp
{
public:
	CRFID_MODEL2_20120328App();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CRFID_MODEL2_20120328App theApp;