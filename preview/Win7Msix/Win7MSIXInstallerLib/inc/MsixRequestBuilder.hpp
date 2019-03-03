#pragma once
#include "IMsixRequest.hpp"
#include "OperationType.hpp"
#include <AppxPackaging.hpp>
namespace Win7MsixInstallerLib
{
	class MsixRequestBuilder
	{
	public:
		static HRESULT Create(Win7MsixInstallerLib::OperationType operationType, std::wstring packageFilePath, std::wstring packageFullName, MSIX_VALIDATION_OPTION validationOption, Win7MsixInstallerLib::IMsixRequest** outInstance);
	private:
		MsixRequestBuilder();
	};
}