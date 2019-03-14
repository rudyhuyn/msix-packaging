#pragma once
namespace Win7MsixInstallerLib
{
    enum InstallationStep {
        InstallationStepStarted,
        InstallationStepPackageInformationAvailable,
        InstallationStepWaitForUserConfirmation,
        InstallationStepCompleted,
        //can be extend for future scenarios.
    };
    class IInstallerUI
    {
    public:
        // Update the progress bar, value from 0 to 100.
        virtual void UpdateProgressBarValue(float value) = 0;
        // Inform the UI about the current installation step
        virtual bool InstallationStepChanged(InstallationStep value) = 0;
    };
}