#pragma once

namespace OGSLib
{
	class ActionInstanceDataAttack
	{
	public:
		int targetx;
		int targety;
		int target_id;
	};
	class ActionInstanceDataAttackCount:public ActionInstanceDataAttack
	{
	public:
		int targetx;
		int targety;
		int target_id;
		int count;
	};
	union ActionInstanceData
	{
		ActionInstanceDataAttack attack;
		ActionInstanceDataAttackCount attackCount;
	};
	class SubAction
	{
	public:
		int mType;
		time_t mTime;
		ActionInstanceData mData;
		SubAction(void);
		virtual ~SubAction(void);
	};
}