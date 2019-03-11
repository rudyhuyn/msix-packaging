#pragma once
#include <Windows.h>
#include "AppxPackaging.hpp"
namespace Win7MsixInstallerLib {

	class IPackageInfo {
	public:
		virtual DWORD GetNumberOfPayloadFiles() = 0;
		virtual UINT64 GetVersion() = 0;
		virtual PCWSTR GetPublisher() = 0;
		virtual IAppxManifestReader * GetManifestReader() = 0;
		virtual IAppxPackageReader * GetPackageReader() = 0;
		virtual std::wstring GetPackageFullName() = 0;
		virtual std::wstring GetPackageDirectoryPath() = 0;
		virtual std::wstring GetExecutableFilePath() = 0;
		virtual std::wstring GetDisplayName() = 0;
		virtual std::wstring GetAppModelUserId() = 0;
	};
}