#pragma once
// Install UI Header
// UI Functions
#include <windows.h>
#include <string>
#include "GeneralUtil.hpp"
#include "IPackageHandler.hpp"
#include "IUI.hpp"


class CreateAndShowUI : IPackageHandler
{
public:
    HRESULT ExecuteForAddRequest();

    static const PCWSTR HandlerName;
    static HRESULT CreateHandler(_In_ MsixRequest* msixRequest, _Out_ IPackageHandler** instance);
    ~CreateAndShowUI() {}
private:
    MsixRequest* m_msixRequest = nullptr;

    CreateAndShowUI() {}
    CreateAndShowUI(_In_ MsixRequest* msixRequest) : m_msixRequest(msixRequest) {}

};