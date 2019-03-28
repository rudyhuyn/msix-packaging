#pragma once
#include "inc/IPackageManager.hpp"
#include "inc/IPackageInfo.hpp"

namespace Win7MsixInstallerLib {

    class PackageManager :
        public IPackageManager
    {
    public:
        PackageManager();
        bool AddPackageAsync(const std::wstring & packageFilePath, DeploymentOptions options, std::function<void(const DeploymentResult&)> callback = nullptr) override;
        bool RemovePackageAsync(const std::wstring & packageFullName, std::function<void(const DeploymentResult&)> callback = nullptr) override;
        IInstalledPackageInfo * FindPackage(const std::wstring & packageFamilyName) override;
        std::vector<IInstalledPackageInfo *> * FindPackages() override;
        IPackageInfo * GetPackageInfoMsix(const std::wstring & msixFullPath) override;
    private:
        IInstalledPackageInfo * GetPackageInfo(const std::wstring & msix7Directory, const std::wstring & directoryPath);
    };
}