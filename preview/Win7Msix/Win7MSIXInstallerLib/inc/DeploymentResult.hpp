#pragma once


enum InstallationStep {
    InstallationStepStarted,
    InstallationStepGetPackageInformation,
    InstallationStepGetExtraction,
    InstallationStepCompleted,
    //can be extend for future scenarios.
};


struct DeploymentResult
{
    unsigned int Progress;
    InstallationStep Status;
};
