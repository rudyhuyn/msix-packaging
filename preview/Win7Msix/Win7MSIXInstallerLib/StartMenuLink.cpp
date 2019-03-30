#include <windows.h>

#include <shlobj_core.h>
#include <CommCtrl.h>
#include <propvarutil.h>
#include <propkey.h>

#include "FilePaths.hpp"
#include "StartMenuLink.hpp"
#include "GeneralUtil.hpp"
#include <TraceLoggingProvider.h>
using namespace Win7MsixInstallerLib;

const PCWSTR StartMenuLink::HandlerName = L"StartMenuLink";

HRESULT StartMenuLink::CreateLink(PCWSTR targetFilePath, PCWSTR linkFilePath, PCWSTR description, PCWSTR appUserModelId)
{
    TraceLoggingWrite(g_MsixTraceLoggingProvider,
        "Creating Link",
        TraceLoggingValue(targetFilePath, "TargetFilePath"),
        TraceLoggingValue(linkFilePath, "LinkFilePath"),
        TraceLoggingValue(appUserModelId, "AppUserModelId"));

    ComPtr<IShellLink> shellLink;
    RETURN_IF_FAILED(CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, reinterpret_cast<LPVOID*>(&shellLink)));
    RETURN_IF_FAILED(shellLink->SetPath(targetFilePath));
    RETURN_IF_FAILED(shellLink->SetArguments(L""));

    if (appUserModelId != NULL && appUserModelId[0] != 0)
    {
        ComPtr<IPropertyStore> propertyStore;
        PROPVARIANT appIdPropVar;
        RETURN_IF_FAILED(shellLink->QueryInterface(IID_IPropertyStore, reinterpret_cast<LPVOID*>(&propertyStore)));
        RETURN_IF_FAILED(InitPropVariantFromString(appUserModelId, &appIdPropVar));
        RETURN_IF_FAILED(propertyStore->SetValue(PKEY_AppUserModel_ID, appIdPropVar));
        RETURN_IF_FAILED(propertyStore->Commit());
        PropVariantClear(&appIdPropVar);
    }

    RETURN_IF_FAILED(shellLink->SetDescription(description));

    ComPtr<IPersistFile> persistFile;
    RETURN_IF_FAILED(shellLink->QueryInterface(IID_IPersistFile, reinterpret_cast<LPVOID*>(&persistFile)));
    RETURN_IF_FAILED(persistFile->Save(linkFilePath, TRUE));
    return S_OK;
}

HRESULT StartMenuLink::ExecuteForAddRequest(Package * packageToInstall, const std::wstring & installDirectoryPath)
{
    std::wstring filePath = FilePathMappings::GetInstance().GetMap()[L"Common Programs"] + L"\\" + packageToInstall->GetDisplayName() + L".lnk";

    std::wstring resolvedExecutableFullPath = installDirectoryPath + packageToInstall->GetRelativeExecutableFilePath();
    std::wstring appUserModelId = packageToInstall->GetId();
    RETURN_IF_FAILED(CreateLink(resolvedExecutableFullPath.c_str(), filePath.c_str(), L"", appUserModelId.c_str()));

    return S_OK;
}

HRESULT StartMenuLink::ExecuteForRemoveRequest(InstalledPackage * packageToUninstall)
{
    std::wstring filePath = FilePathMappings::GetInstance().GetMap()[L"Common Programs"] + L"\\" + packageToUninstall->GetDisplayName() + L".lnk";

    RETURN_IF_FAILED(DeleteFile(filePath.c_str()));
    return S_OK;
}

HRESULT StartMenuLink::CreateHandler(MsixRequest * msixRequest, IPackageHandler ** instance)
{
    std::unique_ptr<StartMenuLink> localInstance(new StartMenuLink(msixRequest));
    if (localInstance == nullptr)
    {
        return E_OUTOFMEMORY;
    }
    *instance = localInstance.release();

    return S_OK;
}
