#include <windows.h>

#include <shlobj_core.h>
#include <CommCtrl.h>

#include "RegistryKey.hpp"
#include "AddRemovePrograms.hpp"
#include "GeneralUtil.hpp"
#include <TraceLoggingProvider.h>
#include "Constants.hpp"
using namespace Win7MsixInstallerLib;

const PCWSTR AddRemovePrograms::HandlerName = L"AddRemovePrograms";

HRESULT AddRemovePrograms::ExecuteForAddRequest(AddRequestInfo &requestInfo)
{
    std::wstring packageFullName = requestInfo.GetPackage()->GetPackageFullName();

    RegistryKey uninstallKey;
    RETURN_IF_FAILED(uninstallKey.Open(HKEY_LOCAL_MACHINE, uninstallKeyPath.c_str(), KEY_WRITE));

    RegistryKey packageKey;
    RETURN_IF_FAILED(uninstallKey.CreateSubKey(packageFullName.c_str(), KEY_WRITE, &packageKey));

    std::wstring displayName = requestInfo.GetPackage()->GetDisplayName();
    RETURN_IF_FAILED(packageKey.SetStringValue(L"DisplayName", displayName));

    RETURN_IF_FAILED(packageKey.SetStringValue(L"InstallLocation", requestInfo.GetInstallationDir()));

    WCHAR filePath[MAX_PATH];
    DWORD lengthCopied = GetModuleFileNameW(nullptr, filePath, MAX_PATH);
    if (lengthCopied == 0)
    {
        RETURN_IF_FAILED(HRESULT_FROM_WIN32(GetLastError()));
    }

    std::wstring uninstallCommand = filePath + std::wstring(L" -RemovePackage ") + packageFullName;
    RETURN_IF_FAILED(packageKey.SetStringValue(L"UninstallString", uninstallCommand));

    std::wstring publisherNameString(requestInfo.GetPackage()->GetPublisherDisplayName());
    RETURN_IF_FAILED(packageKey.SetStringValue(L"Publisher", publisherNameString));

    std::wstring versionString(requestInfo.GetPackage()->GetVersion());
    RETURN_IF_FAILED(packageKey.SetStringValue(L"DisplayVersion", versionString));

    std::wstring packageIconString = requestInfo.GetInstallationDir() + requestInfo.GetPackage()->GetRelativeExecutableFilePath();
    RETURN_IF_FAILED(packageKey.SetStringValue(L"DisplayIcon", packageIconString));

    std::wstring publisherString(requestInfo.GetPackage()->GetPublisher());
    TraceLoggingWrite(g_MsixTraceLoggingProvider,
        "Added Uninstall key successfully",
        TraceLoggingValue(packageFullName.c_str(), "packageFullName"),
        TraceLoggingValue(uninstallCommand.c_str(), "uninstallString"),
        TraceLoggingValue(displayName.c_str(), "displayName"),
        TraceLoggingValue(requestInfo.GetInstallationDir().c_str(), "installLocation"),
        TraceLoggingValue(publisherString.c_str(), "publisher"),
        TraceLoggingValue(versionString.c_str(), "displayVersion"),
        TraceLoggingValue(packageIconString.c_str(), "displayIcon"));

    return S_OK;
}

HRESULT AddRemovePrograms::ExecuteForRemoveRequest(RemoveRequestInfo &requestInfo)
{
    RegistryKey uninstallKey;
    RETURN_IF_FAILED(uninstallKey.Open(HKEY_LOCAL_MACHINE, uninstallKeyPath.c_str(), KEY_WRITE));

    RETURN_IF_FAILED(uninstallKey.DeleteSubKey(requestInfo.GetPackage()->GetPackageFullName().c_str()));

    TraceLoggingWrite(g_MsixTraceLoggingProvider,
        "Removed Uninstall key successfully");
    return S_OK;
}

HRESULT AddRemovePrograms::CreateHandler(IPackageHandler ** instance)
{
    std::unique_ptr<AddRemovePrograms> localInstance(new AddRemovePrograms());
    if (localInstance == nullptr)
    {
        return E_OUTOFMEMORY;
    }
    *instance = localInstance.release();

    return S_OK;
}
