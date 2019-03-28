#pragma once

#include "GeneralUtil.hpp"
#include "IPackageHandler.hpp"
namespace Win7MsixInstallerLib
{
class Protocol : IPackageHandler
{
public:
    HRESULT ExecuteForAddRequest(PackageInfo * packageToInstall, const std::wstring & installDirectoryPath);

    static const PCWSTR HandlerName;
    static HRESULT CreateHandler(_In_ MsixRequest* msixRequest, _Out_ IPackageHandler** instance);
    ~Protocol() {}
private:
    MsixRequest* m_msixRequest = nullptr;

    Protocol() {}
    Protocol(_In_ MsixRequest* msixRequest) : m_msixRequest(msixRequest) {}

};
}