#include "PackageManager.hpp"
#include "MsixRequest.hpp"
#include "Constants.hpp"
#include "PopulatePackageInfo.hpp"
#include <experimental/filesystem>

using namespace std;
using namespace Win7MsixInstallerLib;

PackageManager::PackageManager()
{
}

bool PackageManager::AddPackage(const wstring & packageFilePath, DeploymentOptions options, function<void(const DeploymentResult&)> callback)
{
    MsixRequest *impl;
    auto res = (MsixRequest::Make(OperationType::Add, packageFilePath, L"", MSIX_VALIDATION_OPTION::MSIX_VALIDATION_OPTION_FULL, &impl));
    if (FAILED(res))
    {
        return false;
    }
    impl->SetCallback(callback);
    res = impl->ProcessRequest();
    delete impl;
    return SUCCEEDED(res);
}

bool PackageManager::RemovePackage(const wstring & packageFullName, function<void(const DeploymentResult&)> callback)
{
    MsixRequest *impl;
    auto res = (MsixRequest::Make(OperationType::Remove, L"", packageFullName, MSIX_VALIDATION_OPTION::MSIX_VALIDATION_OPTION_FULL, &impl));
    if (FAILED(res))
    {
        return false;
    }

    res = impl->ProcessRequest();
    impl->SetCallback(callback);
    delete impl;
    return SUCCEEDED(res);
}

IInstalledPackageInfo * PackageManager::GetPackageInfo(const std::wstring & msix7Directory, const std::wstring & directoryPath)
{
    InstalledPackageInfo* packageInfo;
    auto res = PopulatePackageInfo::GetPackageInfoFromManifest(directoryPath.c_str(), MSIX_VALIDATION_OPTION::MSIX_VALIDATION_OPTION_FULL, &packageInfo);
    if (FAILED(res))
    {
        return nullptr;
    }
    return (IInstalledPackageInfo *)packageInfo;
}

IInstalledPackageInfo * PackageManager::FindPackage(const wstring & packageFamilyName)
{
    auto filemapping = FilePathMappings::GetInstance();
    auto res = filemapping.GetInitializationResult();
    if (FAILED(res))
    {
        return nullptr;
    }
    std::wstring msix7Directory = filemapping.GetMsix7Directory();
    std::wstring packageDirectoryPath = msix7Directory + packageFamilyName;
    return GetPackageInfo(msix7Directory, packageDirectoryPath);
}

vector<IInstalledPackageInfo *> * PackageManager::FindPackages()
{
    auto filemapping = FilePathMappings::GetInstance();
    auto res = filemapping.GetInitializationResult();
    if (FAILED(res))
    {
        return nullptr;
    }
    auto packages = new std::vector<IInstalledPackageInfo *>();
    auto msix7Directory = filemapping.GetMsix7Directory();
    for (auto& p : std::experimental::filesystem::directory_iterator(msix7Directory))
    {
        auto packageInfo = GetPackageInfo(msix7Directory, p.path());
        if (packageInfo != nullptr)
        {
            packages->push_back((IInstalledPackageInfo *)packageInfo);
        }
    }

    return packages;
}

IPackageInfo * PackageManager::GetPackageInfoMsix(const wstring & msixFullPath)
{
    auto filemapping = FilePathMappings::GetInstance();
    auto res = filemapping.GetInitializationResult();
    if (FAILED(res))
    {
        return nullptr;
    }
    PackageInfo* packageInfo;
    res = PopulatePackageInfo::GetPackageInfoFromPackage(msixFullPath.c_str(), MSIX_VALIDATION_OPTION::MSIX_VALIDATION_OPTION_FULL, &packageInfo);
    if (FAILED(res))
    {
        return nullptr;
    }
    return (IPackageInfo *)packageInfo;
}