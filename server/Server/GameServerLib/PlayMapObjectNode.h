#pragma once
namespace OGSLib
{
	class PlayMapObject;
	struct PlayMapObjectNode
	{
		PlayMapObjectNode* Next;
		PlayMapObjectNode* Prev;
		PlayMapObject* This;
		PlayMapObjectNode()
		{
			Next = 0;
			Prev = 0;
			This = 0;
		}
		void leave(){
			if( Next && Prev )
			{
				Next->Prev=Prev;
				Prev->Next=Next;
				Next=0;
				Prev=0;
			}
			else
			{
				printf("map object node error leave\n");
			}
		}
		bool used(){
			if( Next != 0 || Prev != 0 )
			{
				return true;
			}
			return false;
		}
	};
	struct PlayMapObjectNodeList
	{
		PlayMapObjectNode Begin;
		PlayMapObjectNode End;
		PlayMapObjectNodeList()
		{
			Begin.Next = &End;
			End.Prev = &Begin;
		}
		PlayMapObjectNode* begin(){
			return Begin.Next;
		}
		PlayMapObjectNode* end(){
			return &End;
		}
		void push_back(PlayMapObjectNode* node){
			if( node->used() ){
				node->leave();
			}
			node->Next=&End;
			node->Prev=End.Prev;
			End.Prev->Next=node;
			End.Prev=node;
		}
		void push_front(PlayMapObjectNode* node){
			if( node->used() ){
				node->leave();
			}
			node->Next=Begin.Next;
			node->Prev=&Begin;
			Begin.Next->Prev=node;
			Begin.Next=node;
		}
		bool empty(){
			return Begin.Next==&End;
		}
	};
}