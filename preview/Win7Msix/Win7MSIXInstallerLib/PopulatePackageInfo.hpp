#pragma once

#include "GeneralUtil.hpp"
#include "Package.hpp"
#include "IPackageHandler.hpp"
#include <string>

namespace Win7MsixInstallerLib
{
class PopulatePackageInfo : IPackageHandler
{
public:
    /// Opens the package to grab the package reader, manifest reader and read the info out of the manifest
    HRESULT ExecuteForAddRequest(AddRequestInfo &requestInfo);

    /// Opens the manifest from the filesystem to create a manifest reader and read the info out of the manifest
    /// @return E_NOT_SET when the package cannot be found
    HRESULT ExecuteForRemoveRequest(RemoveRequestInfo &requestInfo);

    bool IsMandatoryForRemoveRequest() { return true; }

    static const PCWSTR HandlerName;
    static HRESULT CreateHandler(_Out_ IPackageHandler** instance);
    ~PopulatePackageInfo() {}

    static HRESULT GetPackageInfoFromManifest(const std::wstring & directoryPath, MSIX_VALIDATION_OPTION validationOption, InstalledPackage ** packageInfo);
    static HRESULT GetPackageInfoFromPackage(const std::wstring & packageFilePath, MSIX_VALIDATION_OPTION validationOption, Package ** packageInfo);
private:

    PopulatePackageInfo() {}
};
}