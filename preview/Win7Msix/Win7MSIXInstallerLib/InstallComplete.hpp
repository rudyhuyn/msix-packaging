#pragma once

#include "GeneralUtil.hpp"
#include "IPackageHandler.hpp"

namespace Win7MsixInstallerLib
{

class InstallComplete : IPackageHandler
{
public:
	HRESULT ExecuteForAddRequest(AddRequestInfo &requestInfo);

	static const PCWSTR HandlerName;
	static HRESULT CreateHandler(_Out_ IPackageHandler** instance);
	~InstallComplete() {}
private:

	InstallComplete() {}
};
}