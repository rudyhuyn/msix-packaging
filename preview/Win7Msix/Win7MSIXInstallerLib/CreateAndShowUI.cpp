// UI Functions

#include "CreateAndShowUI.hpp"
#include <windows.h>
#include <string>
#include <commctrl.h>
#include <sys/types.h>
#include <thread>

#include <algorithm>
#include <sstream>
#include <iostream>

// MSIXWindows.hpp define NOMINMAX because we want to use std::min/std::max from <algorithm>
// GdiPlus.h requires a definiton for min and max. Use std namespace *BEFORE* including it.
using namespace std;
using namespace Win7MsixInstallerLib;

const PCWSTR CreateAndShowUI::HandlerName = L"UI";

HRESULT CreateAndShowUI::ExecuteForAddRequest()
{
	auto ui = m_msixRequest->GetUI();
	if (ui != NULL)
	{
		RETURN_IF_FAILED(ui->ShowUI());
	}
    return S_OK;
}

HRESULT CreateAndShowUI::CreateHandler(MsixRequestImpl * msixRequest, IPackageHandler ** instance)
{
    std::unique_ptr<CreateAndShowUI> localInstance(new CreateAndShowUI(msixRequest));
    if (localInstance == nullptr)
    {
        return E_OUTOFMEMORY;
    }
    *instance = localInstance.release();

    return S_OK;
}
