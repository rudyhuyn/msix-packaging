#include "RequestInfo.hpp"
#include "FilePaths.hpp"

using namespace std;
using namespace Win7MsixInstallerLib;

void AddRequestInfo::SetPackage(Package * package)
{
    m_package = package;
    if (package != nullptr)
    {
        m_installationDir = FilePathMappings::GetInstance().GetMsix7Directory() + package->GetPackageFullName() + L"\\";
    }
    else
    {
        m_installationDir = nullptr;
    }
}

void RemoveRequestInfo::SetPackage(InstalledPackage * package)
{
    m_package = package;
}

