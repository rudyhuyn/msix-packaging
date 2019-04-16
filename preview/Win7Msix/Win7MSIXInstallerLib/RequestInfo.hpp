#pragma once
#include "Package.hpp"
#include "DeploymentResult.hpp"
#include <functional>
namespace Win7MsixInstallerLib
{

    class RequestInfoWithCallback
    {
    public:
        RequestInfoWithCallback(std::function<void(const DeploymentResult &)> callback = nullptr):m_callback(callback)
        {}

        void SendCallback(const DeploymentResult & result)
        {
            if (m_callback != nullptr)
            {
                m_callback(result);
            }
        }
    private:
        std::function<void(DeploymentResult)> m_callback;
    };

    class AddRequestInfo : public RequestInfoWithCallback
    {
    public:
        AddRequestInfo(std::wstring packageToInstall, MSIX_VALIDATION_OPTION validationOption, std::function<void(const DeploymentResult &)> callback = nullptr):
            RequestInfoWithCallback(callback),
            m_packageFilePathToInstall(packageToInstall),
            m_validationOptions(validationOption),
            m_package(nullptr) {}
        void SetPackage(Package * package);
        inline Package * GetPackage() const { return m_package; }
        inline const std::wstring & GetInstallationDir() const { return m_installationDir; }
        MSIX_VALIDATION_OPTION GetValidationOptions() { return m_validationOptions; }
        const std::wstring & GetPackageFilePathToInstall() { return m_packageFilePathToInstall; }
        bool GetIsInstallCancelled() { return false; }
    private:
        Package * m_package;
        std::wstring m_installationDir;
        std::wstring m_packageFilePathToInstall;
        
        MSIX_VALIDATION_OPTION m_validationOptions = MSIX_VALIDATION_OPTION::MSIX_VALIDATION_OPTION_FULL;
    };

    class RemoveRequestInfo : public RequestInfoWithCallback
    {
    public:
        RemoveRequestInfo(std::wstring packageFullNameToUninstall, std::function<void(const DeploymentResult &)> callback = nullptr) :
            RequestInfoWithCallback(callback),
            m_packageFullNameToUninstall(packageFullNameToUninstall),
            m_package(nullptr) {}
        void SetPackage(InstalledPackage * package);
        inline InstalledPackage * GetPackage() const { return m_package; }
        const std::wstring & GetPackageFullNameToUninstall() { return m_packageFullNameToUninstall; }

    private:
        InstalledPackage * m_package;
        std::wstring m_installationDir;
        std::wstring m_packageFullNameToUninstall;
        std::function<void(DeploymentResult)> m_callback;
    };
}
