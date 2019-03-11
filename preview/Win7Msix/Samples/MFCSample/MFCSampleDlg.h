
// MFCSampleDlg.h : header file
//

#pragma once
#include <IInstallerUI.hpp>

// CMFCSampleDlg dialog
class CMFCSampleDlg : public CDialogEx, Win7MsixInstallerLib::IInstallerUI
{
    // Construction
    public:
        CMFCSampleDlg(CWnd* pParent = nullptr);	// standard constructor

    // Dialog Data
    #ifdef AFX_DESIGN_TIME
        enum { IDD = IDD_MFCSAMPLE_DIALOG };
    #endif

        protected:
        virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

        virtual void UpdateProgressBar();
        virtual HRESULT ShowUI() { return 0; }
    // Implementation
    protected:
        HICON m_hIcon;

        // Generated message map functions
        virtual BOOL OnInitDialog();
        afx_msg void OnPaint();
        afx_msg HCURSOR OnQueryDragIcon();
        DECLARE_MESSAGE_MAP()
    public:
        afx_msg void OnBnClickedButton1();
        afx_msg void OnNMCustomdrawProgress1(NMHDR *pNMHDR, LRESULT *pResult);
};
