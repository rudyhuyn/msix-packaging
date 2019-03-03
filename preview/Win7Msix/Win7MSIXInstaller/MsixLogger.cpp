#include "MsixLogger.h"
#include <windows.h>
#include <winmeta.h>
#include <TraceLoggingProvider.h>
#include "Win7MSIXInstallerLogger.hpp"

MsixLogger::MsixLogger()
{
}


MsixLogger::~MsixLogger()
{
}

int MsixLogger::Log(const char* message, MsixLoggerLevel level)
{
	switch (level)
	{
	case LogWarning:
		break;
	case LogError:
		break;
	default:
		break;
	}
	_TlgWrite_imp(_TlgWrite, g_MsixUITraceLoggingProvider, message, (NULL, NULL), NULL);
	return 0;
}
