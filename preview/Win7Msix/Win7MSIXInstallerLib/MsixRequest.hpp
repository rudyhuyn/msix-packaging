#pragma once
#include "PackageInfo.hpp"
#include "FilePaths.hpp"
#include "IInstallerUI.hpp"
#include "IMsixRequest.hpp"

namespace Win7MsixInstallerLib
{

enum OperationType
{
    Add = 1,
    Remove = 2,
};

/// MsixRequest represents what this instance of the executable will be doing and tracks the state of the current operation
class MsixRequest :IMsixRequest
{
private:
    /// Should always be available via constructor
    std::wstring m_packageFilePath;
    std::wstring m_packageFullName;
    MSIX_VALIDATION_OPTION m_validationOptions = MSIX_VALIDATION_OPTION::MSIX_VALIDATION_OPTION_FULL;
    OperationType m_operationType = Add;
    /// Filled by PopulatePackageInfo
    AutoPtr<PackageInfo> m_packageInfo;

    /// Filled in by CreateAndShowUI 
    AutoPtr<IInstallerUI> m_UI;

protected:
    MsixRequest() {}
public:
    static HRESULT Make(OperationType operationType, const std::wstring & packageFilePath, std::wstring packageFullName, MSIX_VALIDATION_OPTION validationOption, MsixRequest** outInstance);

    /// The main function processes the request based on whichever operation type was requested and then
    /// going through the sequence of individual handlers.
    HRESULT ProcessRequest();

    /// Called by PopulatePackageInfo
    void SetPackageInfo(PackageInfo* packageInfo);

    /// Called by CreateAndShowUI 
    void SetUI(IInstallerUI* ui) {
        m_UI = ui;
    }

    // Getters
    MSIX_VALIDATION_OPTION GetValidationOptions() { return m_validationOptions; }
    PCWSTR GetPackageFilePath() { return m_packageFilePath.c_str(); }
    PCWSTR GetPackageFullName() { return m_packageFullName.c_str(); }

    /// @return can return null if called before PopulatePackageInfo.
    PackageInfo* GetPackageInfo() { return m_packageInfo; }
    IPackageInfo* GetIPackageInfo() { return (IPackageInfo*)GetPackageInfo(); }

    /// @return the UI to display. If NULL, the operations will be quiet.
    IInstallerUI * GetUI() { return m_UI; }

    bool IsRemove()
    {
        return m_operationType == OperationType::Remove;
    }

    bool AllowSignatureOriginUnknown()
    {
        m_validationOptions = static_cast<MSIX_VALIDATION_OPTION>(m_validationOptions | MSIX_VALIDATION_OPTION::MSIX_VALIDATION_OPTION_ALLOWSIGNATUREORIGINUNKNOWN);
        return true;
    }

private:

    /// This handles Add operation and proceeds through each of the AddSequenceHandlers to install the package
    HRESULT ProcessAddRequest();

    /// This handles Remove operation and proceeds through each of the RemoveSequenceHandlers to uninstall the package
    HRESULT ProcessRemoveRequest();
};
}