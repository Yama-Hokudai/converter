#include <caio.h>
#include <string>

class converter{
  std::string DeviceName;
  std::string ErrorString;
  long ret;
  short id;
  short range;
  short channels;
  long samplingTimes;
  long samplingCount;
  float data[5000];
  long status;

  converter(void);
  void converter::errorHandle(void);
  char* converter::getDeviceName(void);
  char* converter::getErrorString(void);
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
