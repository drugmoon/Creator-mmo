#include "stdafx.h"
#include "leudgrid/ToolsConsole.h"

namespace LEUD
{
	void ConsoleColorRed()
	{
		HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hCon,FOREGROUND_RED|FOREGROUND_INTENSITY);
	}
	void ConsoleColorBlue()
	{
		HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hCon,FOREGROUND_BLUE|FOREGROUND_INTENSITY);
	}
	void ConsoleColorGreen()
	{
		HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hCon,FOREGROUND_GREEN|FOREGROUND_INTENSITY);
	}
	void ConsoleColorIntensity()
	{
		HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hCon,FOREGROUND_RED|FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_INTENSITY);
	}
	void ConsoleColorRedBackGreen()
	{
		HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hCon,FOREGROUND_RED|FOREGROUND_INTENSITY|BACKGROUND_GREEN);
	}
	void ConsoleColorRedBackBlue()
	{
		HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hCon,FOREGROUND_RED|FOREGROUND_INTENSITY|BACKGROUND_BLUE);
	}
	void ConsoleColorBlueBackRed()
	{
		HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hCon,FOREGROUND_BLUE|FOREGROUND_INTENSITY|BACKGROUND_RED);
	}
	void ConsoleColorBlueBackGreen()
	{
		HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hCon,FOREGROUND_BLUE|FOREGROUND_INTENSITY|BACKGROUND_GREEN);
	}
	void ConsoleColorGreenBackBlue()
	{
		HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hCon,FOREGROUND_GREEN|FOREGROUND_INTENSITY|BACKGROUND_BLUE);
	}
	void ConsoleColorGreenBackRed()
	{
		HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hCon,FOREGROUND_GREEN|FOREGROUND_INTENSITY|BACKGROUND_RED);
	}
	void ConsoleColorIntensityBackRed()
	{
		HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hCon,FOREGROUND_RED|FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_INTENSITY|BACKGROUND_RED);
	}
	void ConsoleColorIntensityBackGreen()
	{
		HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hCon,FOREGROUND_RED|FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_INTENSITY|BACKGROUND_GREEN);
	}
	void ConsoleColorIntensityBackBlue()
	{
		HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hCon,FOREGROUND_RED|FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_INTENSITY|BACKGROUND_BLUE);
	}
	void ConsolePlusOpen(const char* console_name)
	{
		AllocConsole();
		FILE* file;
		freopen_s(&file,"CONIN$","rt",stdin);
		freopen_s(&file,"CONOUT$","wt",stdout);
		freopen_s(&file,"CONOUT$","wt",stderr);
		SetConsoleTitle(console_name);
	}
	void ConsolePlusClose()
	{
		FreeConsole();
	}
	ConsolePlus::ConsolePlus(const char* console_name)
	{
		ConsolePlusOpen(console_name);
	}
	ConsolePlus::~ConsolePlus()
	{
		ConsolePlusClose();
	}
}