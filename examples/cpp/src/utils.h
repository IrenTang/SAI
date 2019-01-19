#include <map>
#include <string>
#include <vector>
#include "INXInferenceEngine.h"

#define _NX_STICK_NUM_  "stickNum"
#define _NX_THREAD_NUM_ "threadNum"
#define _NX_DECODER_NUM_    "decoderNum"
#define _NX_MODEL_PATH_ "modelPath"
#define _NX_STICK_CNN_  "stickCNN"
#define _NX_HOST_NET_   "hostNet"
#define _NX_LABEL_NAME_ "labelName"
#define _NX_PROC_TYPE_	"procType"
#define _NX_NET_TYPE_   "netType"
#define _NX_ClASS_NUM_  "classNum"
#define _NX_FILELIST_	"fileList"
#define _NX_SG_BEGIN_ID_       "sgBeginID"
#define _NX_DELAY_TIME_       "delayTime"

void printfUsage(void);

std::map<std::string, std::string> ParseArgs(int argc, char *argv[]);

void GenerateEngineConfigFromCmdArgs(NXEngineConf *config, std::map<std::string, std::string> &cmdArgs);

void _GetNameList2(std::string const &name, std::vector<std::string> &vtName);
void _GetNameList(std::string const &name, std::vector<std::string> &vtName);






