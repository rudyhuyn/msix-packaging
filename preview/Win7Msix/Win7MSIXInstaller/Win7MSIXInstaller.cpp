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
#include <MsixRequestBuilder.hpp>
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

    const HRESULT hrCreateRequest = cli.Init();
    if (SUCCEEDED(hrCreateRequest))
    {
		AutoPtr<IMsixRequest> msixRequest;
		switch (cli.GetOperationType())
		{
			case CommandLineOperationType::Add:
			{
				RETURN_IF_FAILED(MsixRequestBuilder::CreateAddPackageRequest(
					cli.GetPackageFilePath(),
					cli.GetValidationOption(),
					&msixRequest)
				);
				break;
			}
			case CommandLineOperationType::Remove:
			{
				RETURN_IF_FAILED(MsixRequestBuilder::CreateRemovePackageRequest(
					cli.GetPackageFullName(),
					&msixRequest)
				);
				break;
			}
			case CommandLineOperationType::FindPackage:
			{
				RETURN_IF_FAILED(MsixRequestBuilder::CreateFindPackageRequest(
					cli.GetPackageFullName(),
					&msixRequest)
				);
				break;
			}
			case CommandLineOperationType::FindAllPackages:
			{
				RETURN_IF_FAILED(MsixRequestBuilder::CreateFindAllPackagesRequest(
					&msixRequest)
				);
				break;
			}
			default:
				return E_NOT_SET;
		}
	
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

