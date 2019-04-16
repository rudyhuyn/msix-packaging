#include <windows.h>

#include <shlobj_core.h>
#include <CommCtrl.h>

#include "FilePaths.hpp"
#include "InstallComplete.hpp"
#include "GeneralUtil.hpp"
#include <TraceLoggingProvider.h>

using namespace Win7MsixInstallerLib;

const PCWSTR InstallComplete::HandlerName = L"InstallComplete";

HRESULT InstallComplete::ExecuteForAddRequest(AddRequestInfo &requestInfo)
{
    if (!requestInfo.GetIsInstallCancelled())
    {
        DeploymentResult result;
        result.Progress = 100;
        result.Status = InstallationStep::InstallationStepCompleted;
        requestInfo.SendCallback(result);
        return S_OK;
    }
}

HRESULT InstallComplete::CreateHandler(IPackageHandler ** instance)
{
    std::unique_ptr<InstallComplete> localInstance(new InstallComplete());
    if (localInstance == nullptr)
    {
        return E_OUTOFMEMORY;
    }
    *instance = localInstance.release();

    return S_OK;
}