#include "StdAfx.h"
#include "TimerSecond.h"
namespace OGSLib
{
	SecondTimerNode::SecondTimerNode(time_t delay,SecondTimerObject* obj,int param)
	{
		mTime = time(0) + delay;
		mObject = obj;
		mParam = param;
	}
	TimerSecond::TimerSecond(void)
	{
		mQueue.reserve(100);
		mPoped.reserve(100);
		mResetOwnerQue.reserve(100);
		mResetOwnerPoped.reserve(100);
		mTick = 0;
	}
	TimerSecond::~TimerSecond(void)
	{
	}
	void TimerSecond::AddTimer(time_t delay,SecondTimerObject* obj,int param)
	{
		mQueue.push_back(SecondTimerNode(delay,obj,param));
		push_heap(mQueue.begin(),mQueue.end(),mComp);
	}
	void TimerSecond::AddTimer2(time_t delay, SecondTimerObject* obj, int param)
	{
		mResetOwnerQue.push_back(SecondTimerNode(delay, obj, param));
		push_heap(mResetOwnerQue.begin(), mResetOwnerQue.end(), mComp2);
	}
	class RemoveObjFuntor
	{
	public:
		SecondTimerObject* obj;
		bool operator() (const SecondTimerNode& p)
		{
			if( obj == p.mObject )
			{
				p.mObject->OnSecondCancelTimer(p.mParam);
				return true;
			}
			return false;
		}
	};
	void TimerSecond::RemoveObject(SecondTimerObject* obj)
	{
		RemoveObjFuntor fun;
		fun.obj = obj;
		mQueue.erase(std::remove_if(mQueue.begin(),mQueue.end(),fun),mQueue.end());
		std::make_heap(mQueue.begin(),mQueue.end(),mComp);
		mResetOwnerQue.erase(std::remove_if(mResetOwnerQue.begin(), mResetOwnerQue.end(), fun), mResetOwnerQue.end());
		std::make_heap(mResetOwnerQue.begin(), mResetOwnerQue.end(), mComp2);
	}
	class RemoveObjParamFuntor
	{
	public:
		SecondTimerObject* obj;
		int param;
		bool operator() (const SecondTimerNode& p)
		{
			if( obj == p.mObject && param == p.mParam )
			{
				p.mObject->OnSecondCancelTimer(p.mParam);
				return true;
			}
			return false;
		}
	};
	void TimerSecond::RemoveTimer(SecondTimerObject* obj,int param)
	{
		RemoveObjParamFuntor fun;
		fun.obj = obj;
		fun.param = param;
		mQueue.erase(std::remove_if(mQueue.begin(),mQueue.end(),fun),mQueue.end());
		std::make_heap(mQueue.begin(),mQueue.end(),mComp);
	}
	void TimerSecond::Update()
	{
		time_t t = time(0);
		if( mTick < t )
		{
			mTick = t;
			mPoped.clear();
			mResetOwnerPoped.clear();
			while(!mQueue.empty())
			{
				SecondTimerNode& stn = mQueue.front();
				if( stn.mTime <= t )
				{
					mPoped.push_back(stn);
					pop_heap(mQueue.begin(),mQueue.end(),mComp);
					mQueue.pop_back();
				}
				else
				{
					break;
				}
			}
			if( ! mPoped.empty() )
			{
				for(std::vector<SecondTimerNode>::iterator pos = mPoped.begin();
					pos != mPoped.end();
					++pos)
				{
					pos->mObject->OnSecondTimer(pos->mParam);
				}
			}
		}
		while (!mResetOwnerQue.empty())
		{
			SecondTimerNode& stn = mResetOwnerQue.front();
			if (stn.mTime <= t)
			{
				mResetOwnerPoped.push_back(stn);
				pop_heap(mResetOwnerQue.begin(), mResetOwnerQue.end(), mComp2);
				mResetOwnerQue.pop_back();
			}
			else
			{
				break;
			}
		}
		if (!mResetOwnerPoped.empty())
		{
			for (std::vector<SecondTimerNode>::iterator pos = mResetOwnerPoped.begin();
				pos != mResetOwnerPoped.end();
				++pos)
			{
				pos->mObject->OnRestItemOwner();
			}
		}
	}

}