#include "stdafx.h"
#include "leudgrid/cryptography.h"
extern "C"
{
#include "md5.h"
}
namespace LEUD
{
	CryptographyRSA::CryptographyRSA()
	{
		memset(m_bPublicKeyBlob,0,sizeof(m_bPublicKeyBlob));
		memset(m_bPrivateKeyBlob,0,sizeof(m_bPrivateKeyBlob));
		m_dwPublicBlobLen=0;
		m_dwPrivateBlobLen=0;
		m_hCryptProv=NULL;
		m_hPublicKey=NULL;
		m_hPrivateKey=NULL;
		m_bInitialized = false;
	}
	CryptographyRSA::~CryptographyRSA()
	{
	}
	bool CryptographyRSA::Init()
	{
		int result=CryptAcquireContext(
			&m_hCryptProv, 
			NULL, 
			MS_ENHANCED_PROV, 
			PROV_RSA_FULL, 
			0);
		if(!result)
		{
			result=CryptAcquireContext(
				&m_hCryptProv, 
				NULL, 
				MS_ENHANCED_PROV, 
				PROV_RSA_FULL, 
				CRYPT_NEWKEYSET);
			if(!result)
			{
				DWORD errorCode=GetLastError();
				printf("Error during CryptAcquireContext! ERROR CODE: %d",errorCode); 
				m_bInitialized =false;
				return false;
			}
		}
		m_bInitialized = true;
		return true;
	}

	void CryptographyRSA::Dest()
	{
		if(m_hPublicKey)
			CryptDestroyKey(m_hPublicKey);
		if(m_hPrivateKey)
			CryptDestroyKey(m_hPrivateKey);
		if(m_hCryptProv)
		{
			if(!(CryptReleaseContext(m_hCryptProv, 0)))
				printf("Error during CryptReleaseContext");
		} 
	}

	bool CryptographyRSA::GenKey(std::string privateKeyFile,std::string publicKeyFile)
	{
		if(!m_bInitialized)
			return false;
		
		HCRYPTKEY hKey; 
		if(CryptGenKey(
			m_hCryptProv, 
			AT_KEYEXCHANGE, 
			CRYPT_EXPORTABLE, 
			&hKey))
		{
			if(_ExportKey(publicKeyFile.c_str(),hKey,PUBLICKEYBLOB)&&_ExportKey(privateKeyFile.c_str(),hKey,PRIVATEKEYBLOB))
				return true;
			else
			{
				printf("Error during GenKey.\n"); 
				return false;
			}

		} 
		else
		{
			printf("Error during GenKey.\n"); 
			return false;
		}
	}
	bool CryptographyRSA::ImportPrivateKey(std::string privateKeyFile)
	{
		if(!m_bInitialized)
			return false;
		DWORD dwBlobLen=1024;
		FILE * file;
		fopen_s(&file,privateKeyFile.c_str(),"rb");
		if( file )
		{
			dwBlobLen = (DWORD)fread(m_bPrivateKeyBlob,1,1024,file);
			m_dwPrivateBlobLen=dwBlobLen;
			fclose(file);
		}
		if(CryptImportKey(
			m_hCryptProv,
			m_bPrivateKeyBlob,
			dwBlobLen,
			0,
			0,
			&m_hPrivateKey))
		{
			//printf("The private key has been imported.\n");
			return true;
		}
		else
		{
			printf("The private key import failed.\n");
			return false;
		}
	}
	bool CryptographyRSA::SetPrivateKey(const BYTE* bPrivateKeyBlob,DWORD dwPrivateBlobLen)
	{
		if(!m_bInitialized)
			return false;
		if(CryptImportKey(
			m_hCryptProv,
			bPrivateKeyBlob,
			dwPrivateBlobLen,
			0,
			0,
			&m_hPrivateKey))
		{
			if(dwPrivateBlobLen>1024)
				return false;
			memset(m_bPrivateKeyBlob,0,1024);
			memcpy(m_bPrivateKeyBlob,bPrivateKeyBlob,dwPrivateBlobLen);
			m_dwPrivateBlobLen=dwPrivateBlobLen;
			return true;
		}
		else
		{
			printf("Set private key failed.\n");
			return false;
		}
	}
	bool CryptographyRSA::ImportPublicKey(std::string publicKeyFile)
	{
		if(!m_bInitialized)
			return false;
		DWORD dwBlobLen=1024;
		FILE * file;
		fopen_s(&file,publicKeyFile.c_str(),"rb");
		if( file )
		{
			dwBlobLen = (DWORD)fread(m_bPublicKeyBlob,1,1024,file);
			m_dwPublicBlobLen=dwBlobLen;
			fclose(file);
		}
		if(CryptImportKey(
			m_hCryptProv,
			m_bPublicKeyBlob,
			dwBlobLen,
			0,
			0,
			&m_hPublicKey))
		{
			//printf("The public key has been imported.\n");
			return true;
		}
		else
		{
			printf("The public key import failed.\n");
			return false;
		}
	}
	bool CryptographyRSA::SetPublicKey(const BYTE* bPublicKeyBlob,DWORD dwPublicBlobLen)
	{
		if(!m_bInitialized)
			return false;
		if(CryptImportKey(
			m_hCryptProv,
			bPublicKeyBlob,
			dwPublicBlobLen,
			0,
			0,
			&m_hPublicKey))
		{
			if(dwPublicBlobLen>1024)
				return false;
			memset(m_bPublicKeyBlob,0,1024);
			memcpy(m_bPublicKeyBlob,bPublicKeyBlob,dwPublicBlobLen);
			m_dwPublicBlobLen=dwPublicBlobLen;
			return true;
		}
		else
		{
			printf("Set public key failed.\n");
			return false;
		}
	}
	bool CryptographyRSA::PublicEncrypt(BYTE* text,DWORD* pDwTextLen,DWORD bufLen)
	{
		if(!m_bInitialized)
			return false;
		DWORD tl = *pDwTextLen;
		DWORD cc = tl/117;
		BYTE* tx = new BYTE[tl];
		memcpy(tx,text,tl);
		BYTE* ep = tx;
		BYTE* dp = text;
		char temp[1024];
		for(DWORD i = 0;i <=cc;i++)
		{
			DWORD dl=117;
			if( i==cc ) dl = tl%117;
			if( dl > 0 )
			{
				memcpy(temp,ep,dl);
				if( ! CryptEncrypt(m_hPublicKey,0,i==cc,0,(BYTE*)temp,&dl,1024) )
				{
					printf("Error during CryptEncrypt. \n"); 
					delete[] tx;
					return false;
				}
				if( (dp+dl-text) > static_cast<signed>(bufLen) )
				{
					delete[] tx;
					return false;
				}
				memcpy(dp,temp,dl);
				dp+=dl;
				ep+=117;
			}
		}
		delete[] tx;
		*pDwTextLen = static_cast<DWORD>(dp-text);
		return true;
	}
	bool CryptographyRSA::PublicDecrypt(BYTE* text,DWORD* pDwTextLen)
	{
		if(!m_bInitialized)
			return false;
		DWORD tl = *pDwTextLen;
		DWORD cc = tl/128;
		BYTE* tx = new BYTE[tl];
		memcpy(tx,text,tl);
		BYTE* ep = tx;
		BYTE* dp = text;
		char temp[1024];
		for(DWORD i = 0;i <=cc;i++)
		{
			DWORD dl=128;
			if( i==cc ) dl = tl%128;
			if( dl > 0 )
			{
				memcpy(temp,ep,dl);
				if( ! CryptDecrypt(m_hPublicKey,0,i==cc,0,(BYTE*)temp,&dl) )
				{
					printf("Error during CryptDecrypt. %x %d %d errorid:%x\n",m_hPublicKey,i,dl,GetLastError()); 
					delete[] tx;
					return false;
				}
				if( (dp+dl-text) > static_cast<signed>(*pDwTextLen) )
				{
					delete[] tx;
					return false;
				}
				memcpy(dp,temp,dl);
				dp+=dl;
				ep+=128;
			}
		}
		delete[] tx;
		*pDwTextLen = static_cast<DWORD>(dp-text);
		return true;
	}
	bool CryptographyRSA::PrivateEncrypt(BYTE* text,DWORD* pDwTextLen,DWORD bufLen)
	{
		if(!m_bInitialized)
			return false;
		DWORD tl = *pDwTextLen;
		DWORD cc = tl/117;
		BYTE* tx = new BYTE[tl];
		memcpy(tx,text,tl);
		BYTE* ep = tx;
		BYTE* dp = text;
		char temp[1024];
		for(DWORD i = 0;i <=cc;i++)
		{
			DWORD dl=117;
			if( i==cc ) dl = tl%117;
			if( dl > 0 )
			{
				memcpy(temp,ep,dl);
				if( ! CryptEncrypt(m_hPrivateKey,0,i==cc,0,(BYTE*)temp,&dl,1024) )
				{
					printf("Error during CryptEncrypt. \n"); 
					delete[] tx;
					return false;
				}
				if( (dp+dl-text) > static_cast<signed>(bufLen) )
				{
					delete[] tx;
					return false;
				}
				memcpy(dp,temp,dl);
				dp+=dl;
				ep+=117;
			}
		}
		delete[] tx;
		*pDwTextLen = static_cast<DWORD>(dp-text);
		return true;
	}
	bool CryptographyRSA::PrivateDecrypt(BYTE* text,DWORD* pDwTextLen)
	{
		if(!m_bInitialized)
			return false;
		DWORD tl = *pDwTextLen;
		DWORD cc = tl/128;
		BYTE* tx = new BYTE[tl];
		memcpy(tx,text,tl);
		BYTE* ep = tx;
		BYTE* dp = text;
		char temp[1024];
		for(DWORD i = 0;i <=cc;i++)
		{
			DWORD dl=128;
			if( i==cc ) dl = tl%128;
			if( dl > 0 )
			{
				memcpy(temp,ep,dl);
				if( ! CryptDecrypt(m_hPrivateKey,0,i==cc,0,(BYTE*)temp,&dl) )
				{
					printf("Error during CryptDecrypt. \n"); 
					delete[] tx;
					return false;
				}
				if( (dp+dl-text) > static_cast<signed>(*pDwTextLen) )
				{
					delete[] tx;
					return false;
				}
				memcpy(dp,temp,dl);
				dp+=dl;
				ep+=128;
			}
		}
		delete[] tx;
		*pDwTextLen = static_cast<DWORD>(dp-text);
		return true;
	}

	bool CryptographyRSA::_ExportKey(const char* filename,HCRYPTKEY hKey,DWORD dwBlobType)
	{
		DWORD dwBlobLen;
		BYTE *pbKeyBlob;
		if(CryptExportKey(   
			hKey,    
			NULL,    
			dwBlobType,
			0,    
			NULL, 
			&dwBlobLen)) 
		{
			//printf("Size of the BLOB for the public key determined. \n");
		}
		else
		{
			printf("Error computing BLOB length.\n");
			return false;
		}

		if(pbKeyBlob = (BYTE*)malloc(dwBlobLen)) 
		{
			//printf("Memory has been allocated for the BLOB. \n");
		}
		else
		{
			printf("Out of memory. \n");
			return false;
		}

		if(CryptExportKey(   
			hKey, 
			NULL,    
			dwBlobType,    
			0,    
			pbKeyBlob,    
			&dwBlobLen))
		{
			//printf("Contents have been written to the BLOB. \n");
		}
		else
		{
			printf("Error exporting key.\n");
			return false;
		}
		
		FILE* file;
		fopen_s(&file,filename,"wb");
		if(file)
		{
			fwrite(pbKeyBlob,1,dwBlobLen,file);
			fclose(file);
		}
		else
		{
			printf("Error open file.\n");
			return false;
		}

		free(pbKeyBlob);
		return true;
	}
	void CryptographyMD5::Do(unsigned char out[16],const unsigned char* buf,unsigned size)
	{
		struct MD5Context md5c;
		MD5Init( &md5c );
		MD5Update( &md5c, buf, size );
		MD5Final( out, &md5c );
	}
	void CryptographySimple::InitKey(unsigned char* key)
	{
		for(int i = 0;i < 16;i++)
		{
			key[i] = (i+7)*5;
		}
	}
	void CryptographySimple::InitDict(unsigned char* dict1,unsigned char* dict2)
	{
		for(int i = 0;i<=255;i++)
		{
			dict1[i] = i;
		}
		int ch[127];
		for(int i = 0;i<127;i++)
		{
			ch[i]=i*2;
		}
		for(int i = 0;i<127;i++)
		{
			dict1[ch[i]]=ch[127-i-1];
		}
		for(int i = 0;i<=255;i++)
		{
			dict2[dict1[i]] = i;
		}
	}
	void CryptographySimple::Encode(unsigned char* buf,unsigned int size,const unsigned char* key,const unsigned char* dict)
	{
		for(size_t i = 0;i < size;i ++ )
		{
			int key_pos = i&0xF;
			unsigned char e = buf[i];
			e = (~e)^key[key_pos];
			buf[i] = dict[e];
		}
	}
	void CryptographySimple::Decode(unsigned char* buf,unsigned int size,const unsigned char* key,const unsigned char* dict)
	{
		for(size_t i = 0;i< size ;i ++ )
		{
			int key_pos = i&0xF;
			unsigned char e = dict[buf[i]];
			e = ~(e^key[key_pos]);
			buf[i] = e;
		}
	}
}