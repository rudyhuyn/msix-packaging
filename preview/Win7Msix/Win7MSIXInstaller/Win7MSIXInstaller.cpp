// Win7MSIXInstaller.cpp :
// The main entry point for Win7MSIXInstaller.exe. This application is
// a working preview for the MSIX/APPX installer for Windows 7
#include "MSIXWindows.hpp"
#include <shlobj_core.h>
#include <CommCtrl.h>

#include <string>
#include <map>
#include <algorithm>
#include <sstream>
#include <iostream>
#include <utility>
#include <iomanip>

#include <cstdio>
#include <TraceLoggingProvider.h>
#include "InstallUI.hpp"
#include "CommandLineInterface.hpp"
#include "Win7MSIXInstallerLogger.hpp"
#include "Util.hpp"
#include "resource.h"
using namespace Win7MsixInstaller;

int main(int argc, char * argv[])
{
    // Register the provider
    TraceLoggingRegister(g_MsixUITraceLoggingProvider);

    HRESULT hrCoInitialize = CoInitializeEx(NULL, COINIT_MULTITHREADED);
    if (FAILED(hrCoInitialize))
    {
        std::wcout << GetStringResource(IDS_STRING_FAILED_COM_INITIALIZATION) << " " << std::hex << hrCoInitialize << std::endl;
        return 1;
    }

    CommandLineInterface cli(argc, argv);

    AutoPtr<IMsixRequest> msixRequest;
    const HRESULT hrCreateRequest = cli.CreateRequest(&msixRequest);
    if (SUCCEEDED(hrCreateRequest))
    {
		if(!cli.IsQuietMode())
			msixRequest->SetUI(new UI(msixRequest));
        const HRESULT hrProcessRequest = msixRequest->ProcessRequest();
        if (FAILED(hrProcessRequest))
        {

            std::wcout << GetStringResource(IDS_STRING_FAILED_REQUEST) << " " << std::hex << hrProcessRequest << std::endl;
            TraceLoggingWrite(g_MsixUITraceLoggingProvider,
                "Failed to process request",
                TraceLoggingLevel(WINEVENT_LEVEL_ERROR),
                TraceLoggingValue(hrProcessRequest, "HR"));
            return 1;
        }
    }
    else
    {
        cli.DisplayHelp();
    }

    
    // Stop TraceLogging and unregister the provider
    TraceLoggingUnregister(g_MsixUITraceLoggingProvider);

    return 0;
}

