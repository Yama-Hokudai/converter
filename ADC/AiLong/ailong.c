#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
//AIOヘッダファイル
#include "caio.h"

int kbhit(void);

int main(void){

	//--------------------------------------------------------------------------------------------------
	//	変数宣言																						
	//--------------------------------------------------------------------------------------------------
	//関数の戻り値
	long	Ret;
	//デバイス名
	char	DeviceName[256];
	//ID
	short	Id;
	//エラーコード文字列
	char	ErrorString[256];
	//レンジ
	short	AiRange;
	//使用チャネル数
	short	AiChannels;
	//変換データ
	#define	DATA_MAX	16000
	float	AiData[DATA_MAX];
	//現在のサンプリング回数
	long	AiSamplingCount;
	//現在のステータス
	long	AiStatus;
	//総取得サンプリング回数
	long	AiTotalSamplingTimes = 0;

	//--------------------------------------------------------------------------------------------------
	//	デバイス名の入力確認																			
	//--------------------------------------------------------------------------------------------------
	printf("\nデバイス名を入力してください : ");
	scanf("%s", DeviceName);

	//--------------------------------------------------------------------------------------------------
	//	デバイスの初期化																				
	//--------------------------------------------------------------------------------------------------
	//初期化処理
	Ret = AioInit(DeviceName, &Id);
	if(Ret != 0){
		AioGetErrorString(Ret, ErrorString);
		printf("AioInit = %ld : %s\n\n", Ret, ErrorString);
		exit(0);
	}

	//デバイスのリセット
	Ret = AioResetDevice(Id);
	if(Ret != 0){
		AioGetErrorString(Ret, ErrorString);
		printf("AioResetDevice = %ld : %s\n\n", Ret, ErrorString);
		Ret = AioExit(Id);
		exit(0);
	}

	//--------------------------------------------------------------------------------------------------
	//	アナログ入力レンジの設定																			
	//--------------------------------------------------------------------------------------------------
	printf("\nアナログ入力レンジを以下の範囲から選択してください\n");
	printf("±10V\t\t : 0\t0～10V\t\t : 50\n");
	printf("±5V\t\t : 1\t0～5V\t\t : 51\n");
	printf("±2.5V\t\t : 2\t0～4.095V\t : 52\n");
	printf("±1.25V\t\t : 3\t0～2.5V\t\t : 53\n");
	printf("±1V\t\t : 4\t0～1.25V\t : 54\n");
	printf("±0.625V\t\t : 5\t0～1V\t\t : 55\n");
	printf("±0.5V\t\t : 6\t0～0.5V\t\t : 56\t\n");
	printf("±0.3125V\t : 7\t0～0.25V\t : 57\n");
	printf("±0.25V\t\t : 8\t0～0.1V\t\t : 58\n");
	printf("±0.125V\t\t : 9\t0～0.05V\t : 59\n");
	printf("±0.1V\t\t : 10\t0～0.025V\t : 60\n");
	printf("±0.05V\t\t : 11\t0～0.0125V\t : 61\n");
	printf("±0.025V\t\t : 12\t0～0.01V\t : 62\n");
	printf("±0.0125V\t : 13\t0～20mA\t\t : 100\n");
	printf("±0.01V\t\t : 14\t4～20mA\t\t : 101\n");
	printf("+/-20mA\t\t : 102\t1～5V\t\t : 150\n");
	scanf("%hd", &AiRange);
	printf("\n");

	//入力レンジの設定
	Ret = AioSetAiRangeAll(Id, AiRange);
	if(Ret != 0){
		AioGetErrorString(Ret, ErrorString);
		printf("AioSetAiRangeAll = %ld : %s\n\n", Ret, ErrorString);
		Ret = AioExit(Id);
		exit(0);
	}

	//--------------------------------------------------------------------------------------------------
	//	変換条件設定																					
	//--------------------------------------------------------------------------------------------------
	//チャネル数の設定：1チャネル
	AiChannels = 1;
	Ret = AioSetAiChannels(Id, AiChannels);
	if(Ret != 0){
		AioGetErrorString(Ret, ErrorString);
		printf("AioSetAiChannels = %ld : %s\n\n", Ret, ErrorString);
		Ret = AioExit(Id);
		exit(0);
	}

	//メモリ形式の設定：FIFO
	Ret = AioSetAiMemoryType(Id, 0);
	if(Ret != 0){
		AioGetErrorString(Ret, ErrorString);
		printf("AioSetAiMemoryType = %ld : %s\n\n", Ret, ErrorString);
		Ret = AioExit(Id);
		exit(0);
	}

	//クロック種類の設定：内部
	Ret = AioSetAiClockType(Id, 0);
	if(Ret != 0){
		AioGetErrorString(Ret, ErrorString);
		printf("AioSetAiClockType = %ld : %s\n\n", Ret, ErrorString);
		Ret = AioExit(Id);
		exit(0);
	}

	//変換速度の設定：1000usec
	Ret = AioSetAiSamplingClock(Id, 1000);
	if(Ret != 0){
		AioGetErrorString(Ret, ErrorString);
		printf("AioSetAiSamplingClock = %ld : %s\n\n", Ret, ErrorString);
		Ret = AioExit(Id);
		exit(0);
	}

	//開始条件の設定：ソフトウェア
	Ret = AioSetAiStartTrigger(Id, 0);
	if(Ret != 0){
		AioGetErrorString(Ret, ErrorString);
		printf("AioSetAiStartTrigger = %ld : %s\n\n", Ret, ErrorString);
		Ret = AioExit(Id);
		exit(0);
	}

	//停止条件の設定：コマンド
	Ret = AioSetAiStopTrigger(Id, 4);
	if(Ret != 0){
		AioGetErrorString(Ret, ErrorString);
		printf("AioSetAiStopTrigger = %ld : %s\n\n", Ret, ErrorString);
		Ret = AioExit(Id);
		exit(0);
	}

	//--------------------------------------------------------------------------------------------------
	//	変換開始																						
	//--------------------------------------------------------------------------------------------------
	//メモリのリセット
	Ret = AioResetAiMemory(Id);
	if(Ret != 0){
		AioGetErrorString(Ret, ErrorString);
		printf("AioResetAiMemory = %ld : %s\n\n", Ret, ErrorString);
		Ret = AioExit(Id);
		exit(0);
	}

	//変換開始
	Ret = AioStartAi(Id);
	if(Ret != 0){
		AioGetErrorString(Ret, ErrorString);
		printf("AioStartAi = %ld : %s\n\n", Ret, ErrorString);
		Ret = AioExit(Id);
		exit(0);
	}
	printf("変換開始、何かキーを押すと変換を停止します\n\n");

	//--------------------------------------------------------------------------------------------------
	//	変換状態取得																					
	//--------------------------------------------------------------------------------------------------
	do{
		//ステータスの取得
		Ret = AioGetAiStatus(Id, &AiStatus);
		if(Ret != 0){
			AioGetErrorString(Ret, ErrorString);
			printf("AioGetAiStatus = %ld : %s\n\n", Ret, ErrorString);
			Ret = AioExit(Id);
			exit(0);
		}

		//メモリ内格納サンプリング数の取得
		Ret = AioGetAiSamplingCount(Id, &AiSamplingCount);
		if(Ret != 0){
			AioGetErrorString(Ret, ErrorString);
			printf("AioGetAiSamplingCount = %ld : %s\n\n", Ret, ErrorString);
			Ret = AioExit(Id);
			exit(0);
		}

		//変換データの取得
		if(AiSamplingCount > 1000){
			//取得サンプリング数がデータ格納配列サイズを上回らないよう調整
			if(AiSamplingCount * AiChannels > DATA_MAX){
				AiSamplingCount = DATA_MAX / AiChannels;
			}
			Ret = AioGetAiSamplingDataEx(Id, &AiSamplingCount, &AiData[0]);
			if(Ret != 0){
				AioGetErrorString(Ret, ErrorString);
				printf("AioGetAiSamplingDataEx = %ld : %s\n\n", Ret, ErrorString);
				Ret = AioExit(Id);
				exit(0);
			}
			AiTotalSamplingTimes += AiSamplingCount;
		}
		printf("総取得サンプリング回数 : %6ld サンプリング回数 : %5ld ステータス : %2xH\r"
			,AiTotalSamplingTimes, AiSamplingCount, (unsigned int)AiStatus);

	}while(((AiStatus & AIS_BUSY) == AIS_BUSY) && (!kbhit()));

	//--------------------------------------------------------------------------------------------------
	//	変換停止																						
	//--------------------------------------------------------------------------------------------------
	printf("\n\n変換停止\n");
	Ret = AioStopAi(Id);
	if(Ret != 0){
		AioGetErrorString(Ret, ErrorString);
		printf("AioStopAi = %ld : %s\n\n", Ret, ErrorString);
		Ret = AioExit(Id);
		exit(0);
	}

	//--------------------------------------------------------------------------------------------------
	//	デバイスの終了																					
	//--------------------------------------------------------------------------------------------------
	Ret = AioExit(Id);
	if(Ret != 0){
		AioGetErrorString(Ret, ErrorString);
		printf("AioExit = %ld : %s\n\n", Ret, ErrorString);
		exit(0);
	}
	exit(0);
}


int kbhit(void)
{
	int ch;
	int oldf;
	struct termios oldt, newt;

	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
	fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
	setbuf(stdin,NULL);
	ch = getchar();
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	fcntl(STDIN_FILENO, F_SETFL, oldf);
	if(ch != EOF)
	{
		ungetc(ch, stdin);
		return 1;
	}
	return 0;
}
