#pragma once
#include "Package.hpp"
#include "MsixRequest.hpp"

namespace Win7MsixInstallerLib
{
/// Interface for a logical chunk of work done on an Msix request
class IPackageHandler
{
public:

    virtual HRESULT ExecuteForAddRequest() = 0;

    virtual HRESULT ExecuteForRemoveRequest() { return S_OK; }
    virtual bool IsMandatoryForRemoveRequest() { return false; }
    virtual bool IsMandatoryForAddRequest() { return true; }

    virtual ~IPackageHandler() {};
};

/// Function responsible for creating an instance of an IPackageHandler object 
/// @return S_OK    of the package.
typedef HRESULT(*CreateHandler)(_In_ MsixRequest* msixRequest, _Out_ IPackageHandler** instance);
}