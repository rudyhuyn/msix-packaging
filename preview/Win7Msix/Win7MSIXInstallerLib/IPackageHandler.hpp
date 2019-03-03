#pragma once
#include "MsixRequestImpl.hpp"
namespace Win7MsixInstallerLib
{
	/// Interface for a logical chunk of work done on an Msix request
	class IPackageHandler
	{
	public:

		virtual HRESULT ExecuteForAddRequest() = 0;

		virtual HRESULT ExecuteForRemoveRequest() { return S_OK; }

		virtual ~IPackageHandler() {};
	};

	/// Function responsible for creating an instance of an IPackageHandler object 
	/// @param msixRequest - the msix request object to act upon
	/// @return S_OK if CreateHandler is to not fail the deployment of the package.
	typedef HRESULT(*CreateHandler)(_In_ MsixRequestImpl* msixRequest, _Out_ IPackageHandler** instance);

}