
// MFCSampleDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MFCSample.h"
#include "MFCSampleDlg.h"
#include "afxdialogex.h"
#include <Win7MSIXInstallerActions.hpp>
#include <IMsixRequest.hpp>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


using namespace Win7MsixInstallerLib;

// CMFCSampleDlg dialog

CMFCSampleDlg::CMFCSampleDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCSAMPLE_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCSampleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMFCSampleDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BUTTON1, &CMFCSampleDlg::OnBnClickedButton1)
    ON_NOTIFY(NM_CUSTOMDRAW, IDC_PROGRESS1, &CMFCSampleDlg::OnNMCustomdrawProgress1)
END_MESSAGE_MAP()


// CMFCSampleDlg message handlers

BOOL CMFCSampleDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMFCSampleDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMFCSampleDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CMFCSampleDlg::UpdateProgressBar()
{
auto cpr = (CProgressCtrl*)GetDlgItem(IDC_PROGRESS1);
cpr->StepIt();
}

void CMFCSampleDlg::OnBnClickedButton1()
{

    IMsixRequest * msixRequest;
    auto res = Win7MsixInstaller_CreateAddPackageRequest(
        L"C:\\Users\\rudy\\repos\\Win7Msix\\Tests\\notepadplus.msix",
        MSIX_VALIDATION_OPTION::MSIX_VALIDATION_OPTION_FULL,
        &msixRequest);
    if (res == 0)
    {
        msixRequest->SetUI(this);
        msixRequest->ProcessRequest();
    }

}

void CMFCSampleDlg::OnNMCustomdrawProgress1(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
    // TODO: Add your control notification handler code here
    *pResult = 0;
}
