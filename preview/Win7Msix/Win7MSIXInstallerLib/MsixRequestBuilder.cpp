#include "MsixRequestBuilder.hpp"
#include "MsixRequestImpl.hpp"
using namespace Win7MsixInstallerLib;

HRESULT MsixRequestBuilder::Create(OperationType operationType, std::wstring packageFilePath, std::wstring packageFullName, MSIX_VALIDATION_OPTION validationOption, IMsixRequest** outInstance)
{
	MsixRequestImpl *impl;
	RETURN_IF_FAILED(MsixRequestImpl::Make(operationType, packageFilePath, packageFullName, validationOption, &impl));
	*outInstance = (IMsixRequest*)impl;
	return S_OK;
}
