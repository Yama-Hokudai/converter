#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <math.h>
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
	short	AoRange;
	//使用チャネル数
	short	AoChannels;
	//変換データ
	#define	DATA_MAX	16000
	float	AoData[1000];
	//変換データ(電圧)
	float	AoVolt;
	//設定サンプリング回数
	long	AoSamplingTimes;
	//現在のサンプリング回数
	long	AoSamplingCount;
	//現在のステータス
	long	AoStatus;

	long	i;
	short	j;

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
	//	アナログ出力レンジの設定																			
	//--------------------------------------------------------------------------------------------------
	printf("\nアナログ出力レンジを以下の範囲から選択してください\n");
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
	scanf("%hd", &AoRange);
	printf("\n");

	//出力レンジの設定
	Ret = AioSetAoRangeAll(Id, AoRange);
	if(Ret != 0){
		AioGetErrorString(Ret, ErrorString);
		printf("AioSetAoRangeAll = %ld : %s\n\n", Ret, ErrorString);
		Ret = AioExit(Id);
		exit(0);
	}

	//--------------------------------------------------------------------------------------------------
	//	変換条件設定																					
	//--------------------------------------------------------------------------------------------------
	//チャネル数の設定：1チャネル
	AoChannels = 1;
	Ret = AioSetAoChannels(Id, AoChannels);
	if(Ret != 0){
		AioGetErrorString(Ret, ErrorString);
		printf("AioSetAoChannels = %ld : %s\n\n", Ret, ErrorString);
		Ret = AioExit(Id);
		exit(0);
	}

	//メモリ形式の設定：FIFO
	Ret = AioSetAoMemoryType(Id, 0);
	if(Ret != 0){
		AioGetErrorString(Ret, ErrorString);
		printf("AioSetAoMemoryType = %ld : %s\n\n", Ret, ErrorString);
		Ret = AioExit(Id);
		exit(0);
	}

	//クロック種類の設定：内部
	Ret = AioSetAoClockType(Id, 0);
	if(Ret != 0){
		AioGetErrorString(Ret, ErrorString);
		printf("AioSetAoClockType = %ld : %s\n\n", Ret, ErrorString);
		Ret = AioExit(Id);
		exit(0);
	}

	//変換速度の設定：1000usec
	Ret = AioSetAoSamplingClock(Id, 1000);
	if(Ret != 0){
		AioGetErrorString(Ret, ErrorString);
		printf("AioSetAoSamplingClock = %ld : %s\n\n", Ret, ErrorString);
		Ret = AioExit(Id);
		exit(0);
	}

	//開始条件の設定：ソフトウェア
	Ret = AioSetAoStartTrigger(Id, 0);
	if(Ret != 0){
		AioGetErrorString(Ret, ErrorString);
		printf("AioSetAoStartTrigger = %ld : %s\n\n", Ret, ErrorString);
		Ret = AioExit(Id);
		exit(0);
	}

	//停止条件の設定：設定回数変換終了
	Ret = AioSetAoStopTrigger(Id, 0);
	if(Ret != 0){
		AioGetErrorString(Ret, ErrorString);
		printf("AioSetAoStopTrigger = %ld : %s\n\n", Ret, ErrorString);
		Ret = AioExit(Id);
		exit(0);
	}

	//メモリのリセット
	Ret = AioResetAoMemory(Id);
	if(Ret != 0){
		AioGetErrorString(Ret, ErrorString);
		printf("AioResetAoMemory = %ld : %s\n\n", Ret, ErrorString);
		Ret = AioExit(Id);
		exit(0);
	}

	//出力サンプリング回数がデータ格納配列サイズを上回らないよう調整
	AoSamplingTimes = 1000;
	if(AoSamplingTimes * AoChannels > DATA_MAX){
		AoSamplingTimes = DATA_MAX / AoChannels;
	}

	//出力データ作成：サイン波形
	for(i = 0 ; i < AoSamplingTimes ; i++){
		AoVolt = (float)(5 * sin(2 * 3.14159265358979 * i / AoSamplingTimes));
		for(j = 0 ; j < AoChannels ; j++){
			AoData[i * AoChannels + j] = AoVolt;
		}
	}

	//出力データ、サンプリング回数の設定：1000回
	Ret = AioSetAoSamplingDataEx(Id, AoSamplingTimes, &AoData[0]);
	if(Ret != 0){
		AioGetErrorString(Ret, ErrorString);
		printf("AioSetAoSamplingDataEx = %ld : %s\n\n", Ret, ErrorString);
		Ret = AioExit(Id);
		exit(0);
	}
	printf("変換条件設定 : 正常終了\n何かキーを押すと変換を開始します\n\n");
	while(!kbhit());

	//--------------------------------------------------------------------------------------------------
	//	変換開始																						
	//--------------------------------------------------------------------------------------------------
	Ret = AioStartAo(Id);
	if(Ret != 0){
		AioGetErrorString(Ret, ErrorString);
		printf("AioStartAo = %ld : %s\n\n", Ret, ErrorString);
		Ret = AioExit(Id);
		exit(0);
	}
	printf("変換開始、何かキーを押すと変換を停止します\n\n");

	//--------------------------------------------------------------------------------------------------
	//	変換状態取得																					
	//--------------------------------------------------------------------------------------------------
	do{
		//ステータスの取得
		Ret = AioGetAoStatus(Id, &AoStatus);
		if(Ret != 0){
			AioGetErrorString(Ret, ErrorString);
			printf("AioGetAoStatus = %ld : %s\n\n", Ret, ErrorString);
			Ret = AioExit(Id);
			exit(0);
		}

		//出力済サンプリング数の取得
		Ret = AioGetAoSamplingCount(Id, &AoSamplingCount);
		if(Ret != 0){
			AioGetErrorString(Ret, ErrorString);
			printf("AioGetAoSamplingCount = %ld : %s\n\n", Ret, ErrorString);
			Ret = AioExit(Id);
			exit(0);
		}

		//メモリ内未出力サンプリング数の取得
		Ret = AioGetAoSamplingTimes(Id, &AoSamplingTimes);
		if(Ret != 0){
			AioGetErrorString(Ret, ErrorString);
			printf("AioGetAoSamplingTimes = %ld : %s\n\n", Ret, ErrorString);
			Ret = AioExit(Id);
			exit(0);
		}

		//変換データの設定
		if(AoSamplingTimes < 500){
			Ret = AioSetAoSamplingDataEx(Id, 1000, &AoData[0]);
			if(Ret != 0){
				AioGetErrorString(Ret, ErrorString);
				printf("AioSetAoSamplingDataEx = %ld : %s\n\n", Ret, ErrorString);
				Ret = AioExit(Id);
				exit(0);
			}
		}
		printf("出力済サンプリング回数 : %4ld 未出力サンプリング回数 : %4ld ステータス : %xH\r"
			,AoSamplingCount, AoSamplingTimes, (unsigned int)AoStatus);

	}while(((AoStatus & AOS_BUSY) == AOS_BUSY) && (!kbhit()));

	//--------------------------------------------------------------------------------------------------
	//	変換終了																						
	//--------------------------------------------------------------------------------------------------
	printf("\n\n変換終了\n");

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