#pragma once

#pragma once

#include "GeneralUtil.hpp"
#include "IPackageHandler.hpp"

class PopulatePackageInfo : IPackageHandler
{
public:
    /// Opens the package to grab the package reader, manifest reader and read the info out of the manifest
    HRESULT ExecuteForAddRequest();
    
    /// Opens the manifest from the filesystem to create a manifest reader and read the info out of the manifest
    /// @return E_NOT_SET when the package cannot be found
    HRESULT ExecuteForRemoveRequest();

    static const PCWSTR HandlerName;
    static HRESULT CreateHandler(_In_ MsixRequest* msixRequest, _Out_ IPackageHandler** instance);
    ~PopulatePackageInfo() {}
private:
    MsixRequest* m_msixRequest = nullptr;

    PopulatePackageInfo() {}
    PopulatePackageInfo(_In_ MsixRequest* msixRequest) : m_msixRequest(msixRequest) {}
   
    HRESULT CreatePackageReader();
};
