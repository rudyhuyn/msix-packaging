#pragma once
#include "GeneralUtil.hpp"
#include "IPackageHandler.hpp"
#include "RegistryKey.hpp"
#include <vector>
namespace Win7MsixInstallerLib
{
struct Interface
{
    std::wstring id;
    std::wstring proxyStubClsid;
    std::wstring typeLibId;
    std::wstring typeLibVersion;
};

struct Version
{
    std::wstring displayName;
    std::wstring versionNumber;
    std::wstring localeId;
    std::wstring libraryFlag;
    std::wstring helpDirectory;
    std::wstring win32Path;
    std::wstring win64Path;
};

struct TypeLib
{
    std::wstring id;
    std::vector<Version> version;
};

class ComInterface : IPackageHandler
{
public:
    HRESULT ExecuteForAddRequest(AddRequestInfo &requestInfo);

    static const PCWSTR HandlerName;
    
    static HRESULT CreateHandler(_Out_ IPackageHandler** instance);
    ~ComInterface() {}
private:
    RegistryKey m_classesKey;

    std::vector<Interface> m_interfaces;
    std::vector<TypeLib> m_typeLibs;

    /// Parses the manifest and fills in the m_ComInfos vector of ComInfo data
    /// @param package to parse
    HRESULT ParseManifest(Package * package);

    /// Parses the manifest element to populate one Interface struct entry of the m_ComInfos vector
    /// 
    /// @param interfaceElement - the manifest element representing an interface
    HRESULT ParseComInterfaceElement(IMsixElement* interfaceElement);

    HRESULT ParseTypeLibElement(IMsixElement* typeLibElement);

    HRESULT ParseVersionElement(TypeLib & typeLib, IMsixElement * versionElement);

    HRESULT ProcessInterfaceForAddRequest(AddRequestInfo & request, Interface & comInterface);

    HRESULT ProcessTypeLibForAddRequest(AddRequestInfo & request, TypeLib & typeLib);

    ComInterface() {}
};
}
