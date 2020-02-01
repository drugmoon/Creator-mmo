#pragma once
namespace LEUD
{
	void ConsoleColorRed();
	void ConsoleColorBlue();
	void ConsoleColorGreen();
	void ConsoleColorIntensity();
	void ConsoleColorRedBackGreen();
	void ConsoleColorRedBackBlue();
	void ConsoleColorBlueBackRed();
	void ConsoleColorBlueBackGreen();
	void ConsoleColorGreenBackBlue();
	void ConsoleColorGreenBackRed();
	void ConsoleColorIntensityBackRed();
	void ConsoleColorIntensityBackGreen();
	void ConsoleColorIntensityBackBlue();
	void ConsolePlusOpen(const char* console_name);
	void ConsolePlusClose();
	class ConsolePlus
	{
	public:
		ConsolePlus(const char* console_name);
		~ConsolePlus();
	};
}