#pragma once
#include "Package.hpp"

namespace Win7MsixInstallerLib
{
class AddRequestInfo
{
public:
    AddRequestInfo(): m_package(nullptr) {}
    void SetPackage(Package * package);
    inline Package * GetPackage() const { return m_package; }
    inline const std::wstring & GetInstallationDir() const { return m_installationDir; }
private:
    Package * m_package;
    std::wstring m_installationDir;
};

class RemoveRequestInfo
{
public:
    RemoveRequestInfo(): m_package(nullptr) {}
    void SetPackage(InstalledPackage * package);
    inline InstalledPackage * GetPackage() const { return m_package; }
private:
    InstalledPackage * m_package;
};
}
