#pragma once
#include "MsixRequest.hpp"
#include "RequestInfo.hpp"
#include "Package.hpp"

namespace Win7MsixInstallerLib
{
/// Interface for a logical chunk of work done on an Msix request
class IPackageHandler
{
public:

    virtual HRESULT ExecuteForAddRequest(AddRequestInfo & requestInfo) = 0;

    virtual HRESULT ExecuteForRemoveRequest(RemoveRequestInfo & requestInfo) { return S_OK; }
    virtual bool IsMandatoryForRemoveRequest() { return false; }
    virtual bool IsMandatoryForAddRequest() { return true; }

    virtual ~IPackageHandler() {};
};

/// Function responsible for creating an instance of an IPackageHandler object 
/// @param msixRequest - the msix request object to act upon
/// @return S_OK if CreateHandler is to not fail the deployment of the package.
typedef HRESULT(*CreateHandler)(_In_ MsixRequest* msixRequest, _Out_ IPackageHandler** instance);

}