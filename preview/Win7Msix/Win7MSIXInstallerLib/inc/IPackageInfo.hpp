#pragma once
#include <Windows.h>
namespace Win7MsixInstallerLib {

    class IPackageInfo {
    public:
        virtual unsigned long long GetVersion() = 0;
        virtual std::wstring GetPublisher() = 0;
        virtual std::wstring GetPublisherName() = 0;
        virtual std::wstring GetPackageFullName() = 0;
        virtual std::wstring GetDisplayName() = 0;
        virtual std::wstring GetAppModelUserId() = 0;
        virtual IStream* GetLogo() = 0;
    };

    class IInstalledPackageInfo : public IPackageInfo
    {
    public:
        virtual std::wstring GetFullExecutableFilePath() = 0;
        virtual std::wstring GetPackageDirectoryPath() = 0;
    };
}