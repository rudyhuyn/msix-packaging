#pragma once
#include <string>
#include <vector>
#include <functional>
#include "IPackageInfo.hpp"
#include "DeploymentOptions.hpp"
#include "DeploymentResult.hpp"

namespace Win7MsixInstallerLib {
    class IPackageManager
    {
    public:
        virtual bool AddPackageAsync(const std::wstring & packageFilePath, DeploymentOptions options, std::function<void(const DeploymentResult &)> callback = nullptr) = 0;
        virtual bool RemovePackageAsync(const std::wstring & packageFullName, std::function<void(const DeploymentResult &)> callback = nullptr) = 0;
        virtual IInstalledPackageInfo * FindPackage(const std::wstring & packageFamilyName) = 0;
        virtual std::vector<IInstalledPackageInfo*> * FindPackages() = 0;
        virtual IPackageInfo * GetPackageInfoMsix(const std::wstring & msixFullPath) = 0;
    };
}
