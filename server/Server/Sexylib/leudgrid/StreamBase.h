#pragma once

namespace LEUD
{
	namespace Stream
	{
		static const unsigned int netorder = 0x1;
		static const unsigned int supportstruct = 0x2;
		static const unsigned int stringheader = 0x4;
		static const unsigned int stringintheader = 0x8;
	}

	template<class EX>
	class StreamBase
	{
	public:
		enum StreamState
		{
			StateMask = 0x7
		};
		static const StreamState goodbit = (StreamState)0x0;
		static const StreamState eofbit = (StreamState)0x1;
		static const StreamState failbit = (StreamState)0x2;
		static const StreamState badbit = (StreamState)0x4;
	private:
		StreamState		m_State;
		unsigned int	m_Flag;
		volatile long	m_RefCount;
	public:
		StreamBase():m_State(goodbit),m_Flag(0),m_RefCount(1)
		{
		}
	protected:
		StreamState setState( StreamState set, StreamState unset = goodbit )
		{
			m_State = (StreamState)(m_State | set);
			m_State = (StreamState)(m_State & ~unset);
			return m_State;
		}
		void clearbit()
		{
			m_State = goodbit;
		}
	public:
		unsigned int SetFlag(unsigned int flag)
		{
			m_Flag |= flag;
			return m_Flag;
		}
		unsigned int GetFlag()const
		{
			return m_Flag;
		}
		bool IsSetFlag(unsigned int flag)const
		{
			return 0!=(m_Flag & flag);
		}
		unsigned int ClearFlag(unsigned int flag)
		{
			m_Flag &= ~flag;
			return m_Flag;
		}
		unsigned int ResetFlag()
		{
			m_Flag = 0;
			return m_Flag;
		}
		unsigned int SetFailFlag()
		{
			return setState(failbit,goodbit);
		}
	public:
		EX& operator<<(EX& (*_Pfn)(EX&))
		{
			return ((*_Pfn)(*(EX*)this));
		}
		EX& operator>>(EX& (*_Pfn)(EX&))
		{
			return ((*_Pfn)(*(EX*)this));
		}
	public:
		template<class T> EX& operator<<(const T& t){
			if( IsSetFlag(Stream::supportstruct) ){return ((EX*)this)->In( (const char*)&t,sizeof(t) );}
			else{setState(failbit,goodbit);}
			return *(EX*)this;
		}
		template<class T> EX& operator>>(T& t){
			if( IsSetFlag(Stream::supportstruct) ){return ((EX*)this)->Out((char*)&t,sizeof(t));}
			else{setState(failbit,goodbit);}
			return *(EX*)this;
		}
	public:
		EX& operator<<(const long long& i){
			return ((EX*)this)->In( (const char*)&i,sizeof(long long) );
		}
		EX& operator<<(const int& i){
			if( IsSetFlag(Stream::netorder) ) {int temp=htonl(i);return ((EX*)this)->In( (const char*)&temp,sizeof(int) );}
			else{return ((EX*)this)->In( (const char*)&i,sizeof(int) );}
		}
		EX& operator<<(const short& s){
			if( IsSetFlag(Stream::netorder) ) {short temp=htons(s);return ((EX*)this)->In( (const char*)&temp,sizeof(short) );}
			else{return ((EX*)this)->In( (const char*)&s,sizeof(short) );}
		}
		EX& operator<<(const char& c){
			return ((EX*)this)->In( (const char*)&c,sizeof(char) );
		}
		EX& operator<<(const unsigned int& i){
			if( IsSetFlag(Stream::netorder) ) {int temp=htonl(*(int*)(void*)&i);return ((EX*)this)->In( (const char*)&temp,sizeof(int) );}
			else{return ((EX*)this)->In( (const char*)&i,sizeof(int) );}
		}
		EX& operator<<(const unsigned short& s){
			if( IsSetFlag(Stream::netorder) ) {short temp=htons(*(short*)(void*)&s);return ((EX*)this)->In( (const char*)&temp,sizeof(short) );}
			else{return ((EX*)this)->In( (const char*)&s,sizeof(short) );}
		}
		EX& operator<<(const unsigned char& c){
			return ((EX*)this)->In( (const char*)&c,sizeof(char) );
		}
		EX& operator<<(const float& i){
			if( IsSetFlag(Stream::netorder) ) {int temp=htonl(*(int*)(void*)&i);return ((EX*)this)->In( (const char*)&temp,sizeof(float) );}
			else{return ((EX*)this)->In( (const char*)&i,sizeof(float) );}
		}
 		EX& operator<<(const double& i){
			if( IsSetFlag(Stream::netorder) ) {int temp=htonl(*(int*)(void*)&i);return ((EX*)this)->In( (const char*)&temp,sizeof(double) );}
			else{return ((EX*)this)->In( (const char*)&i,sizeof(double) );}
		}
		EX& operator>>(long long& i){
			return ((EX*)this)->Out( (char*)&i,sizeof(long long) );
		}
		EX& operator>>(int& i){
			if( IsSetFlag(Stream::netorder) ) {int temp;return ((EX*)this)->Out((char*)&temp,sizeof(int) );i=ntohl(temp);}
			else{return ((EX*)this)->Out((char*)&i,sizeof(int) );}
		}
		EX& operator>>(short& s){
			if( IsSetFlag(Stream::netorder) ) {short temp;return ((EX*)this)->Out((char*)&temp,sizeof(short) );s=ntohs(temp);}
			else{return ((EX*)this)->Out((char*)&s,sizeof(short) );}
		}
		EX& operator>>(char& c){
			return ((EX*)this)->Out((char*)&c,sizeof(char) );
		}
		EX& operator>>(unsigned int& i){
			if( IsSetFlag(Stream::netorder) ) {int temp;EX& out = ((EX*)this)->Out((char*)&temp,sizeof(int) );temp=ntohl(temp);i=*(unsigned int*)(void*)&temp;return out;}
			else{return ((EX*)this)->Out((char*)&i,sizeof(int) );}
		}
		EX& operator>>(unsigned short& s){
			if( IsSetFlag(Stream::netorder) ) {short temp;EX& out = ((EX*)this)->Out((char*)&temp,sizeof(short) );temp=ntohs(temp);s=*(unsigned short*)(void*)&temp;return out;}
			else{return ((EX*)this)->Out((char*)&s,sizeof(short) );}
		}
		EX& operator>>(unsigned char& c){
			return ((EX*)this)->Out((char*)&c,sizeof(char) );
		}
		EX& operator>>(float& i){
			if( IsSetFlag(Stream::netorder) ) {float temp;EX& out = ((EX*)this)->Out((char*)&temp,sizeof(float) );temp=ntohl(temp);i=*(float*)(void*)&temp;return out;}
			else{return ((EX*)this)->Out((char*)&i,sizeof(float) );}
		}
		EX& operator>>(double& i){
			if( IsSetFlag(Stream::netorder) ) {float temp;EX& out = ((EX*)this)->Out((char*)&temp,sizeof(double) );temp=ntohl(temp);i=*(double*)(void*)&temp;return out;}
			else{return ((EX*)this)->Out((char*)&i,sizeof(double) );}
		}
	public:
		operator bool() const
		{
			return m_State == goodbit;
		}
	public:
		long AddRef(){
			return ::InterlockedIncrement(&m_RefCount);
		}
		long Release(){
			if( ::InterlockedDecrement(&m_RefCount) == 0 ){
				delete (EX*)this;
				return 0;
			}
			return m_RefCount;
		}
	};
	namespace Stream
	{
		template<class EX> inline EX& FailFlag(EX& stream)
		{
			stream.SetFailFlag();
			return (stream);
		}
		template<class EX> inline EX& ResetFlag(EX& stream)
		{
			stream.ResetFlag();
			return (stream);
		}
		template<class EX> inline EX& NetLocalTran(EX& stream)
		{
			stream.SetFlag(Stream::netorder);
			return (stream);
		}
		template<class EX> inline EX& NoNetLocalTran(EX& stream)
		{
			stream.ClearFlag(Stream::netorder);
			return (stream);
		}
		template<class EX> inline EX& SupportStruct(EX& stream)
		{
			stream.SetFlag(Stream::supportstruct);
			return (stream);
		}
		template<class EX> inline EX& NoSupportStruct(EX& stream)
		{
			stream.ClearFlag(Stream::supportstruct);
			return (stream);
		}
		template<class EX> inline EX& StringHeader(EX& stream)
		{
			stream.SetFlag(Stream::stringheader);
			return (stream);
		}
		template<class EX> inline EX& NoStringHeader(EX& stream)
		{
			stream.ClearFlag(Stream::stringheader);
			return (stream);
		}
		template<class EX> inline EX& StringIntHeader(EX& stream)
		{
			stream.SetFlag(Stream::stringintheader);
			return (stream);
		}
		template<class EX> inline EX& NoNetStringIntHeader(EX& stream)
		{
			stream.ClearFlag(Stream::stringintheader);
			return (stream);
		}
	}
}
#include "streambaseex.h"