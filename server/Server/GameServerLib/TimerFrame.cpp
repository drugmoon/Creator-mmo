#include "StdAfx.h"
#include "TimerFrame.h"
#include "GameServerLib.h"
namespace OGSLib
{
	float TimerFrame::FRAME_TICK = 1/30.0f;

	TimerFrame::TimerFrame(int FrameCount,GameServerLib* gsl)
	{
		mFrameCount = FrameCount;
		mNodes = new NodeVector*[mFrameCount];
		mNodesBack = new NodeVector*[mFrameCount];
		for( int i = 0;i< mFrameCount;i++ )
		{
			mNodes[i] = new NodeVector;
		}
		mBigNodeList = &mBigNodeListA;
		mCurrentFrame=0;
		mUsedFrame = 0;
		mTimer.Restart();
		mLastLeft = 0;
		mUpdateTime=0;
		mUpdateDuration=TimerFrame::FRAME_TICK;
	}
	TimerFrame::~TimerFrame(void)
	{
		for( int i = 0;i< mFrameCount;i++ )
		{
			delete mNodes[i];
		}
		delete[] mNodes;
		delete[] mNodesBack;
		mFrameCount = 0;
		mCurrentFrame= 0;
	}
	void TimerFrame::AddTimer(int frame,FrameTimerObject* obj,int param)
	{
		int leftFrame = mFrameCount - mCurrentFrame;
		FrameTimerNode node;
		node.mObject = obj;
		node.mParam = param;
		if( leftFrame <= frame )
		{
			int insertFrame = frame - leftFrame;
			(*mBigNodeList)[insertFrame].push_back(node);
		}
		else
		{
			int insertFrame = mCurrentFrame + frame;
			mNodes[insertFrame]->push_back(node);
			if( insertFrame > mUsedFrame )
			{
                mUsedFrame = insertFrame;
			}
		}
	}
	void TimerFrame::RemoveObject(FrameTimerObject* obj)
	{
		for(int i = mCurrentFrame; i<=mUsedFrame; i++ )
		{
			NodeVector* nv = mNodes[i];
			for( NodeVector::iterator pos = nv->begin();
				pos != nv->end();
				++ pos )
			{
				if( obj == pos->mObject )
				{
					pos->mObject->OnFrameCancelTimer(pos->mParam);
					pos->mObject = 0;
					pos->mParam = 0;
				}
			}
		}
		for( BigNodeMap::iterator pos = (*mBigNodeList).begin();
			pos != (*mBigNodeList).end();
			++ pos )
		{
			NodeVector* nv = &pos->second;
			for( NodeVector::iterator pos = nv->begin();
				pos != nv->end();
				++ pos )
			{
				if( obj == pos->mObject )
				{
					pos->mObject->OnFrameCancelTimer(pos->mParam);
					pos->mObject = 0;
					pos->mParam = 0;
				}
			}
		}
	}
	void TimerFrame::RemoveTimer(FrameTimerObject* obj,int param)
	{
		for(int i = mCurrentFrame; i<=mUsedFrame; i++ )
		{
			NodeVector* nv = mNodes[i];
			for( NodeVector::iterator pos = nv->begin();
				pos != nv->end();
				++ pos )
			{
				if( obj == pos->mObject && pos->mParam == param )
				{
					pos->mObject->OnFrameCancelTimer(pos->mParam);
					pos->mObject = 0;
					pos->mParam = 0;
				}
			}
		}
		for( BigNodeMap::iterator pos = (*mBigNodeList).begin();
			pos != (*mBigNodeList).end();
			++ pos )
		{
			NodeVector* nv = &pos->second;
			for( NodeVector::iterator pos = nv->begin();
				pos != nv->end();
				++ pos )
			{
				if( obj == pos->mObject && pos->mParam == param )
				{
					pos->mObject->OnFrameCancelTimer(pos->mParam);
					pos->mObject = 0;
					pos->mParam = 0;
				}
			}
		}
	}
	void TimerFrame::Update(double time)
	{
		if( mUpdateTime + mUpdateDuration < time )
		{
			mUpdateTime = time;
			double elapsed = mTimer.Elapsed(true);
			mLastLeft += elapsed;
			while( mLastLeft > TimerFrame::FRAME_TICK )
			{
				UpdateFrame();
				mLastLeft -= TimerFrame::FRAME_TICK;
			}
		}
	}
	void TimerFrame::UpdateFrame()
	{
		NodeVector* nv = mNodes[mCurrentFrame];
		for( NodeVector::iterator pos = nv->begin();
			pos != nv->end();
			++ pos )
		{
			FrameTimerObject* obj = pos->mObject;
			if( obj )
			{
				obj->OnFrameTimer(pos->mParam);
			}
		}
		nv->clear();
		mCurrentFrame ++;
		if( mCurrentFrame > mFrameCount / 2 )
		{
			memcpy(mNodesBack,mNodes,sizeof(NodeVector*) * mCurrentFrame);
			memcpy(&mNodes[0],&mNodes[mCurrentFrame],sizeof(NodeVector*) * (mFrameCount - mCurrentFrame));
			memcpy(&mNodes[mFrameCount - mCurrentFrame],&mNodesBack[0],sizeof(NodeVector*) * mCurrentFrame);
			mUsedFrame = mUsedFrame-mCurrentFrame;
			int leftframe = mFrameCount-mCurrentFrame;
			mCurrentFrame = 0;

			BigNodeMap* old = mBigNodeList;
			if( mBigNodeList == &mBigNodeListA)
			{
				mBigNodeList = &mBigNodeListB;
			}else
			{
				mBigNodeList = &mBigNodeListA;
			}
			for( BigNodeMap::iterator pos = (*old).begin();
				pos != (*old).end();
				++ pos )
			{
				for( NodeVector::iterator p = pos->second.begin();
					p != pos->second.end();
					++ p )
				{
					AddTimer(pos->first+leftframe,p->mObject,p->mParam);
				}
				pos->second.clear();
			}
		}
	}
}