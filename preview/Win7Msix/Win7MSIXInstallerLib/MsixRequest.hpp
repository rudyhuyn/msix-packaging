#pragma once
#include "PackageInfo.hpp"
#include "FilePaths.hpp"
#include "DeploymentResult.hpp"
#include <functional>
namespace Win7MsixInstallerLib
{

    enum OperationType
    {
        Add = 1,
        Remove = 2,
    };

    /// MsixRequest represents what this instance of the executable will be doing and tracks the state of the current operation
    class MsixRequest
    {
    private:
        /// Should always be available via constructor
        std::wstring m_packageFilePath;
        std::wstring m_packageFullName;
        MSIX_VALIDATION_OPTION m_validationOptions = MSIX_VALIDATION_OPTION::MSIX_VALIDATION_OPTION_FULL;
        OperationType m_operationType = Add;
        std::function<void(DeploymentResult)> m_callback;

    protected:
        MsixRequest() {}
    public:
        static HRESULT Make(OperationType operationType, const std::wstring & packageFilePath, std::wstring packageFullName, MSIX_VALIDATION_OPTION validationOption, MsixRequest** outInstance);

        /// The main function processes the request based on whichever operation type was requested and then
        /// going through the sequence of individual handlers.
        HRESULT ProcessRequest();

        MSIX_VALIDATION_OPTION GetValidationOptions() { return m_validationOptions; }

        bool IsRemove()
        {
            return m_operationType == OperationType::Remove;
        }

        bool AllowSignatureOriginUnknown()
        {
            m_validationOptions = static_cast<MSIX_VALIDATION_OPTION>(m_validationOptions | MSIX_VALIDATION_OPTION::MSIX_VALIDATION_OPTION_ALLOWSIGNATUREORIGINUNKNOWN);
            return true;
        }

        void SetCallback(std::function<void(const DeploymentResult &)> callback) {
            m_callback = callback;
        }

        void SendCallback(const DeploymentResult & result)
        {
            if (m_callback != nullptr)
            {
                m_callback(result);
            }
        }

    private:
        /// This handles Add operation and proceeds through each of the AddSequenceHandlers to install the package
        HRESULT ProcessAddRequest();

        /// This handles Remove operation and proceeds through each of the RemoveSequenceHandlers to uninstall the package
        HRESULT ProcessRemoveRequest();

    };
}