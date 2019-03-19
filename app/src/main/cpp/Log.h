//
// Created by 李錚 on 2017/9/28.
//

#ifndef FFPLAYER_LOG_H
#define FFPLAYER_LOG_H
#include <android/log.h>

#define LOGE(FORMAT,...) __android_log_print(ANDROID_LOG_ERROR,"lizheng",FORMAT,##__VA_ARGS__);
#endif //FFPLAYER_LOG_H
