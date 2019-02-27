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

// MSIXWindows.hpp define NOMINMAX because we want to use std::min/std::max from <algorithm>
// GdiPlus.h requires a definiton for min and max. Use std namespace *BEFORE* including it.
using namespace std;
#include <GdiPlus.h>

// Global variables
static std::wstring g_messageText = L"";
static std::wstring g_displayText = L"";


Gdiplus::Image* g_image = nullptr;

static const int g_width = 500;  // width of window
static const int g_heigth = 400; // height of window

//
// Gets the stream of a file.
//
// Parameters:
//   package - The package reader for the app package.
//   name - Name of the file.
//   stream - The stream for the file.
//
HRESULT GetStreamFromFile(IAppxPackageReader* package, LPCWCHAR name, IStream** stream)
{
    *stream = nullptr;

    ComPtr<IAppxFilesEnumerator> files;
    RETURN_IF_FAILED(package->GetPayloadFiles(&files));

    BOOL hasCurrent = FALSE;
    RETURN_IF_FAILED(files->GetHasCurrent(&hasCurrent));
    while (hasCurrent)
    {
        ComPtr<IAppxFile> file;
        RETURN_IF_FAILED(files->GetCurrent(&file));
        Text<WCHAR> fileName;
        file->GetName(&fileName);
        if (wcscmp(fileName.Get(), name) == 0)
        {
            RETURN_IF_FAILED(file->GetStream(stream));
            return S_OK;
        }
        RETURN_IF_FAILED(files->MoveNext(&hasCurrent));
    }
    return S_OK;
}

//
// PURPOSE: This compiles the information displayed on the UI when the user selects an msix
//
// windowText: pointer to a wstring that the window message will be saved to
HRESULT UI::DisplayPackageInfo(HWND hWnd, RECT windowRect, std::wstring& displayText, std::wstring& messageText)
{
    PackageInfo* packageInfo = m_msixRequest->GetPackageInfo();
    CreateProgressBar(hWnd, windowRect, packageInfo->GetNumberOfPayloadFiles());

    ComPtr<IMsixDocumentElement> domElement;
    RETURN_IF_FAILED(packageInfo->GetManifestReader()->QueryInterface(UuidOfImpl<IMsixDocumentElement>::iid, reinterpret_cast<void**>(&domElement)));

    ComPtr<IMsixElement> element;
    RETURN_IF_FAILED(domElement->GetDocumentElement(&element));

    // Obtain the Display Name and Logo
    ComPtr<IMsixElementEnumerator> veElementEnum;
    RETURN_IF_FAILED(element->GetElements(
        L"/*[local-name()='Package']/*[local-name()='Applications']/*[local-name()='Application']/*[local-name()='VisualElements']",
        &veElementEnum));

    ComPtr<IStream> logoStream;
    Text<WCHAR> displayName;
    Text<WCHAR> logo;
    std::wstring tmpLogoFile;

    BOOL hc = FALSE;
    RETURN_IF_FAILED(veElementEnum->GetHasCurrent(&hc));
    if (hc)
    {
        ComPtr<IMsixElement> visualElementsElement;
        RETURN_IF_FAILED(veElementEnum->GetCurrent(&visualElementsElement));
        RETURN_IF_FAILED(visualElementsElement->GetAttributeValue(L"DisplayName", &displayName));
        RETURN_IF_FAILED(visualElementsElement->GetAttributeValue(L"Square150x150Logo", &logo));
        RETURN_IF_FAILED(GetStreamFromFile(packageInfo->GetPackageReader(), logo.Get(), &logoStream));
    }

    // Show only the CommonName of the publisher
    auto wpublisher = std::wstring(packageInfo->GetPublisher());
    auto publisherCommonName = wpublisher.substr(wpublisher.find_first_of(L"=") + 1,
        wpublisher.find_first_of(L",") - wpublisher.find_first_of(L"=") - 1);

    displayText = L"Install " + std::wstring(displayName.Get()) + L"?";

    messageText = L"Publisher: " + publisherCommonName + L"\nVersion: " + ConvertVersionToString(packageInfo->GetVersion());
    ChangeText(hWnd, displayText, messageText, logoStream.Get());

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
        LaunchButton(hWnd, windowRect);
        break;
    case WM_PAINT:
    {
        if (!g_displayInfo)
        {
            HRESULT hr = ui->DisplayPackageInfo(hWnd, windowRect, g_displayText, g_messageText);
            if (FAILED(hr))
            {
                std::wstring failure = L"Loading Package failed";
                std::wstringstream wstringstream;
                wstringstream << L"Failed getting package information with: 0x" << std::hex << hr;
                g_messageText = wstringstream.str();
                ChangeText(hWnd, failure, g_messageText);
            }
            g_displayInfo = true;
        }
        if (g_displayCompleteText)
        {
            ChangeText(hWnd, GetStringResource(IDS_STRING_UI_INSTALL_COMPLETE), GetStringResource(IDS_STRING_UI_COMPLETION_MESSAGE));
            g_displayCompleteText = false;
        }

        break;
    }
    case WM_COMMAND:
        if (!g_installed)
        {
            ui->SetButtonClicked();
            return HideButtonWindow();
        }
        else
        {
            PostQuitMessage(0);
            exit(0);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        exit(0);
        break;
    case WM_SIZE:
    case WM_SIZING:
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
        break;
    }

    return 0;
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
    wcex.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = windowClass.c_str();
    wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

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

HRESULT UI::ShowUI()
{
    std::thread thread(StartUIThread, this);
    thread.detach();

    DWORD waitResult = WaitForSingleObject(m_buttonClickedEvent, INFINITE);
    
    return S_OK;
}

// FUNCTION: UpdateProgressBar
//
// PURPOSE: Increment the progress bar one tick based on preset tick
void UI::UpdateProgressBar()
{
	SendMessage(g_progressHWnd, PBM_STEPIT, 0, 0);
}

// FUNCTION: CreateProgressBar(HWND parentHWnd, RECT parentRect, int count)
//
// PURPOSE: Creates the progress bar
//
// parentHWnd: the HWND of the window to add the progress bar to
// parentRect: the dimmensions of the parent window
// count: the number of objects to be iterated through in the progress bar
BOOL CreateProgressBar(HWND parentHWnd, RECT parentRect, int count)
{
    int scrollHeight = GetSystemMetrics(SM_CYVSCROLL);

    // Creates progress bar on window
    g_progressHWnd = CreateWindowEx(
        0,
        PROGRESS_CLASS,
        (LPTSTR)NULL,
        WS_CHILD | WS_VISIBLE,
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
    SendMessage(g_progressHWnd, PBM_SETRANGE, 0, MAKELPARAM(0, count)); // set range
    SendMessage(g_progressHWnd, PBM_SETSTEP, (WPARAM)1, 0); // set increment
    return TRUE;
}

// FUNCTION: LaunchButton(HWND parentHWnd, RECT parentRect)
//
// PURPOSE: Create the lower right button
// 
// parentHWnd: the HWND of the window to add the button to
// parentRect: the specs of the parent window
BOOL LaunchButton(HWND parentHWnd, RECT parentRect) {
    LPVOID buttonPointer = nullptr;
    g_buttonHWnd = CreateWindowEx(
        WS_EX_LEFT, // extended window style
        L"BUTTON",
        L"Install",  // text
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON | BS_FLAT, // style
        parentRect.right - 100 - 50, // x coord
        parentRect.bottom - 50 - 50,  // y coord
        100,  // width
        50,  // height
        parentHWnd,  // parent
        NULL, // menu
        reinterpret_cast<HINSTANCE>(GetWindowLongPtr(parentHWnd, GWLP_HINSTANCE)),
        buttonPointer); // pointer to button
    return TRUE;
}

// FUNCTION: ChangeButtonText(LPARAM newMessage)
//
// PURPOSE: Changes the text of the lower right button
//
// newMessage: the message to change the button to
BOOL ChangeButtonText(const std::wstring& newMessage)
{
    SendMessage(g_buttonHWnd, WM_SETTEXT, NULL, reinterpret_cast<LPARAM>(newMessage.c_str()));
    return ShowWindow(g_buttonHWnd, SW_SHOW);
}

BOOL HideButtonWindow()
{
    return ShowWindow(g_buttonHWnd, SW_HIDE);
}

// FUNCTION: ChangeText(HWND parentHWnd, std::wstring& windowText)
//
// PURPOSE: Change the text of the installation window based on the given input
//
// parentHWnd: the HWND of the window to be changed
// windowText: the text to change the window to
BOOL ChangeText(HWND parentHWnd, std::wstring displayName, std::wstring messageText, IStream* logoStream)
{
    PAINTSTRUCT paint;
    HDC deviceContext = BeginPaint(parentHWnd, &paint);

    Gdiplus::Graphics graphics(deviceContext);

    Gdiplus::RectF layoutRect(50, 50, g_width - 144, 100);
    Gdiplus::Font displayNameFont(L"Arial", 16);
    Gdiplus::Font messageFont(L"Arial", 10);
    Gdiplus::StringFormat format;
    format.SetAlignment(Gdiplus::StringAlignmentNear);
    Gdiplus::SolidBrush blackBrush(Gdiplus::Color(255, 0, 0, 0));

    graphics.DrawString(displayName.c_str(), -1, &displayNameFont, layoutRect, &format, &blackBrush);
    layoutRect.Y += 40;
    graphics.DrawString(messageText.c_str(), -1, &messageFont, layoutRect, &format, &blackBrush);

    if (logoStream != nullptr)
    {
        // We shouldn't fail if the image can't be loaded, just don't show it.
        g_image = Gdiplus::Image::FromStream(logoStream, FALSE);
            
    }
        
    if (g_image != nullptr)
    {
        Gdiplus::Status status = graphics.DrawImage(g_image, g_width - 200, 25);
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
    HWND hWnd = CreateWindow(
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

