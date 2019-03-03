#pragma once

#include<IMsixLogger.hpp>

class MsixLogger:IMsixLogger
{
public:
	MsixLogger();
	~MsixLogger();
	int Log(const char* message, MsixLoggerLevel level);

};

