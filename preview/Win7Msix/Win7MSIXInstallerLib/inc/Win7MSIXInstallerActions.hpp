#pragma once
#include "DllExport.hpp"
#include "IPackageManager.hpp"

extern "C" DLLEXPORT HRESULT Win7MsixInstallerLib_CreatePackageManager(Win7MsixInstallerLib::IPackageManager** packageManager);
