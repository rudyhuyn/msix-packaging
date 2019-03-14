#pragma once

#pragma once

#include "GeneralUtil.hpp"
#include "IPackageHandler.hpp"
namespace Win7MsixInstallerLib
{
    class PopulatePackageInfo : IPackageHandler
    {
    public:
        /// Opens the package to grab the package reader, manifest reader and read the info out of the manifest
        HRESULT ExecuteForAddRequest();

        /// Opens the manifest from the filesystem to create a manifest reader and read the info out of the manifest
        /// @return E_NOT_SET when the package cannot be found
        HRESULT ExecuteForRemoveRequest();

        static const PCWSTR HandlerName;
        static HRESULT CreateHandler(_In_ MsixRequest* msixRequest, _Out_ IPackageHandler** instance);
        ~PopulatePackageInfo() {}
        static HRESULT GetPackageInfoFromPackage(PCWSTR packageFilePath, MSIX_VALIDATION_OPTION validationOption, std::wstring msix7DirectoryPath, PackageInfo ** packageInfo);
        static HRESULT GetPackageInfoFromManifest(PCWSTR manifestPath, MSIX_VALIDATION_OPTION validationOption, PCWSTR msix7DirectoryPath, PackageInfo ** packageInfo);
    private:
        MsixRequest* m_msixRequest = nullptr;

        PopulatePackageInfo() {}
        PopulatePackageInfo(_In_ MsixRequest* msixRequest) : m_msixRequest(msixRequest) {}

        HRESULT CreatePackageReader();
    };
}