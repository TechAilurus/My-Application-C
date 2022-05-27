#include <jni.h>
#include <string>
//#include "crypto/hex.h"
//#include "crypto/sha.h"
//#include "crypto/filters.h"

const jint FLAG_GET_SIGNATURES = 0x00000040;

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_myapplicationc_MainActivity_appIDFromJNI(JNIEnv *env, jobject obj) {
    jclass clazz = env->GetObjectClass(obj);
    jmethodID app_id_method_id = env->GetMethodID(clazz, "getPackageName", "()Ljava/lang/String;");
    auto applicationId = reinterpret_cast<jstring>(env->CallObjectMethod(obj, app_id_method_id));
    return applicationId;
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_example_myapplicationc_MainActivity_signatureFromJNI(JNIEnv *env, jobject obj) {
    jclass native_class = env->GetObjectClass(obj);
    jmethodID pm_id = env->GetMethodID(native_class, "getPackageManager",
                                       "()Landroid/content/pm/PackageManager;");
    jobject pm_obj = env->CallObjectMethod(obj, pm_id);
    jclass pm_clazz = env->GetObjectClass(pm_obj);
    jmethodID package_info_id = env->GetMethodID(pm_clazz, "getPackageInfo",
                                                 "(Ljava/lang/String;I)Landroid/content/pm/PackageInfo;");
    jclass native_classs = env->GetObjectClass(obj);
    jmethodID mId = env->GetMethodID(native_classs, "getPackageName", "()Ljava/lang/String;");
    auto pkg_str = reinterpret_cast<jstring>(env->CallObjectMethod(obj, mId));
    jobject pi_obj = env->CallObjectMethod(pm_obj, package_info_id, pkg_str, FLAG_GET_SIGNATURES);
    jclass pi_clazz = env->GetObjectClass(pi_obj);
    jfieldID signatures_fieldId = env->GetFieldID(pi_clazz, "signatures",
                                                  "[Landroid/content/pm/Signature;");
    jobject signatures_obj = env->GetObjectField(pi_obj, signatures_fieldId);
    auto signaturesArray = (jobjectArray) signatures_obj;
    jsize size = env->GetArrayLength(signaturesArray);
    jobject signature_obj = env->GetObjectArrayElement(signaturesArray, 0);
    jclass signature_clazz = env->GetObjectClass(signature_obj);
    jmethodID string_id = env->GetMethodID(signature_clazz, "toCharsString",
                                           "()Ljava/lang/String;");
    auto str = reinterpret_cast<jstring>(env->CallObjectMethod(signature_obj, string_id));
    return str;
}

//extern "C"
//JNIEXPORT jstring JNICALL
//Java_com_example_myapplicationc_MainActivity_signatureFromJNI(JNIEnv *env, jobject obj) {
//    jclass native_class = env->GetObjectClass(obj);
//    jmethodID pm_id = env->GetMethodID(native_class, "getPackageManager",
//                                       "()Landroid/content/pm/PackageManager;");
//    jobject pm_obj = env->CallObjectMethod(obj, pm_id);
//    jclass pm_clazz = env->GetObjectClass(pm_obj);
//    jmethodID package_info_id = env->GetMethodID(pm_clazz, "getPackageInfo",
//                                                 "(Ljava/lang/String;I)Landroid/content/pm/PackageInfo;");
//    jclass native_classs = env->GetObjectClass(obj);
//    jmethodID mId = env->GetMethodID(native_classs, "getPackageName", "()Ljava/lang/String;");
//    auto pkg_str = reinterpret_cast<jstring>(env->CallObjectMethod(obj, mId));
//    jobject pi_obj = env->CallObjectMethod(pm_obj, package_info_id, pkg_str, FLAG_GET_SIGNATURES);
//    jclass pi_clazz = env->GetObjectClass(pi_obj);
//    jfieldID signatures_fieldId = env->GetFieldID(pi_clazz, "signatures",
//                                                  "[Landroid/content/pm/Signature;");
//    jobject signatures_obj = env->GetObjectField(pi_obj, signatures_fieldId);
//    auto signaturesArray = (jobjectArray) signatures_obj;
//    jobject signature_obj = env->GetObjectArrayElement(signaturesArray, 0);
//    jclass signature_clazz = env->GetObjectClass(signature_obj);
//
//    jmethodID bytes_id = env->GetMethodID(signature_clazz, "toByteArray", "()[B");
//    auto certArray = reinterpret_cast<jbyteArray>(env->CallObjectMethod(signature_obj, bytes_id));
//
//    CryptoPP::SHA256 hash;
//    CryptoPP::byte digest[CryptoPP::SHA256::DIGESTSIZE];
//    hash.CalculateDigest(digest, (const CryptoPP::byte *) certArray, 1);
//
//    CryptoPP::HexEncoder encoder;
//    std::string output;
//    encoder.Attach(new CryptoPP::StringSink(output));
//    encoder.Put(digest, sizeof(digest));
//    encoder.MessageEnd();
//
//    return env->NewStringUTF(output.c_str());
//}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_example_myapplicationc_MainActivity_versionNameFromJNI(JNIEnv *env, jobject obj) {
    jclass native_class = env->GetObjectClass(obj);
    jmethodID pm_id = env->GetMethodID(native_class, "getPackageManager",
                                       "()Landroid/content/pm/PackageManager;");
    jobject pm_obj = env->CallObjectMethod(obj, pm_id);
    jclass pm_clazz = env->GetObjectClass(pm_obj);
    jmethodID package_info_id = env->GetMethodID(pm_clazz, "getPackageInfo",
                                                 "(Ljava/lang/String;I)Landroid/content/pm/PackageInfo;");
    jclass native_classs = env->GetObjectClass(obj);
    jmethodID mId = env->GetMethodID(native_classs, "getPackageName", "()Ljava/lang/String;");
    auto pkg_str = reinterpret_cast<jstring>(env->CallObjectMethod(obj, mId));
    jobject pi_obj = env->CallObjectMethod(pm_obj, package_info_id, pkg_str, 0);
    jclass pi_clazz = env->GetObjectClass(pi_obj);
    jfieldID version_name_fieldId = env->GetFieldID(pi_clazz, "versionName", "Ljava/lang/String;");
    jobject version_name_obj = env->GetObjectField(pi_obj, version_name_fieldId);
    return reinterpret_cast<jstring>(version_name_obj);
}