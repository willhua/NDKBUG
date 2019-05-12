#include <jni.h>
#include <string>
#include <android/log.h>

#define LOG_SWITCH 00

#define    LOGD(...)     __android_log_print(ANDROID_LOG_DEBUG, "willhua", __VA_ARGS__)


void merge2_even222(uint32_t result[3], const float *interx, const float *intery, int offset)
{

    const float UNIT = 32.0;
    uint32_t xyi[8]={0};
    uint32_t xyf[8]={0};
    for(int i = 0, j = 0; i < 8; i+=2, ++j){
        xyi[i+1] = static_cast<uint32_t>(interx[offset + j]);
        xyi[i] = static_cast<uint32_t>(intery[offset + j]);
        xyf[i+1] = static_cast<uint32_t>((interx[offset + j] - xyi[i]) * UNIT);
        xyf[i] = static_cast<uint32_t>((intery[offset + j] - xyi[i+1]) * UNIT);
    }

    for (int i = 2; i < 8; i+=2) {
        xyi[i] = xyi[i] + 4 - xyi[0];
        xyi[i+1] = xyi[i+1] + 4 - xyi[1];
    }


    uint64_t *  xyf64 = (uint64_t*)(xyf);
    uint64_t  *result64 = (uint64_t*)(result + 1);


#if LOG_SWITCH
    LOGD("result64 %016llx", result64[0]);
    LOGD("   xyf  %08x %08x  %08x  %08x      %08x %08x %08x %08x ", xyf[0], xyf[1], xyf[2], xyf[3], xyf[4], xyf[5], xyf[6], xyf[7] );
    LOGD(" xyf64  %016llx, %016llx, %016llx, %016llx, ", xyf64[0], xyf64[1], xyf64[2], xyf64[3]);
#endif


    result[0] = (xyi[0] << 16);
    result[0] |= xyi[1];
    (*result64) = (xyf64[0] << 27);
    (*result64) |= (xyf64[1] << 22);
    (*result64) |= (xyf64[2] << 17);
    (*result64) |= (xyf64[3] << 12);


#if LOG_SWITCH

    LOGD("   xyf  %08x %08x  %08x  %08x      %08x %08x %08x %08x ", xyf[0], xyf[1], xyf[2], xyf[3], xyf[4], xyf[5], xyf[6], xyf[7] );
    LOGD(" xyf64  %016llx, %016llx, %016llx, %016llx, ", xyf64[0], xyf64[1], xyf64[2], xyf64[3]);
    LOGD("result64 %016llx", result64[0]);
#endif

    uint64_t *  xyi64 = (uint64_t*)(xyi);
    result[1] |= (xyi64[1] << 8);
    result[2] |= (xyi64[2] << 4);
    result[1] |= (xyi64[3]);
}



extern "C" JNIEXPORT jstring JNICALL
Java_com_willhua_ndkbug_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    if(true)
    {
        uint32_t result[3] = {0};
        float interx[4] = {1.2,2.6,3.597,4.5698};
        float intery[4] = {1.42,2.86,3.5977,4.56798};
        merge2_even222(result, interx, intery, 0);
        LOGD("test log_switch:%d      result: %08x %08x %08x", LOG_SWITCH, result[0], result[1], result[2]);
    }
    return env->NewStringUTF(hello.c_str());
}
