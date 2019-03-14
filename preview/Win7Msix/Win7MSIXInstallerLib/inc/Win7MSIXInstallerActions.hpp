#pragma once
#include "DllExport.hpp"
#include "IMsixRequest.hpp"
#include <AppxPackaging.hpp>
#include <vector>

extern "C" DLLEXPORT HRESULT Win7MsixInstallerLib_CreateAddPackageRequest(std::wstring packageFilePath, MSIX_VALIDATION_OPTION validationOption, Win7MsixInstallerLib::IMsixRequest** outInstance);
extern "C" DLLEXPORT HRESULT Win7MsixInstallerLib_CreateRemovePackageRequest(std::wstring packageFullName, Win7MsixInstallerLib::IMsixRequest** outInstance);
extern "C" DLLEXPORT HRESULT Win7MsixInstallerLib_GetPackageInfo(std::wstring packageFullName, Win7MsixInstallerLib::IPackageInfo** outInstance);
extern "C" DLLEXPORT HRESULT Win7MsixInstallerLib_ListAllPackages(std::vector<std::wstring>** packages);
