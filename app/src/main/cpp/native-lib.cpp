#include <jni.h>
#include <string>
#include <chrono>
#include <stdlib.h>
#include <android/bitmap.h>
#include "BitBuffer.hpp"
#include "QrCode.hpp"

#include <android/log.h>

#define APPNAME "MyApp"

using std::uint8_t;
using qrcodegen::QrCode;
using qrcodegen::QrSegment;
using namespace std::chrono;

void white(u_int32_t *pixel) {
    uint32_t alpha = 255;
    uint32_t blue = 255;
    u_short green = 255;
    u_char red = 255;
    *pixel = (alpha << 24) | (red << 16) | (green << 8) | blue;
}

void black(u_int32_t *pixel) {
    uint32_t alpha = 255;
    uint32_t blue = 0;
    u_short green = 0;
    u_char red = 0;
    *pixel = (alpha << 24) | (red << 16) | (green << 8) | blue;
}

void draw_white(void *pixels, u_short x, u_short y) {
    u_int32_t *pixel = ((u_int32_t *) pixels) + x;
    white(pixel);
}

void draw_black(void *pixels, u_short x, u_short y) {
    u_int32_t *pixel = ((u_int32_t *) pixels) + x;
    black(pixel);
}

extern "C" JNIEXPORT jstring

JNICALL
Java_com_hipsort_hipsort_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}
extern "C" JNIEXPORT void
JNICALL
Java_com_hipsort_hipsort_MainActivity_renderQR(JNIEnv *env, jobject instance, jobject bmp) {
//    __android_log_print(ANDROID_LOG_VERBOSE, APPNAME,"FUUUUUUUUUUCK");
    milliseconds ms = duration_cast<milliseconds>(system_clock::now().time_since_epoch());

    const QrCode qr = QrCode::encodeText(std::to_string(ms.count()).c_str(), QrCode::Ecc::MEDIUM);
//    __android_log_print(ANDROID_LOG_VERBOSE, APPNAME,"%s", std::to_string(ms.count()).c_str());
    AndroidBitmapInfo info;
    void *pixels;

    if (AndroidBitmap_getInfo(env, bmp, &info) < 0) {
        return;
    }

    if (info.format != ANDROID_BITMAP_FORMAT_RGBA_8888) {
        return;
    }

    if (AndroidBitmap_lockPixels(env, bmp, &pixels) < 0) {
        return;
    }
    uint32_t alpha = 255;
    uint32_t blue = 0;
    u_short green = 0;
    u_char red = 0;
    u_int32_t black = (alpha << 24) | (red << 16) | (green << 8) | blue;
    blue = 255;
    green = 255;
    red = 255;
    u_int32_t white = (alpha << 24) | (red << 16) | (green << 8) | blue;

//    __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, info);
    u_int32_t *pixs = (u_int32_t *) pixels;
    for (int y = 0; y < 21; y++) {
        u_int32_t *line = &pixs[y * 21];
        for (int x = 0; x < 21; x++) {
            if (qr.getModule(x, y)) {
                line[x] = black;
            } else {
                line[x] = white;
            }
        }
    }

    AndroidBitmap_unlockPixels(env, bmp);

}
