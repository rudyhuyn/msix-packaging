#include "Win7MSIXInstallerActions.hpp"
#include "MsixRequest.hpp"
#include <experimental/filesystem>
#include <shlobj_core.h>
#include <KnownFolders.h>
#include "PopulatePackageInfo.hpp"
#include "Constants.hpp"
#include "PackageManager.hpp"

using namespace Win7MsixInstallerLib;

HRESULT Win7MsixInstallerLib_CreatePackageManager(Win7MsixInstallerLib::IPackageManager** packageManager)
{
    *packageManager = new PackageManager();
    return S_OK;
}
