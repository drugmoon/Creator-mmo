#pragma once

namespace LEUD
{
	class CryptographyRSA
	{
	public:
		CryptographyRSA();
		~CryptographyRSA();
		bool Init();
		void Dest();

		bool GenKey(std::string privateKeyFile,std::string publicKeyFile);
		bool ImportPrivateKey(std::string privateKeyFile);
		bool SetPrivateKey(const BYTE* bPrivateKeyBlob,DWORD dwPrivateBlobLen);
		bool ImportPublicKey(std::string publicKeyFile);
		bool SetPublicKey(const BYTE* bPublicKeyBlob,DWORD dwPublicBlobLen);

		bool PublicEncrypt(BYTE* text,DWORD* pDwTextLen,DWORD bufLen);
		bool PublicDecrypt(BYTE* text,DWORD* pDwTextLen);
		bool PrivateEncrypt(BYTE* text,DWORD* pDwTextLen,DWORD bufLen);
		bool PrivateDecrypt(BYTE* text,DWORD* pDwTextLen);
	private:
		bool _ExportKey(const char* filename,HCRYPTKEY hKey,DWORD dwBlobType);
		HCRYPTPROV m_hCryptProv;
		HCRYPTKEY m_hPublicKey;
		HCRYPTKEY m_hPrivateKey;
		BYTE m_bPublicKeyBlob[1024];
		DWORD m_dwPublicBlobLen;
		BYTE m_bPrivateKeyBlob[1024];
		DWORD m_dwPrivateBlobLen;
		bool m_bInitialized;
	};
	class CryptographyMD5
	{
	public:
		static void Do(unsigned char out[16],const unsigned char* buf,unsigned size);
	};
	class CryptographySimple
	{
	public:
		static void InitKey(unsigned char* key);
		static void InitDict(unsigned char* dict1,unsigned char* dict2);
		static void Encode(unsigned char* buf,unsigned int size,const unsigned char* key,const unsigned char* dict);
		static void Decode(unsigned char* buf,unsigned int size,const unsigned char* key,const unsigned char* dict);
	};
}