#pragma once

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
#define MAC_DIM					6
#define MACHINE_CODE_DIM		6
#define SOFTWAREKEY_DIM			6
namespace LEUD
{

	// ---------------------------------------------------------------------------
	// ---------------------------------------------------------------------------
	enum RETVALUE
	{
		SUCCESS,
		ERROR_API_CALL_FAILED,
		ERROR_FAILURE_WHILE_LOADING_LIBRARY,
		ERROR_OS_VERSION_NOT_SUPPORTED,
		ERROR_SOFTWAREKEY_NOT_FOUND,
		ERROR_CONVERSION_CHAR_2_WCHAR_FAILED
	};

	// ---------------------------------------------------------------------------
	// ---------------------------------------------------------------------------
	class CSoftwareKey
	{
	public:

		static RETVALUE RetrieveMACAddress(
			BYTE				pMACaddress[MAC_DIM]);

		static RETVALUE ComputeMachineCode(
			const BYTE			pMACaddress[MAC_DIM], 
			BYTE				pMachineCode[MACHINE_CODE_DIM]); 

		static RETVALUE ComputeSoftwareKey(
			const BYTE			pMachineCode[MACHINE_CODE_DIM], 
			BYTE				pSoftwareKey[SOFTWAREKEY_DIM]); 

		static RETVALUE VerifySoftwareKey(
			const char*			pSoftwareKeyString, 
			bool*				pIsValid);

		static RETVALUE GetSoftwareKeyStringFromIniFile(
			const char*			pFilePath, 
			char**				pSoftwareKeyString);

		static RETVALUE Buffer2String(
			const BYTE*			pBuffer, 
			const unsigned int	pBufferSize, 
			char**				pString);

		static RETVALUE String2Buffer(
			const char*			pString,
			BYTE**				pBuffer, 
			unsigned int*		pBufferSize);
	};

}
