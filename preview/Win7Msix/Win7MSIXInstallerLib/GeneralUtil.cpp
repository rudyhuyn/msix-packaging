#include "Util.hpp"
#include <string>
#include <codecvt>
#include <iostream>
using namespace Win7MsixInstallerLib;

TRACELOGGING_DEFINE_PROVIDER(
	g_MsixTraceLoggingProvider,
	"MsixTraceLoggingProvider",
	(0x033321d3, 0xd599, 0x48e0, 0x86, 0x8d, 0xc5, 0x9f, 0x15, 0x90, 0x16, 0x37));

/// Converts a wstring from utf16 to utf8
///
/// @param utf16string - A utf16 wstring
/// @return utf8 string
std::string Win7MsixInstaller_utf16_to_utf8(const std::wstring& utf16string)
{
	auto converted = std::wstring_convert<std::codecvt_utf8<wchar_t>>{}.to_bytes(utf16string.data());
	std::string result(converted.begin(), converted.end());
	return result;
}

/// Helper to convert version number to a version string of the form a.b.c.d
///
/// @param version - version number
/// @return a.b.c.d string representation of version
std::wstring Win7MsixInstaller_ConvertVersionToString(UINT64 version)
{
    return std::to_wstring((version >> 0x30) & 0xFFFF) + L"."
        + std::to_wstring((version >> 0x20) & 0xFFFF) + L"."
        + std::to_wstring((version >> 0x10) & 0xFFFF) + L"."
        + std::to_wstring((version) & 0xFFFF);
}