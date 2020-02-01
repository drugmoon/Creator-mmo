#pragma once

namespace OGSLib
{
	enum TASK_STATE
	{
		TASK_STATE_NULL = 0,
		TASK_STATE_ACCEPTED = 3,
		TASK_STATE_DONE = 4,
		TASK_STATE_COMPLETED = 8,
	};
	class SubTask
	{
	public:
		int TaskID;
		std::string TaskName;
		std::string TaskDesp;
		std::string TaskShortDesp;
		int TaskState;
		short TaskParam1;
		short TaskParam2;
		short TaskParam3;
		int TaskParam4;
		SubTask(void);
		virtual ~SubTask(void);
	};
}