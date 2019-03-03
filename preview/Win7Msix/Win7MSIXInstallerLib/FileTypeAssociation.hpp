#pragma once

#include "GeneralUtil.hpp"
#include "IPackageHandler.hpp"
namespace Win7MsixInstallerLib
{
	class FileTypeAssociation : IPackageHandler
	{
	public:
		/// Adds the file type associations to the registry so this application can handle specific file types.
		HRESULT ExecuteForAddRequest();

		static const PCWSTR HandlerName;
		static HRESULT CreateHandler(_In_ MsixRequestImpl* msixRequest, _Out_ IPackageHandler** instance);
		~FileTypeAssociation() {}
	private:
		MsixRequestImpl* m_msixRequest = nullptr;

		FileTypeAssociation() {}
		FileTypeAssociation(_In_ MsixRequestImpl* msixRequest) : m_msixRequest(msixRequest) {}

		HRESULT AddFta(PCWSTR name, PCWSTR parameters);
	};
}