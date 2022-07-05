#include <jni.h>
#include <string>

using namespace std;

static const jint FLAG_GET_SIGNATURES = 0x00000040;
static const char *DIGEST_TYPE = "SHA256";
static const char *VALID_APPLICATION_IDS[1] = {"com.mobileconnected.myapplicationcxxx"};
static const int AES_KEY[16] = {110, 40, 85, 28, -68, -24, -52, 19, 127, -76, -93, -25, -6, -45, -66, -16};
static const char *currentApplicationId;
static bool isValidApplicationID;

bool isApplicationIdValid(const char *currentApplicationID) {
    bool isAllowed = false;
    for (const auto &item: VALID_APPLICATION_IDS) {
        if (strcmp(currentApplicationId, item) == 0) {
            isAllowed = true;
            break;
        }
    }
    return isAllowed;
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_mobileconnected_c_1encrypt_EncryptJNI_appIDFromJNI(JNIEnv *env, jobject, jobject context) {
    if (currentApplicationId == nullptr) {
        jclass clazz = env->GetObjectClass(context);
        jmethodID app_id_method_id = env->GetMethodID(clazz, "getPackageName", "()Ljava/lang/String;");
        auto applicationId = reinterpret_cast<jstring>(env->CallObjectMethod(context, app_id_method_id));
        currentApplicationId = env->GetStringUTFChars(applicationId, nullptr);
        isValidApplicationID = isApplicationIdValid(currentApplicationId);
    }

    if (isValidApplicationID) {//如果相等
        return env->NewStringUTF(currentApplicationId);
    } else {
        return nullptr;
    }
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_mobileconnected_c_1encrypt_EncryptJNI_signatureFromJNI(JNIEnv *env, jobject, jobject context) {
    jclass context_clazz = env->GetObjectClass(context);
    jmethodID pm_mid = env->GetMethodID(context_clazz, "getPackageManager", "()Landroid/content/pm/PackageManager;");
    jobject pm_obj = env->CallObjectMethod(context, pm_mid);

    jclass pm_clazz = env->GetObjectClass(pm_obj);
    jmethodID pi_mid = env->GetMethodID(pm_clazz, "getPackageInfo",
                                        "(Ljava/lang/String;I)Landroid/content/pm/PackageInfo;");
    jmethodID pn_mid = env->GetMethodID(context_clazz, "getPackageName", "()Ljava/lang/String;");
    auto pkg_name = reinterpret_cast<jstring>(env->CallObjectMethod(context, pn_mid));
    jobject pi_obj = env->CallObjectMethod(pm_obj, pi_mid, pkg_name, FLAG_GET_SIGNATURES);

    jclass pi_clazz = env->GetObjectClass(pi_obj);
    jfieldID signatures_fId = env->GetFieldID(pi_clazz, "signatures", "[Landroid/content/pm/Signature;");
    auto signatures = (jobjectArray) env->GetObjectField(pi_obj, signatures_fId);
    jobject signature_obj = env->GetObjectArrayElement(signatures, 0);

    jclass signature_clazz = env->GetObjectClass(signature_obj);
    jmethodID ba_mid = env->GetMethodID(signature_clazz, "toByteArray", "()[B");
    jobject signature_array = env->CallObjectMethod(signature_obj, ba_mid);

    jclass messageDigestClass = env->FindClass("java/security/MessageDigest");
    jmethodID messageDigestInstanceMethodId = env->GetStaticMethodID(messageDigestClass, "getInstance",
                                                                     "(Ljava/lang/String;)Ljava/security/MessageDigest;");
    jobject messageDigest = env->CallStaticObjectMethod(messageDigestClass, messageDigestInstanceMethodId,
                                                        env->NewStringUTF(DIGEST_TYPE));
    jmethodID digest_mid = env->GetMethodID(messageDigestClass, "digest", "([B)[B");
    jobject digest_array = env->CallObjectMethod(messageDigest, digest_mid, signature_array);

    jclass hexClass = env->FindClass("cn/hutool/core/util/HexUtil");
    jmethodID hexStringMethodId = env->GetStaticMethodID(hexClass, "encodeHexStr", "([B)Ljava/lang/String;");
    auto hexSignatureString = reinterpret_cast<jstring>(env->CallStaticObjectMethod(hexClass, hexStringMethodId,
                                                                                    digest_array));
    return hexSignatureString;
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_mobileconnected_c_1encrypt_EncryptJNI_versionNameFromJNI(JNIEnv *env, jobject, jobject context) {
    jclass native_class = env->GetObjectClass(context);
    jmethodID pm_id = env->GetMethodID(native_class, "getPackageManager", "()Landroid/content/pm/PackageManager;");
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

extern "C"
JNIEXPORT jstring JNICALL
Java_com_mobileconnected_c_1encrypt_EncryptJNI_decryptFromJNI(JNIEnv *env, jobject, jobject context, jstring content) {
    jbyteArray key = env->NewByteArray(16);
    env->SetByteArrayRegion(key, 0, 16, reinterpret_cast<const jbyte *>(AES_KEY));
    jclass secureUtilClass = env->FindClass("cn/hutool/crypto/SecureUtil");
    jmethodID aes_mid = env->GetStaticMethodID(secureUtilClass, "aes", "([B)Lcn/hutool/crypto/symmetric/AES;");
    jobject aes = env->CallStaticObjectMethod(secureUtilClass, aes_mid, key);

    jclass aesClass = env->GetObjectClass(aes);
    jmethodID decryptStr_mid = env->GetMethodID(aesClass, "decryptStr", "(Ljava/lang/String;)Ljava/lang/String;");
    jobject decryptedString = env->CallObjectMethod(aes, decryptStr_mid, content);

    return reinterpret_cast<jstring>(decryptedString);
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_mobileconnected_c_1encrypt_EncryptJNI_encryptFromJNI(JNIEnv *env, jobject, jobject context, jstring content) {
    jbyteArray key = env->NewByteArray(16);
    env->SetByteArrayRegion(key, 0, 16, reinterpret_cast<const jbyte *>(AES_KEY));
    jclass secureUtilClass = env->FindClass("cn/hutool/crypto/SecureUtil");
    jmethodID aes_mid = env->GetStaticMethodID(secureUtilClass, "aes", "([B)Lcn/hutool/crypto/symmetric/AES;");
    jobject aes = env->CallStaticObjectMethod(secureUtilClass, aes_mid, key);

    jclass aesClass = env->GetObjectClass(aes);
    jmethodID encryptStr_mid = env->GetMethodID(aesClass, "encryptHex", "(Ljava/lang/String;)Ljava/lang/String;");
    jobject encryptedString = env->CallObjectMethod(aes, encryptStr_mid, content);

    return reinterpret_cast<jstring>(encryptedString);
}