#pragma once

#pragma once

#include "GeneralUtil.hpp"
#include "IPackageHandler.hpp"
namespace Win7MsixInstallerLib
{
	class PopulatePackageInfo : IPackageHandler
	{
	public:
		/// Opens the package to grab the package reader, manifest reader and read the info out of the manifest
		HRESULT ExecuteForAddRequest();

		/// Opens the manifest from the filesystem to create a manifest reader and read the info out of the manifest
		/// @return E_NOT_SET when the package cannot be found
		HRESULT ExecuteForRemoveRequest();

		static const PCWSTR HandlerName;
		static HRESULT CreateHandler(_In_ MsixRequestImpl* msixRequest, _Out_ IPackageHandler** instance);
		~PopulatePackageInfo() {}
	private:
		MsixRequestImpl* m_msixRequest = nullptr;

		PopulatePackageInfo() {}
		PopulatePackageInfo(_In_ MsixRequestImpl* msixRequest) : m_msixRequest(msixRequest) {}

		HRESULT CreatePackageReader();
	};
}