/// \file
/// \brief 카메라 스트림 콜백 방식 이미지 수집 예제
/// \example GrabCallback.cpp

//**********************************************************************
// 이 예제는 SDK의 기본 사용법을 보여주는 간단한 데모입니다.
// 실행 전에 카메라 IP 주소와 PC의 IP 주소가 같은 네트워크 대역(서브넷)인지
// 확인하세요. 다를 경우, 카메라 클라이언트 소프트웨어로 IP를 맞춰주세요.
//**********************************************************************
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>
#include "IMVApi.h"

// 데이터 프레임 콜백 함수 - 카메라에서 프레임이 들어올 때마다 자동으로 호출됨
static void onGetFrame(IMV_Frame* pFrame, void* pUser)
{
	if (pFrame == NULL)
	{
		printf("pFrame is NULL\n");
		return;
	}

	printf("Get frame blockId = %llu\n", pFrame->frameInfo.blockId);

	return;
}

// ***********아래 함수들은 SDK와 직접 관련 없는 보조 함수들로, 장치 목록 출력에 사용됩니다 ***********
static void displayDeviceInfo(IMV_DeviceList deviceInfoList)
{
	IMV_DeviceInfo*	pDevInfo = NULL;
	unsigned int cameraIndex = 0;
	char vendorNameCat[11];
	char cameraNameCat[16];

	// 헤더 줄 출력
	printf("\nIdx Type Vendor     Model      S/N             DeviceUserID    IP Address    \n");
	printf("------------------------------------------------------------------------------\n");

	for (cameraIndex = 0; cameraIndex < deviceInfoList.nDevNum; cameraIndex++)
	{
		pDevInfo = &deviceInfoList.pDevInfo[cameraIndex];
		// 카메라 인덱스 번호 출력 (최대 3자리)
		printf("%-3d", cameraIndex + 1);

		// 카메라 인터페이스 타입 출력 (GigE, U3V, CL, PCIe)
		switch (pDevInfo->nCameraType)
		{
			case typeGigeCamera:printf(" GigE");break;
			case typeU3vCamera:printf(" U3V ");break;
			case typeCLCamera:printf(" CL  ");break;
			case typePCIeCamera:printf(" PCIe");break;
			default:printf("     ");break;
		}

		// 제조사 이름 출력 (최대 10자리, 초과 시 "..."으로 줄임)
		if (strlen(pDevInfo->vendorName) > 10)
		{
			memcpy(vendorNameCat, pDevInfo->vendorName, 7);
			vendorNameCat[7] = '\0';
			strcat(vendorNameCat, "...");
			printf(" %-10.10s", vendorNameCat);
		}
		else
		{
			printf(" %-10.10s", pDevInfo->vendorName);
		}

		// 카메라 모델명 출력 (최대 10자리)
		printf(" %-10.10s", pDevInfo->modelName);

		// 시리얼 번호 출력 (최대 15자리)
		printf(" %-15.15s", pDevInfo->serialNumber);

		// 사용자 지정 카메라 이름 출력 (최대 15자리, 초과 시 "..."으로 줄임)
		if (strlen(pDevInfo->cameraName) > 15)
		{
			memcpy(cameraNameCat, pDevInfo->cameraName, 12);
			cameraNameCat[12] = '\0';
			strcat(cameraNameCat, "...");
			printf(" %-15.15s", cameraNameCat);
		}
		else
		{
			printf(" %-15.15s", pDevInfo->cameraName);
		}

		// GigE 카메라인 경우 IP 주소 출력
		if (pDevInfo->nCameraType == typeGigeCamera)
		{
			printf(" %s", pDevInfo->DeviceSpecificInfo.gigeDeviceInfo.ipAddress);
		}

		printf("\n");
	}

	return;
}

static char* trim(char* pStr)
{
	char* pDst = pStr;
	char* pTemStr = NULL;

	if (pDst != NULL)
	{
		pTemStr = pDst + strlen(pStr) - 1;
		while (*pDst == ' ')
		{
			pDst++;
		}
		while ((pTemStr > pDst) && (*pTemStr == ' '))
		{
			*pTemStr-- = '\0';
		}
	}

	return pDst;
}

static int isInputValid(char* pInpuStr)
{
	char numChar;
	char* pStr = pInpuStr;
	while (*pStr != '\0')
	{
		numChar = *pStr;
		if ((numChar > '9') || (numChar < '0'))
		{
			return -1;
		}
		pStr++;
	}

	return 0;
}

static unsigned int selectDevice(unsigned int cameraCnt)
{
	char inputStr[256];
	char* pTrimStr;
	int inputIndex = -1;
	int ret = -1;
	char* find = NULL;

	printf("\nPlease input the camera index: ");
	while (1)
	{
		memset(inputStr, 0, sizeof(inputStr));
		fgets(inputStr, sizeof(inputStr), stdin);
		// 입력 버퍼 비우기
		fflush(stdin);

		// fgets는 개행문자('\n')까지 읽으므로 제거해줌
		find = strchr(inputStr, '\n');
		if (find) { *find = '\0'; }

		pTrimStr = trim(inputStr);
		ret = isInputValid(pTrimStr);
		if (ret == 0)
		{
			inputIndex = atoi(pTrimStr);
			// 사용자 입력은 1부터 시작하므로 0 기반 인덱스로 변환
			inputIndex -= 1;
			if ((inputIndex >= 0) && (inputIndex < (int)cameraCnt))
			{
				break;
			}
		}

		printf("Input invalid! Please input the camera index: ");
	}

	return (unsigned int)inputIndex;
}

// ***********보조 함수 끝 ***********

int main()
{
	int ret = IMV_OK;
	unsigned int cameraIndex = 0;
	IMV_HANDLE devHandle = NULL;

	// 연결 가능한 카메라 장치 탐색
	IMV_DeviceList deviceInfoList;
	ret = IMV_EnumDevices(&deviceInfoList, interfaceTypeAll);
	if (IMV_OK != ret)
	{
		printf("Enumeration devices failed! ErrorCode[%d]\n", ret);
		getchar();
		return -1;
	}

	if (deviceInfoList.nDevNum < 1)
	{
		printf("no camera\n");
		getchar();
		return -1;
	}

	// 카메라 기본 정보 출력 (인덱스, 타입, 제조사, 모델명, 시리얼번호, 사용자ID, IP주소)
	displayDeviceInfo(deviceInfoList);

	// 연결할 카메라 선택
	cameraIndex = selectDevice(deviceInfoList.nDevNum);

	do
	{
		// 카메라 핸들 생성 (이후 모든 API 호출에 이 핸들을 사용)
		ret = IMV_CreateHandle(&devHandle, modeByIndex, (void*)&cameraIndex);
		if (IMV_OK != ret)
		{
			printf("Create devHandle failed! ErrorCode[%d]\n", ret);
			break;
		}

		// 카메라 열기
		ret = IMV_Open(devHandle);
		if (IMV_OK != ret)
		{
			printf("Open camera failed! ErrorCode[%d]\n", ret);
			break;
		}

		// 프레임 수신 콜백 함수 등록 (프레임 도착 시 onGetFrame 자동 호출)
		ret = IMV_AttachGrabbing(devHandle, onGetFrame, NULL);
		if (IMV_OK != ret)
		{
			printf("Attach grabbing failed! ErrorCode[%d]\n", ret);
			break;
		}

		// 트리거 모드 Off 설정 (연속 촬영 모드로 전환)
		ret = IMV_SetEnumFeatureSymbol(devHandle, "TriggerMode", "Off");
		if (IMV_OK != ret)
		{
			printf("Set triggerMode value failed! ErrorCode[%d]\n", ret);
			break;
		}
	
		// 이미지 스트림 수집 시작
		ret = IMV_StartGrabbing(devHandle);
		if (IMV_OK != ret)
		{
			printf("Start grabbing failed! ErrorCode[%d]\n", ret);
			break;
		}

		// 2초 동안 프레임 수집
		Sleep(2000);

		// 이미지 스트림 수집 중지
		ret = IMV_StopGrabbing(devHandle);
		if (IMV_OK != ret)
		{
			printf("Stop grabbing failed! ErrorCode[%d]\n", ret);
			break;
		}

		// 카메라 닫기
		ret = IMV_Close(devHandle);
		if (IMV_OK != ret)
		{
			printf("Close camera failed! ErrorCode[%d]\n", ret);
			break;
		}
	} while (false);

	if (devHandle != NULL)
	{
		// 카메라 핸들 해제 (메모리 정리)
		IMV_DestroyHandle(devHandle);
	}

	printf("Press enter key to exit...\n");
	getchar();

	return 0;
}
