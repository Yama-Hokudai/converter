// Tracking2.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//

#define _USE_MATH_DEFINES
#define DAC (char*)"Aio000"
#define ADC (char*)"Aio001"
#include <Windows.h>"
#include <atlstr.h>
#include <iomanip>
#include <iostream>
#include <bitset>
#include <fstream>
#include <sstream>
#include <thread>
#include <cstdlib>
#include <cmath>
#include "Caio.h"

float debugTmp = -1;
bool dbgFlag = true;
//initDAで使うAioResetProcessはデバッグ時だけ
void errHandle(long ret, char* FuncName);
short initDAC(char* DeviceName, short AoChannels, short AoTransferMode,
			  short AoMemoryType, short AoClockType, float AoSamplingClock,
			  short AoStartTrigger, short AoStopTrigger, long AoSamplingTimes);

short initADC(char *DeviceName, short AiInputMethod, short AiChannels,
			  short AiRange, short AiTransferMode, short AiMemoryType,
			  float AiSamplingClock, short AiStartTrigger, short AiStopTrigger,
			  long AiStopDelayTimes, long AiRepeatTimes);

long _stdcall CallBackProcDA(short ID, short Message, WPARAM wParam, LPARAM lParam, void *Param);
//long _stdcall CallBackProcAD(short ID, short Message, WPARAM wParam, LPARAM lParam, void *Param);

void dataRegist(short ID, float** DataFrm, short channels, int size);

//void ListCorrelate(float *kernel, float *list, long kSize, long lSize, float *outList, long outSize);
void ComplexSine(int freq, long size, float *re, float *im);
float syncPattern(int freq1, int freq2, float phase1, float phase2, long t);

//ADC
void errHandle(long ret, char* FuncName) {
	char ErrorString[256];
	if (ret != 0) {
		AioGetErrorString(ret, ErrorString);
		std::cout << FuncName << ErrorString << std::endl;
		std::cout << "強制終了します。" << std::endl;
		Sleep(10000);
		exit(1);
	}
}

short initDAC(char* DeviceName, short AoChannels = 4, short AoTransferMode = 0,
			  short AoMemoryType = 0, short AoClockType = 0, float AoSamplingClock = 20,
			  short AoStartTrigger = 0, short AoStopTrigger = 0, long AoSamplingTimes = 50) {
	//ここまで関数宣言

	long ret = 0;
	short ID;

	ret = AioInit(DeviceName, &ID);
	errHandle(ret, (char*)"AioInit");

	if (dbgFlag == true) {
		ret = AioResetProcess(ID);
		errHandle(ret, (char*)"AioResetProcess");
	}

	ret = AioResetDevice(ID);
	errHandle(ret, (char*)"AioResetDevice");

	ret = AioSetAoChannels(ID, AoChannels);
	errHandle(ret, (char*)"AioSetAoChannels");

	ret = AioSetAoTransferMode(ID, AoTransferMode);
	errHandle(ret, (char*)"AioSetAoTransferMode");

	ret = AioSetAoMemoryType(ID, AoMemoryType);
	errHandle(ret, (char*)"AioSetAoMemoryType");

	ret = AioResetAoMemory(ID);
	errHandle(ret, (char*)"AioReseetAoMemory");

	ret = AioSetAoClockType(ID, AoClockType);
	errHandle(ret, (char*)"AioSetAoClockType");

	ret = AioSetAoSamplingClock(ID, AoSamplingClock);
	errHandle(ret, (char*)"AioSetAoSamplingClock");

	ret = AioSetAoStartTrigger(ID, AoStartTrigger);
	errHandle(ret, (char*)"AioSetAoStartTrigger");

	ret = AioSetAoStopTrigger(ID, AoStopTrigger);
	errHandle(ret, (char*)"AioSetAoStopTrigger");

	ret = AioSetAoCallBackProc(ID, &CallBackProcDA, AOE_END | AOE_DATA_NUM, NULL);
	errHandle(ret, (char*)"AioSetAoCallBackProc");

	ret = AioSetAoEventSamplingTimes(ID, AoSamplingTimes);
	errHandle(ret, (char*)"AioSetAoEventSamplingTimes");

	return ID;
}

short initADC(char *DeviceName, short AiInputMethod = 0, short AiChannels = 1,
			  short AiRange = PM25, short AiTransferMode = 0, short AiMemoryType = 1,
			  float AiSamplingClock = 20, short AiStartTrigger = 0, short AiStopTrigger = 4,
			  long AiStopDelayTimes = 0, long AiRepeatTimes = 0) {
	long ret = -1;
	short ID;

	ret = AioInit(DeviceName, &ID);
	errHandle(ret, (char*)"AioInit");

	if (dbgFlag == true) {
		ret = AioResetProcess(ID);
		errHandle(ret, (char*)"AioResetProcess");
	}

	ret = AioResetDevice(ID);
	errHandle(ret, (char*)"AioResetDevice");


	ret = AioSetAiInputMethod(ID, AiInputMethod);
	errHandle(ret, (char*)"AioSetAiInputMethod");

	ret = AioSetAiChannels(ID, AiChannels);
	errHandle(ret, (char*)"AioSetAiChannels");

	ret = AioSetAiRangeAll(ID, AiRange);
	errHandle(ret, (char*)"AioSetAiRangeAll");

	ret = AioSetAiTransferMode(ID, AiTransferMode);
	errHandle(ret, (char*)"AioSetAiTransferMode");

	ret = AioSetAiMemoryType(ID, AiMemoryType);
	errHandle(ret, (char*)"AioSetAiMemoryType");

	ret = AioSetAiSamplingClock(ID, AiSamplingClock);
	errHandle(ret, (char*)"AioSetAiSamplingClock");

	ret = AioSetAiStartTrigger(ID, AiStartTrigger);
	errHandle(ret, (char*)"AioSetAiStartTrigger");

	ret = AioSetAiStopTrigger(ID, AiStopTrigger);
	errHandle(ret, (char*)"AioSetAiStopTrigger");

	ret = AioSetAiStopDelayTimes(ID, AiStopDelayTimes);
	errHandle(ret, (char*)"AioSetAiStopDelayTimes");

	ret = AioSetAiRepeatTimes(ID, AiRepeatTimes);
	errHandle(ret, (char*)"AioSetAiRepeatTimes");

	//ret = AioSetAiCallBackProc(ID, CallBackProcAD, AIE_OFERR, NULL);
//	errHandle(ret, (char*)"AioSetAiCallBackProc");

	ret = AioResetAiMemory(ID);
	errHandle(ret, (char*)"AioResetAiMemory");

	return ID;
}


long __stdcall CallBackProcDA(short ID, short Message, WPARAM wParam, LPARAM lParam, void *Param) {
	switch (Message) {
	case AIOM_AOE_END:
		std::cout << "\n\nデバイス動作終了イベント発生\n" << std::endl;
		break;
	case AIOM_AOE_DATA_NUM:
		std::cout << "+";
		//		dataRegist(ID, (float**)Param, 4, 2000);
	}
	return 0;
}
//long _stdcall CallBackProcAD(short ID, short Message, WPARAM wParam, LPARAM lParam, void *Param) {
//	std::cout << "A/DC Device Buffer Over Flow." << std::endl;
//	exit(1);
//	return 0;
//}

void dataRegist(short ID, float** DataFrm, short channels, int size) {
	long ret = -1;
	char ErrorString[256];

	float *tmp = new float[channels*size];/* (float*)malloc(sizeof(float) * channels * size);*/
	
	for (int i = 0; i < size; i++) {
		for (short j = 0; j < channels; j++) {
			tmp[j + channels * i] = DataFrm[j][i];
		}
	}
	int i = 0;
	while (true) {
		char ErrorString[256];
		ret = AioSetAoSamplingDataEx(ID, size, tmp);
		AioGetErrorString(ret, ErrorString);
		if (ret == 0) {
			//			std::cout << dataRegistCount << std::endl;
			//			dataRegistCount++;
			break;
		}
		else {
			//std::cout << "AioSetAoSamplingDataEx Error #" << i << std::endl;
			i++;
		}
	}
	delete[] tmp;
	if (ret != 0) {
		long cntOfSmpl;
		std::cout << "Some error caused" << std::endl;
		AioGetAoSamplingCount(ID, &cntOfSmpl);
		std::cout << "Count of sampling : " << cntOfSmpl << std::endl;
		AioGetErrorString(ret, ErrorString);
		std::cout << "AioSetAoSamplingDataEx: " << ErrorString << std::endl;
	}

}


//Numerical Computation
void ComplexSine(int freq, long size, double *re, double *im) {
	for (int i = 0; i < size; i++) {
		re[i] = std::cos(2 * freq * M_PI * (i - size / 2)/50000);
		im[i] = - std::sin(2 * freq * M_PI * (i - size / 2)/50000);
	}
}
/*void ListCorrelate(float *kernel, float *list, long kSize, long lSize, float *outList, long outSize) {

	if (outSize != lSize - kSize + 1) {
		std::cout << "ListCorrelate Failure." << std::endl;
		exit(1);
	}

	for (long i = 0; i < outSize; i++) {
		outList[i] = 0;
		for (long j = 0; j < kSize; j++) {
			outList[i] += kernel[j] * list[i + j];
		}
	}
}*/

float syncPattern(int freq1, int freq2, float phi, float theta, long t) {
	return (float)(1.0*(std::sin(freq1 * 2 * M_PI * t / 50000 + phi) + std::sin(freq2 * 2 * M_PI * t / 50000 + phi + theta)));
}


int main(void){

	//SYSTEMTIME st;
	//GetSystemTime(&st);
	//std::string strMessage;

	//std::ostringstream ossMessage;
	//ossMessage << st.wYear << "-"
	//	<< std::setw(2) << std::setfill('0') << st.wMonth << "-"
	//	<< std::setw(2) << std::setfill('0') << st.wDay << "-"
	//	<< std::setw(2) << std::setfill('0') << st.wHour << "-"
	//	<< std::setw(2) << std::setfill('0') << st.wMinute << "-"
	//	<< std::setw(2) << std::setfill('0') << st.wSecond;
	//strMessage = ossMessage.str();

	std::string filename;
	filename = "recDataFeb26.txt";
	std::ofstream recData(filename);

	std::string filename2;
	filename2 = "Feb26sym1_1.txt";
	std::ofstream sym1(filename2);

	std::string filename3;
	filename3 = "Feb26sym2_1.txt";
	std::ofstream sym2(filename3);

	std::string filename4;
	filename4 = "DeltaT1_2_5.txt";
	std::ofstream dt12(filename4);

	std::string filename5;
	filename5 = "DeltaT3_4_5.txt";
	std::ofstream dt34(filename5);

	int countDown = 10;
	for (int countDown = 3; countDown >= 0; countDown--) {
		std::cout << countDown << std::endl;
		Sleep(1000);
	}

	int freq[6][2] = { { 14750, 15250}, { 15750, 16250}, { 16750, 17250}, { 17750, 18250}, { 18750, 19250}, { 19750, 20250} };
	double amplitude[4] = { 5.0 , 5.0
		, 5.0, 5.0 };
	double amplitude2[4] = { 5.0, 2.5, 5.0, 2.5 };
	bool flg = true;
	long ret = -1;
	char *adc = ADC;
	char *dac = DAC;
//init each variants
	long signalSpan = 5000;
	long SpotComSize = signalSpan * 10;
	long TDMSize = 5000;// 40000;
	long tSize = 20 * 1024;
	long AoChannels = 4;
	short idADC = initADC(adc);
	short idDAC = initDAC(dac);

	float *AiData = new float[SpotComSize];
	float **AoData = new float*[AoChannels];
	for (int i = 0; i < AoChannels; i++) {
		AoData[i] = new float[tSize];
	}
	long baseLine[4] = { 0,15,100,115 };

	float phi[2][5] = {};
	long g = 0;
	for (int i = 0; i < 5; i++) {
		phi[1][i] = 2 * (float)M_PI * g / 5;
		if (phi[0][i] > (float)M_PI) {
			phi[0][i] -= 2 * (float)M_PI;
		} else if (phi[0][i] < -(float)M_PI) {
			phi[0][i] += 2 * (float)M_PI;
		}
		g += 3 - 5 * (i % 2);
	}
	float delTheta = (float)M_PI;
	float theta[5] = {0, M_PI/3, M_PI/3, M_PI, 5*M_PI/3};
	for (int i = 0; i < 5; i++) {
		//theta[i] = theta[i - 1] + delTheta;
		//if (theta[i] > (float)M_PI) {
		//	theta[i] -= 2 * (float)M_PI;
		//} else if (theta[i] < -(float)M_PI) {
		//	theta[i] += 2 * (float)M_PI;
		//}
		//outSym << theta[i] << std::endl;
	}
	//outSym.close();

	//prepare syncpattern & complex sine
	long T = 100;
	long T2 = 200;
	double ****ComSine = new double***[6];
	for (int i = 0; i < 6; i++) {
		ComSine[i] = new double**[2];
		for (int j = 0; j < 2; j++) {
			ComSine[i][j] = new double*[2];
			for (int k = 0; k < 2; k++) {
				ComSine[i][j][k] = new double[T];
			}
			ComplexSine(freq[i][j], T, ComSine[i][j][0], ComSine[i][j][1]);
		}
	}

	float **syncPatternList = new float*[6];
	for (int i = 0; i < 6; i++) {
		syncPatternList[i] = new float[T];
		for (int j = 0; j < T; j++) {
			syncPatternList[i][j] = syncPattern(freq[i][0], freq[i][1], (float)0, (float)0, j - T / 2);
		}
	}
	
	long AoSamplingTimes = -1;
	ret = AioGetAoSamplingTimes(idDAC, &AoSamplingTimes);
	std::cout <<"AoSamplingTimes :"<< AoSamplingTimes << std::endl;
	long t = 0;
	for (t = 0; t < tSize; t++) {
		for (int j = 0; j < AoChannels; j++) {
			if (t >= j * 600 + 50 && t < j * 600 + 150) {
				AoData[j][t] = amplitude[j] * syncPattern(freq[j][0], freq[j][1], (float)0, (float)0, t);
			}
			else {
				AoData[j][t] = 0;
			}
		}
		//outData1 << AoData[0][t] << std::endl;
		//outData2 << AoData[1][t] << std::endl;
		//outData3 << AoData[2][t] << std::endl;
		//outData4 << AoData[3][t] << std::endl;		
	}
	dataRegist(idDAC, AoData, AoChannels, tSize);

	ret = AioStartAi(idADC);
	errHandle(ret, (char*)"AioStartAi");
	ret = AioStartAo(idDAC);
	errHandle(ret, (char*)"AioStartAo");

	std::cout << "TDM-TDoA Positioning started. This will last for 5seconds." << std::endl;
	long AiSamplingCount = -1;
	long NextCount = TDMSize;
	long preCount = 0;
	long dt[4] = {};
	long ts[4] = {};
	int retrieveSpan[2] = { 30, 500 };
	std::thread TDoATransmission([&] {
		for (t = t; t < 50000 * 500; t++) {
			for (int j = 0; j < AoChannels; j++) {
				if (t % (TDMSize/2) >= j*600 + 50 && t % (TDMSize/2) < j*600 + 150) {
					AoData[j][t%tSize] = amplitude[j] * syncPattern(freq[j][0], freq[j][1], (float)0, (float)0, t);
				}
				else {
					AoData[j][t%tSize] = 0;
				}
			}
			if ((t + 1) % tSize == 0) {
				dataRegist(idDAC, AoData, AoChannels, tSize);
			}
			//outData1 << AoData[0][t%tSize] << std::endl;
			//outData2 << AoData[1][t%tSize] << std::endl;
			//outData3 << AoData[2][t%tSize] << std::endl;
			//outData4 << AoData[3][t%tSize] << std::endl;
//			std::cout << "動作中" << std::endl;
		}
	});

	long tmpCount = 0;
	std::thread TDoAReciever([&] {
		while(flg == true) {
			tmpCount++;
			while (true) {
				ret = AioGetAiSamplingCount(idADC, &AiSamplingCount);
				errHandle(ret, (char*)"AioGetAiSamplingCount");
				if (AiSamplingCount >= NextCount && (preCount < NextCount || AiSamplingCount < preCount)) {
					ret = AioGetAiSamplingDataEx(idADC, &TDMSize, AiData);
					errHandle(ret, (char*)"AioGetAiSamplingDataEx");
					preCount = AiSamplingCount;
					NextCount = (AiSamplingCount + TDMSize/2)%65536;
					break;
				}
			}

			//std::thread output([&] {
			//	for (int i = 0; i < TDMSize; i++) {
			//		tdm << AiData[i] << std::endl;
			//	}
			//});
			long maxIndex[4] = { -1,-1,-1,-1 };
			float maxCor = -1;
			for (int j = 0; j <= TDMSize / 2 - T; j++) {
				float tmp = 0;
				for (int k = 0; k < T; k++) {
					tmp += AiData[j + k] * syncPatternList[0][k];
				}
				if (std::abs(tmp) > maxCor) {
					maxIndex[0] = j;
					maxCor = std::abs(tmp);
				}
			}

			std::thread t2([&] {
				float maxCor2 = -1;
				for (int j = maxIndex[0]; j <= maxIndex[0] + TDMSize/2; j++) {
					float tmp = 0;
					for (int k = 0; k < T; k++) {
						tmp += AiData[j + k] * syncPatternList[1][k];
					}
					if (std::abs(tmp) > maxCor2) {
						maxIndex[1] = j;
						maxCor2 = std::abs(tmp);
					}
				}

				double tmpRe7 = 0;
				double tmpIm7 = 0;
				double tmpRe2 = 0;
				double tmpIm2 = 0;
				for (int j = 0; j < T; j++) {
					tmpRe7 += AiData[maxIndex[1] + j] * ComSine[1][0][0][j];
					tmpIm7 += AiData[maxIndex[1] + j] * ComSine[1][0][1][j];
					tmpRe2 += AiData[maxIndex[1] + j] * ComSine[1][1][0][j];
					tmpIm2 += AiData[maxIndex[1] + j] * ComSine[1][1][1][j];
					
				}
				dt[1] = (long)round(50 * (atan2(tmpRe7, -tmpIm7) - atan2(tmpRe2, -tmpIm2)) / M_PI);
				if (dt[1] > 50) {
					dt[1] -= 100;
				}
				else if (dt[1] < -50) {
					dt[1] += 100;
				}
				dt[1] += maxIndex[1];
			});
			std::thread t3([&] {
				float maxCor3 = -1;
				for (int j = maxIndex[0]; j <= maxIndex[0] + TDMSize / 2; j++) {
					float tmp = 0;
					for (int k = 0; k < T; k++) {
						tmp += AiData[j + k] * syncPatternList[2][k];
					}
					if (std::abs(tmp) > maxCor3) {
						maxIndex[2] = j;
						maxCor3 = std::abs(tmp);
					}
				}

				double tmpRe7 = 0;
				double tmpIm7 = 0;
				double tmpRe2 = 0;
				double tmpIm2 = 0;
				for (int j = 0; j < T; j++) {
					tmpRe7 += AiData[maxIndex[2] + j] * ComSine[2][0][0][j];
					tmpIm7 += AiData[maxIndex[2] + j] * ComSine[2][0][1][j];
					tmpRe2 += AiData[maxIndex[2] + j] * ComSine[2][1][0][j];
					tmpIm2 += AiData[maxIndex[2] + j] * ComSine[2][1][1][j];
				}
				dt[2] = (long)round(50 * (atan2(tmpRe7, -tmpIm7) - atan2(tmpRe2, -tmpIm2)) / M_PI);
				if (dt[2] > 50) {
					dt[2] -= 100;
				}
				else if (dt[2] < -50) {
					dt[2] += 100;
				}
				dt[2] += maxIndex[2];
			});
			std::thread t4([&] {
				float maxCor4 = -1;
				for (int j = maxIndex[0]; j <= maxIndex[0] + TDMSize / 2; j++) {
					float tmp = 0;
					for (int k = 0; k < T; k++) {
						tmp += AiData[j + k] * syncPatternList[3][k];
					}
					if (std::abs(tmp) > maxCor4) {
						maxIndex[3] = j;
						maxCor4 = std::abs(tmp);
					}
				}

				double tmpRe7 = 0;
				double tmpIm7 = 0;
				double tmpRe2 = 0;
				double tmpIm2 = 0;
				for (int j = 0; j < T; j++) {
					tmpRe7 += AiData[maxIndex[3] + j] * ComSine[3][0][0][j];
					tmpIm7 += AiData[maxIndex[3] + j] * ComSine[3][0][1][j];
					tmpRe2 += AiData[maxIndex[3] + j] * ComSine[3][1][0][j];
					tmpIm2 += AiData[maxIndex[3] + j] * ComSine[3][1][1][j];

				}
				dt[3] = (long)round(50 * (atan2(tmpRe7, -tmpIm7) - atan2(tmpRe2, -tmpIm2)) / M_PI);
				if (dt[3] > 50) {
					dt[3] -= 100;
				}
				else if (dt[3] < -50) {
					dt[3] += 100;
				}
				dt[3] += maxIndex[3];
			});

			double tmpRe7 = 0;
			double tmpIm7 = 0;
			double tmpRe2 = 0;
			double tmpIm2 = 0;
			for (int j = 0; j < T; j++) {
				tmpRe7 += AiData[maxIndex[0] + j] * ComSine[0][0][0][j];
				tmpIm7 += AiData[maxIndex[0] + j] * ComSine[0][0][1][j];
				tmpRe2 += AiData[maxIndex[0] + j] * ComSine[0][1][0][j];
				tmpIm2 += AiData[maxIndex[0] + j] * ComSine[0][1][1][j];
			}
			dt[0] = (long)round(50 * (atan2(tmpRe7, -tmpIm7) - atan2(tmpRe2, -tmpIm2)) / M_PI);
			if (dt[0] > 50) {
				dt[0] -= 100;
			}
			else if (dt[0] < -50) {
				dt[0] += 100;
			}
			dt[0] += maxIndex[0];

			t2.join();
			t3.join();
			t4.join();
//			output.join();
			for (int j = 3; j >= 0; j--) {			  
				dt[j] = dt[j] - dt[j - j % 2] - (j % 2) * 600;
			}
			for (int i = 0; i < AoChannels; i++) {
				std::cout << i << ":" << dt[i] << std::endl;
			}
			std::cout << std::endl;

			//for (int i = 0; i < AoChannels; i++) {
			//	std::cout << ts[i] << " : " << dt[i] << ":" << maxIndex[i] << std::endl;
			//}
			//std::cout << std::endl;
			//coord2 << ts[1] << std::endl;
			//coord3 << ts[2] << std::endl;
			//coord4 << ts[3] << std::endl;
			//outputfile1 << dt[0] << std::endl;
			//outputfile2 << dt[1] << std::endl;
			//outputfile3 << dt[2] << std::endl;
			//outputfile4 << dt[3] << std::endl;
		}
	});

	TDoATransmission.join();
	flg = false;
	TDoAReciever.join();
	std::cout << tmpCount << std::endl;
	ret = AioStopAo(idDAC);
	errHandle(ret, (char*)"AioStopAo");
	ret = AioResetAoMemory(idDAC);
	errHandle(ret, (char*)"AioResetAoMemory");

	//start of SpotCommunication
	ret = AioStopAi(idADC);
	errHandle(ret, (char*)"AioStpoAi");
	ret = AioResetAiMemory(idADC);
	errHandle(ret, (char*)"AioResetAiMemory");
	NextCount = SpotComSize;


	for (int j = 0; j < AoChannels; j++) {
		ts[j] = dt[j];
		std::cout << j << ": " << ts[j] << std::endl;
	}
	ts[0] = 0;
	ts[1] = -20;
	ts[2] = 0;
	ts[3] = -2;
	long symNum = 0;
	for (t = 0; t < tSize; t++) {
		for (int j = 0; j < AoChannels; j++) {
	/*		if (j != 2) {
				AoData[j][t%tSize] = 0;
			}else */if (t % signalSpan >= signalSpan/2 + 1200*( j- j%2)/2 + 650 - ts[j] - (long)(50 * theta[symNum % 5] / (float)M_PI) && t % signalSpan < signalSpan/2 + 1200*(j-j%2)/2 + 750 - ts[j] - (long)(50 * theta[symNum % 5] / (float)M_PI)) {//通信用。未完
				AoData[j][t%tSize] = amplitude2[j] * syncPattern(freq[(j - j % 2) / 2 + 4][0], freq[(j - j % 2) / 2 + 4][1], phi[j % 2][symNum % 5], theta[symNum % 5], t + ts[j]);
			}
			else if (t% signalSpan >= j * 600 + 50 && t%signalSpan < j * 600 + 150) {//測位用。未完
				AoData[j][t%tSize] = amplitude[j]*syncPattern(freq[j][0], freq[j][1], 0, 0, t);
			}
			else {
				AoData[j][t%tSize] = 0;
			}
		}
		if ((t+1) % signalSpan == 0) {
//			std::cout << t << ":" << symNum << std::endl;
			symNum++;
//			std::cout << symNum << "\n" << std::endl;
		}
	}

	dataRegist(idDAC, AoData, AoChannels, tSize);
	ret = AioStartAo(idDAC);
	errHandle(ret, (char*)"AioStartAo");
	ret = AioStartAi(idADC);
	errHandle(ret, (char*)"AioStartAi");
	long howLong = 60;
	long count = 0;
	std::cout << "SpotCommunication start and will last for " << howLong << "secs." << std::endl;
	std::thread SpotTransmission([&] {
		for (int i = symNum; i < howLong*50000/signalSpan; i++) {
			for (int j = 0; j < AoChannels; j++) {
				ts[j] = dt[j];
			}
			for (t = t; t < signalSpan * (i + 1); t++) {
				for (int j = 0; j < AoChannels; j++) {
					if (t % signalSpan >= signalSpan / 2 + 1200 * (j - j % 2) / 2 + 650 - ts[j] - (long)(50 * theta[symNum % 5] / (float)M_PI) && t % signalSpan < signalSpan / 2 + 1200 * (j - j % 2) / 2 + 750 - ts[j] - (long)(50 * theta[symNum % 5] / (float)M_PI)) {//通信用。未完
						AoData[j][t%tSize] = amplitude2[j] * syncPattern(freq[(j - j % 2) / 2 + 4][0], freq[(j - j % 2) / 2 + 4][1], phi[j % 2][symNum % 5], theta[symNum % 5], t + ts[j]);
					}
					else if (t% signalSpan >= j * 600 + 50 && t%signalSpan < j * 600 + 150) {//測位用。
						AoData[j][t%tSize] = amplitude[j] * syncPattern(freq[j][0], freq[j][1], 0, 0, t);
					}
					else {
						AoData[j][t%tSize] = 0;
					}
				}
				if ((t + 1) % signalSpan == 0) {
					//					std::cout << t << ":" << symNum << std::endl;
					symNum++;
					//					std::cout << symNum << "\n" << std::endl;
				}
				
				//				std::cout << std::endl;
//				outData1 << AoData[0][t%tSize] << std::endl;
//				outData2 << AoData[1][t%tSize] << std::endl;
				
				if ((t + 1) % tSize == 0) {
					dataRegist(idDAC, AoData, AoChannels, tSize);
				}
			}
		}
	});
	Sleep(2000);
	std::thread SpotComputation([&] {
		while (flg == false) {
			std::cout << count << std::endl;
			count++;
			while (true) {
				ret = AioGetAiSamplingCount(idADC, &AiSamplingCount);
				errHandle(ret, (char*)"AioGetAiSamplingCount");
				if (AiSamplingCount >= NextCount && (preCount < NextCount || AiSamplingCount < preCount)) {
					ret = AioGetAiSamplingDataEx(idADC, &SpotComSize, AiData);
					errHandle(ret, (char*)"AioGetAiSamplingDataEx");
					preCount = AiSamplingCount;
					NextCount = (AiSamplingCount + SpotComSize)%65536;
					break;
				}
			}
			std::thread output([&] {
				for (int i = 0; i < SpotComSize; i++) {
					recData << AiData[i] << std::endl;
				}
			});


			std::thread locCor([&] {
				long locPeakIndex[4] = { -1,-1,-1,-1 };
				float locPeak[4] = { -1,-1,-1,-1 };
				for (int i = 0; i <= signalSpan * 2; i++) {
					float tmp = 0;
					for (int j = 0; j < T; j++) {
						tmp += AiData[i + j] * syncPatternList[0][j];
					}
//					correlation << tmp << std::endl;
					if (std::abs(tmp) > locPeak[0]) {
						locPeakIndex[0] = i;
						locPeak[0] = std::abs(tmp);
					}
				}

				for (int i = 1; i < AoChannels; i++) {
					for (int j = 0; j < TDMSize / 2; j++) {
						float tmp = 0;
						for (int k = 0; k < T; k++) {
							tmp += AiData[locPeakIndex[0] + j + k] * syncPatternList[i][k];
						}
						if (std::abs(tmp) > locPeak[i]) {
							locPeakIndex[i] = j + locPeakIndex[0];
							locPeak[i] = std::abs(tmp);
						}
					}
				}


				double corSum[4][2][2] = {};
				for (int i = 0; i < T; i++) {
					for (int j = 0; j < AoChannels; j++) {
						for (int k = 0; k < 2; k++) {
							for (int l = 0; l < 2; l++) {
								corSum[j][k][l] += AiData[locPeakIndex[j] + i] * ComSine[j][k][l][i];
							}
						}
					}
				}

				long time[4] = {};
				for (int i = 0; i < AoChannels; i++) {
					double arg = atan2(corSum[i][0][0], -corSum[i][0][1]) - atan2(corSum[i][1][0], -corSum[i][1][1]);
					if (arg > M_PI) {
						arg -= 2 * M_PI;
					}
					else if (arg < -M_PI) {
						arg += 2 * M_PI;
					}
					time[i] = 50*round(arg / M_PI) + locPeakIndex[i];
				}

				for (int i = 0; i < AoChannels; i++) {
					dt[i] = time[i] - time[i - i % 2] - 600 * (i % 2);
					//std::cout << dt[i] << std::endl;
				}
				dt[0] = 0;
				dt[1] = -20;
				dt[2] = 0;
				dt[3] = -2;
				dt12 << dt[1] << std::endl;
				dt34 << dt[3] << std::endl;
				std::cout << std::endl;
			});


			//std::thread deModu1([&] {
			//	double preArg = 0;
			//	long Epoch = 0;
			//	float SpotPeak = -1;
			//	long SpotPeakIndex = -1;
			//	for (int j = 0; j <= signalSpan - T; j++) {
			//		float tmp = 0;
			//		for (int k = 0; k < T; k++) {
			//			tmp += AiData[j + k] * syncPatternList[4][k];
			//		}
			//		if (std::abs(tmp) > SpotPeak) {
			//			SpotPeakIndex = j;
			//			SpotPeak = std::abs(tmp);
			//		}
			//	}
			//	long corSum[2][2] = {};
			//	for (int j = 0; j < T; j++) {
			//		for (int k = 0; k < 2; k++) {
			//			for (int l = 0; l < 2; l++) {
			//				corSum[k][l] += AiData[SpotPeakIndex + j] * ComSine[4][k][l][j];
			//			}
			//		}
			//	}
			//	double arg = atan2(corSum[0][0], -corSum[0][1]) - atan2(corSum[1][0], corSum[1][1]);
			//	if (arg > M_PI) {
			//		arg -= 2 * M_PI;
			//	}
			//	else if (arg < -M_PI) {
			//		arg += 2 * M_PI;
			//	}
			//	Epoch = (long)round(50 * arg / M_PI);
			//	Epoch += SpotPeakIndex;				
			//	for (int i = 1; i < 10; i++) {
			//		//float SpotPeak = -1;
			//		//long SpotPeakIndex = -1;
			//		//for (int j = 0; j <= signalSpan - T; j++) {
			//		//	float tmp = 0;
			//		//	for (int k = 0; k < T; k++) {
			//		//		tmp += AiData[i * signalSpan + j + k] * syncPatternList[4][k];
			//		//	}
			//		//	if (std::abs(tmp) > SpotPeak) {
			//		//		SpotPeakIndex = i * signalSpan + j;
			//		//		SpotPeak = std::abs(tmp);
			//		//	}
			//		//}

			//		long corSum[2][2] = {};
			//		for (int j = 0; j < T; j++) {
			//			for (int k = 0; k < 2; k++) {
			//				for (int l = 0; l < 2; l++) {
			//					corSum[k][l] += AiData[Epoch + i * 5000 + j] * ComSine[4][k][l][j];
			//				}
			//			}
			//		}
			//		double arg = atan2(corSum[0][0], -corSum[0][1]) - atan2(corSum[1][0], corSum[1][1]);
			//		std::cout << arg << std::endl;
			//		if (arg > M_PI) {
			//			arg -= 2 * M_PI;
			//		}
			//		else if (arg < -M_PI) {
			//			arg += 2 * M_PI;
			//		}
			//		sym1 << arg << std::endl;
			//		preArg = arg;
			//	}
			//});
			std::thread deModu1([&] {
				long preEpoch = 0;
				long Epoch = 0;
				double symbol;
				for (int i = 0; i < 10; i++) {
					float SpotPeak = -1;
					long SpotPeakIndex = -1;
					for (int j = 0; j <= signalSpan - T; j++) {
						float tmp = 0;
						for (int k = 0; k < T; k++) {
							tmp += AiData[i * signalSpan + j + k] * syncPatternList[4][k];
						}
						if (std::abs(tmp) > SpotPeak) {
							SpotPeakIndex = i * signalSpan + j;
							SpotPeak = std::abs(tmp);
						}
					}

					long corSum[2][2] = {};
					for (int j = 0; j < T; j++) {
						for (int k = 0; k < 2; k++) {
							for (int l = 0; l < 2; l++) {
								corSum[k][l] += AiData[SpotPeakIndex + i * 5000 + j] * ComSine[4][k][l][j];
							}
						}
					}
					double arg = atan2(corSum[0][0], -corSum[0][1]) - atan2(corSum[1][0], -corSum[1][1]);
					if (arg > M_PI) {
						arg -= 2 * M_PI;
					}
					else if (arg < -M_PI) {
						arg += 2 * M_PI;
					}
					Epoch = SpotPeakIndex + 50 * round(arg) / M_PI;
					symbol = -M_PI * (Epoch - preEpoch - 5000) / 50;
					if (symbol > M_PI) {
						symbol -= 2 * M_PI;
					}
					else if (symbol < -M_PI) {
						symbol += 2 * M_PI;
					}
					if (symbol > -5 * M_PI / 6 && symbol <= -M_PI / 2) {
						symbol = -2 * M_PI / 3;
					}
					else if (symbol <= -M_PI / 6) {
						symbol = -M_PI / 3;
					}
					else if (symbol <= M_PI / 6) {
						symbol = 0;
					}
					else if (symbol <= M_PI / 2) {
						symbol = M_PI / 3;
					}
					else if (symbol <= 5 * M_PI / 6) {
						symbol = 2 * M_PI / 3;
					}
					else {
						symbol = M_PI;
					}
					sym1 << symbol << std::endl;
					preEpoch = Epoch;
				}
			});

			std::thread deModu2([&] {
				long preEpoch = 0;
				long Epoch = 0;
				double symbol = 0;
				for (int i = 0; i < 10; i++) {
					float SpotPeak = -1;
					long SpotPeakIndex = -1;
					for (int j = 0; j <= signalSpan - T; j++) {
						float tmp = 0;
						for (int k = 0; k < T; k++) {
							tmp += AiData[i * signalSpan + j + k] * syncPatternList[5][k];
						}
						if (std::abs(tmp) > SpotPeak) {
							SpotPeakIndex = i * signalSpan + j;
							SpotPeak = std::abs(tmp);
						}
					}

					long corSum[2][2] = {};
					for (int j = 0; j < T; j++) {
						for (int k = 0; k < 2; k++) {
							for (int l = 0; l < 2; l++) {
								corSum[k][l] += AiData[SpotPeakIndex + i * 5000 + j] * ComSine[5][k][l][j];
							}
						}
					}
					double arg = atan2(corSum[0][0], -corSum[0][1]) - atan2(corSum[1][0], -corSum[1][1]);
					if (arg > M_PI) {
						arg -= 2 * M_PI;
					}
					else if (arg < -M_PI) {
						arg += 2 * M_PI;
					}
					Epoch = SpotPeakIndex + 50 * round(arg) / M_PI;
					symbol = -M_PI * (Epoch - preEpoch - 5000) / 50;
					if (symbol > M_PI) {
						symbol -= 2 * M_PI;
					}
					else if (symbol < -M_PI) {
						symbol += 2 * M_PI;
					}
					if (symbol > -5 * M_PI / 6 && symbol <= -M_PI / 2) {
						symbol = -2 * M_PI / 3;
					}
					else if (symbol <= -M_PI / 6) {
						symbol = -M_PI / 3;
					}
					else if (symbol <= M_PI / 6) {
						symbol = 0;
					}
					else if (symbol <= M_PI / 2) {
						symbol = M_PI / 3;
					}
					else if (symbol <= 5 * M_PI / 6) {
						symbol = 2 * M_PI / 3;
					}
					else {
						symbol = M_PI;
					}
					sym2 << symbol << std::endl;
					preEpoch = Epoch;
				}

			//	double preArg = 0;
			//	long Epoch = 0;
			//	float SpotPeak = -1;
			//	long SpotPeakIndex = -1;
			//	for (int j = 0; j <= signalSpan - T; j++) {
			//		float tmp = 0;
			//		for (int k = 0; k < T; k++) {
			//			tmp += AiData[j + k] * syncPatternList[5][k];
			//		}
			//		if (std::abs(tmp) > SpotPeak) {
			//			SpotPeakIndex = j;
			//			SpotPeak = std::abs(tmp);
			//		}
			//	}
			//	long corSum[2][2] = {};
			//	for (int j = 0; j < T; j++) {
			//		for (int k = 0; k < 2; k++) {
			//			for (int l = 0; l < 2; l++) {
			//				corSum[k][l] += AiData[SpotPeakIndex + j] * ComSine[5][k][l][j];
			//			}
			//		}
			//	}
			//	double arg = atan2(corSum[0][0], -corSum[0][1]) - atan2(corSum[1][0], corSum[1][1]);
			//	if (arg > M_PI) {
			//		arg -= 2 * M_PI;
			//	}
			//	else if (arg < -M_PI) {
			//		arg += 2 * M_PI;
			//	}
			//	Epoch = (long)round(50 * arg / M_PI);
			//	Epoch += SpotPeakIndex;
			//	for (int i = 1; i < 10; i++) {
			//		//float SpotPeak = -1;
			//		//long SpotPeakIndex = -1;
			//		//for (int j = 0; j <= signalSpan - T; j++) {
			//		//	float tmp = 0;
			//		//	for (int k = 0; k < T; k++) {
			//		//		tmp += AiData[i * signalSpan + j + k] * syncPatternList[4][k];
			//		//	}
			//		//	if (std::abs(tmp) > SpotPeak) {
			//		//		SpotPeakIndex = i * signalSpan + j;
			//		//		SpotPeak = std::abs(tmp);
			//		//	}
			//		//}

			//		long corSum[2][2] = {};
			//		for (int j = 0; j < T; j++) {
			//			for (int k = 0; k < 2; k++) {
			//				for (int l = 0; l < 2; l++) {
			//					corSum[k][l] += AiData[Epoch + i * 5000 + j] * ComSine[5][k][l][j];
			//				}
			//			}
			//		}
			//		double arg = atan2(corSum[0][0], -corSum[0][1]) - atan2(corSum[1][0], corSum[1][1]);
			//		if (arg > M_PI) {
			//			arg -= 2 * M_PI;
			//		}
			//		else if (arg < -M_PI) {
			//			arg += 2 * M_PI;
			//		}
			//		sym2 << (arg - preArg) << std::endl;
			//		preArg = arg;
			//	}
			});
			locCor.join();
			deModu1.join();
			deModu2.join();
			output.join();
		}
	});

	SpotTransmission.join();
	flg = true;
	SpotComputation.join();
	recData.close();
	sym1.close();
	sym2.close();
	dt12.close();
	dt34.close();
	//end of SpotCommunication
	ret = AioStopAo(idDAC);
	errHandle(ret, (char*)"AioStopAo");
	ret = AioStopAi(idADC);
	errHandle(ret, (char*)"AioStopAi");
	delete[] AiData;
	std::cout << "AiData deleted" << std::endl;
	for (int i = 0; i < AoChannels; i++) {
		delete[] AoData[i];
	}
	delete[] AoData;
	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 2; j++) {
			for (int k = 0; k < 2; k++) {
				delete[] ComSine[i][j][k];
			}
			delete[] ComSine[i][j];
		}
		delete[] ComSine[i];
	}
	delete[] ComSine;

	for (int i = 0; i < 6; i++) {
		delete[] syncPatternList[i];
	}
	delete[] syncPatternList;
	

	return 0;
}


// プログラムの実行: Ctrl + F5 または [デバッグ] > [デバッグなしで開始] メニュー
// プログラムのデバッグ: F5 または [デバッグ] > [デバッグの開始] メニュー

// 作業を開始するためのヒント: 
//    1. ソリューション エクスプローラー ウィンドウを使用してファイルを追加/管理します 
//   2. チーム エクスプローラー ウィンドウを使用してソース管理に接続します
//   3. 出力ウィンドウを使用して、ビルド出力とその他のメッセージを表示します
//   4. エラー一覧ウィンドウを使用してエラーを表示します
//   5. [プロジェクト] > [新しい項目の追加] と移動して新しいコード ファイルを作成するか、[プロジェクト] > [既存の項目の追加] と移動して既存のコード ファイルをプロジェクトに追加します
//   6. 後ほどこのプロジェクトを再び開く場合、[ファイル] > [開く] > [プロジェクト] と移動して .sln ファイルを選択します
