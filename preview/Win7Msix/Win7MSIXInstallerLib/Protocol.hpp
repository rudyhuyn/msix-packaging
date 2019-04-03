#pragma once

#include "GeneralUtil.hpp"
#include "IPackageHandler.hpp"
#include "RegistryKey.hpp"
#include <vector>

namespace Win7MsixInstallerLib
{

/// Data structs to be filled in from the information in the manifest
struct ProtocolData
{
    std::wstring name;          // name of the URI scheme, such as "mailto"
    std::wstring parameters;    // parameters to be passed into the executable when invoked
    std::wstring displayName;   // friendly name to be displayed to users
    std::wstring logo;          // filepath to logo file 
};

class Protocol : IPackageHandler
{
public:
    
    HRESULT ExecuteForAddRequest(AddRequestInfo &requestInfo);
    HRESULT ExecuteForRemoveRequest(RemoveRequestInfo& requestInfo);

    static const PCWSTR HandlerName;
    static HRESULT CreateHandler(_Out_ IPackageHandler** instance);
    ~Protocol() {}
private:
    RegistryKey m_classesKey;
    std::vector<ProtocolData> m_protocols;

    Protocol() {}

    /// Parse one protocol element, containing one ProtocolData to be added to the m_protocols vector.
    ///
    /// @param protocolElement - the IMsixElement representing the uap:Protocol element from the manifest
    HRESULT ParseProtocolElement(IMsixElement * protocolElement, const std::wstring & installDirectoryPath);

    /// Parses the manifest and fills out the m_protocols vector of ProtocolData containing data from the manifest
    HRESULT ParseManifest(PackageBase * package, const std::wstring & installDirectoryPath);

    /// Adds the protocol data to the system registry
    ///
    /// @param requestInfo.GetPackage() - the package to install
    /// @param installDirectoryPath - the full directory path where to install the package
    /// @param protocol - the protocol data to be added
    HRESULT ProcessProtocolForAdd(AddRequestInfo &requestInfo, ProtocolData & protocol);

    /// Removes the protocol data from the system registry
    ///
    /// @param requestInfo.GetPackage() - the package to uninstall
    /// @param protocol - the protocol data to be removed
    HRESULT ProcessProtocolForRemove(InstalledPackage * package, ProtocolData& protocol);

    /// Determines if the protocol is currently associated with the package
    ///
    /// @param fullExecutableFilePath - the full path of the exe associated to the package
    /// @param name - the name of the protocol
    bool IsCurrentlyAssociatedWithPackage(const std::wstring & fullExecutableFilePath, PCWSTR name);
};
}