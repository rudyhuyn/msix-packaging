#pragma once
#include "GeneralUtil.hpp"
#include "IPackageHandler.hpp"
#include "RegistryKey.hpp"
#include <vector>
namespace Win7MsixInstallerLib
{
struct ExeServerClass
{
    std::wstring id;
    std::wstring displayName;
    std::wstring enableOleDefaultHandler;
    std::wstring progId;
    std::wstring versionIndependentProgId;
    std::wstring autoConvertTo;
    std::wstring insertableObject;
    std::wstring shortDisplayName;
    std::vector<std::wstring> implementedCategories;
    std::wstring conversionReadableFormat;
    std::wstring conversionReadWritableFormat;
    std::wstring defaultFileDataFormat;
    std::vector<std::wstring> dataFormats;
};

struct ExeServer
{
    std::wstring executable;
    std::wstring arguments;
    std::wstring displayName;
    std::wstring launchAndActivationPermission;
    std::vector<ExeServerClass> classes;
};

struct ProgId
{
    std::wstring id;
    std::wstring clsid;
    std::wstring currentVersion;
};

class ComServer : IPackageHandler
{
public:
    HRESULT ExecuteForAddRequest(AddRequestInfo &requestInfo);

    static const PCWSTR HandlerName;
    static HRESULT CreateHandler(_Out_ IPackageHandler** instance);
    ~ComServer() {}
private:
    RegistryKey m_classesKey;

    std::vector<ExeServer> m_exeServers;
    std::vector<ProgId> m_progIds;

    /// Parses the manifest
    HRESULT ParseManifest(AddRequestInfo & request);

    /// Parses the manifest element to populate one 
    /// 
    /// @param comServerElement - the manifest element representing an comServer
    HRESULT ParseExeServerElement(IMsixElement* exeServerElement);

    HRESULT ParseExeServerClassElement(ExeServer& exeServer, IMsixElement* classElement);

    HRESULT ParseDataFormats(IMsixElement * classElement, ExeServerClass & exeServerClass);

    HRESULT ParseDataFormat(IMsixElement * dataFormatsElement, ExeServerClass & exeServerClass);

    HRESULT ParseConversionFormats(IMsixElement * classElement, const std::wstring & formatsQuery, std::wstring & formats);

    HRESULT ParseProgIdElement(IMsixElement* progIdElement);

    HRESULT ProcessExeServerForAdd(AddRequestInfo & request, ExeServer & exeServer);

    HRESULT ProcessProgIdForAdd(ProgId & progId);

    ComServer() {}

};
}