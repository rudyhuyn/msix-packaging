#pragma once
#include <string>
#include <vector>
#include <functional>
#include "IPackage.hpp"
#include "DeploymentOptions.hpp"
#include "DeploymentResult.hpp"

namespace Win7MsixInstallerLib {
    class IPackageManager
    {
    public:
        virtual bool AddPackage(const std::wstring & packageFilePath, DeploymentOptions options, std::function<void(const DeploymentResult &)> callback = nullptr) = 0;
        virtual bool RemovePackage(const std::wstring & packageFullName, std::function<void(const DeploymentResult &)> callback = nullptr) = 0;
        virtual IInstalledPackageInfo * FindPackage(const std::wstring & packageFamilyName) = 0;
        virtual std::vector<IInstalledPackageInfo*> * FindPackages() = 0;
        virtual IPackage * GetPackageInfoMsix(const std::wstring & msixFullPath) = 0;
    };
}
