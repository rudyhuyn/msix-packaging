#include "Win7MSIXInstallerActions.hpp"
#include "MsixRequest.hpp"
#include <experimental/filesystem>
#include <shlobj_core.h>
#include <KnownFolders.h>
#include "PopulatePackageInfo.hpp"
#include "Constants.hpp"

using namespace Win7MsixInstallerLib;

HRESULT Win7MsixInstallerLib_CreateAddPackageRequest(std::wstring packageFilePath, MSIX_VALIDATION_OPTION validationOption, Win7MsixInstallerLib::IMsixRequest** outInstance)
{
    MsixRequest *impl;
    RETURN_IF_FAILED(MsixRequest::Make(OperationType::Add, packageFilePath, L"", validationOption, &impl));
    *outInstance = (IMsixRequest*)impl;
    return S_OK;
}

HRESULT Win7MsixInstallerLib_CreateRemovePackageRequest(std::wstring packageFullName, Win7MsixInstallerLib::IMsixRequest** outInstance)
{
    MsixRequest *impl;
    RETURN_IF_FAILED(MsixRequest::Make(OperationType::Remove, L"", packageFullName, MSIX_VALIDATION_OPTION::MSIX_VALIDATION_OPTION_FULL, &impl));
    *outInstance = (IMsixRequest*)impl;
    return S_OK;
}

HRESULT Win7MsixInstallerLib_GetPackageInfo(std::wstring packageFullName, IPackageInfo** outInstance)
{
    auto filemapping = FilePathMappings::GetInstance();
    RETURN_IF_FAILED(filemapping.Initialize());

    std::wstring msix7Directory = filemapping.GetMsix7Directory();
    std::wstring packageDirectoryPath = msix7Directory + packageFullName;
    std::experimental::filesystem::path directory = packageDirectoryPath;
    std::wstring manifestPath = packageDirectoryPath + manifestFile;

    PackageInfo* packageInfo;
    RETURN_IF_FAILED(PopulatePackageInfo::GetPackageInfoFromManifest(manifestPath.c_str(), MSIX_VALIDATION_OPTION::MSIX_VALIDATION_OPTION_FULL, msix7Directory.c_str(), &packageInfo));
    *outInstance = (IPackageInfo*)packageInfo;
    return S_OK;
}

HRESULT Win7MsixInstallerLib_ListAllPackages(std::vector<std::wstring> **outPackages)
{
    std::vector<std::wstring> *packages = new std::vector<std::wstring>();
    auto filemapping = FilePathMappings::GetInstance();
    RETURN_IF_FAILED(filemapping.Initialize());
    auto folderPath = filemapping.GetMsix7Directory();
    for (auto& p : std::experimental::filesystem::directory_iterator(folderPath))
    {
        packages->push_back(std::wstring(p.path().filename()));
    }
    *outPackages = packages;
    return S_OK;
}
