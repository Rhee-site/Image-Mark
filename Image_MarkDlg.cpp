
// Image_MarkDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "Image_Mark.h"
#include "Image_MarkDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
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


// CImageMarkDlg 대화 상자



CImageMarkDlg::CImageMarkDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_IMAGE_MARK_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CImageMarkDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_PIC1, m_Pic1);
	DDX_Control(pDX, IDC_EDIT1, m_Edit1);
	DDX_Control(pDX, IDC_LIST1, m_List1);
}

BEGIN_MESSAGE_MAP(CImageMarkDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CImageMarkDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CImageMarkDlg::OnBnClickedButton2)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, &CImageMarkDlg::OnNMClickList1)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONDOWN()
	ON_WM_DESTROY()
	ON_COMMAND_RANGE(BTN_ID_1, BTN_ID_10, OnButtonEvent)
END_MESSAGE_MAP()


// CImageMarkDlg 메시지 처리기

BOOL CImageMarkDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
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

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	m_bCheckUpdate = false;
	m_bCheckProcessing = false;
	m_nSrcFrameHeight = 540;
	m_nSrcFrameWidth = 940;
	m_nCurrentClassNumber = 0;

	m_VecstrImage.clear();
	m_VecstrImageName.clear();
	m_VecstrText.clear();
	m_VecstrTextName.clear();

	m_VecstrObjectClass.clear();

	m_VecwstrImageFiles.clear();
	m_VecwstrTextFiles.clear();

	m_VecImageInformation.clear();
	m_ImageInformation.nFrameHeight = 0;
	m_ImageInformation.nFrameIndex = 0;
	m_ImageInformation.nFrameWidth = 0;
	m_ImageInformation.strImageName.clear();
	m_ImageInformation.Object.clear();

	GetClassDefine();

	m_List1.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_List1.InsertColumn(0, _T(""), LVCFMT_CENTER, 0, -1);
	m_List1.InsertColumn(1, _T("Index"), LVCFMT_CENTER, 100, -1);
	m_List1.InsertColumn(2, _T("Image Name"), LVCFMT_CENTER, 200, -1);
	m_List1.InsertColumn(3, _T("Object Count"), LVCFMT_CENTER, 100, -1);

	m_pBtn = new CButton * [m_VecstrObjectClass.size()];
	CString caption = _T("");
	for (int index = 0; index < m_VecstrObjectClass.size(); index++) {
		m_pBtn[index] = new CButton();
		caption = m_VecstrObjectClass[index].c_str();
		int nTemp = index / 5;
		m_pBtn[index]->Create(caption, WS_CHILD | WS_VISIBLE | WS_GROUP | BS_AUTORADIOBUTTON,
			CRect(970 + 80 * index, 125 + 35 * nTemp, 80, 30), this, BTN_ID_1 + index);
		m_pBtn[index]->SetCheck(0);
	}
	m_pBtn[0]->SetCheck(1);

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CImageMarkDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CImageMarkDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		this->SetWindowPos(NULL, 100, 100, 1395, 590, SWP_NOREPOSITION);
		GetDlgItem(IDC_STATIC_PIC1)->SetWindowPos(NULL, 5, 5, 960, 540, SWP_NOREPOSITION);
		GetDlgItem(IDC_STATIC_TEXT1)->SetWindowPos(NULL, 970, 5, 100, 20, SWP_NOREPOSITION);
		GetDlgItem(IDC_EDIT1)->SetWindowPos(NULL, 970, 30, 300, 30, SWP_NOREPOSITION);
		GetDlgItem(IDC_BUTTON1)->SetWindowPos(NULL, 1270, 30, 100, 30, SWP_NOREPOSITION);
		GetDlgItem(IDC_BUTTON2)->SetWindowPos(NULL, 970, 65, 400, 30, SWP_NOREPOSITION);
		GetDlgItem(IDC_STATIC_TEXT2)->SetWindowPos(NULL, 970, 100, 100, 20, SWP_NOREPOSITION);
		GetDlgItem(IDC_LIST1)->SetWindowPos(NULL, 970, 250, 400, 295, SWP_NOREPOSITION);

		for (int index = 0; index < m_VecstrObjectClass.size(); index++) {
			int nTemp = index / 5;
			GetDlgItem(BTN_ID_1 + index)->SetWindowPos(NULL, 970 + 80 * (index - 5 * nTemp), 125 + 35 * nTemp, 80, 30, SWP_NOREPOSITION);
		}


		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CImageMarkDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CImageMarkDlg::OnBnClickedButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//Browse Button
	BROWSEINFO BrInfo;
	TCHAR szBuffer[512];

	::ZeroMemory(&BrInfo, sizeof(BROWSEINFO));
	::ZeroMemory(szBuffer, 512);

	BrInfo.hwndOwner = GetSafeHwnd();
	BrInfo.lpszTitle = _T("폴더를 선택하세요");
	BrInfo.ulFlags = BIF_NEWDIALOGSTYLE | BIF_EDITBOX | BIF_RETURNONLYFSDIRS;
	LPITEMIDLIST pItemIdList = ::SHBrowseForFolder(&BrInfo);
	::SHGetPathFromIDList(pItemIdList, szBuffer);

	CString str;
	str.Format(_T("%s"), szBuffer);
	SetDlgItemText(IDC_EDIT1, str);

	m_strCurrentPath = (string(CT2CA(str.operator LPCWSTR())));

	m_VecImageInformation.clear();

	m_VecwstrImageFiles = m_Util.GetFileListInFolder(szBuffer, L"*.jpg");
	if (m_VecwstrImageFiles.size()) {
		m_VecstrImage.clear();
		m_VecstrImageName.clear();

		for (int index = 0; index < m_VecwstrImageFiles.size(); index++) {
			CString cstrName(m_VecwstrImageFiles[index].c_str());
			string strName = (string(CT2CA(cstrName.operator LPCWSTR())));
			m_VecstrImage.push_back(strName);

			char chArrValue[BUFSIZE];
			strcpy(chArrValue, strName.c_str());
			char* tok = strtok(chArrValue, ".");
			m_VecstrImageName.push_back(string(tok));
		}

		m_VecwstrTextFiles = m_Util.GetFileListInFolder(szBuffer, L"*.txt");
		m_VecstrText.clear();
		m_VecstrTextName.clear();

		for (int index = 0; index < m_VecwstrTextFiles.size(); index++) {
			CString cstrName(m_VecwstrTextFiles[index].c_str());
			string strName = (string(CT2CA(cstrName.operator LPCWSTR())));
			m_VecstrText.push_back(strName);

			char chArrValue[BUFSIZE];
			strcpy(chArrValue, strName.c_str());
			char* tok = strtok(chArrValue, ".");
			m_VecstrTextName.push_back(string(tok));
		}
	}
	else {
		AfxMessageBox(_T(".Jpg File Not Exist"));
	}

	InitList();
}


void CImageMarkDlg::OnBnClickedButton2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// Save Button
	if (m_bCheckUpdate) {
		m_VecImageInformation.push_back(m_ImageInformation);
		m_bCheckUpdate = false;
	}

	for (int index = 0; index < m_VecImageInformation.size(); index++) {
		string strTextFilePath = m_strCurrentPath + "/" + m_VecImageInformation[index].strImageName + ".txt";

		ofstream out;
		out.open(strTextFilePath);

		for (int i = 0; i < m_VecImageInformation[index].Object.size(); i++) {

			Point2i n2Center;
			n2Center.x = (m_VecImageInformation[index].Object[i].n2Start.x + m_VecImageInformation[index].Object[i].n2End.x) / 2;
			n2Center.y = (m_VecImageInformation[index].Object[i].n2Start.y + m_VecImageInformation[index].Object[i].n2End.y) / 2;

			int nWidth = m_VecImageInformation[index].Object[i].n2End.x - m_VecImageInformation[index].Object[i].n2Start.x;
			int nHeight = m_VecImageInformation[index].Object[i].n2End.y - m_VecImageInformation[index].Object[i].n2Start.y;

			float fCenterXRatio = m_Util.ConvertPointToRatio(m_VecImageInformation[index].nFrameWidth, n2Center.x);
			float fCenterYRatio = m_Util.ConvertPointToRatio(m_VecImageInformation[index].nFrameHeight, n2Center.y);
			float fWidthRatio = m_Util.ConvertPointToRatio(m_VecImageInformation[index].nFrameWidth, nWidth);
			float fHeightRatio = m_Util.ConvertPointToRatio(m_VecImageInformation[index].nFrameHeight, nHeight);

			out << m_VecImageInformation[index].Object[i].nClassNumber << " " << fCenterXRatio << " " << fCenterYRatio << " " << fWidthRatio << " " << fHeightRatio << endl;

		}

		out.close();
	}
#if LOG
	Log.close();
#endif
	AfxMessageBox(_T("Save Finish"));
}


void CImageMarkDlg::OnNMClickList1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	
	if (pNMItemActivate->iItem != -1) {
		CString cstrName = m_List1.GetItemText(pNMItemActivate->iItem, 2);
		string strName = (string(CT2CA(cstrName.operator LPCWSTR())));
		string strPath = m_strCurrentPath + "/" + strName;
		m_CurrentFrame = imread(strPath);
		if (m_CurrentFrame.data != NULL) {
			CreateBitmapInfo(m_CurrentFrame.size().width, m_CurrentFrame.size().height, 24);
			DrawImage(m_CurrentFrame, m_Pic1);

			CString cstrIndex = m_List1.GetItemText(pNMItemActivate->iItem, 1);
			string strIndex = (string(CT2CA(cstrIndex.operator LPCWSTR())));
			m_nCurrentIndex = stoi(strIndex);

			m_nDstFrameWidth = m_CurrentFrame.cols;
			m_nDstFrameHeight = m_CurrentFrame.rows;

			m_fWidthRatio = (float)m_nDstFrameWidth / (float)m_nSrcFrameWidth;
			m_fHeightRatio = (float)m_nDstFrameHeight / (float)m_nSrcFrameHeight;

			if (m_ImageInformation.Object.size()) {
				m_VecImageInformation.push_back(m_ImageInformation);
			}

			bool bCheckExist = false;
			for (int index = 0; index < m_VecImageInformation.size(); index++) {
				if (m_VecImageInformation[index].nFrameIndex == m_nCurrentIndex) {
					bCheckExist = true;
					m_ImageInformation = m_VecImageInformation[index];
					m_VecImageInformation.erase(m_VecImageInformation.begin() + index);
					break;
				}
			}

			if (!bCheckExist) {
				m_ImageInformation.nFrameWidth = m_nDstFrameWidth;
				m_ImageInformation.nFrameHeight = m_nDstFrameHeight;
				m_ImageInformation.nFrameIndex = m_nCurrentIndex;
				m_ImageInformation.strImageName = m_VecstrImageName[m_nCurrentIndex];
				m_ImageInformation.Object.clear();
			}

			m_bCheckUpdate = false;
		}
	}

	*pResult = 0;
}


void CImageMarkDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	GetCursorPos(&point);
	GetDlgItem(IDC_STATIC_PIC1)->ScreenToClient(&point);

	if (point.x < 0 || point.y < 0 || point.x > 960 || point.y > 540)
	{

	}
	else
	{
		if (!m_bCheckProcessing) {
			if (!m_ImageInformation.Object.size()) {
				m_ImageInformation.nFrameWidth = m_nDstFrameWidth;
				m_ImageInformation.nFrameHeight = m_nDstFrameHeight;
				m_ImageInformation.nFrameIndex = m_nCurrentIndex;
				m_ImageInformation.strImageName = m_VecstrImageName[m_nCurrentIndex];
			}

			ObjectInfo Temp;
			Temp.nClassNumber = m_nCurrentClassNumber;
			m_ImageInformation.Object.push_back(Temp);

			m_bCheckProcessing = true;
			m_bCheckUpdate = true;
		}

		Point2i n2CurrentPoint;
		n2CurrentPoint.x = point.x * m_fWidthRatio;
		n2CurrentPoint.y = point.y * m_fHeightRatio;

		if (!m_Vecn2Point.size()) {
			m_Vecn2Point.push_back(n2CurrentPoint);
			m_ImageInformation.Object.back().n2Start = n2CurrentPoint;
		}
		else if (m_Vecn2Point.size() == 1) {
			m_Vecn2Point.push_back(n2CurrentPoint);
			m_ImageInformation.Object.back().n2End = n2CurrentPoint;
			m_Vecn2Point.clear();
			m_bCheckProcessing = false;

			CString cstrTemp;
			cstrTemp.Format(_T("%d"), m_ImageInformation.Object.size());
			m_List1.SetItemText(m_nCurrentIndex, 3, cstrTemp);
		}
	}

	CDialogEx::OnLButtonDown(nFlags, point);
}


void CImageMarkDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	Mat DrawFrame = m_CurrentFrame.clone();
	if (DrawFrame.data != NULL) {
		GetCursorPos(&point);
		GetDlgItem(IDC_STATIC_PIC1)->ScreenToClient(&point);

		if (point.x < 0 || point.y < 0 || point.x > 960 || point.y > 540) {

		}
		else {
			if (m_bCheckProcessing) {
				Point2i n2CurrentPoint;
				n2CurrentPoint.x = point.x * m_fWidthRatio;
				n2CurrentPoint.y = point.y * m_fHeightRatio;
				if (m_Vecn2Point.size() == 1) {
					rectangle(DrawFrame, m_Vecn2Point[0], n2CurrentPoint, Scalar(255, 0, 0), 5);
				}
			}
		}

		for (int index = 0; index < m_ImageInformation.Object.size(); index++) {
			rectangle(DrawFrame, m_ImageInformation.Object[index].n2Start, m_ImageInformation.Object[index].n2End, Scalar(255, 255, 255), 5);
			char chArrObjectName[BUFSIZE];
			sprintf_s(chArrObjectName, sizeof(chArrObjectName), "%d", m_ImageInformation.Object[index].nClassNumber);
			putText(DrawFrame, chArrObjectName, Point2i(m_ImageInformation.Object[index].n2Start.x + 20, m_ImageInformation.Object[index].n2Start.y + 50), cv::FONT_HERSHEY_SIMPLEX, 2, cv::Scalar(255, 255, 255), 2);
		}

		CreateBitmapInfo(DrawFrame.cols, DrawFrame.rows, 24);
		DrawImage(DrawFrame, m_Pic1);
	}

	CDialogEx::OnMouseMove(nFlags, point);
}


void CImageMarkDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	GetCursorPos(&point);
	GetDlgItem(IDC_STATIC_PIC1)->ScreenToClient(&point);

	if (point.x < 0 || point.y < 0 || point.x > 960 || point.y > 540) {

	}
	else {
		if (!m_bCheckProcessing) {
			for (int index = 0; index < m_ImageInformation.Object.size(); index++) {
				if (m_Util.CheckScope(Point2i(point.x * m_fWidthRatio, point.y * m_fHeightRatio), m_ImageInformation.Object[index].n2Start, m_ImageInformation.Object[index].n2End) % 2) {
					m_ImageInformation.Object.erase(m_ImageInformation.Object.begin() + index);

					CString cstrTemp;
					cstrTemp.Format(_T("%d"), m_ImageInformation.Object.size());
					m_List1.SetItemText(m_nCurrentIndex, 3, cstrTemp);

					break;
				}
			}
		}
	}

	CDialogEx::OnRButtonDown(nFlags, point);
}


void CImageMarkDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.

	if (m_pBtn != NULL)
	{
		for (int i = 0; i < m_VecstrObjectClass.size(); i++)
		{
			delete m_pBtn[i];
			m_pBtn[i] = NULL;
		}
		delete[] m_pBtn;
	}
}

void CImageMarkDlg::OnButtonEvent(UINT ID)
{
	int nButtonID = ID - BTN_ID_1;
	for (int index = 0; index < m_VecstrObjectClass.size(); index++) {
		if (nButtonID == index) {
			this->m_pBtn[index]->SetCheck(1);
			m_nCurrentClassNumber = nButtonID;
		}
		else {
			this->m_pBtn[index]->SetCheck(0);
		}
	}
}

void CImageMarkDlg::CreateBitmapInfo(int w, int h, int bpp)
{
	if (m_pBitmapInfo != NULL)
	{
		delete[]m_pBitmapInfo;
		m_pBitmapInfo = NULL;
	}

	if (bpp == 8)
		m_pBitmapInfo = (BITMAPINFO*) new BYTE[sizeof(BITMAPINFO) + 255 * sizeof(RGBQUAD)];
	else // 24 or 32bit
		m_pBitmapInfo = (BITMAPINFO*) new BYTE[sizeof(BITMAPINFO)];

	m_pBitmapInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	m_pBitmapInfo->bmiHeader.biPlanes = 1;
	m_pBitmapInfo->bmiHeader.biBitCount = bpp;
	m_pBitmapInfo->bmiHeader.biCompression = BI_RGB;
	m_pBitmapInfo->bmiHeader.biSizeImage = 0;
	m_pBitmapInfo->bmiHeader.biXPelsPerMeter = 0;
	m_pBitmapInfo->bmiHeader.biYPelsPerMeter = 0;
	m_pBitmapInfo->bmiHeader.biClrUsed = 0;
	m_pBitmapInfo->bmiHeader.biClrImportant = 0;

	if (bpp == 8)
	{
		for (int i = 0; i < 256; i++)
		{
			m_pBitmapInfo->bmiColors[i].rgbBlue = (BYTE)i;
			m_pBitmapInfo->bmiColors[i].rgbGreen = (BYTE)i;
			m_pBitmapInfo->bmiColors[i].rgbRed = (BYTE)i;
			m_pBitmapInfo->bmiColors[i].rgbReserved = 0;
		}
	}

	m_pBitmapInfo->bmiHeader.biWidth = w;
	m_pBitmapInfo->bmiHeader.biHeight = -h;
}

void CImageMarkDlg::DrawImage(Mat& frame, CStatic& m_pic)
{
	m_pDC = m_pic.GetDC();
	m_pic.GetClientRect(m_rect);
	SetStretchBltMode(*m_pDC, COLORONCOLOR);
	StretchDIBits(*m_pDC, 0, 0, m_rect.Width(), m_rect.Height(), 0, 0, frame.cols, frame.rows, frame.data, m_pBitmapInfo, DIB_RGB_COLORS, SRCCOPY);
	::ReleaseDC(m_pic.m_hWnd, *m_pDC);
}

void CImageMarkDlg::GetClassDefine()
{
	ifstream in;
	in.open("ini/obj.txt");
	if (in.is_open()) {
		while (!in.eof()) {
			char arr[BUFSIZE];
			in.getline(arr, BUFSIZE);
			m_VecstrObjectClass.push_back(arr);
		}
	}
	in.close();
}

void CImageMarkDlg::InitList()
{
	int nListCount = m_List1.GetItemCount();
	for (int index = 0; index < nListCount; index++) {
		m_List1.DeleteItem(0);
	}

	InitImageListInfo();
	InitTextListInfo();
}

void CImageMarkDlg::InitImageListInfo()
{
	int nImageListCount = m_VecstrImage.size();
	for (int index = 0; index < nImageListCount; index++) {
		int nNum = m_List1.GetItemCount();
		CString cstrTemp;
		m_List1.InsertItem(nNum, cstrTemp);
		cstrTemp.Format(_T("%d"), nNum);
		m_List1.SetItemText(nNum, 1, cstrTemp);
		cstrTemp = m_VecstrImage[index].c_str();
		m_List1.SetItemText(nNum, 2, cstrTemp);
	}
}

void CImageMarkDlg::InitTextListInfo()
{
	int nTextListCount = m_VecstrText.size();
	for (int index = 0; index < nTextListCount; index++) {
		ifstream in;
		string strTextFilePath = m_strCurrentPath + "/" + m_VecstrText[index];
		in.open(strTextFilePath);

		char chArrValue[BUFSIZE];
		char* context;
		int nObjectCount = 0;

		vector<vector<string>> VecTemp;

		while (in.getline(chArrValue, BUFSIZE)) {
			vector<string> VecstrTemp;
			char* tok = strtok_s(chArrValue, " ", &context);
			while (tok != NULL) {
				VecstrTemp.push_back(tok);
				tok = strtok_s(NULL, " ", &context);
			}
			VecTemp.push_back(VecstrTemp);
			nObjectCount++;
		}

		in.close();

		int nTargetIndex = m_Util.CheckSameNameIndex(m_VecstrImageName, m_VecstrTextName[index]);
		if (nTargetIndex > -1) {
			CString cstrTemp;
			cstrTemp.Format(_T("%d"), nObjectCount);
			m_List1.SetItemText(nTargetIndex, 3, cstrTemp);

			string strImageFilePath = m_strCurrentPath + "/" + m_VecstrImage[nTargetIndex];
			Mat TempFrame = imread(strImageFilePath);
			int nFrameWidth = TempFrame.cols;
			int nFrameHeight = TempFrame.rows;

			ImageInfo TempImageInfomation;
			TempImageInfomation.nFrameWidth = nFrameWidth;
			TempImageInfomation.nFrameHeight = nFrameHeight;
			TempImageInfomation.nFrameIndex = nTargetIndex;
			TempImageInfomation.strImageName = m_VecstrImageName[nTargetIndex];

			for (int i = 0; i < VecTemp.size(); i++) {
				ObjectInfo TempObjectInfomation;

				TempObjectInfomation.nClassNumber = stoi(VecTemp[i][0]);

				Point2i n2Center;
				n2Center.x = m_Util.ConvertRatioToPoint(nFrameWidth, stof(VecTemp[i][1]));
				n2Center.y = m_Util.ConvertRatioToPoint(nFrameHeight, stof(VecTemp[i][2]));
				int nTempWidth = m_Util.ConvertRatioToPoint(nFrameWidth, stof(VecTemp[i][3]));
				int nTempHeight = m_Util.ConvertRatioToPoint(nFrameHeight, stof(VecTemp[i][4]));

				TempObjectInfomation.n2Start = Point2i(n2Center.x - (nTempWidth / 2), n2Center.y - (nTempHeight / 2));
				TempObjectInfomation.n2End = Point2i(n2Center.x + (nTempWidth / 2), n2Center.y + (nTempHeight / 2));

				TempImageInfomation.Object.push_back(TempObjectInfomation);
			}

			m_VecImageInformation.push_back(TempImageInfomation);
		}
	}
}
