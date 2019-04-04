#pragma once
#include <string>

enum InstallationStep {
    InstallationStepUnknown,
    InstallationStepStarted,
    InstallationStepGetPackageInformation,
    InstallationStepExtraction,
    InstallationStepCompleted,
    InstallationStepError,
    //can be extend for future scenarios.
};


struct DeploymentResult
{
    unsigned int Progress = 0;
    InstallationStep Status = InstallationStepUnknown;
    std::wstring ErrorText;
    int ExtendedErrorCode = 0;
};
