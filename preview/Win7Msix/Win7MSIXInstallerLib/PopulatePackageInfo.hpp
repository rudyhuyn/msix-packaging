#pragma once

#include "GeneralUtil.hpp"
#include "Package.hpp"
#include <string>

namespace Win7MsixInstallerLib
{
class PopulatePackageInfo
{
public:
    static HRESULT GetPackageInfoFromPackage(PCWSTR packageFilePath, MSIX_VALIDATION_OPTION validationOption, Package ** packageInfo);
    static HRESULT GetPackageInfoFromManifest(const std::wstring & directoryPath, MSIX_VALIDATION_OPTION validationOption, InstalledPackageInfo ** packageInfo);
};
}