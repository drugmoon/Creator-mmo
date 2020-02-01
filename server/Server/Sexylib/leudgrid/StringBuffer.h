#pragma once

namespace LEUD {

const size_t MAX_BUFFER_SIZE = 512;

class CStringImp
{
public:
	CStringImp()
	{
		m_nLength = 0;
		m_pBuffer = NULL;
	}

	explicit CStringImp(const char* pBuf, size_t nLength)
	{
		m_nLength = nLength;
		m_pBuffer = (char * )pBuf;
	}

	explicit CStringImp(const char * pszBuf)
	{
		m_nLength=::strlen(pszBuf);
		m_pBuffer=(char *)pszBuf;
	}

	const CStringImp & operator = (const CStringImp & stringSrc)
	{
		m_nLength = stringSrc.GetLength();
		m_pBuffer = (char *)stringSrc.GetBuffer();
		return (* this);
	}

	virtual ~CStringImp() {};

	const char* GetBuffer() const
	{
		return m_pBuffer;
	}

	size_t GetLength()const
	{
		return m_nLength;
	}

	operator LPCSTR() const
	{
		return m_pBuffer;
	};

	///为了提高效率，减少内存拷贝，该函数会把对象的cDiv字符修改为0

	//////////////////////////////////////////////////////////////////////////
	//
	//    ::OnRead(const char * pbuf,buflen)  在一个OnRead函数中
	//    pBuf ="(109/sunway/09/123/1)";
	//    CStringImp strImp(pbuf,buflen);
	//    CStringImp strBody;
	//
	//    strImp.ExtractString(strBody,'(',')');  //检查返回值
	//
	//    CStringImp strCmd,strLoginId,strFeedType,strFeedSecs,strNum;
	//
	//    strBody.FetchString('/',strCmd);    
	//    strBody.FetchString('/',strLoginId);
	//    strBody.FetchString('/',strFeedType);
	//    strBody.FetchString('/',strFeedSecs);
	//
	//    strNum = strBody;
	//    
	//
	//////////////////////////////////////////////////////////////////////////

	bool FetchString(const char cDiv, CStringImp& strPosImp)
	{
		char* psOldBuf = m_pBuffer;

		for (size_t i=0;i<m_nLength;++i)
		{
			if (psOldBuf[i] == cDiv)
			{
				psOldBuf[i] = 0;
				strPosImp.m_pBuffer = m_pBuffer;
				strPosImp.m_nLength = i;

				this->m_pBuffer += (i+1);
				this->m_nLength -= (i+1);

				return true;
			}
		}

		return false;
	}

	///为了提高效率，减少内存拷贝，该函数会把对象的cEnd字符修改为0

	bool ExtractString(CStringImp & strImp,const char cBegin,const char cEnd)
	{
		bool bFindHead = false;
		char * psOldBuf = m_pBuffer;
		size_t nBeginPos = 0;

		for (size_t i=0; i<m_nLength; ++i )
		{
			if (!bFindHead)
			{
				if (psOldBuf[i] == cBegin)
				{
					bFindHead =true;
					nBeginPos = i+1;
				}
			}
			else
			{
				if (psOldBuf[i] == cEnd)
				{
					psOldBuf[i] = 0;

					strImp.m_pBuffer = &psOldBuf[nBeginPos];

					strImp.m_nLength = i-nBeginPos;

					this->m_pBuffer += (i+1);
					this->m_nLength -= (i+1);

					return true;
				}
			}
		}

		return false;
	}

	int Pos(const char PosChar)
	{
		for(size_t i=0; i<m_nLength; ++i)
		{
			if(m_pBuffer[i]==PosChar)
			{
				return (int)i;
			}
		}

		return -1;
	}

	size_t size() const
	{
		return m_nLength;
	}

protected:
	char * m_pBuffer;
	size_t m_nLength;
};

template <size_t max_buffer_size=MAX_BUFFER_SIZE>
class CStringBuffer : public CStringImp
{
public:
	CStringBuffer(void)
	{
		m_pBuffer = m_Buffer;
	};

	/// 拷贝构造从 strImp构造
	explicit CStringBuffer(const CStringImp& strImp)
	{
		m_pBuffer = m_Buffer;

		if (strImp.GetLength() <= max_buffer_size)
		{
			memcpy(m_Buffer,strImp.GetBuffer(),strImp.GetLength());
			m_nLength = strImp.GetLength();
		}
	};

	/// 拷贝构造函数
	template<size_t size>
		explicit CStringBuffer(const CStringBuffer<size>& strbuffer)
	{
		m_pBuffer = m_Buffer;

		if (strbuffer.GetLength() <= max_buffer_size)
		{
			memcpy(this->m_Buffer,strbuffer,strbuffer.GetLength());
			m_nLength = strbuffer.GetLength();
		}
	}

	/// 拷贝构造从以0结尾的字符串
	explicit CStringBuffer(const char* pszStr)
	{
		size_t nLength = strlen(pszStr);

		if (nLength < max_buffer_size)
		{
			memcpy(m_Buffer,pszStr,nLength);
			m_Buffer[nLength] = 0;
			m_nLength = nLength + 1;
		}

		m_pBuffer = m_Buffer;
	};

	/// 拷贝构造从 pBuf开头的nLength的一段字符串
	explicit CStringBuffer(const char* pBuf,size_t nLength)
	{
		if (nLength <= max_buffer_size)
		{
			memcpy(m_Buffer,pBuf,nLength);
			m_nLength = nLength;
		}

		m_pBuffer = m_Buffer;
	}

	virtual ~CStringBuffer(void) {};

	/// 返回缓冲区总的容量
	size_t GetCapacity() const
	{
		return max_buffer_size;
	}

	/// 用cFill填充 m_Buffer的缓冲区，如果不提供该参数就用0字符填充
	void FillChar(const char cFill=0)
	{
		memset(m_Buffer, cFill, max_buffer_size);
	}

	/// 得到当前缓冲区的剩余空间的头地址
	char * GetFreeBuffer() 
	{ 
		return &m_Buffer[m_nLength];
	};

	/// 得到剩余空间的长度
	int GetFreeLength() 
	{
		return (int)(max_buffer_size-m_nLength);
	};

	/// 增加长度计数，为了提供给一些不知道长度的函数使用
	bool AddBuffer(size_t nLen)
	{
		if (m_nLength+nLen <= max_buffer_size)
		{
			m_nLength += nLen;
			return true;
		}
		else
		{
			return false;
		}
	}

	/// 得到缓冲区头地址
	char* GetData() const
	{
		return m_Buffer;
	};

	/// 向缓冲区中加入新的缓冲 
	bool AddBuffer(const char* buffer,size_t len)
	{
		if (len+m_nLength>max_buffer_size)
		{
			return false;
		}

		memcpy(&m_Buffer[m_nLength],buffer,len);
		m_nLength += len;

		return true;
	}

	/// 把stringImp的数据加入到缓冲区中
	bool AddBuffer(const CStringImp & stringImp)
	{
		if (stringImp.GetLength()+m_nLength>max_buffer_size)
		{
			return false;
		}

		memcpy(&m_Buffer[m_nLength],
			stringImp.GetBuffer(),
			stringImp.GetLength());

		m_nLength += stringImp.GetLength();

		return true;
	}

	/// 拷贝字符到缓冲的尾部
	bool AddChar(const char cAddChr)
	{
		if (m_nLength+1<= (size_t)max_buffer_size)
		{
			m_Buffer[m_nLength] = cAddChr;

			++m_nLength;
			return true;
		}

		return false;
	}

	/// 拷贝字符串的有效部分加入到缓冲中，不追加0字符
	bool AddString(const char* pszStr)
	{
		size_t nLength = strlen(pszStr);
		return AddBuffer(pszStr, nLength);
	}

	/// 拷贝字符串的有效部分和0到缓冲区中，包含0字符
	bool AddZeroString(const char * pszStr)
	{
		return AddStringDivision(pszStr,0);
	}

	/// 拷贝字符串的有效部分到缓冲中，在该字符串后面追加cDiv字符
	bool AddStringDivision(const char * pszStr,const char cDiv)
	{
		size_t nAddLen = strlen(pszStr);

		if (m_nLength+nAddLen+1>max_buffer_size)
		{
			return false;
		}

		memcpy(&m_Buffer[m_nLength],pszStr,nAddLen);

		m_nLength += nAddLen;
		m_Buffer[m_nLength] = cDiv;
		m_nLength += 1;

		return true;
	}; 

	/// 拷贝stringImp的缓冲到对象缓冲中，同时在缓冲后追加cDiv字符
	bool AddStringDivision(const CStringImp & stringImp,const char cDiv)
	{
		size_t nAddLen=stringImp.GetLength();

		if(m_nLength+nAddLen+1>max_buffer_size)
		{
			return false;
		}

		memcpy(&m_Buffer[m_nLength],stringImp,nAddLen);

		m_nLength += nAddLen;

		m_Buffer[m_nLength] = cDiv;
		m_nLength += 1;

		return true;
	}; 

	/// 清理nLen部分的数据，把后面的数据移动到缓冲头
	void ReleaseBuffer(size_t nLen)
	{
		if (nLen >= m_nLength)
		{
			m_nLength = 0;
		}
		else
		{
			if (nLen>0)
			{
				m_nLength -= nLen;
				memcpy(m_Buffer,&m_Buffer[nLen],m_nLength);
			}
		}
	}

	/// 在已经增加的缓冲区后面格式化一段内容
	/// 这个函数注意在开始的时候要给缓冲分配足够大，否则有问题
	/// 注意总长度没有包含后面的0字符的长度
	size_t AddFormat(LPCSTR lpszFormat, ...)
	{
		va_list	ap;

		va_start(ap, lpszFormat);
		m_nLength += vsprintf(m_Buffer+m_nLength, lpszFormat, ap);
		va_end(ap);

		return  m_nLength;
	}

	/// 重新设置缓冲标记
	void Clear()
	{
		m_nLength = 0;
		m_pBuffer = m_Buffer;
	}

	/// 在缓冲区开始部分输出一段格式化字符
	/// 注意在该函数以前加入的字符或者数据会丢失
	/// 注意总长度没有包含后面的0字符的长度
	size_t Format(LPCSTR lpszFormat, ...)
	{
		va_list	ap;

		va_start(ap, lpszFormat);
		m_nLength = vsprintf(m_Buffer, lpszFormat, ap);
		va_end(ap);

		return  m_nLength;
	}

	/// 返回索引对应的字符
	/// 该函数不检查安全性
	char operator[](int nIndex) const
	{
		return m_Buffer[nIndex];
	}

	/// 返回索引对应的字符，该函数检查安全性
	/// 如果索引越界 返回 0
	const char GetAt(int nIndex) const
	{
		if (nIndex<m_nLength)
		{
			return m_Buffer[m_nLength];
		}
		else
		{
			return 0x0;
		}
	}

	/// 设置nIndex对应的字符
	/// 如果超过缓冲区已有的长度，不设置
	void SetAt(int nIndex,const char cSet)
	{
		if (nIndex<m_nLength)
		{
			m_Buffer[m_nLength] = cSet;
		} 
	}

	bool LoadFromFile(const char * pszFileName)
	{
		FILE * pFile = fopen(pszFileName,"r");
		if (pFile == NULL)
			return false;

		int nLen = fseek(pFile,0,SEEK_END);

		if (nLen > m_nLength)
		{
			fclose(pFile);
			return false;
		}

		fseek(pFile,0,SEEK_SET);

		int nRet = fread(m_Buffer,nLen,max_buffer_size,pFile);

		fclose(pFile);

		///尾巴上加上0
		if (nRet == nLen)
		{
			m_Buffer[nLen] = 0;
			m_nLength = nLen;
			return true;
		}
		else
		{
			return false;
		}
	}

protected:
	//加4个字节防止溢出
	char   m_Buffer[max_buffer_size+4];
};

}