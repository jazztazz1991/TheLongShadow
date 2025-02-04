// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <string>
#include <Windows.h>
#include <fstream>
#include <chrono>
#include <iostream>

#define LOGPREFACE "\\TLSLog_"

enum ERRORLEVEL
{
	EL_DEBUG = 0,
	EL_WARNING = 1,
	EL_ERROR = 2,
	EL_CRITICAL = 3
};

class Logger
{
private:
	static Logger *pInstance;

	std::string Path()
	{
		std::string Ret;
		char ExecPath[FILENAME_MAX];
		GetCurrentDirectoryA(sizeof(ExecPath), ExecPath);
		Ret = ExecPath;
		Ret += LOGPREFACE;
		Ret += TodaysDateAsString();
		Ret += ".txt";
		return Ret;
	};

#pragma warning(disable : 4996)

	std::string TodaysDateAsString()
	{
		std::string Ret;
		time_t Now;
		time(&Now);

		struct tm *pTime;
		pTime = localtime(&Now);
		Ret += int2string(pTime->tm_year + 1900);
		Ret += "-";
		Ret += int2string(pTime->tm_mon + 1);
		Ret += "-";
		Ret += int2string(pTime->tm_mday);
		return Ret;
	};

	std::string GetCurrentTimeStamp()
	{
		std::string Ret;
		time_t Now;
		time(&Now);
		struct tm *pTime;
		pTime = std::localtime(&Now);

		Ret += int2string(pTime->tm_hour);
		Ret += ":";
		Ret += int2string(pTime->tm_min);
		Ret += ":";
		Ret += int2string(pTime->tm_sec);
		return Ret;
	};

#pragma warning(default : 4996)

	std::string int2string(const int &i, bool padded = true)
	{
		std::ostringstream format_message;
		format_message << i;
		std::string Ret = format_message.str();
		if (padded && Ret.length() == 1)
		{
			Ret = "0" + Ret;
		}
		return Ret;
	};

	std::string ErrorLevelAsString(ERRORLEVEL level)
	{
		switch (level)
		{
		case ERRORLEVEL::EL_DEBUG:
			return "[DEBUGGING] ";
		case ERRORLEVEL::EL_WARNING:
			return "[WARNING] ";
		case ERRORLEVEL::EL_ERROR:
			return "[ERROR] ";
		case ERRORLEVEL::EL_CRITICAL:
			return "[!!!! CRITICAL ERROR !!!!] ";
		default:
			return "UNKNOWN ERROR LEVEL";
		}
	};

public:
	static Logger *GetInstance()
	{
		if (pInstance == nullptr)
		{
			pInstance = new Logger();
		}
		return pInstance;
	};

	void AddMessage(std::string Msg, ERRORLEVEL level)
	{
		std::string LogLine;
		LogLine += TodaysDateAsString();
		LogLine += " ";
		LogLine += GetCurrentTimeStamp();
		LogLine += ": ";
		LogLine += ErrorLevelAsString(level);
		LogLine += Msg;
		LogLine += "\n";
		std::ofstream fout;
		std::string path = Path();
		fout.open(path, std::ios_base::app);
		fout << LogLine;
		fout.close();
	};
};