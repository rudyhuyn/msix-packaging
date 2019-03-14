    // UI Functions

#include "InstallUI.hpp"
#include <windows.h>
#include <string>
#include <commctrl.h>
#include <sys/types.h>
#include <thread>

#include <algorithm>
#include <sstream>
#include <iostream>
#include "resource.h"
#include "GeneralUtil.hpp"
#include "Win7MSIXInstallerLogger.hpp"
// MSIXWindows.hpp define NOMINMAX because we want to use std::min/std::max from <algorithm>
// GdiPlus.h requires a definiton for min and max. Use std namespace *BEFORE* including it.
using namespace std;
using namespace Win7MsixInstallerLib;
#include <GdiPlus.h>

static const int g_width = 500;  // width of window
static const int g_heigth = 400; // height of window

//
// PURPOSE: This compiles the information displayed on the UI when the user selects an msix
//
// hWnd: the HWND of the window to draw controls
// windowRect: the size of the window

HRESULT UI::DrawPackageInfo(HWND hWnd, RECT windowRect)
{
    if (SUCCEEDED(m_loadingPackageInfoCode))
    {
        auto displayText = L"Install " + m_displayName + L"?";
        auto messageText = L"Publisher: " + m_publisherCommonName + L"\nVersion: " + m_version;
        ChangeText(hWnd, displayText, messageText, m_logoStream.Get());
        ChangeText(hWnd, GetStringResource(IDS_STRING_UI_INSTALL_COMPLETE), GetStringResource(IDS_STRING_UI_COMPLETION_MESSAGE));
    }
    else
    {
        std::wstringstream wstringstream;
        wstringstream << L"Failed getting package information with: 0x" << std::hex << m_loadingPackageInfoCode;
        auto g_messageText = wstringstream.str();
        ChangeText(hWnd, L"Loading Package failed", g_messageText);
    }
    return S_OK;
}

//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    UI* ui = (UI*)GetWindowLongPtr(hWnd, GWLP_USERDATA);

    RECT windowRect;
    GetClientRect(hWnd, &windowRect);
    switch (message)
    {
    case WM_CREATE:
        ui->LaunchButton(hWnd, windowRect);
        ui->CreateCheckbox(hWnd, windowRect);
        break;
    case WM_PAINT:
    {
        if (ui != NULL)
        {
            ui->DrawPackageInfo(hWnd, windowRect);
        }
        break;
    }
    case WM_COMMAND:
        switch (LOWORD(wParam)) 
        {
            case IDC_INSTALLBUTTON:
            {                
                if (!g_installed)
                {
                    DestroyWindow(g_buttonHWnd);
                    ui->CreateCancelButton(hWnd, windowRect);
                    UpdateWindow(hWnd);
                    if (ui != NULL)
                    {
                        ui->CreateProgressBar(hWnd, windowRect);
                    }
                    ShowWindow(g_progressHWnd, SW_SHOW); //Show progress bar only when install is clicked
                    if (ui != NULL)
                    {
                        ui->SetButtonClicked();
                    }
                }
                else
                {
                    PostQuitMessage(0);
                    exit(0);
                }
            }
            break;
            case IDC_LAUNCHCHECKBOX:
            {
                if (SendMessage(GetDlgItem(hWnd, IDC_LAUNCHCHECKBOX), BM_GETCHECK, 0, 0) == BST_CHECKED) 
                {
                    g_launchCheckBoxState = true;
                }
                else
                {
                    g_launchCheckBoxState = false;
                }
            }
            break;
            case IDC_LAUNCHBUTTON:
                ui->LaunchInstalledApp();
                break;
        }
        break;
    case WM_INSTALLCOMPLETE_MSG:
    {
        DestroyWindow(g_CancelbuttonHWnd);
        ui->CreateLaunchButton(hWnd, windowRect);
        UpdateWindow(hWnd);
        ShowWindow(g_progressHWnd, SW_HIDE); //hide progress bar
        ShowWindow(g_checkboxHWnd, SW_HIDE); //hide launch check box
        if (g_launchCheckBoxState) {
            ui->LaunchInstalledApp(); // launch app
            DestroyWindow(hWnd); // close msix app installer
        }
        else
        {
            //wait for user to click launch button or close the window
            while (true)
            {
                switch (MsgWaitForMultipleObjects(0, NULL, FALSE, INFINITE, QS_ALLINPUT))
                {
                case WAIT_OBJECT_0:
                    MSG msg;
                    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
                    {
                        TranslateMessage(&msg);
                        DispatchMessage(&msg);
                    }
                    break;
                }
            }
        }
        break;
    }
    case WM_SIZE:
    case WM_SIZING:
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        exit(0);
        break;
	case WM_CTLCOLORSTATIC:
	{
		switch (::GetDlgCtrlID((HWND)lParam))
		{
			case IDC_LAUNCHCHECKBOX:
			{
				HBRUSH hbr = (HBRUSH)DefWindowProc(hWnd, message, wParam, lParam);
				::DeleteObject(hbr);
				SetBkMode((HDC)wParam, TRANSPARENT);
				return (LRESULT)::GetStockObject(NULL_BRUSH);
			}
		}

		break;
	}
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
        break;
    }

    return 0;
}

HRESULT UI::LaunchInstalledApp()
{
    auto packageInfo = m_msixRequest->GetIPackageInfo();
    std::wstring resolvedExecutableFullPath = packageInfo->GetExecutableFilePath();
    //check for error while launching app here                     Win
    ShellExecute(NULL, NULL, resolvedExecutableFullPath.c_str(), NULL, NULL, SW_SHOW);
    return S_OK;
}

void StartParseFile(HWND hWnd)
{
    //auto result = ParseAndRun(hWnd);
    int result = 0;

    if (result != 0)
    {
        std::cout << "Error: " << std::hex << result << " while extracting the appx package" << std::endl;
        Text<char> text;
        auto logResult = GetLogTextUTF8(MyAllocate, &text);
        if (0 == logResult)
        {
            std::cout << "LOG:" << std::endl << text.content << std::endl;
        }
        else
        {
            std::cout << "UNABLE TO GET LOG WITH HR=" << std::hex << logResult << std::endl;
        }
    }
}

void CommandFunc(HWND hWnd, RECT windowRect) {
    std::thread t1(StartParseFile, hWnd);
    t1.detach();
    return;
}

void StartUIThread(UI* ui)
{
    // Free the console that we started with
    FreeConsole();

    // Register WindowClass and create the window
    HINSTANCE hInstance = GetModuleHandle(NULL);

    std::wstring windowClass = GetStringResource(IDS_STRING_UI_TITLE);
    std::wstring title = GetStringResource(IDS_STRING_UI_TITLE);

    WNDCLASSEX wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICONBIG));
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = windowClass.c_str();
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ICONSMALL));

    if (!RegisterClassEx(&wcex))
    {
        MessageBox(NULL, L"Call to RegisterClassEx failed!", title.c_str(), NULL);
        return;
    }

    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
    ui->CreateInitWindow(hInstance, SW_SHOWNORMAL, windowClass, title);
    Gdiplus::GdiplusShutdown(gdiplusToken);

}

void UI::LoadInfo()
{
    m_loadingPackageInfoCode = ParseInfoFromPackage();
}

HRESULT UI::ParseInfoFromPackage() 
{
    auto packageInfo = m_msixRequest->GetIPackageInfo();
    // Obtain publisher name
    m_publisherCommonName = packageInfo->GetPublisherName();

    // Obtain version number
    ConvertVersionToString(packageInfo->GetVersion());

    //Obtain the number of files
    m_displayName = packageInfo->GetDisplayName();
    m_logoStream = packageInfo->GetLogo();
    return S_OK;
}

bool UI::ShowUI(Win7MsixInstallerLib::InstallerUIType isAddPackage)
{
    LoadInfo();
    std::thread thread(StartUIThread, this);
    thread.detach();

    DWORD waitResult = WaitForSingleObject(m_buttonClickedEvent, INFINITE);
    
    return true;
}

// FUNCTION: UpdateProgressBar
//
// PURPOSE: Modify the value of the progress bar
void UI::UpdateProgressBarStep(float value)
{
	SendMessage(g_progressHWnd, PBM_SETPOS, value * 100, 0);
}

// FUNCTION: CreateProgressBar(HWND parentHWnd, RECT parentRect)
//
// PURPOSE: Creates the progress bar
//
// parentHWnd: the HWND of the window to add the progress bar to
// parentRect: the dimensions of the parent window
BOOL UI::CreateProgressBar(HWND parentHWnd, RECT parentRect)
{
    int scrollHeight = GetSystemMetrics(SM_CYVSCROLL);

    // Creates progress bar on window
    g_progressHWnd = CreateWindowEx(
        0,
        PROGRESS_CLASS,
        (LPTSTR)NULL,
        WS_CHILD,
        parentRect.left + 50, // x coord
        parentRect.bottom - scrollHeight - 125, // y coord
        parentRect.right - 100, // width
        scrollHeight, // height
        parentHWnd, // parent
        (HMENU)0,
        NULL,
        NULL
    );

    // Set defaults for range and increments
    SendMessage(g_progressHWnd, PBM_SETRANGE, 0, MAKELPARAM(0, 100)); // set range
    return TRUE;
}

// FUNCTION: LaunchButton(HWND parentHWnd, RECT parentRect)
//
// PURPOSE: Create the lower right button
// 
// parentHWnd: the HWND of the window to add the button to
// parentRect: the specs of the parent window
BOOL UI::LaunchButton(HWND parentHWnd, RECT parentRect) {
    LPVOID buttonPointer = nullptr;
    g_buttonHWnd = CreateWindowEx(
        WS_EX_LEFT, // extended window style
        L"BUTTON",
        L"Install",  // text
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON | BS_FLAT, // style
        parentRect.right - 100 - 50, // x coord
        parentRect.bottom - 60,  // y coord
        120,  // width
        35,  // height
        parentHWnd,  // parent
        (HMENU)IDC_INSTALLBUTTON, // menu
        reinterpret_cast<HINSTANCE>(GetWindowLongPtr(parentHWnd, GWLP_HINSTANCE)),
        buttonPointer); // pointer to button
    return TRUE;
}

// FUNCTION: CreateCheckbox(HWND parentHWnd, RECT parentRect)
//
// PURPOSE: Create the launch checkbox on the bottom left
// 
// parentHWnd: the HWND of the window to add the checkbox to
// parentRect: the specs of the parent window
BOOL UI::CreateCheckbox(HWND parentHWnd, RECT parentRect)
{
    g_checkboxHWnd = CreateWindowEx(
        WS_EX_LEFT, // extended window style
        L"BUTTON",
        L"Launch when ready",  // text
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, // style
        parentRect.left + 50, // x coord
        parentRect.bottom - 60,  // y coord
        165,  // width
        35,  // height
        parentHWnd,  // parent
        (HMENU)IDC_LAUNCHCHECKBOX, // menu
        reinterpret_cast<HINSTANCE>(GetWindowLongPtr(parentHWnd, GWLP_HINSTANCE)),
        NULL);

    //Set default checkbox state to checked
    SendMessage(g_checkboxHWnd, BM_SETCHECK, BST_CHECKED, 0);
    return TRUE;
}

// FUNCTION: CancelButton(HWND parentHWnd, RECT parentRect)
//
// PURPOSE: Create the lower right cancel button when install is clicked
// 
// parentHWnd: the HWND of the window to add the button to
// parentRect: the specs of the parent window
BOOL CreateCancelButton(HWND parentHWnd, RECT parentRect) {
    LPVOID buttonPointer = nullptr;
    g_CancelbuttonHWnd = CreateWindowEx(
        WS_EX_LEFT, // extended window style
        L"BUTTON",
        L"Cancel",  // text
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON | BS_FLAT, // style
        parentRect.right - 100 - 50, // x coord
        parentRect.bottom - 60,  // y coord
        120,  // width
        35,  // height
        parentHWnd,  // parent
        (HMENU)IDC_CANCELBUTTON, // menu
        reinterpret_cast<HINSTANCE>(GetWindowLongPtr(parentHWnd, GWLP_HINSTANCE)),
        buttonPointer); // pointer to button
    return TRUE;
}

// FUNCTION: CancelButton(HWND parentHWnd, RECT parentRect)
//
// PURPOSE: Create the lower right cancel button when install is clicked
// 
// parentHWnd: the HWND of the window to add the button to
// parentRect: the specs of the parent window
BOOL UI::CreateCancelButton(HWND parentHWnd, RECT parentRect)
{
    LPVOID buttonPointer = nullptr;
    g_CancelbuttonHWnd = CreateWindowEx(
        WS_EX_LEFT, // extended window style
        L"BUTTON",
        L"Cancel",  // text
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON | BS_FLAT, // style
        parentRect.right - 100 - 50, // x coord
        parentRect.bottom - 60,  // y coord
        120,  // width
        35,  // height
        parentHWnd,  // parent
        (HMENU)IDC_CANCELBUTTON, // menu
        reinterpret_cast<HINSTANCE>(GetWindowLongPtr(parentHWnd, GWLP_HINSTANCE)),
        buttonPointer); // pointer to button
    return TRUE;
}

// FUNCTION: CreateLaunchButton(HWND parentHWnd, RECT parentRect)
//
// PURPOSE: Create the launch button on the botton right after app has been installed
// 
// parentHWnd: the HWND of the window to add the checkbox to
// parentRect: the specs of the parent window
BOOL UI::CreateLaunchButton(HWND parentHWnd, RECT parentRect) 
{
    LPVOID buttonPointer = nullptr;
    g_LaunchbuttonHWnd = CreateWindowEx(
        WS_EX_LEFT, // extended window style
        L"BUTTON",
        L"Launch",  // text
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON | BS_FLAT, // style
        parentRect.right - 100 - 50, // x coord
        parentRect.bottom - 60,  // y coord
        120,  // width
        35,  // height
        parentHWnd,  // parent
        (HMENU)IDC_LAUNCHBUTTON, // menu
        reinterpret_cast<HINSTANCE>(GetWindowLongPtr(parentHWnd, GWLP_HINSTANCE)),
        buttonPointer); // pointer to button
    return TRUE;
}

// FUNCTION: ChangeButtonText(LPARAM newMessage)
//
// PURPOSE: Changes the text of the lower right button
//
// newMessage: the message to change the button to
BOOL UI::ChangeButtonText(const std::wstring& newMessage)
{
    SendMessage(g_buttonHWnd, WM_SETTEXT, NULL, reinterpret_cast<LPARAM>(newMessage.c_str()));
    return ShowWindow(g_buttonHWnd, SW_SHOW);
}

BOOL UI::HideButtonWindow()
{
    return ShowWindow(g_buttonHWnd, SW_HIDE);
}

// FUNCTION: ChangeText(HWND parentHWnd, std::wstring& windowText)
//
// PURPOSE: Change the text of the installation window based on the given input
//
// parentHWnd: the HWND of the window to be changed
// windowText: the text to change the window to
BOOL UI::ChangeText(HWND parentHWnd, std::wstring displayName, std::wstring messageText, IStream* logoStream)
{
    PAINTSTRUCT paint;
    HDC deviceContext = BeginPaint(parentHWnd, &paint);

    Gdiplus::Graphics graphics(deviceContext);

    Gdiplus::RectF layoutRect(50, 50, g_width - 144, 100);
    Gdiplus::Font displayNameFont(L"Arial", 16);
    Gdiplus::Font messageFont(L"Arial", 10);
    Gdiplus::StringFormat format;
    format.SetAlignment(Gdiplus::StringAlignmentNear);
	auto windowsTextColor = Gdiplus::Color();
	windowsTextColor.SetFromCOLORREF(GetSysColor(COLOR_WINDOWTEXT));
	Gdiplus::SolidBrush textBrush(windowsTextColor);

    graphics.DrawString(displayName.c_str(), -1, &displayNameFont, layoutRect, &format, &textBrush);
    layoutRect.Y += 40;
    graphics.DrawString(messageText.c_str(), -1, &messageFont, layoutRect, &format, &textBrush);

    if (logoStream != nullptr)
    {
        // We shouldn't fail if the image can't be loaded, just don't show it.
        auto image = Gdiplus::Image::FromStream(logoStream, FALSE);
        if (image != nullptr)
        {
            Gdiplus::Status status = graphics.DrawImage(image, g_width - 200, 25);
            delete image;
        }
    }

    EndPaint(parentHWnd, &paint);

    return TRUE;
}

// FUNCTION: CreateInitWindow(HINSTANCE hInstance, int nCmdShow, TCHAR windowClass[], TCHAR windowTitle[])
//
// PURPOSE: Creates the initial installation UI window
// windowClass: the class text of the window
// windowTitle: the window title
int UI::CreateInitWindow(HINSTANCE hInstance, int nCmdShow, const std::wstring& windowClass, const std::wstring& title)
{
    hWnd = CreateWindow(
        const_cast<wchar_t*>(windowClass.c_str()),
        const_cast<wchar_t*>(title.c_str()),
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU,
        CW_USEDEFAULT, CW_USEDEFAULT,
        g_width,  // width of window
        g_heigth, // height of window
        NULL, // A handle to the parent or owner window of the window being created
        NULL, // a handle to a menu, or specifies a child-window identifier depending on the window style
        hInstance, // a handle to the instance o the module to be associated with the window
        NULL // Pointer to the window through the CREATESTRUCT structure
    );

    if (!hWnd)
    {
        MessageBox(NULL, L"Call to CreateWindow failed!", title.c_str(), NULL);
        return 1;
    }

    SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)this); 
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    // Main message loop:
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return static_cast<int>(msg.wParam);
}

bool UI::InstallCompleted()
{
    SendMessage(hWnd, WM_INSTALLCOMPLETE_MSG, NULL, NULL);
    return true;
}