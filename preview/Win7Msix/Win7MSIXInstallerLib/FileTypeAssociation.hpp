#pragma once

#include "GeneralUtil.hpp"
#include "IPackageHandler.hpp"
#include "RegistryKey.hpp"
#include "RegistryDevirtualizer.hpp"

namespace Win7MsixInstallerLib
{
/// Data structs to be filled in from the information in the manifest
struct Verb
{
    std::wstring verb;
    std::wstring parameter;
};

struct Fta
{
    std::wstring name;
    std::wstring progID;
    std::vector<std::wstring> extensions;
    std::wstring logo;
    std::vector<Verb> verbs;
};

class FileTypeAssociation : IPackageHandler
{
public:
    /// Adds the file type associations to the registry so this application can handle specific file types.
    HRESULT ExecuteForAddRequest(AddRequestInfo &requestInfo);

    /// Removes the file type associations from the registry.
    HRESULT ExecuteForRemoveRequest(RemoveRequestInfo& requestInfo);

    static const PCWSTR HandlerName;
    static HRESULT CreateHandler(_Out_ IPackageHandler** instance);
    ~FileTypeAssociation() {}
private:
    RegistryKey m_classesKey;
    AutoPtr<RegistryDevirtualizer> m_registryDevirtualizer;
    std::vector<Fta> m_Ftas;

    FileTypeAssociation() {}

    /// Parses the manifest and fills in the m_Ftas vector of FileTypeAssociation (Fta) data
    HRESULT ParseManifest(PackageBase * package, const std::wstring & installationDirectoryPath);

    /// Parses the manifest element to populate one Fta struct entry of the m_Ftas vector
    /// 
    /// @param ftaElement - the manifest element representing an Fta
    HRESULT ParseFtaElement(PackageBase * package, const std::wstring & installDirectoryPath, IMsixElement* ftaElement);

    /// Adds the file type association (fta) entries if necessary
    HRESULT ProcessFtaForAdd(PackageBase * package, const std::wstring & installDirectoryPath, Fta& fta);

    /// Removes the file type association (fta) entries if necessary
    HRESULT ProcessFtaForRemove(Fta& fta);

    /// Creates a ProgID from the name of the fta. Simply take the package name and prepend it to the fta
    std::wstring CreateProgID(PackageBase * package, PCWSTR name);
};
}