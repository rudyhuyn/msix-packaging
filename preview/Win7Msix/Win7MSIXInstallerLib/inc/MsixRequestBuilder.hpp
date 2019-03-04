#pragma once
#include "DllExport.hpp"
#include "IMsixRequest.hpp"
#include <AppxPackaging.hpp>
namespace Win7MsixInstallerLib
{
	class DLLEXPORT MsixRequestBuilder
	{
	public:
		static HRESULT CreateAddPackageRequest(std::wstring packageFilePath, MSIX_VALIDATION_OPTION validationOption, Win7MsixInstallerLib::IMsixRequest** outInstance);
		static HRESULT CreateRemovePackageRequest(std::wstring packageFullName, Win7MsixInstallerLib::IMsixRequest** outInstance);
		static HRESULT CreateFindPackageRequest(std::wstring packageFullName, Win7MsixInstallerLib::IMsixRequest** outInstance);
		static LPCWSTR GetInstallationDirectoryPath();
	private:
		MsixRequestBuilder();
	};
}