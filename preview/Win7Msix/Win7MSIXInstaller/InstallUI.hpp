#pragma once
// Install UI Header
// UI Functions
#include <windows.h>
#include <string>
#include <IPackageManager.hpp>
#include "GeneralUtil.hpp"
// Child window identifiers
#define IDC_LAUNCHCHECKBOX 1
#define IDC_INSTALLBUTTON 2
#define IDC_CANCELBUTTON 3
#define IDC_LAUNCHBUTTON 4

// Global variables
static HWND hWnd = NULL; // parent window hwnd
static HWND g_buttonHWnd = NULL;
static HWND g_checkboxHWnd = NULL;
static HWND g_progressHWnd = NULL;
static HWND g_CancelbuttonHWnd = NULL;
static HWND g_LaunchbuttonHWnd = NULL;
static bool g_installed = false;
static bool g_launchCheckBoxState = true; // launch checkbox is checked by default

enum UIType { InstallUIAdd, InstallUIRemove};

class UI
{
public:
    UI(_In_ Win7MsixInstallerLib::IPackageManager* packageManager, _In_ const std::wstring & path, UIType type) : m_packageManager(packageManager), m_type(type)
    {
        m_path = std::wstring(path);
        m_closeUI = CreateEvent(NULL, FALSE, FALSE, NULL);
    }
    HRESULT LaunchInstalledApp();
    ~UI() {}
    bool Show();

private:
    Win7MsixInstallerLib::IPackageManager* m_packageManager = nullptr;
    Win7MsixInstallerLib::IPackageInfo* m_packageInfo = nullptr;
    std::wstring m_path;
    
    HANDLE m_closeUI;

    //Cached information used to draw the dialog
    std::wstring m_displayName = L"";
    std::wstring m_publisherCommonName = L"";
    ComPtr<IStream> m_logoStream;
    std::wstring m_version = L"";

    HRESULT m_loadingPackageInfoCode = 0;
    UIType m_type;
public:
    HRESULT DrawPackageInfo(HWND hWnd, RECT windowRect);
    int CreateInitWindow(HINSTANCE hInstance, int nCmdShow, const std::wstring& windowClass, const std::wstring& title);
    void ButtonClicked();

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

    // FUNCTION: ChangeText(HWND parentHWnd, std::wstring& windowText)
    //
    // PURPOSE: Change the text of the installation window based on the given input
    //
    // parentHWnd: the HWND of the window to be changed
    // windowText: the text to change the window to
    BOOL ChangeText(HWND parentHWnd, std::wstring displayText, std::wstring  messageText, IStream* logoStream = nullptr);

private:
    HRESULT ParseInfoFromPackage();
    void ShowCompletedUI();
};
