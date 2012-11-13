// RFID_MODEL2_20120328Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "RFID_MODEL2_20120328.h"
#include "RFID_MODEL2_20120328Dlg.h"

vector<Good> gr;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CRFID_MODEL2_20120328Dlg 对话框




CRFID_MODEL2_20120328Dlg::CRFID_MODEL2_20120328Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRFID_MODEL2_20120328Dlg::IDD, pParent)
	, m_input1(_T(""))
	, m_input2(_T(""))
	, m_output(_T(""))
	, curRegion(0)
	, curPosition(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CRFID_MODEL2_20120328Dlg::DoDataExchange(CDataExchange* pDX)    //数据交换
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_INPUT1, m_input1);
	DDX_Text(pDX, IDC_EDIT_INPUT2, m_input2);
	DDX_Text(pDX, IDC_EDIT_OUTPUT, m_output);
}

BEGIN_MESSAGE_MAP(CRFID_MODEL2_20120328Dlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_QUERY, &CRFID_MODEL2_20120328Dlg::OnBnClickedButtonQuery)
	ON_BN_CLICKED(IDC_READ, &CRFID_MODEL2_20120328Dlg::OnBnClickedRead)
	ON_BN_CLICKED(IDC_CLEAR, &CRFID_MODEL2_20120328Dlg::OnBnClickedClear)
END_MESSAGE_MAP()


// CRFID_MODEL2_20120328Dlg 消息处理程序

BOOL CRFID_MODEL2_20120328Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	curRegion = -1;
	curPosition = -1;

	rfid = new RFID_Position();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CRFID_MODEL2_20120328Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CRFID_MODEL2_20120328Dlg::OnPaint()    //绘制函数
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
		CDialog::OnPaint();

		CClientDC dc(this);

		const int cells=4, blocks=3;//cells为一个区中有几沓板，blocks为每沓板有几层即几块板
		const int regionsRows=3, regionsCols=2;
		const int regions=regionsRows * regionsCols;
		const int housesRows=2, housesCols=2;
		const int houses=housesRows * housesCols;

		const int offx=13, offy=130;
		const int regionXspaces = 90,
			      regionYspaces = 5;
		const int inHouseXoffset = 8,
		      	  inHouseYoffset = 8;

		const int blockXLength = 20, blockYLength = 20;
		const int cellXLength = blockXLength,
			      cellYLength = blockYLength*blocks;
		const int regionXLength = cellXLength*cells, 
			      regionYLength = cellYLength;
		const int houseXLength = regionXLength*regionsCols 
			                   + regionXspaces*(regionsCols-1) 
							   + inHouseXoffset*2, 
			      houseYLength = regionYLength*regionsRows 
				               + regionYspaces*(regionsRows-1) 
							   + inHouseYoffset*2;

		const COLORREF backColor[2] = { RGB(192,192,192), RGB(128,128,128) };
		const COLORREF curGoodsColor = RGB(255,0,0);
		const COLORREF emptyGoodsColor = RGB(255,255,255);
		const COLORREF cellsColor[cells] = { RGB(0,255,0), RGB(0,128,192), RGB(0,255,255),RGB(128,0,255) };


		//画背景
		CRect rect( offx, offy, offx+houseXLength, offy+houseYLength );
		for( int r=0; r<housesRows; r++ ){
			for( int c=0; c<housesCols; c++ ){
				rect.MoveToXY( offx+c*houseXLength, offy+r*houseYLength );
				CBrush brush( backColor[ (r+c)%2 ] );
				dc.FillRect( &rect, &brush );
			}
		}
		/*
int x = offx + inHouseXoffset
				         + (house%housesCols)*houseXLength 
				         + (region%regionsCols)*( regionXLength+regionXspaces)
					     + cell*cellXLength;
			int y = offy + inHouseYoffset
				         + (house/housesCols)*houseYLength 
				         + (region/regionsCols)*( regionYLength+regionYspaces )
						 + (blocks-1-block)*blockYLength;
						 */
		//画所有可放物体的地方，用空白表示
		for( int house=0; house<houses; house++ )
		for( int region=0; region<regions; region++ )
		for( int cell=0; cell<cells; cell++ )
		for( int block=0; block<blocks; block++ ){
			int x = offx + inHouseXoffset
						 + (house%housesCols)*houseXLength 
						 + (region/regionsRows)*( regionXLength+regionXspaces)
						 + cell*cellXLength;
			int y = offy + inHouseYoffset
						 + (house/housesCols)*houseYLength 
						 + (region%regionsRows)*( regionYLength+regionYspaces )
						 + (blocks-1-block)*blockYLength;
			//CRect rect( x, y, x+blockXLength-2, y+blockYLength );
			//CBrush brush( emptyGoodsColor );
			//dc.FillRect( &rect, &brush );
			dc.Draw3dRect( x, y, blockXLength-2, blockYLength,emptyGoodsColor,emptyGoodsColor );
		}

		//画所有货物
		for(size_t i=0;i<gr.size();i++){
			int region_num = gr[i].getRegionID()-1, position=gr[i].getPos()-1;
//		for( int i=0; i<100; i++ ){
//			int region_num = i/12, position=i%12;
			int house  = region_num / regions;
			int region = region_num % regions;
			int cell   = position / blocks;
			int color = cellsColor[cell];
			if( (region/regionsRows)%2==1 ){
				cell = cells-1-cell;
			}
			
			int block  = position % blocks;
			int x = offx + inHouseXoffset
						 + (house%housesCols)*houseXLength 
						 + (region/regionsRows)*( regionXLength+regionXspaces)
						 + cell*cellXLength;
			int y = offy + inHouseYoffset
						 + (house/housesCols)*houseYLength 
						 + (region%regionsRows)*( regionYLength+regionYspaces )
						 + (blocks-1-block)*blockYLength;
			CRect rect( x+1, y+1, x+blockXLength-3, y+blockYLength-1 );
			//CBrush brush( cellsColor[cell] );
			CBrush brush( color );
			dc.FillRect( &rect, &brush );
		}

		//画当前查询的货物
		if( curRegion>=1 && curPosition>=1 ){
			int region_num = curRegion-1, position=curPosition-1;
			int house  = region_num / regions;
			int region = region_num % regions;
			int cell   = position / blocks;
			int block  = position % blocks;
			if( (region/regionsRows)%2==1 ){
				cell = cells-1-cell;
			}
			int x = offx + inHouseXoffset
						 + (house%housesCols)*houseXLength 
						 + (region/regionsRows)*( regionXLength+regionXspaces)
						 + cell*cellXLength;
			int y = offy + inHouseYoffset
						 + (house/housesCols)*houseYLength 
						 + (region%regionsRows)*( regionYLength+regionYspaces )
						 + (blocks-1-block)*blockYLength;
			CRect rect( x+1, y+1, x+blockXLength-3, y+blockYLength-1 );
			CBrush brush( curGoodsColor );
			dc.FillRect( &rect, &brush );
		}
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CRFID_MODEL2_20120328Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


// 根据货物ID查询货物位置
bool CRFID_MODEL2_20120328Dlg::query(const CString & goodsID, CString & ans, int & region, int & position)
{
	//数据处理
	std::string in = CStringToString(goodsID);
	int reg_num=0,pos=0;
	int res=rfid->searchGood(in);
	if( res==0 ){
		region = rfid->getCurRegion();
		position = rfid->getCurPos();;
		ans.Format( L"region: %d ;  position: %d", region, position );
		return true;
	}else if( res==-1 ){
		ans=L"There is not such a goods!";
	}

	return false;
}

void CRFID_MODEL2_20120328Dlg::OnBnClickedButtonQuery()
{
	UpdateData(TRUE);
	m_output = L"";
	curRegion = -1;
	curPosition = -1;
	if( !m_input1.IsEmpty() ){
		this->query( m_input1, m_output,curRegion,curPosition );
	}else{
		m_output = L"GoodsID can not be empty!";
		gr.clear();
		rfid->showAll(gr);
		OnPaint();
	}
	UpdateData(FALSE);
	OnPaint();
}

BOOL CRFID_MODEL2_20120328Dlg::PreTranslateMessage(MSG* pMsg)
{
	if( pMsg->message == WM_KEYDOWN && pMsg->wParam==VK_RETURN ){
		return TRUE;
	}
	return CDialog::PreTranslateMessage(pMsg);
}

CRFID_MODEL2_20120328Dlg::~CRFID_MODEL2_20120328Dlg()
{
	delete rfid;
}
void CRFID_MODEL2_20120328Dlg::OnBnClickedRead()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	curRegion = -1;
	curPosition = -1;

	if( m_input1.IsEmpty() )
	{
		m_output = L"GoodsID can not be empty!";
		rfid->showForklift(m_output);
		UpdateData(FALSE);
		return;
	}

	if(m_input2.IsEmpty())
	{
		rfid->addGood(CStringToString(m_input1));
		rfid->showForklift(m_output);
		UpdateData(FALSE);
		return;
	}

	if(rfid->getForkliftSize()>0)
	{
		// put down goods
		if(rfid->putdownGoods(CStringToString(m_input2)) ==-1)
			m_output = L"The region is full! Can not put down goods";
		else
		{
			gr.clear();
			rfid->showAll(gr);
			OnPaint();
		}
		rfid->showForklift(m_output);
	}
	else
	{
		// lift up goods

		if(rfid->liftupGoods(CStringToString(m_input1),CStringToString(m_input2))==-1)
			m_output = L"Can not lift up goods";
		else
		{
			gr.clear();
			rfid->showAll(gr);
			
			OnPaint();
		}
		rfid->showForklift(m_output);
	}

	UpdateData(FALSE);

}

void CRFID_MODEL2_20120328Dlg::OnBnClickedClear()
{
	UpdateData(TRUE);
	int out = MessageBox(L"Are you sure to clear the forklift?",L"Query",MB_OK);
	
	if(out==1) //点了确定
		rfid->clearFork();

	m_output = L"Clear the forklift";
	UpdateData(FALSE);
}
