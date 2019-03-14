#pragma once
// Install UI Header
// UI Functions
#include <windows.h>
#include <string>
#include <IMsixRequest.hpp>
#include <IInstallerUI.hpp>
#include "GeneralUtil.hpp"

// Child window identifiers
#define IDC_LAUNCHCHECKBOX 1
#define IDC_INSTALLBUTTON 2
#define IDC_CANCELBUTTON 3
#define IDC_LAUNCHBUTTON 4
#define WM_INSTALLCOMPLETE_MSG (WM_APP+1)

// Global variables
static HWND hWnd = NULL; // parent window hwnd
static HWND g_buttonHWnd = NULL;
static HWND g_checkboxHWnd = NULL;
static HWND g_progressHWnd = NULL;
static HWND g_CancelbuttonHWnd = NULL;
static HWND g_LaunchbuttonHWnd = NULL;
static bool g_installed = false;
static bool g_launchCheckBoxState = true; // launch checkbox is checked by default

class UI : public Win7MsixInstallerLib::IInstallerUI
{
public:
    UI(_In_ Win7MsixInstallerLib::IMsixRequest* msixRequest) : m_msixRequest(msixRequest) { m_buttonClickedEvent = CreateEvent(NULL, FALSE, FALSE, NULL); }
    bool ShowUI(Win7MsixInstallerLib::InstallerUIType isAddPackage);
    HRESULT LaunchInstalledApp();
    ~UI() {}
private:
    Win7MsixInstallerLib::IMsixRequest* m_msixRequest = nullptr;

    HANDLE m_buttonClickedEvent;
    std::wstring m_displayName = L"";
    std::wstring m_publisherCommonName = L"";
    ComPtr<IStream> m_logoStream;
    std::wstring m_version = L"";
    HRESULT m_loadingPackageInfoCode = 0;

public:
    HRESULT DrawPackageInfo(HWND hWnd, RECT windowRect);
    int CreateInitWindow(HINSTANCE hInstance, int nCmdShow, const std::wstring& windowClass, const std::wstring& title);
    void LoadInfo();
    void SetButtonClicked() { SetEvent(m_buttonClickedEvent); }
    void UpdateProgressBarStep(float value);
    bool InstallCompleted();

    // FUNCTION: CreateProgressBar(HWND parentHWnd, RECT parentRect)
    //
    // PURPOSE: Creates the progress bar
    //
    // parentHWnd: the HWND of the window to add the progress bar to
    // parentRect: the dimmensions of the parent window
    // count: the number of objects to be iterated through in the progress bar
    BOOL CreateProgressBar(HWND parentHWnd, RECT parentRect);

    // FUNCTION: LaunchButton(HWND parentHWnd, RECT parentRect)
    //
    // PURPOSE: Create the lower right install button
    // 
    // parentHWnd: the HWND of the window to add the button to
    // parentRect: the specs of the parent window
    BOOL LaunchButton(HWND parentHWnd, RECT parentRect);

    // FUNCTION: CreateCheckbox(HWND parentHWnd, RECT parentRect)
    //
    // PURPOSE: Create the launch when ready checkbox on the bottom left
    // 
    // parentHWnd: the HWND of the window to add the checkbox to
    // parentRect: the specs of the parent window
    BOOL CreateCheckbox(HWND parentHWnd, RECT parentRect);

    // FUNCTION: CreateCancelButton(HWND parentHWnd, RECT parentRect)
    //
    // PURPOSE: Create the cancel button on the bottom right corner when user clicks on install
    // 
    // parentHWnd: the HWND of the window to add the checkbox to
    // parentRect: the specs of the parent window
    BOOL CreateCancelButton(HWND parentHWnd, RECT parentRect);

    // FUNCTION: CreateLaunchButton(HWND parentHWnd, RECT parentRect)
    //
    // PURPOSE: Create the launch button on the botton right after app has been installed
    // 
    // parentHWnd: the HWND of the window to add the checkbox to
    // parentRect: the specs of the parent window
    BOOL CreateLaunchButton(HWND parentHWnd, RECT parentRect);

    // FUNCTION: ChangeButtonText(LPARAM newMessage)
    //
    // PURPOSE: Changes the text of the lower right button
    //
    // newMessage: the message to change the button to
    BOOL ChangeButtonText(const std::wstring& newMessage);

    // FUNCTION: HideButtonWindow()
    //
    // PURPOSE: Hides the lower right button
    //
    BOOL HideButtonWindow();

    // FUNCTION: SendInstallCompleteMsg
    //
    // PURPOSE: Sends the WM_INSTALLCOMPLETE_MSG message to the main window when app installation is complete
    void SendInstallCompleteMsg();
    // FUNCTION: ChangeText(HWND parentHWnd, std::wstring& windowText)
    //
    // PURPOSE: Change the text of the installation window based on the given input
    //
    // parentHWnd: the HWND of the window to be changed
    // windowText: the text to change the window to
    BOOL ChangeText(HWND parentHWnd, std::wstring displayText, std::wstring  messageText, IStream* logoStream = nullptr);

private:
    HRESULT ParseInfoFromPackage();
};
