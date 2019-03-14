#include <windows.h>

#include "PopulatePackageInfo.hpp"
#include "GeneralUtil.hpp"
#include <TraceLoggingProvider.h>
#include <experimental/filesystem> // C++-standard header file name
#include "Constants.hpp"
using namespace Win7MsixInstallerLib;

const PCWSTR PopulatePackageInfo::HandlerName = L"PopulatePackageInfo";

HRESULT PopulatePackageInfo::GetPackageInfoFromPackage(PCWSTR packageFilePath, MSIX_VALIDATION_OPTION validationOption, std::wstring msix7DirectoryPath, PackageInfo ** packageInfo)
{
    ComPtr<IStream> inputStream;
    RETURN_IF_FAILED(CreateStreamOnFileUTF16(packageFilePath, /*forRead */ true, &inputStream));

    // On Win32 platforms CoCreateAppxFactory defaults to CoTaskMemAlloc/CoTaskMemFree
    // On non-Win32 platforms CoCreateAppxFactory will return 0x80070032 (e.g. HRESULT_FROM_WIN32(ERROR_NOT_SUPPORTED))
    // So on all platforms, it's always safe to call CoCreateAppxFactoryWithHeap, just be sure to bring your own heap!
    ComPtr<IAppxFactory> appxFactory;
    RETURN_IF_FAILED(CoCreateAppxFactoryWithHeap(Win7MsixInstallerLib_MyAllocate, Win7MsixInstallerLib_MyFree, validationOption, &appxFactory));

    // Create a new package reader using the factory.
    ComPtr<IAppxPackageReader> packageReader;
    RETURN_IF_FAILED(appxFactory->CreatePackageReader(inputStream.Get(), &packageReader));

    RETURN_IF_FAILED(PackageInfo::MakeFromPackageReader(packageReader.Get(), msix7DirectoryPath, packageInfo));

    return S_OK;
}

HRESULT PopulatePackageInfo::GetPackageInfoFromManifest(PCWSTR manifestPath, MSIX_VALIDATION_OPTION validationOption, PCWSTR msix7DirectoryPath, PackageInfo ** packageInfo)
{
    ComPtr<IStream> stream;
    RETURN_IF_FAILED(CreateStreamOnFileUTF16(manifestPath, true /*forRead*/, &stream));

    ComPtr<IAppxFactory> appxFactory;
    RETURN_IF_FAILED(CoCreateAppxFactoryWithHeap(Win7MsixInstallerLib_MyAllocate, Win7MsixInstallerLib_MyFree, validationOption, &appxFactory));

    ComPtr<IAppxManifestReader> manifestReader;
    RETURN_IF_FAILED(appxFactory->CreateManifestReader(stream.Get(), &manifestReader));

    RETURN_IF_FAILED(PackageInfo::MakeFromManifestReader(manifestReader.Get(), std::wstring(msix7DirectoryPath), packageInfo));

    return S_OK;
}

HRESULT PopulatePackageInfo::CreatePackageReader()
{
    ComPtr<IStream> inputStream;
    RETURN_IF_FAILED(CreateStreamOnFileUTF16(m_msixRequest->GetPackageFilePath(), /*forRead */ true, &inputStream));

    // On Win32 platforms CoCreateAppxFactory defaults to CoTaskMemAlloc/CoTaskMemFree
    // On non-Win32 platforms CoCreateAppxFactory will return 0x80070032 (e.g. HRESULT_FROM_WIN32(ERROR_NOT_SUPPORTED))
    // So on all platforms, it's always safe to call CoCreateAppxFactoryWithHeap, just be sure to bring your own heap!
    ComPtr<IAppxFactory> appxFactory;
    RETURN_IF_FAILED(CoCreateAppxFactoryWithHeap(Win7MsixInstallerLib_MyAllocate, Win7MsixInstallerLib_MyFree, m_msixRequest->GetValidationOptions(), &appxFactory));

    // Create a new package reader using the factory.
    ComPtr<IAppxPackageReader> packageReader;
    RETURN_IF_FAILED(appxFactory->CreatePackageReader(inputStream.Get(), &packageReader));

    AutoPtr<PackageInfo> packageInfo;
    RETURN_IF_FAILED(PackageInfo::MakeFromPackageReader(packageReader.Get(), FilePathMappings::GetInstance().GetMsix7Directory(), &packageInfo));
    m_msixRequest->SetPackageInfo(packageInfo.Detach());

    return S_OK;
}

HRESULT PopulatePackageInfo::ExecuteForAddRequest()
{
    RETURN_IF_FAILED(CreatePackageReader());

    TraceLoggingWrite(g_MsixTraceLoggingProvider,
        "PackageInfo",
        TraceLoggingValue(m_msixRequest->GetPackageInfo()->GetPackageFullName().c_str(), "PackageFullName"),
        TraceLoggingValue(m_msixRequest->GetPackageInfo()->GetNumberOfPayloadFiles(), "NumberOfPayloadFiles"),
        TraceLoggingValue(m_msixRequest->GetPackageInfo()->GetExecutableFilePath().c_str(), "ExecutableFilePath"),
        TraceLoggingValue(m_msixRequest->GetPackageInfo()->GetDisplayName().c_str(), "DisplayName"));

    return S_OK;
}

HRESULT PopulatePackageInfo::ExecuteForRemoveRequest()
{
    std::wstring packageDirectoryPath = FilePathMappings::GetInstance().GetMsix7Directory() + m_msixRequest->GetPackageFullName();
    std::experimental::filesystem::path directory = packageDirectoryPath;
    if (!std::experimental::filesystem::exists(directory))
    {
        TraceLoggingWrite(g_MsixTraceLoggingProvider,
            "Could not find package directory",
            TraceLoggingLevel(WINEVENT_LEVEL_ERROR),
            TraceLoggingValue(m_msixRequest->GetPackageFullName(), "PackageFullName"),
            TraceLoggingValue(directory.c_str(), "PackageDirectoryPath"));
        RETURN_IF_FAILED(E_NOT_SET);
    }

    std::wstring manifestPath = packageDirectoryPath + manifestFile;
    ComPtr<IStream> stream;
    RETURN_IF_FAILED(CreateStreamOnFileUTF16(manifestPath.c_str(), true /*forRead*/, &stream));

    ComPtr<IAppxFactory> appxFactory;
    RETURN_IF_FAILED(CoCreateAppxFactoryWithHeap(Win7MsixInstallerLib_MyAllocate, Win7MsixInstallerLib_MyFree, m_msixRequest->GetValidationOptions(), &appxFactory));

    ComPtr<IAppxManifestReader> manifestReader;
    RETURN_IF_FAILED(appxFactory->CreateManifestReader(stream.Get(), &manifestReader));

    AutoPtr<PackageInfo> packageInfo;
    RETURN_IF_FAILED(PackageInfo::MakeFromManifestReader(manifestReader.Get(), FilePathMappings::GetInstance().GetMsix7Directory(), &packageInfo));
    m_msixRequest->SetPackageInfo(packageInfo.Detach());

    return S_OK;
}

HRESULT PopulatePackageInfo::CreateHandler(MsixRequest * msixRequest, IPackageHandler ** instance)
{
    std::unique_ptr<PopulatePackageInfo> localInstance(new PopulatePackageInfo(msixRequest));
    if (localInstance == nullptr)
    {
        return E_OUTOFMEMORY;
    }
    *instance = localInstance.release();

    return S_OK;
}
