#include <windows.h>

#include <shlobj_core.h>
#include <CommCtrl.h>

#include "FilePaths.hpp"
#include "InstallComplete.hpp"
#include "GeneralUtil.hpp"
#include <TraceLoggingProvider.h>

using namespace Win7MsixInstallerLib;

const PCWSTR InstallComplete::HandlerName = L"InstallComplete";

HRESULT InstallComplete::ExecuteForAddRequest()
{
    if (!m_msixRequest->GetMsixResponse()->GetIsInstallCancelled())
    {
        m_msixRequest->GetMsixResponse()->SendCallback(InstallationStep::InstallationStepCompleted, 100);
    }
    return S_OK;
}

HRESULT InstallComplete::CreateHandler(MsixRequest * msixRequest, IPackageHandler ** instance)
{
    std::unique_ptr<InstallComplete> localInstance(new InstallComplete(msixRequest));
    if (localInstance == nullptr)
    {
        return E_OUTOFMEMORY;
    }
    *instance = localInstance.release();

    return S_OK;
}