#pragma once
namespace LEUD
{
	template<class T> inline T& operator<<(T& stream,char* str)
	{
		return stream<<(const_cast<const char*>(str));
	}
	template<class T> inline T& operator<<(T& stream,const char* str)
	{
		size_t length=strlen(str);
		if( stream.IsSetFlag(Stream::stringheader) )
		{
			if( stream.IsSetFlag(Stream::stringintheader) )
			{
				unsigned int i=static_cast<unsigned int>(length);
				stream<<i;
			}
			else
			{
				unsigned short l = static_cast<unsigned short>(length);
				stream<<l;
			}
		}
		else
		{
			length ++;
		}
		stream.In(str,length);
		return stream;
	}
	template<class T> inline T& operator<<(T& stream,const std::string& str)
	{
		size_t length=str.size();
		if( stream.IsSetFlag(Stream::stringheader) )
		{
			if( stream.IsSetFlag(Stream::stringintheader) )
			{
				unsigned int i=static_cast<unsigned int>(length);
				stream<<i;
			}
			else
			{
				unsigned short l = static_cast<unsigned short>(length);
				stream<<l;
			}
		}
		else
		{
			length ++;
		}
		stream.In(str.c_str(),length);
		return stream;
	}
	template<class T> inline T& operator>>(T& stream,char* str)
	{
		size_t length = 0;
		size_t assign_length = 0;
		if( stream.IsSetFlag(Stream::stringheader) )
		{
			if( stream.IsSetFlag(Stream::stringintheader) )
			{
				unsigned int i;
				stream>>i;
				assign_length = length = i;
			}
			else
			{
				unsigned short l;
				stream>>l;
				assign_length = length = l;
			}
		}
		else
		{
			length = stream.findchar(0);
			assign_length = length - 1;
		}
		if( stream && stream.left() >= length )
		{
			if( length <= 0 )				
			{
				str[0] = 0;
			}
			else if( length < 511 )
			{
				stream.Out(str,length);
				str[length] = 0;
			}
			else
			{
				str[0]=0;
			}
		}
		return stream;
	}
	template<class T> inline T& operator>>(T& stream,std::string& str)
	{
		size_t length = 0;
		size_t assign_length = 0;
		if( stream.IsSetFlag(Stream::stringheader) )
		{
			if( stream.IsSetFlag(Stream::stringintheader) )
			{
				unsigned int i;
				stream>>i;
				assign_length = length = i;
			}
			else
			{
				unsigned short l;
				stream>>l;
				assign_length = length = l;
			}
		}
		else
		{
			length = stream.findchar(0);
			assign_length = length - 1;
		}
		if( stream && stream.left() >= length )
		{
			if( length <= 0 )				
			{
				str = "";
			}
			else if( length < 1024 )
			{
				char buf[1024];
				stream.Out(buf,length);
				str.assign( buf, assign_length );
			}
			else
			{
				std::vector<char> buf;
				buf.reserve( length );
				stream.Out(&buf[0],length);
				str.assign( &buf[0],assign_length );
			}
		}
		return stream;
	}
}