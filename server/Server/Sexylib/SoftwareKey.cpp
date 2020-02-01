#include "stdafx.h"
#include "leudgrid/SoftwareKey.h"   
#include "Iphlpapi.h"
#pragma warning(disable:4996)
#pragma comment(lib,"Iphlpapi.lib")
namespace LEUD
{

	// ---------------------------------------------------------------------------
	// ---------------------------------------------------------------------------
	RETVALUE CSoftwareKey::ComputeSoftwareKey(
		const BYTE pMachineCode[MACHINE_CODE_DIM], 
		BYTE pSoftwareKey[SOFTWAREKEY_DIM])
	{
		RETVALUE		lReturnValue = SUCCESS;

		pSoftwareKey[0] = ((pMachineCode[1]<<4)|(pMachineCode[3]>>4))^0x34;
		pSoftwareKey[1] = ((pMachineCode[4]<<4)|(pMachineCode[5]>>4))^0x78;
		pSoftwareKey[2] = ((pMachineCode[2]<<4)|(pMachineCode[0]>>4))^0x93;
		pSoftwareKey[3] = ((pMachineCode[3]<<4)|(pMachineCode[2]>>4))^0x51;
		pSoftwareKey[4] = ((pMachineCode[5]<<4)|(pMachineCode[4]>>4))^0xA0;
		pSoftwareKey[5] = ((pMachineCode[0]<<4)|(pMachineCode[1]>>4))^0xD7;

		return lReturnValue;
	}

	// ---------------------------------------------------------------------------
	// ---------------------------------------------------------------------------
	RETVALUE CSoftwareKey::VerifySoftwareKey(
		const char* pSoftwareKeyString, 
		bool* pIsValid)
	{
		RETVALUE		lReturnValue = SUCCESS;

		bool			lIsValid = true;

		BYTE lMACaddress[MAC_DIM];
		lReturnValue = RetrieveMACAddress(lMACaddress);

		BYTE lMachineCode[MACHINE_CODE_DIM];
		lReturnValue = ComputeMachineCode(lMACaddress, lMachineCode);

		BYTE lSoftwareKey[SOFTWAREKEY_DIM];
		lReturnValue = ComputeSoftwareKey(lMachineCode, lSoftwareKey); 

		char* lSoftwareKeyString = NULL;
		lReturnValue = Buffer2String(
			lSoftwareKey, 
			SOFTWAREKEY_DIM, 
			&lSoftwareKeyString);


		unsigned int lSoftwareKeyStringLen = static_cast<unsigned int>( strlen(lSoftwareKeyString) );
		unsigned int lpSoftwareKeyStringLen = static_cast<unsigned int>( strlen(pSoftwareKeyString) );

		if (lSoftwareKeyStringLen != lpSoftwareKeyStringLen)
		{
			lIsValid = false;
			*pIsValid = lIsValid;
			delete[] lSoftwareKeyString;
			return lReturnValue;
		}

		for (unsigned int i = 0; i<lSoftwareKeyStringLen; i++)
		{
			BYTE lKey1 = lSoftwareKeyString[i];
			BYTE lKey2 = pSoftwareKeyString[i];

			if (lKey1 != lKey2)
			{
				lIsValid = false;
				break;
			}
		}

		*pIsValid = lIsValid;
		delete[] lSoftwareKeyString;
		return lReturnValue;
	}

	// ---------------------------------------------------------------------------
	// ---------------------------------------------------------------------------
	RETVALUE CSoftwareKey::Buffer2String(
		const BYTE* pBuffer, 
		const unsigned int pBufferSize, 
		char** pString)
	{
		RETVALUE		lReturnValue = SUCCESS;

		unsigned int	lOutStringSize = (pBufferSize*3) + 1;

		// it is the caller that it is on charge to delete this pointer
		char*			lOutString = new char[lOutStringSize];
		sprintf(lOutString,"%s","");

		char lAppString[4];
		for (unsigned int i = 0; i < pBufferSize; i++)
		{
			BYTE b = pBuffer[i];

			// NOTE: 3 chars in out string for each byte in input buffer
			sprintf(lAppString,"%03u",b); 
			strcat(lOutString,lAppString);
		}

		*pString = lOutString;

		return lReturnValue;
	}


	// ---------------------------------------------------------------------------
	// ---------------------------------------------------------------------------
	RETVALUE CSoftwareKey::String2Buffer(
		const char*		pString,
		BYTE**			pBuffer, 
		unsigned int*	pBufferSize)
	{
		RETVALUE		lReturnValue = SUCCESS;

		unsigned int	lOutBufferSize = static_cast<unsigned int> ( strlen(pString) / 3 );


		// it is the caller that it is on charge to delete this pointer
		BYTE*	lOutBuffer = new BYTE[lOutBufferSize];


		char lAppString[4];

		unsigned int j	= 0;
		unsigned int j1 = 0;
		unsigned int j2 = 0;
		unsigned int j3 = 0;

		for (unsigned int i = 0; i < lOutBufferSize; i++)
		{
			j1 = j++;
			j2 = j++;
			j3 = j++;

			sprintf(
				lAppString,
				"%c%c%c",
				pString[j1],
				pString[j2],
				pString[j3]);

			// NOTE: one byte in output buffer for each 3 chars in input string
			BYTE b = (BYTE) atoi(lAppString);

			lOutBuffer[i] = b;
		}

		*pBuffer = lOutBuffer;
		*pBufferSize = lOutBufferSize;

		return lReturnValue;
	}


	// ---------------------------------------------------------------------------
	// ---------------------------------------------------------------------------
	RETVALUE CSoftwareKey::GetSoftwareKeyStringFromIniFile(
		const char* pFilePath, 
		char** pSoftwareKeyString)
	{
		RETVALUE		lReturnValue = SUCCESS;

		unsigned int l_SoftwareKeyAsStringLen = SOFTWAREKEY_DIM * 3 + 1;
		char* l_SoftwareKeyAsString = new char[l_SoftwareKeyAsStringLen];

		DWORD l_NumCopied = GetPrivateProfileStringA (
			"protection",					// section name
			"softwarekey",					// key name
			"",								// default string
			l_SoftwareKeyAsString,			// destination buffer
			l_SoftwareKeyAsStringLen,		// size of destination buffer
			pFilePath);						// initialization file name

		if (l_NumCopied == 0)
		{
			lReturnValue = ERROR_SOFTWAREKEY_NOT_FOUND;
			return lReturnValue;
		}

		*pSoftwareKeyString = l_SoftwareKeyAsString;

		return lReturnValue;
	}

	// ---------------------------------------------------------------------------
	// ---------------------------------------------------------------------------
	RETVALUE CSoftwareKey::ComputeMachineCode(
		const BYTE pMACaddress[MAC_DIM], 
		BYTE pMachineCode[6])
	{
		RETVALUE		lReturnValue = SUCCESS;

		pMachineCode[0] = (pMACaddress[5]<<4)|(pMACaddress[5]>>4);
		pMachineCode[1] = (pMACaddress[0]<<4)|(pMACaddress[0]>>4);
		pMachineCode[2] = (pMACaddress[4]<<4)|(pMACaddress[4]>>4);
		pMachineCode[3] = (pMACaddress[1]<<4)|(pMACaddress[1]>>4);
		pMachineCode[4] = (pMACaddress[3]<<4)|(pMACaddress[3]>>4);
		pMachineCode[5] = (pMACaddress[2]<<4)|(pMACaddress[2]>>4);

		return lReturnValue;
	}

	// ---------------------------------------------------------------------------
	// ---------------------------------------------------------------------------
	RETVALUE CSoftwareKey::RetrieveMACAddress(BYTE pMACaddress[MAC_DIM])
	{
		IP_ADAPTER_INFO* pAdptInfo	= NULL;
		IP_ADAPTER_INFO* pNextAd	= NULL;	
		ULONG ulLen					= 0;
		int nCnt					= 0;
		DWORD erradapt;


		erradapt = ::GetAdaptersInfo( pAdptInfo, &ulLen );
		if( erradapt == ERROR_BUFFER_OVERFLOW ) {
			pAdptInfo = ( IP_ADAPTER_INFO* )malloc( ulLen );
			erradapt = ::GetAdaptersInfo( pAdptInfo, &ulLen );		
		}
		for(int i = 0;i<MAC_DIM;i++)
		{
			pMACaddress[i] = MAC_DIM+i*2;
		}
		pNextAd = pAdptInfo;
		while( pNextAd ) {
			if( pNextAd->AddressLength == MAC_DIM )
			{
				for(int i=0;i<MAC_DIM;i++)
				{
					pMACaddress[i] ^= pNextAd->Address[i];
					nCnt++;
				}
			}
			unsigned long ip = inet_addr(pNextAd->IpAddressList.IpAddress.String);
			if( ip != 0 )
			{
				char* pp = (char*)&ip;
				for(int i = 0;i<4;i++)
				{
					pMACaddress[i] ^= pp[i];
					nCnt++;
				}
			}
			//printf("\tAdapter Name: \t%s\n", pNextAd->AdapterName);
			//printf("\tAdapter Desc: \t%s\n", pNextAd->Description);
			//printf("\tAdapter Addr: \t%ld\n", pNextAd->Address);
			//printf("\tIP Address: \t%s\n", pNextAd->IpAddressList.IpAddress.String);
			//printf("\tIP Mask: \t%s\n", pNextAd->IpAddressList.IpMask.String);
			pNextAd = pNextAd->Next;
		}
		DWORD sn;
		if( GetVolumeInformation("c:\\",0,0,&sn,0,0,0,0) )
		{
			char* pp = (char*)&sn;
			for(int i = 0;i<4;i++)
			{
				pMACaddress[i] ^= pp[i];
				nCnt++;
			}
		}
		if( nCnt )
		{
			free(pAdptInfo);
			return SUCCESS;
		}
		for(int i = 0;i<MAC_DIM;i++)
		{
			pMACaddress[i] = 0;
		}
		free(pAdptInfo);
		return ERROR_OS_VERSION_NOT_SUPPORTED;
	}

}
