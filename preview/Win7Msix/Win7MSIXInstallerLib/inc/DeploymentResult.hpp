#pragma once
#include <string>

enum InstallationStep {
    InstallationStepStarted,
    InstallationStepGetPackageInformation,
    InstallationStepGetExtraction,
    InstallationStepCompleted,
    InstallationStepError,
    //can be extend for future scenarios.
};


struct DeploymentResult
{
    unsigned int Progress;
    InstallationStep Status;
    std::wstring ErrorText;
    int ExtendedErrorCode;
};
