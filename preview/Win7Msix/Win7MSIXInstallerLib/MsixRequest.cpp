#include "generalutil.hpp"
#include "MsixRequest.hpp"

#include <string>
#include <map>
#include <algorithm>
#include <utility>
#include <iomanip>
#include <iostream>
#include <functional>
#include <thread>

#include "FootprintFiles.hpp"
#include "FilePaths.hpp"
#include <cstdio>
#include <experimental/filesystem> // C++-standard header file name
#include <filesystem> // Microsoft-specific implementation header file name
#include <TraceLoggingProvider.h>

// handlers
#include "Extractor.hpp"
#include "PopulatePackageInfo.hpp"
#include "StartMenuLink.hpp"
#include "AddRemovePrograms.hpp"
#include "Protocol.hpp"
#include "FileTypeAssociation.hpp"
#include "InstallComplete.hpp"
#include "GeneralUtil.hpp"
#include "Constants.hpp"

// MSIXWindows.hpp define NOMINMAX because we want to use std::min/std::max from <algorithm>
// GdiPlus.h requires a definiton for min and max. Use std namespace *BEFORE* including it.
using namespace std;
#include <GdiPlus.h>
using namespace Win7MsixInstallerLib;
struct HandlerInfo
{
    CreateHandler create;
    PCWSTR nextHandler;
};

std::map<PCWSTR, HandlerInfo> AddHandlers =
{
    //HandlerName                       Function to create                   NextHandler
    {Extractor::HandlerName,            {Extractor::CreateHandler,           StartMenuLink::HandlerName }},
    {StartMenuLink::HandlerName,        {StartMenuLink::CreateHandler,       AddRemovePrograms::HandlerName}},
    {AddRemovePrograms::HandlerName,    {AddRemovePrograms::CreateHandler,   Protocol::HandlerName}},
    {Protocol::HandlerName,             {Protocol::CreateHandler,            FileTypeAssociation::HandlerName}},
    {FileTypeAssociation::HandlerName,  {FileTypeAssociation::CreateHandler, InstallComplete::HandlerName }},
    {InstallComplete::HandlerName,      {InstallComplete::CreateHandler,     nullptr}},
};

std::map<PCWSTR, HandlerInfo> RemoveHandlers =
{
    //HandlerName                       Function to create                   NextHandler
    {StartMenuLink::HandlerName,        {StartMenuLink::CreateHandler,       AddRemovePrograms::HandlerName}},
    {AddRemovePrograms::HandlerName,    {AddRemovePrograms::CreateHandler,   Protocol::HandlerName}},
    {Protocol::HandlerName,             {Protocol::CreateHandler,            FileTypeAssociation::HandlerName}},
    {FileTypeAssociation::HandlerName,  {FileTypeAssociation::CreateHandler, Extractor::HandlerName}},
    {Extractor::HandlerName,            {Extractor::CreateHandler,           nullptr}},
};

HRESULT MsixRequest::Make(OperationType operationType, const std::wstring & packageFilePath, std::wstring packageFullName, MSIX_VALIDATION_OPTION validationOption, MsixRequest ** outInstance)
{
    MsixRequest* instance = new MsixRequest();
    instance->m_operationType = operationType;
    instance->m_packageFilePath = packageFilePath;
    instance->m_packageFullName = packageFullName;
    instance->m_validationOptions = validationOption;
    auto filepathMappings = FilePathMappings::GetInstance();
    RETURN_IF_FAILED(filepathMappings.GetInitializationResult());
    *outInstance = instance;
    return S_OK;
}

HRESULT MsixRequest::ProcessRequest()
{
    auto result = DeploymentResult();
    result.Progress = 0;
    result.Status = InstallationStep::InstallationStepStarted;
    SendCallback(result);

    switch (m_operationType)
    {
    case OperationType::Add:
    {
        RETURN_IF_FAILED(ProcessAddRequest());
        break;
    }
    case OperationType::Remove:
    {
        RETURN_IF_FAILED(ProcessRemoveRequest());
        break;
    }
    default:
        return HRESULT_FROM_WIN32(ERROR_NOT_SUPPORTED);
    }

    return S_OK;
}

HRESULT MsixRequest::ProcessAddRequest()
{
    auto filemapping = FilePathMappings::GetInstance();
    auto res = filemapping.GetInitializationResult();
    if (FAILED(res))
    {
        return E_FAIL;
    }
    PackageInfo* packageInfo;
    PopulatePackageInfo::GetPackageInfoFromPackage(this->m_packageFilePath.data(), this->m_validationOptions, &packageInfo);

    if (packageInfo == nullptr)
    {
        return E_FAIL;
    }

    PCWSTR currentHandlerName = Extractor::HandlerName;

    auto installationPath = filemapping.GetMsix7Directory() + packageInfo->GetPackageFullName() + L"\\";
    while (currentHandlerName != nullptr)
    {
        TraceLoggingWrite(g_MsixTraceLoggingProvider,
            "Executing handler",
            TraceLoggingValue(currentHandlerName, "HandlerName"));

        HandlerInfo currentHandler = AddHandlers[currentHandlerName];
        AutoPtr<IPackageHandler> handler;
        RETURN_IF_FAILED(currentHandler.create(this, &handler));
        RETURN_IF_FAILED(handler->ExecuteForAddRequest(packageInfo, installationPath));

        currentHandlerName = currentHandler.nextHandler;
    }

    return S_OK;
}

HRESULT MsixRequest::ProcessRemoveRequest()
{
    auto filemapping = FilePathMappings::GetInstance();
    auto res = filemapping.GetInitializationResult();
    if (FAILED(res))
    {
        return E_FAIL;
    }
    std::wstring msix7Directory = filemapping.GetMsix7Directory();

    InstalledPackageInfo* installedPackageInfo;
    std::wstring manifestPath = msix7Directory + m_packageFullName + manifestFile;

    PopulatePackageInfo::GetPackageInfoFromManifest(manifestPath.data(), this->m_validationOptions, &installedPackageInfo);

    if (installedPackageInfo == nullptr)
    {
        return E_FAIL;
    }
    PCWSTR currentHandlerName = StartMenuLink::HandlerName;

    while (currentHandlerName != nullptr)
    {
        TraceLoggingWrite(g_MsixTraceLoggingProvider,
            "Executing handler",
            TraceLoggingValue(currentHandlerName, "HandlerName"));

        HandlerInfo currentHandler = RemoveHandlers[currentHandlerName];
        AutoPtr<IPackageHandler> handler;
        RETURN_IF_FAILED(currentHandler.create(this, &handler));
        HRESULT hrExecute = handler->ExecuteForRemoveRequest(installedPackageInfo);
        if (FAILED(hrExecute))
        {
            TraceLoggingWrite(g_MsixTraceLoggingProvider,
                "Handler failed -- removal is best effort so error is non-fatal",
                TraceLoggingLevel(WINEVENT_LEVEL_WARNING),
                TraceLoggingValue(currentHandlerName, "HandlerName"),
                TraceLoggingValue(hrExecute, "HR"));
        }

        currentHandlerName = currentHandler.nextHandler;
    }

    return S_OK;
}
