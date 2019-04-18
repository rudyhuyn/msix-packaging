#pragma once
#include "GeneralUtil.hpp"
#include "AppxPackaging.hpp"
#include "MSIXWindows.hpp"
#include "IPackage.hpp"

namespace Win7MsixInstallerLib
{

    class PackageBase
    {
    protected:
        std::wstring m_packageFullName;
        std::wstring m_packageFamilyName;
        std::wstring m_relativeExecutableFilePath;
        std::wstring m_displayName;
        std::wstring m_appUserModelId;
        std::wstring m_applicationId;
        unsigned long long m_version = 0;
        std::wstring m_publisher;
        std::wstring m_publisherName;
        std::wstring m_relativeLogoPath;

        ComPtr<IAppxManifestReader> m_manifestReader;
        unsigned int m_numberOfPayloadFiles = 0;

        /// Sets the executable path by reading it from the manifest element
        HRESULT SetExecutableAndAppIdFromManifestElement(IMsixElement * element);

        /// Sets the display name by reading it from the manifest element
        HRESULT SetDisplayNameFromManifestElement(IMsixElement * element);
    public:
        std::wstring GetPackageFullName() { return m_packageFullName; }
        std::wstring GetPackageFamilyName() { return m_packageFamilyName; }
        std::wstring GetRelativeExecutableFilePath() { return m_relativeExecutableFilePath; }
        std::wstring GetDisplayName() { return m_displayName; }
        std::wstring GetId() { return m_appUserModelId; }
        unsigned long long GetVersionNumber() { return m_version; }
        std::wstring GetVersion();
        std::wstring GetPublisher() { return m_publisher; }
        std::wstring GetPublisherDisplayName() { return m_publisherName; }

    protected:
        PackageBase() {}
        /// Sets the manifest reader, and other fields derived from the manifest
        /// Specifically, packageFullName, packageDirectoryPath, executableFilePath, displayname, version and publisher.
        ///
        /// @param manifestReader - manifestReader to set
        HRESULT SetManifestReader(IAppxManifestReader* manifestReader);
    public:
        virtual ~PackageBase()
        {
            ReleaseManifest();
        }
        unsigned int GetNumberOfPayloadFiles() { return m_numberOfPayloadFiles; }

        // Getters
        IAppxManifestReader * GetManifestReader() { return m_manifestReader.Get(); }
        /// This is meant only to be called when deleting the manifest file; the reader needs to first be released so it can be deleted
        void ReleaseManifest() {
            m_manifestReader.Release();
        }
    };


    class Package : public PackageBase, IPackage
    {
    public:
        std::wstring GetPackageFullName() { return m_packageFullName; }
        std::wstring GetPackageFamilyName() { return m_packageFamilyName; }
        std::wstring GetRelativeExecutableFilePath() { return m_relativeExecutableFilePath; }
        std::wstring GetDisplayName() { return m_displayName; }
        std::wstring GetId() { return m_appUserModelId; }
        unsigned long long GetVersionNumber() { return m_version; }
        std::wstring GetVersion() { return PackageBase::GetVersion(); }
        std::wstring GetPublisher() { return m_publisher; }
        std::wstring GetPublisherDisplayName() { return m_publisherName; }
        IStream* GetLogo();
        IAppxPackageReader * GetPackageReader() { return m_packageReader.Get(); }

        /// Create a Package using the package reader. This is intended for Add scenarios where
        /// the actual .msix package file is given.
        static HRESULT MakeFromPackageReader(IAppxPackageReader* packageReader, Package** packageInfo);

        virtual ~Package()
        {
            m_packageReader.Release();
        }
    private:
        Package() :PackageBase() {}

        /// PackageReader and payloadFiles are available on Add, but not Remove because it's created off the original package itself which is no longer available once it's been installed.
        ComPtr<IAppxPackageReader> m_packageReader;
    };

    class InstalledPackage : public PackageBase, IInstalledPackageInfo
    {
    public:
        std::wstring GetPackageFullName() { return m_packageFullName; }
        std::wstring GetPackageFamilyName() { return m_packageFamilyName; }
        std::wstring GetRelativeExecutableFilePath() { return m_relativeExecutableFilePath; }
        std::wstring GetDisplayName() { return m_displayName; }
        std::wstring GetId() { return m_appUserModelId; }
        unsigned long long GetVersionNumber() { return m_version; }
        std::wstring GetVersion() { return PackageBase::GetVersion(); }
        std::wstring GetPublisher() { return m_publisher; }
        std::wstring GetPublisherDisplayName() { return m_publisherName; }
        IStream* GetLogo();

        virtual std::wstring GetFullExecutableFilePath()
        {
            return m_packageDirectoryPath + m_relativeExecutableFilePath;
        }
        virtual std::wstring GetInstalledLocation()
        {
            return m_packageDirectoryPath;
        }

        /// Create a InstalledPackage using the manifest reader and directory path. This is intended for Remove scenarios where
        /// the actual .msix package file is no longer accessible.
        static HRESULT MakeFromManifestReader(const std::wstring & directoryPath, IAppxManifestReader* manifestReader, InstalledPackage** packageInfo);
    private:
        InstalledPackage() :PackageBase() {}

        std::wstring m_packageDirectoryPath;
    };
}