#include "webResponse.h"

webResponse::HttpCode  webResponse::requestAction() {
    std::cout << "Default File Path: " << filePath << std::endl;
    struct stat st;
    if(stat(filePath.c_str(),&st) < 0)  return NoResource;
    if(!(st.st_mode & S_IROTH)) return ForbidenRequest;
    if(S_ISDIR(st.st_mode)) {
        //处理目录请求
        return 
    }

}
