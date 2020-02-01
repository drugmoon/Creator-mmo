#pragma once
namespace OGSLib
{
	class SecondTimerObject
	{
	public:
		virtual ~SecondTimerObject(){}
		virtual void OnSecondTimer(int param){}
		virtual void OnSecondCancelTimer(int param){}
		virtual void OnRestItemOwner(){}
	};
	class SecondTimerNode
	{
	public:
		SecondTimerNode(time_t delay,SecondTimerObject* obj,int param);
		time_t mTime;
		SecondTimerObject* mObject;
		int mParam;
	};
	class SecondTimerComp
	{
	public:
		bool operator() (const SecondTimerNode& p1,const SecondTimerNode& p2) const
		{
			return p1.mTime>p2.mTime;
		}
	};
	class TimerSecond
	{
		std::vector<SecondTimerNode> mQueue;
		std::vector<SecondTimerNode> mPoped;
		std::vector<SecondTimerNode> mResetOwnerQue;////1min后重设物品owner的队列
		std::vector<SecondTimerNode> mResetOwnerPoped;////1min后重设物品owner的队列
		SecondTimerComp mComp;
		SecondTimerComp mComp2;
		time_t mTick;
	public:
		TimerSecond(void);
		virtual ~TimerSecond(void);
	public:
		void AddTimer(time_t delay,SecondTimerObject* obj,int param);
		void AddTimer2(time_t delay, SecondTimerObject* obj, int param);
		void RemoveObject(SecondTimerObject* obj);
		void RemoveTimer(SecondTimerObject* obj,int param);
		void Update();
	};
}