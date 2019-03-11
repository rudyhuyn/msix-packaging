#include "Win7MSIXInstallerActions.hpp"
#include "MsixRequestImpl.hpp"
#include <experimental/filesystem>
#include <shlobj_core.h>
#include <KnownFolders.h>
#include "PopulatePackageInfo.hpp"
#include "Constants.hpp"

using namespace Win7MsixInstallerLib;

HRESULT Win7MsixInstaller_CreateAddPackageRequest(std::wstring packageFilePath, MSIX_VALIDATION_OPTION validationOption, Win7MsixInstallerLib::IMsixRequest** outInstance)
{
	MsixRequestImpl *impl;
	RETURN_IF_FAILED(MsixRequestImpl::Make(OperationType::Add, packageFilePath, L"", validationOption, &impl));
	*outInstance = (IMsixRequest*)impl;
	return S_OK;
}

HRESULT Win7MsixInstaller_CreateRemovePackageRequest(std::wstring packageFullName, Win7MsixInstallerLib::IMsixRequest** outInstance)
{
	MsixRequestImpl *impl;
	RETURN_IF_FAILED(MsixRequestImpl::Make(OperationType::Remove, L"", packageFullName, MSIX_VALIDATION_OPTION::MSIX_VALIDATION_OPTION_FULL, &impl));
	*outInstance = (IMsixRequest*)impl;
	return S_OK;
}

HRESULT Win7MsixInstaller_GetPackageInfo(std::wstring packageFullName, IPackageInfo** outInstance)
{
	auto msix7Directory = FilePathMappings::GetInstallationDirectoryPath();
	std::wstring packageDirectoryPath = msix7Directory + packageFullName;
	std::experimental::filesystem::path directory = packageDirectoryPath;
	std::wstring manifestPath = packageDirectoryPath + manifestFile;

	PackageInfo* packageInfo;
	RETURN_IF_FAILED(PopulatePackageInfo::GetPackageInfoFromManifest(manifestPath.c_str(), MSIX_VALIDATION_OPTION::MSIX_VALIDATION_OPTION_FULL, msix7Directory, &packageInfo));
	*outInstance = (IPackageInfo*)packageInfo;
	return S_OK;
}

LPCWSTR Win7MsixInstaller_GetInstallationDirectoryPath()
{
	return FilePathMappings::GetInstallationDirectoryPath();
}
