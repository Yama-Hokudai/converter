#include <caio.h>
#include <unistd.h>
#include <string>
#include <converter.hpp>

class converter{
  std::string DeviceName;
  std::string ErrorString;
  std::string funcName;
  long ret;
  short id;
  short range;
  short channels;
  long samplingTimes;
  long samplingCount;
  float data[5000];
  long status;

  converter(void);
  converter~(void);
  void converter::errorHandle(void){
    if(ret != 0){
      char *errorString;
      AioGetErrorString(ret, errorString);
      ErrorString = errorString;
      std::cout << funcName << ": " << ErrorString << std::endl;
      std::cout << "強制終了します" << std::endl;
      sleep(10);
      exit(1);
    }
  }
  int converter::getDeviceName(char **name){
    char *tmp = DeviceName.c_str();
    int size = DeviceName.length();
    *name = new char[size];

    for(int i = 0; i < size; i++){
      (*name)[i] = tmp[i];
    }
    return size;
  }
  int converter::getErrorString(char **errorString){
    char *tmp = ErrorString.c_str();
    int size = ErrorString.length();
    *errorString = new char[size];

    for(int i = 0; i < size; i++){
      (*errorString)[i] = tmp[i];
    }
    return size;
  }
}
// short initDAC(char *DeviceName, short AoRange, short AoChannels, long AoSamplingTimes, long AoSamplingCount);
// {
//   /* /\*   //関数の戻り値 *\/ */
//   /* /\*   long	Ret; *\/ */
//   /* /\*   //デバイス名 *\/ */
//   /* /\*   //char	DeviceName[] = "AIO001"; *\/ */
//   /* /\*   //ID *\/ */
//   /* /\*   short	Id; *\/ */
//   /* /\*   //エラーコード文字列 *\/ */
//   /* /\*   char	ErrorString[256]; *\/ */
//   /* /\*   //レンジ *\/ */

//   /* /\*   //使用チャネル数 *\/ */

//   /* /\*   //変換データ *\/ */
//   /* /\* #define	DATA_MAX	16000 *\/ */
//   /* /\*   float	AoData[1000]; *\/ */
//   /* /\*   //変換データ(電圧) *\/ */
//   /* /\*   float	AoVolt; *\/ */
//   /* /\*   //設定サンプリング回数 *\/ */

//   /* /\*   //現在のサンプリング回数 *\/ */

//   /* /\*   //現在のステータス *\/ */
//   /* /\*   long	AoStatus; *\/ */

//   /* /\*   long	i; *\/ */
//   /* /\*   short	j; *\/ */
