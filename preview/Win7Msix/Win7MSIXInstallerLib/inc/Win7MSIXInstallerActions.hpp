#pragma once
#include "DllExport.hpp"
#include "IMsixRequest.hpp"
#include <AppxPackaging.hpp>
#include <vector>

HRESULT DLLEXPORT Win7MsixInstaller_CreateAddPackageRequest(std::wstring packageFilePath, MSIX_VALIDATION_OPTION validationOption, Win7MsixInstallerLib::IMsixRequest** outInstance);
HRESULT DLLEXPORT Win7MsixInstaller_CreateRemovePackageRequest(std::wstring packageFullName, Win7MsixInstallerLib::IMsixRequest** outInstance);
HRESULT DLLEXPORT Win7MsixInstaller_GetPackageInfo(std::wstring packageFullName, Win7MsixInstallerLib::IPackageInfo** outInstance);
HRESULT DLLEXPORT Win7MsixInstaller_ListAllPackages(std::vector<std::wstring>** packages);
