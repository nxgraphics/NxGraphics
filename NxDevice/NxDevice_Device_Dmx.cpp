#include "NxDevicePreCompiled.h"
#include "../nxdeps/include/FTDI/FTD2XX.H"
#include "../nxdeps/include/SimpleDmx/timedDMX.h"

// Enttec Pro definitions
#define GET_WIDGET_PARAMS 3
#define GET_WIDGET_SN 10
#define GET_WIDGET_PARAMS_REPLY 3
#define SET_WIDGET_PARAMS 4
#define SET_DMX_RX_MODE 5
#define SET_DMX_TX_MODE 6
#define SEND_DMX_RDM_TX 7
#define RECEIVE_DMX_ON_CHANGE 8
#define RECEIVED_DMX_COS_TYPE 9
#define ONE_BYTE 1
#define DMX_START_CODE 0x7E 
#define DMX_END_CODE 0xE7 
#define OFFSET 0xFF
#define DMX_HEADER_LENGTH 4
#define BYTE_LENGTH 8
#define HEADER_RDM_LABEL 5
#define NO_RESPONSE 0
#define DMX_PACKET_SIZE 512
#pragma pack(1)
typedef struct {
        unsigned char FirmwareLSB;
        unsigned char FirmwareMSB;
        unsigned char BreakTime;
        unsigned char MaBTime;
        unsigned char RefreshRate;
}DMXUSBPROParamsType;

typedef struct {
        unsigned char UserSizeLSB;
        unsigned char UserSizeMSB;
        unsigned char BreakTime;
        unsigned char MaBTime;
        unsigned char RefreshRate;
}DMXUSBPROSetParamsType;
#pragma pack()

struct ReceivedDmxCosStruct
{
	unsigned char start_changed_byte_number;
	unsigned char changed_byte_array[5];
	unsigned char changed_byte_data[40];
};

#define MAX_PROS 20
#define SEND_NOW 0
#define TRUE 1
#define FALSE 0
#define HEAD 0
#define IO_ERROR 9
 
#define DMX_DATA_LENGTH 513 //start code + data

namespace NxDevice_Namespace {

struct NxDeviceDmxUtil
{
public:
	unsigned int DmxDevice;
	unsigned char DmxData[DMX_DATA_LENGTH];

	bool mUsePro;
	HANDLE mHandleThread;
	bool mThreadRunning;

	//Pro Enttec
	FT_HANDLE device_handle;
	DMXUSBPROParamsType PRO_Params;

	NxDeviceDmxUtil() : DmxDevice(0)
	{
		mUsePro = false;
		mThreadRunning = true;	 
	}

	~NxDeviceDmxUtil()
	{

	}
}; 

int FTDI_SendData( NxDeviceDmxUtil * mDevice, int label, unsigned char *data, int length)
{
	unsigned char end_code = DMX_END_CODE;
	FT_STATUS res = 0;
	DWORD bytes_to_write = length;
	DWORD bytes_written = 0;
	HANDLE event = NULL;
	int size=0;
	// Form Packet Header
	unsigned char header[DMX_HEADER_LENGTH];
	header[0] = DMX_START_CODE;
	header[1] = label;
	header[2] = length & OFFSET;
	header[3] = length >> BYTE_LENGTH;
	// Write The Header
	res = FT_Write(	mDevice->device_handle,(unsigned char *)header,DMX_HEADER_LENGTH,&bytes_written);
	if (bytes_written != DMX_HEADER_LENGTH) return  NO_RESPONSE;
	// Write The Data
	res = FT_Write(	mDevice->device_handle,(unsigned char *)data,length,&bytes_written);
	if (bytes_written != length) return  NO_RESPONSE;
	// Write End Code
	res = FT_Write(	mDevice->device_handle,(unsigned char *)&end_code,ONE_BYTE,&bytes_written);
	if (bytes_written != ONE_BYTE) return  NO_RESPONSE;
	if (res == FT_OK)
		return TRUE;
	else
		return FALSE; 
}

/* Function : FTDI_ReceiveData
 * Author	: ENTTEC
 * Purpose  : Receive Data (DMX or other packets) from the PRO
 * Parameters: Label, Pointer to Data Structure, Length of Data
 **/
int FTDI_ReceiveData(NxDeviceDmxUtil * mDevice, int label, unsigned char *data, unsigned int expected_length)
{
	FT_STATUS res = 0;
	DWORD length = 0;
	DWORD bytes_to_read = 1;
	DWORD bytes_read =0;
	unsigned char byte = 0;
	HANDLE event = NULL;
	char buffer[600];
	// Check for Start Code and matching Label
	while (byte != label)
	{
		while (byte != DMX_START_CODE)
		{
			res = FT_Read(mDevice->device_handle,(unsigned char *)&byte,ONE_BYTE,&bytes_read);
			if(bytes_read== NO_RESPONSE) return  NO_RESPONSE;
		}
		res = FT_Read(mDevice->device_handle,(unsigned char *)&byte,ONE_BYTE,&bytes_read);
		if (bytes_read== NO_RESPONSE) return  NO_RESPONSE;
	}
	// Read the rest of the Header Byte by Byte -- Get Length
	res = FT_Read(mDevice->device_handle,(unsigned char *)&byte,ONE_BYTE,&bytes_read);
	if (bytes_read== NO_RESPONSE) return  NO_RESPONSE;
	length = byte;
	res = FT_Read(mDevice->device_handle,(unsigned char *)&byte,ONE_BYTE,&bytes_read);
	if (res != FT_OK) return  NO_RESPONSE;
	length += ((unsigned long)byte)<<BYTE_LENGTH;	
	// Check Length is not greater than allowed
	if (length > DMX_PACKET_SIZE)
		return  NO_RESPONSE;
	// Read the actual Response Data
	res = FT_Read(mDevice->device_handle,buffer,length,&bytes_read);
	if(bytes_read != length) return  NO_RESPONSE;
	// Check The End Code
	res = FT_Read(mDevice->device_handle,(unsigned char *)&byte,ONE_BYTE,&bytes_read);
	if(bytes_read== NO_RESPONSE) return  NO_RESPONSE;
	if (byte != DMX_END_CODE) return  NO_RESPONSE;
	// Copy The Data read to the buffer passed
	memcpy(data,buffer,expected_length);
	return TRUE;
}

void FTDI_ClosePort( NxDeviceDmxUtil * mDevice )
{
	if (mDevice->device_handle != NULL) FT_Close(mDevice->device_handle);
}

void FTDI_PurgeBuffer( NxDeviceDmxUtil * mDevice )
{
	FT_Purge (mDevice->device_handle,FT_PURGE_TX);
	FT_Purge (mDevice->device_handle,FT_PURGE_RX);
}

NxDeviceDmx::NxDeviceDmx()
{
	GenerateID();
	mDevice = new NxDeviceDmxUtil();
}

NxDeviceDmx::~NxDeviceDmx()
{
	delete mDevice;
	mDevice = NULL;
}

bool NxDeviceDmx::loadPropertySheet()
{
	return true ;
}

bool NxDeviceDmx::SavePropertySheet()
{
	return true;
}

DWORD WINAPI DmxSenderThread( void * param )
{
	NxDeviceDmx * Ptr = ( NxDeviceDmx *  ) param;
	while( Ptr->mDevice->mThreadRunning )
	{
		FTDI_SendData( Ptr->mDevice, SET_DMX_TX_MODE, Ptr->mDevice->DmxData, DMX_DATA_LENGTH); 
		Sleep(25);
	}
	return true;
}

bool NxDeviceDmx::ConnectDevice()
{
	int RTimeout =120;
	int WTimeout =100;

	int VersionMSB =0;
	int VersionLSB =0;
	unsigned char temp[4];
	long version;
	unsigned char major_ver,minor_ver,build_ver;
	int recvd =0;
	unsigned char byte = 0;
	int size = 0;
	int res = 0;
	int tries =0;
	unsigned char latencyTimer;
	FT_STATUS ftStatus;
	int BreakTime;
	int MABTime;



	mDevice->mUsePro = false; // to do if Log("Waiting for GET_WIDGET_PARAMS_REPLY packet... "); is noresponse
	
	if( mDevice->mUsePro )
	{
		int device_num = 0;
		
		FT_STATUS ftStatus = FT_Open(device_num,&mDevice->device_handle);
		if( ftStatus == FT_OK )
		{
			Log(" opened Pro enttec Device");
		}
		else
		{
			Log(" Cant open Pro enttec Device");
		}

 
		// GET D2XX Driver Version
		ftStatus = FT_GetDriverVersion(mDevice->device_handle,(LPDWORD)&version);
		if (ftStatus == FT_OK) 
		{
			major_ver = (unsigned char) version >> 16;
			minor_ver = (unsigned char) version >> 8;
			build_ver = (unsigned char) version & 0xFF;
			//Log("\nD2XX Driver Version:: %02X.%02X.%02X ",major_ver,minor_ver,build_ver);
			Log("Found Driver Version");
		}
		else
			Log("Unable to Get D2XX Driver Version. Module Connected ?") ;

		// GET Latency Timer
		ftStatus = FT_GetLatencyTimer (mDevice->device_handle,(PUCHAR)&latencyTimer);
		if (ftStatus == FT_OK) 
			Log("Found Latency Timer"); //latencyTimer
		else
			Log("Unable to Get Latency Timer") ;
		// SET Default Read & Write Timeouts (in micro sec ~ 100)
		FT_SetTimeouts(mDevice->device_handle,RTimeout,WTimeout);
		// Piurges the buffer
		FT_Purge (mDevice->device_handle,FT_PURGE_RX);
		// Send Get Widget Parameters to get Device Info
		Log("Sending GET_WIDGET_PARAMS packet... ");
 		res = FTDI_SendData( mDevice, GET_WIDGET_PARAMS,(unsigned char *)&size,2);
		// Check Response
		if (res == NO_RESPONSE)
		{
			FT_Purge (mDevice->device_handle,FT_PURGE_TX);
 			res = FTDI_SendData( mDevice, GET_WIDGET_PARAMS,(unsigned char *)&size,2);
			if (res == NO_RESPONSE)
			{
				Log("no response");
				FTDI_ClosePort(mDevice);
				return  NO_RESPONSE;
			}
		}
		else
			Log("----PRO Connected Succesfully");
		// Receive Widget Response
		Log("Waiting for GET_WIDGET_PARAMS_REPLY packet... ");
		res=FTDI_ReceiveData(mDevice, GET_WIDGET_PARAMS_REPLY,(unsigned char *)&mDevice->PRO_Params,sizeof(DMXUSBPROParamsType));
		// Check Response
		if (res == NO_RESPONSE)
		{
			// Recive Widget Response packet
			res=FTDI_ReceiveData(mDevice, GET_WIDGET_PARAMS_REPLY,(unsigned char *)&mDevice->PRO_Params,sizeof(DMXUSBPROParamsType));
			if (res == NO_RESPONSE)
			{
				FTDI_ClosePort(mDevice);
				return  NO_RESPONSE;
			}
		}
		else
			Log("GET WIDGET REPLY Received ... ");
		// Firmware  Version
		VersionMSB = mDevice->PRO_Params.FirmwareMSB;
		VersionLSB = mDevice->PRO_Params.FirmwareLSB;
		// GET PRO's serial number 
		res = FTDI_SendData(mDevice, GET_WIDGET_SN,(unsigned char *)&size,2);
		res=FTDI_ReceiveData(mDevice, GET_WIDGET_SN,(unsigned char *)&temp,4);
		// Display All PRO Parametrs & Info avialable
		Log("\n-----------::PRO Connected [Information Follows]::------------");
		Log("FIRMWARE VERSION ok " ) ;// VersionMSB,VersionLSB);

		
		BreakTime = (int) (mDevice->PRO_Params.BreakTime * 10.67) + 100;
		Log("\n\t\t  BREAK TIME: %d micro sec ",BreakTime);
		MABTime = (int) (mDevice->PRO_Params.MaBTime * 10.67);
		Log("\n\t\t  MAB TIME: %d micro sec",MABTime);
		Log("\n\t\t  SEND REFRESH RATE: %d packets/sec",mDevice->PRO_Params.RefreshRate);

		//FT_SetLatencyTimer(lngHandle, 1)
		
		memset(mDevice->DmxData,0,DMX_DATA_LENGTH);//set all to 0;

		DWORD Id;
		mDevice->mHandleThread = CreateThread(NULL, NULL, DmxSenderThread, this, NULL, &Id);//create a thread to send periodiclly the data/ 25 fps
	}
	else
	{
		mDevice->DmxDevice = open((DMXINT)512, (DMXINT)25);
		if(isSimulation(mDevice->DmxDevice)) return false;
		memset(mDevice->DmxData,0,DMX_DATA_LENGTH-1);//set all to 0;
		quickLoad( mDevice->DmxDevice, mDevice->DmxData);	
	}

	return true;
}

bool NxDeviceDmx::DisconnectDevice()
{
	if( mDevice->mUsePro )
	{
		FTDI_ClosePort( mDevice ) ;
		mDevice->mThreadRunning = false;
		WaitForSingleObject( mDevice->mHandleThread, INFINITE);
		mDevice->mHandleThread = NULL;
	}
	else
	{
		close( mDevice->DmxDevice );
	}
	mDevice->DmxDevice = NULL ;
	return true;
}

void NxDeviceDmx::SendMessage( const unsigned int Channel, const unsigned char Value )
{
	if( mDevice->mUsePro )
	{
		mDevice->DmxData[Channel] = Value;
		mDevice->DmxData[0] = 0; // start code
	}
	else
	{
		setSingle( mDevice->DmxDevice, Channel-1, Value );	
	}
}

bool NxDeviceDmx::IsOpenDmxPro()
{
	return mDevice->mUsePro;
}

void NxDeviceDmx::SetGrandMaster( const unsigned char Level )
{
	if( !mDevice->mUsePro ) setGrandMaster( mDevice->DmxDevice, Level );
}

int NxDeviceDmx::GetNumDevices()
{
	FT_STATUS ftStatus;
	DWORD numDevs=0;
	ftStatus = FT_ListDevices((PVOID)&numDevs,NULL,FT_LIST_NUMBER_ONLY);
	if(ftStatus == FT_OK) return numDevs;
	return 0; 
}

} // namespace