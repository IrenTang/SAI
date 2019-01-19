#ifdef _WIN32
#include <windows.h>
#else
#include <sys/stat.h>
#include <unistd.h>
#endif

#include <fstream>
#include "utils.h"


const char *g_usage[] = {
    _NX_STICK_NUM_,
    _NX_THREAD_NUM_,
    _NX_MODEL_PATH_,
    _NX_STICK_CNN_,
    _NX_HOST_NET_,
    _NX_LABEL_NAME_,
    _NX_PROC_TYPE_,
    _NX_DECODER_NUM_,
    _NX_NET_TYPE_,
    _NX_ClASS_NUM_,
	_NX_DELAY_TIME_,
	_NX_SG_BEGIN_ID_,
	_NX_FILELIST_
};

static std::string GetDefaultModelPath(void) {
	char path[512] = { 0 };
#ifdef _WIN32
	GetModuleFileName(0, path, 512);
	char *tail = path + strlen(path);
	while (tail != path) {
		if (*tail == '\\' || *tail == '/') break;
		else --tail;
	}
	*tail = 0;
#else
	getcwd(path, 512);
#endif
	return std::string(path) + "/model";
}

std::map<std::string, std::string> ParseArgs(int argc, char *argv[]) {
    std::map<std::string, std::string> cmdArgs;
    for (size_t i = 0; i < sizeof(g_usage) / sizeof(char *); ++i) {
        cmdArgs[g_usage[i]] = std::string();
    }
    for (int i = 1; i < argc; i += 2) {
        if (cmdArgs.find(argv[i]) != cmdArgs.end()) {
            cmdArgs[argv[i]] = argv[i + 1];
        }
    }
	if (cmdArgs[_NX_MODEL_PATH_].empty()) { cmdArgs[_NX_MODEL_PATH_] = GetDefaultModelPath(); };
    if (cmdArgs[_NX_MODEL_PATH_].back() != '/' && cmdArgs[_NX_MODEL_PATH_].back() != '\\') cmdArgs[_NX_MODEL_PATH_] += '/';
    return cmdArgs;
}

static int NXatoi(std::string const &str) {
    if (str.empty()) return 0;
    else return atoi(str.c_str());
}

void GenerateEngineConfigFromCmdArgs(NXEngineConf *config, std::map<std::string, std::string> &cmdArgs) {
    std::string *modelPath = new std::string(cmdArgs[_NX_MODEL_PATH_]);
    std::string *stickCNNName = new std::string(*modelPath + cmdArgs[_NX_STICK_CNN_]);
	std::string *hostNetName = new std::string(*modelPath + cmdArgs[_NX_HOST_NET_]);
	config->modelPath = modelPath->c_str();
	config->hostNetName = hostNetName->c_str();
    config->stickCNNName = stickCNNName->c_str();

    config->stickNum = NXatoi(cmdArgs[_NX_STICK_NUM_]);
    config->threadNum = NXatoi(cmdArgs[_NX_THREAD_NUM_]);
    config->netType = NXatoi(cmdArgs[_NX_NET_TYPE_]);
    config->classNum = NXatoi(cmdArgs[_NX_ClASS_NUM_]);
	config->sg_beginID = NXatoi(cmdArgs[_NX_SG_BEGIN_ID_]);
	config->delayTime = NXatoi(cmdArgs[_NX_DELAY_TIME_]);
	//if (config->delayTime == 0)
	//{
	//	config->delayTime = 7000;
	//}
}

void printfUsage(void) {
	printf("\nParameter Error. Usage: \n\
-------- Inference Engine ---------\n\
*   stickNum    1           \n\
*   threadNum   6           \n\
*   modelPath   c_string(default path is ./Model)    \n\
*   netType   1: teeNet1; 2: teeNet2; 3: teeNet3   \n\
*   classNum      \n\
*   sg_BeginID   Linux: 2; Windows: 0 (Adjust this value based on your system)        \n\
*   delayTime    12000 (Adjust this value based on your computer hardware and models)  \n\
*   stickCNN    c_string    \n\
*   hostNet     c_string    \n\
*   labelName   c_string    \n\
*   fileList    c_string    \n\
-----------------------------------\n");
}

static std::string _NXGetLine2(std::ifstream &fIn) {
	std::string line;
	getline(fIn, line);
	if (line.empty()) return line;
	while (isspace(line.back())) {
		line.pop_back();
	}
	return line;
}
void _GetNameList2(std::string const &name, std::vector<std::string> &vtName) {
	std::ifstream fIn(name);
	if (!fIn.is_open()) return;
	while (true) {
		std::string line = _NXGetLine2(fIn);
		if (line.empty()) break;
		vtName.push_back(line);
	}
}

std::string _getPath(std::string const &name) {
	std::string::size_type pos1 = name.rfind('\\');
	std::string::size_type pos2 = name.rfind('/');
	std::string::size_type pos = std::string::npos;
	if (pos1 == std::string::npos) pos = pos2;
	else if (pos2 == std::string::npos) pos = pos1;
	else pos = pos1 > pos2 ? pos1 : pos2;
	if (pos == std::string::npos) return std::string();
	return std::string(name.begin(), name.begin() + pos);
}

void _GetNameList(std::string const &name, std::vector<std::string> &vtName) {
	std::ifstream fIn(name);
	if (!fIn.is_open()) return;
	std::string npath = _getPath(name);
	while (true) {
		std::string line = _NXGetLine2(fIn);
		if (line.empty()) break;
		vtName.push_back(npath + "/" + line);
	}
}

