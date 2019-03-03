#pragma once

#include "GeneralUtil.hpp"
#include "IPackageHandler.hpp"

namespace Win7MsixInstallerLib
{
	class Protocol : IPackageHandler
	{
	public:
		HRESULT ExecuteForAddRequest();

		static const PCWSTR HandlerName;
		static HRESULT CreateHandler(_In_ MsixRequestImpl* msixRequest, _Out_ IPackageHandler** instance);
		~Protocol() {}
	private:
		MsixRequestImpl* m_msixRequest = nullptr;

		Protocol() {}
		Protocol(_In_ MsixRequestImpl* msixRequest) : m_msixRequest(msixRequest) {}

	};
}