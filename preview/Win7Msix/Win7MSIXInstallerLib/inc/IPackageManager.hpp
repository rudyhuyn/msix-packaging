#pragma once
#include <string>
#include <vector>
#include <functional>
#include "IPackageInfo.hpp"
#include "DeploymentOptions.hpp"
#include "IDeploymentResult.hpp"
#include "IInstallerUI.hpp"

namespace Win7MsixInstallerLib {
    class IPackageManager
    {
    public:
        virtual bool AddPackageAsync(const std::wstring & packageFilePath, DeploymentOptions options, std::function<IDeploymentResult()> callback) = 0;
        virtual bool RemovePackageAsync(const std::wstring & packageFullName, std::function<IDeploymentResult()> callback) = 0;
        virtual IPackageInfo * FindPackage(const std::wstring & packageFamilyName) = 0;
        virtual std::vector<IPackageInfo *> * FindPackages() = 0;
        virtual IPackageInfo * GetPackageInfoMsix(const std::wstring & msixFullPath) = 0;
    };
}
