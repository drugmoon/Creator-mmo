#pragma once
namespace LEUD
{
	template <class T>
	class LeudDeque
	{
	public:
		T* mFirst;
		T* mLast;
		size_t mNum;
		LeudDeque():mFirst(0),mLast(0),mNum(0){}
        ~LeudDeque()
        {
            while (!empty()) {
                delete front();
                pop_front();
            }
        }
		bool empty(){return mNum==0;}
		T* front(){return mFirst;}
		size_t size(){return mNum;}
		void push_back(T* t)
		{
			if( mLast )
			{
				mLast->m_Next = t;
				t->m_Next = 0;
				mLast = t;
			}
			else
			{
				mFirst = mLast = t;
				t->m_Next = 0;
			}
			mNum++;
		}
		void push_front(T* t)
		{
			if( mFirst )
			{
				t->m_Next = mFirst;
				mFirst = t;
			}
			else
			{
				mFirst = mLast = t;
				t->m_Next = 0;
			}
			mNum++;
		}
		void pop_front()
		{
			if( mFirst )
			{
				if( mFirst->m_Next )
				{
					mFirst = (T*)mFirst->m_Next;
				}
				else
				{
					mFirst = 0;
					mLast = 0;
				}
				mNum--;
			}
		}
	};
}