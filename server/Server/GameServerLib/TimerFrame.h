#pragma once
#include "TimerFix.h"
namespace OGSLib
{
	class FrameTimerObject
	{
	public:
		virtual ~FrameTimerObject(void){}
		virtual void OnFrameTimer(int param){};
		virtual void OnFrameCancelTimer(int param){};
	};
	class FrameTimerNode
	{
	public:
		FrameTimerObject* mObject;
		int	mParam;
	};
	class GameServerLib;
	class TimerFrame
	{
		typedef std::vector<FrameTimerNode> NodeVector;
		NodeVector** mNodes;
		NodeVector** mNodesBack;
		typedef std::map<int,NodeVector> BigNodeMap;
		BigNodeMap* mBigNodeList;
		BigNodeMap  mBigNodeListA;
		BigNodeMap  mBigNodeListB;
		int mCurrentFrame;
		int mUsedFrame;
		int mFrameCount;
		TimerFix mTimer;
		double mLastLeft;
		double mUpdateTime;
		double mUpdateDuration;
	public:
		static float FRAME_TICK;

		TimerFrame(int FrameCount,GameServerLib* gsl);
		virtual ~TimerFrame(void);
	public:
		void AddTimer(int frame,FrameTimerObject* obj,int param);
		void RemoveObject(FrameTimerObject* obj);
		void RemoveTimer(FrameTimerObject* obj,int param);
		void Update(double time);
	private:
		void TimerFrame::UpdateFrame();
	};
}