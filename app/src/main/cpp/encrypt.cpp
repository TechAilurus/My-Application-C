#include <jni.h>
#include <string>
#include "../../../../distribution/cryptopp/include/sha.h"
#include "../../../../distribution/cryptopp/include/hex.h"

static const jint FLAG_GET_SIGNATURES = 0x00000040;
static const char *DIGEST_TYPE = "SHA256";

extern "C" JNIEXPORT jstring JNICALL
Java_com_mobileconnected_c_1encrypt_EncryptJNI_appIDFromJNI(JNIEnv *env, jobject, jobject context) {
    jclass clazz = env->GetObjectClass(context);
    jmethodID app_id_method_id = env->GetMethodID(clazz, "getPackageName", "()Ljava/lang/String;");
    auto applicationId = reinterpret_cast<jstring>(env->CallObjectMethod(context,
                                                                         app_id_method_id));
    return applicationId;
}

//extern "C"
//JNIEXPORT jstring JNICALL
//Java_com_mobileconnected_c_1encrypt_EncryptJNI_signatureFromJNI(JNIEnv *env, jobject,
//                                                                jobject context) {
//    jclass context_clazz = env->GetObjectClass(context);
//    jmethodID pm_mid = env->GetMethodID(context_clazz, "getPackageManager",
//                                        "()Landroid/content/pm/PackageManager;");
//    jobject pm_obj = env->CallObjectMethod(context, pm_mid);
//
//    jclass pm_clazz = env->GetObjectClass(pm_obj);
//    jmethodID pi_mid = env->GetMethodID(pm_clazz, "getPackageInfo",
//                                        "(Ljava/lang/String;I)Landroid/content/pm/PackageInfo;");
//    jmethodID pn_mid = env->GetMethodID(context_clazz, "getPackageName", "()Ljava/lang/String;");
//    auto pkg_name = reinterpret_cast<jstring>(env->CallObjectMethod(context, pn_mid));
//    jobject pi_obj = env->CallObjectMethod(pm_obj, pi_mid, pkg_name, FLAG_GET_SIGNATURES);
//
//    jclass pi_clazz = env->GetObjectClass(pi_obj);
//    jfieldID signatures_fId = env->GetFieldID(pi_clazz, "signatures",
//                                              "[Landroid/content/pm/Signature;");
//    auto signatures = (jobjectArray) env->GetObjectField(pi_obj, signatures_fId);
//    jobject signature_obj = env->GetObjectArrayElement(signatures, 0);
//
//    jclass signature_clazz = env->GetObjectClass(signature_obj);
//    jmethodID ba_mid = env->GetMethodID(signature_clazz, "toByteArray", "()[B");
//    jobject signature_array = env->CallObjectMethod(signature_obj, ba_mid);
//
//    jclass messageDigestClass = env->FindClass("java/security/MessageDigest");
//    jmethodID messageDigestInstanceMethodId = env->GetStaticMethodID(messageDigestClass,
//                                                                     "getInstance",
//                                                                     "(Ljava/lang/String;)Ljava/security/MessageDigest;");
//    jobject messageDigest = env->CallStaticObjectMethod(messageDigestClass,
//                                                        messageDigestInstanceMethodId,
//                                                        env->NewStringUTF(DIGEST_TYPE));
//    jmethodID digest_mid = env->GetMethodID(messageDigestClass, "digest", "([B)[B");
//    jobject digest_array = env->CallObjectMethod(messageDigest, digest_mid, signature_array);
//
//    jclass hexClass = env->FindClass("org/apache/commons/codec/binary/Hex");
//    jmethodID hexStringMethodId = env->GetStaticMethodID(hexClass, "encodeHexString",
//                                                         "([B)Ljava/lang/String;");
//    auto hexSignatureString = reinterpret_cast<jstring>(env->CallStaticObjectMethod(hexClass,
//                                                                                    hexStringMethodId,
//                                                                                    digest_array));
//    return hexSignatureString;
//}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_mobileconnected_c_1encrypt_EncryptJNI_signatureFromJNI(JNIEnv *env, jobject,
                                                                jobject context) {
    jclass native_class = env->GetObjectClass(context);
    jmethodID pm_id = env->GetMethodID(native_class, "getPackageManager",
                                       "()Landroid/content/pm/PackageManager;");
    jobject pm_obj = env->CallObjectMethod(context, pm_id);
    jclass pm_clazz = env->GetObjectClass(pm_obj);
    jmethodID package_info_id = env->GetMethodID(pm_clazz, "getPackageInfo",
                                                 "(Ljava/lang/String;I)Landroid/content/pm/PackageInfo;");
    jclass native_classs = env->GetObjectClass(context);
    jmethodID mId = env->GetMethodID(native_classs, "getPackageName", "()Ljava/lang/String;");
    auto pkg_str = reinterpret_cast<jstring>(env->CallObjectMethod(context, mId));
    jobject pi_obj = env->CallObjectMethod(pm_obj, package_info_id, pkg_str, FLAG_GET_SIGNATURES);
    jclass pi_clazz = env->GetObjectClass(pi_obj);
    jfieldID signatures_fieldId = env->GetFieldID(pi_clazz, "signatures",
                                                  "[Landroid/content/pm/Signature;");
    jobject signatures_obj = env->GetObjectField(pi_obj, signatures_fieldId);
    auto signaturesArray = (jobjectArray) signatures_obj;
    jobject signature_obj = env->GetObjectArrayElement(signaturesArray, 0);
    jclass signature_clazz = env->GetObjectClass(signature_obj);

    jmethodID bytes_id = env->GetMethodID(signature_clazz, "toByteArray", "()[B");
    auto certArray = reinterpret_cast<jbyteArray>(env->CallObjectMethod(signature_obj, bytes_id));

    CryptoPP::SHA256 hash;
    CryptoPP::byte digest[CryptoPP::SHA256::DIGESTSIZE];
    hash.CalculateDigest(digest, (const CryptoPP::byte *) certArray, 1);

    CryptoPP::HexEncoder encoder;
    std::string output;
    encoder.Attach(new CryptoPP::StringSink(output));
    encoder.Put(digest, sizeof(digest));
    encoder.MessageEnd();

    return env->NewStringUTF(output.c_str());
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_mobileconnected_c_1encrypt_EncryptJNI_versionNameFromJNI(JNIEnv *env, jobject,
                                                                  jobject context) {
    jclass native_class = env->GetObjectClass(context);
    jmethodID pm_id = env->GetMethodID(native_class, "getPackageManager",
                                       "()Landroid/content/pm/PackageManager;");
    jobject pm_obj = env->CallObjectMethod(context, pm_id);
    jclass pm_clazz = env->GetObjectClass(pm_obj);
    jmethodID package_info_id = env->GetMethodID(pm_clazz, "getPackageInfo",
                                                 "(Ljava/lang/String;I)Landroid/content/pm/PackageInfo;");
    jclass native_classs = env->GetObjectClass(context);
    jmethodID mId = env->GetMethodID(native_classs, "getPackageName", "()Ljava/lang/String;");
    auto pkg_str = reinterpret_cast<jstring>(env->CallObjectMethod(context, mId));
    jobject pi_obj = env->CallObjectMethod(pm_obj, package_info_id, pkg_str, 0);
    jclass pi_clazz = env->GetObjectClass(pi_obj);
    jfieldID version_name_fieldId = env->GetFieldID(pi_clazz, "versionName", "Ljava/lang/String;");
    jobject version_name_obj = env->GetObjectField(pi_obj, version_name_fieldId);
    return reinterpret_cast<jstring>(version_name_obj);
}