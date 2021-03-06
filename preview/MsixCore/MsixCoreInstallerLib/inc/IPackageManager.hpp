#pragma once
#include <string>
#include <vector>
#include <functional>
#include "IMsixResponse.hpp"
#include "IPackage.hpp"
#include "DeploymentOptions.hpp"


namespace MsixCoreLib {
    class IPackageManager
    {
    public:
        virtual std::shared_ptr<IMsixResponse> AddPackageAsync(const std::wstring & packageFilePath, DeploymentOptions options, std::function<void(const IMsixResponse&)> callback = nullptr) = 0;
        virtual HRESULT AddPackage(const std::wstring & packageFilePath, DeploymentOptions options) = 0;
        virtual std::shared_ptr<IMsixResponse> RemovePackageAsync(const std::wstring & packageFullName, std::function<void(const IMsixResponse&)> callback = nullptr) = 0;
        virtual HRESULT RemovePackage(const std::wstring & packageFullName) = 0;
        virtual std::shared_ptr<IInstalledPackage> FindPackage(const std::wstring & packageFamilyName) = 0;
        virtual std::shared_ptr<IInstalledPackage> FindPackageByFamilyName(const std::wstring & packageFamilyName) = 0;
        virtual std::unique_ptr<std::vector<std::shared_ptr<IInstalledPackage>>> FindPackages() = 0;
        virtual std::shared_ptr<IPackage> GetMsixPackageInfo(const std::wstring & msixFullPath) = 0;
    };
}
