#pragma once
#include "DllExport.hpp"
#include <Windows.h>
#include <string>
#include "IInstallerUI.hpp"
#include "IPackageInfo.hpp"
namespace Win7MsixInstallerLib {

	class DLLEXPORT IMsixRequest
	{
	public:
		virtual void SetUI(IInstallerUI* ui) = 0;
		virtual IPackageInfo* GetIPackageInfo() = 0;

		/// The main function processes the request based on whichever operation type was requested and then
		/// going through the sequence of individual handlers.
		virtual HRESULT ProcessRequest() = 0;
	};
}
