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
#include <Win7MSIXInstallerActions.hpp>
#include <experimental/filesystem>
#include "GeneralUtil.hpp"
#include "resource.h"

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
        AutoPtr<IMsixRequest> msixRequest = NULL;
        switch (cli.GetOperationType())
        {
        case CommandLineOperationType::Add:
        {
            RETURN_IF_FAILED(Win7MsixInstaller_CreateAddPackageRequest(
                cli.GetPackageFilePath(),
                cli.GetValidationOption(),
                &msixRequest)
            );
            break;
        }
        case CommandLineOperationType::Remove:
        {
            RETURN_IF_FAILED(Win7MsixInstaller_CreateRemovePackageRequest(
                cli.GetPackageFullName(),
                &msixRequest)
            );
            break;
        }
        case CommandLineOperationType::FindPackage:
        {
            AutoPtr<IPackageInfo> packageInfo;
            RETURN_IF_FAILED(Win7MsixInstaller_GetPackageInfo(
                cli.GetPackageFullName(),
                &packageInfo)
            );
            if (packageInfo == NULL)
            {
                std::wcout << std::endl;
                std::wcout << L"No packages found" << std::endl;
                std::wcout << std::endl;
            }
            else {
                std::wcout << std::endl;
                std::wcout << L"PackageFullName: " << packageInfo->GetPackageFullName().c_str() << std::endl;
                std::wcout << L"DisplayName: " << packageInfo->GetDisplayName().c_str() << std::endl;
                std::wcout << L"DirectoryPath: " << packageInfo->GetPackageDirectoryPath().c_str() << std::endl;
                std::wcout << std::endl;
            }
            return S_OK;
        }
        case CommandLineOperationType::FindAllPackages:
        {
            {
                auto folderPath = Win7MsixInstaller_GetInstallationDirectoryPath();
                if (folderPath == NULL)
                {
                    return E_FAIL;
                }

                int numPackages = 0;
                for (auto& p : std::experimental::filesystem::directory_iterator(folderPath))
                {
                    std::cout << p.path().filename() << std::endl;
                    numPackages++;
                }

                std::cout << numPackages << " Packages found" << std::endl;

                return S_OK;
            }
            break;
        }
        default:
            return E_NOT_SET;
        }

        if (msixRequest == NULL)
        {
            return E_NOT_SET;
        }
        if (!cli.IsQuietMode())
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

