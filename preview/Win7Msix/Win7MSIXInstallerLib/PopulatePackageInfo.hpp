#pragma once

#include "GeneralUtil.hpp"
#include "PackageInfo.hpp"
#include <string>

namespace Win7MsixInstallerLib
{
class PopulatePackageInfo
{
public:
    static HRESULT GetPackageInfoFromPackage(PCWSTR packageFilePath, MSIX_VALIDATION_OPTION validationOption, PackageInfo ** packageInfo);
    static HRESULT GetPackageInfoFromManifest(const std::wstring & directoryPath, MSIX_VALIDATION_OPTION validationOption, InstalledPackageInfo ** packageInfo);
};
}