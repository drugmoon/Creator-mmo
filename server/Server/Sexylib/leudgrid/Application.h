#pragma once
namespace LEUD
{
	class SessionBase;
}
#ifdef NDEBUG
#define CHECK_DEBUG if( IsDebuggerPresent() ){exit(0);}
#else
#define CHECK_DEBUG 
#endif

namespace LEUD
{
	class Application
	{
	public:
		enum ExeCommandReturn
		{
			EXECOMMAND_RETURN_ERROR = 100,
			EXECOMMAND_RETURN_MORE = 200,
			EXECOMMAND_RETURN_NEXT = 300,
			EXECOMMAND_RETURN_FINISH = 400,
		};
	private:
		int								m_argc;
		char**							m_argv;
		HANDLE							m_ThreadHandle;
		HANDLE							m_StartEvent;
		HANDLE							m_StopEvent;
		HANDLE							m_WakeEvent;
		HANDLE							m_CommandEvent;
		HANDLE							m_CommandRetEvent;
		char							m_Command[256];
		char							m_CommandRetMsg[1024];
		DWORD							m_HeartBeat;
		//std::vector<HANDLE>				m_EventVector;
		HANDLE							m_Events[64];
		int								m_EventNum;
	public:
		Application();
		virtual ~Application();
	public:
		virtual void Start(int argc,char** argv);
		virtual void Stop();
		virtual int QueryStartState();
		virtual int QueryQuitState();
		virtual ExeCommandReturn ExeCommand(std::string cmd,std::string& result);
		virtual std::string Help(int argc, char** argv);
	public:
		bool AddEventHandle(HANDLE handle);
		bool RemoveEventHandle(HANDLE handle);
		void Wake();
		void HeartBeat(DWORD heartbeat);
	public:
		virtual int OnStart()=0;
		virtual int OnStop()=0;
		virtual void OnUpdate()=0;
		virtual std::string OnCommand(const char* command)=0;
	private:
		void RunCommand();
		int  Run();
		static unsigned int __stdcall MainThread( void* data );
	};
}