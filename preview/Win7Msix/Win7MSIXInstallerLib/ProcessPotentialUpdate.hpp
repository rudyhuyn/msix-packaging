#pragma once

#include "GeneralUtil.hpp"
#include "IPackageHandler.hpp"
#include "Package.hpp"

namespace Win7MsixInstallerLib
{
/// Determines if the incoming add request is actually an update to an existing package.
/// If it is, it'll remove the outdated package
class ProcessPotentialUpdate : IPackageHandler
{
public:
    /// Determines if the incoming add request is actually an update to an existing package.
    /// If it is, it'll remove the outdated package. This handler is only run on an AddRequest.
    HRESULT ExecuteForAddRequest(AddRequestInfo &requestInfo);

    static const PCWSTR HandlerName;
    static HRESULT CreateHandler(_Out_ IPackageHandler** instance);
    ~ProcessPotentialUpdate() {}
private:

    ProcessPotentialUpdate() {}

    /// Synchronously removes the outdated package before allowing the current request to proceed
    HRESULT RemovePackage(std::wstring packageFullName);
};
}