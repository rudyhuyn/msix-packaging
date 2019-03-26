#pragma once
#include "inc/IPackageManager.hpp"
#include "inc/IPackageInfo.hpp"

namespace Win7MsixInstallerLib {

    class PackageManager :
        public IPackageManager
    {
    public:
        PackageManager();
        bool AddPackageAsync(const std::wstring & packageFilePath, DeploymentOptions options, std::function<IDeploymentResult()> callback) override;
        bool RemovePackageAsync(const std::wstring & packageFullName, std::function<IDeploymentResult()> callback) override;
        IPackageInfo * FindPackage(const std::wstring & packageFamilyName) override;
        std::vector<IPackageInfo *> * FindPackages() override;
        IPackageInfo * GetPackageInfoMsix(const std::wstring & msixFullPath) override;
    private:
        IPackageInfo * GetPackageInfo(const std::wstring & msix7Directory, const std::wstring & directoryPath);
    };
}