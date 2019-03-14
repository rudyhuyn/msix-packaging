#pragma once
namespace Win7MsixInstallerLib {

    enum InstallerUIType{
        InstallerUITypeAddPackage,
        InstallerUITypeRemovePackage,
    };
	class IInstallerUI
	{
	public:
		virtual void UpdateProgressBarStep(float value) = 0;
		virtual bool ShowUI(InstallerUIType isAddPackage) = 0;
        virtual bool InstallCompleted() = 0;
	};
}