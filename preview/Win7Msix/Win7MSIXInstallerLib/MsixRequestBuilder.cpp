#include "MsixRequestBuilder.hpp"
#include "MsixRequestImpl.hpp"
using namespace Win7MsixInstallerLib;

HRESULT MsixRequestBuilder::CreateAddPackageRequest(std::wstring packageFilePath, MSIX_VALIDATION_OPTION validationOption, Win7MsixInstallerLib::IMsixRequest** outInstance)
{
	MsixRequestImpl *impl;
	RETURN_IF_FAILED(MsixRequestImpl::Make(OperationType::Add, packageFilePath, L"", validationOption, &impl));
	*outInstance = (IMsixRequest*)impl;
	return S_OK;
}

HRESULT MsixRequestBuilder::CreateRemovePackageRequest(std::wstring packageFullName, Win7MsixInstallerLib::IMsixRequest** outInstance)
{
	MsixRequestImpl *impl;
	RETURN_IF_FAILED(MsixRequestImpl::Make(OperationType::Remove, L"", packageFullName, MSIX_VALIDATION_OPTION::MSIX_VALIDATION_OPTION_FULL, &impl));
	*outInstance = (IMsixRequest*)impl;
	return S_OK;
}

HRESULT MsixRequestBuilder::CreateFindAllPackagesRequest(Win7MsixInstallerLib::IMsixRequest** outInstance)
{
	MsixRequestImpl *impl;
	RETURN_IF_FAILED(MsixRequestImpl::Make(OperationType::FindAllPackages, L"", L"", MSIX_VALIDATION_OPTION::MSIX_VALIDATION_OPTION_FULL, &impl));
	*outInstance = (IMsixRequest*)impl;
	return S_OK;
}

HRESULT MsixRequestBuilder::CreateFindPackageRequest(std::wstring packageFullName, Win7MsixInstallerLib::IMsixRequest** outInstance)
{
	MsixRequestImpl *impl;
	RETURN_IF_FAILED(MsixRequestImpl::Make(OperationType::FindPackage, L"", packageFullName, MSIX_VALIDATION_OPTION::MSIX_VALIDATION_OPTION_FULL, &impl));
	*outInstance = (IMsixRequest*)impl;
	return S_OK;
}
