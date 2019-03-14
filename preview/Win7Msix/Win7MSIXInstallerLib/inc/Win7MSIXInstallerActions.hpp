#pragma once
#include "DllExport.hpp"
#include "IMsixRequest.hpp"
#include <AppxPackaging.hpp>
#include <vector>

HRESULT DLLEXPORT Win7MsixInstallerLib_CreateAddPackageRequest(std::wstring packageFilePath, MSIX_VALIDATION_OPTION validationOption, Win7MsixInstallerLib::IMsixRequest** outInstance);
HRESULT DLLEXPORT Win7MsixInstallerLib_CreateRemovePackageRequest(std::wstring packageFullName, Win7MsixInstallerLib::IMsixRequest** outInstance);
HRESULT DLLEXPORT Win7MsixInstallerLib_GetPackageInfo(std::wstring packageFullName, Win7MsixInstallerLib::IPackageInfo** outInstance);
HRESULT DLLEXPORT Win7MsixInstallerLib_ListAllPackages(std::vector<std::wstring>** packages);
