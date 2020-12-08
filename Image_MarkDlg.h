
// Image_MarkDlg.h: 헤더 파일
//

#pragma once

#include "Util.h"

#define MAX_BTN 10
#define BTN_ID_1 10001
#define BTN_ID_2 10002
#define BTN_ID_3 10003
#define BTN_ID_4 10004
#define BTN_ID_5 10005
#define BTN_ID_6 10006
#define BTN_ID_7 10007
#define BTN_ID_8 10008
#define BTN_ID_9 10009
#define BTN_ID_10 10010

// CImageMarkDlg 대화 상자
class CImageMarkDlg : public CDialogEx
{
// 생성입니다.
public:
	CImageMarkDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_IMAGE_MARK_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CStatic m_Pic1;
	CEdit m_Edit1;
	CListCtrl m_List1;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnNMClickList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
	afx_msg void OnButtonEvent(UINT ID);

public:
	void CreateBitmapInfo(int w, int h, int bpp);
	void DrawImage(Mat& frame, CStatic& m_pic);

	void GetClassDefine();
	void InitList();
	void InitImageListInfo();
	void InitTextListInfo();

public:
	Util m_Util;

	string m_strCurrentPath;

	vector<wstring> m_VecwstrImageFiles;
	vector<wstring> m_VecwstrTextFiles;

	vector<string> m_VecstrObjectClass;
	vector<string> m_VecstrImageName;
	vector<string> m_VecstrImage;
	vector<string> m_VecstrTextName;
	vector<string> m_VecstrText;

	bool m_bCheckUpdate;
	bool m_bCheckProcessing;
	int m_nCurrentIndex;
	int m_nCurrentClassNumber;
	Mat m_CurrentFrame;
	int m_nSrcFrameWidth;
	int m_nSrcFrameHeight;
	int m_nDstFrameWidth;
	int m_nDstFrameHeight;
	float m_fWidthRatio;
	float m_fHeightRatio;

	vector<ImageInfo> m_VecImageInformation;
	ImageInfo m_ImageInformation;
	vector<Point2i> m_Vecn2Point;

	CDC* m_pDC;
	CRect m_rect;
	BITMAPINFO* m_pBitmapInfo;

	CButton** m_pBtn;
};
