#include <jni.h>
#include <string>

using namespace std;

static const int AES_KEY[16] = {110, 40, 85, 28, -68, -24, -52, 19, 127, -76, -93, -25, -6, -45, -66, -16};

static const char *ALPHA_NUM_SETS = "abcdefghijklmnopqrstuvwxyz1234567890";
static const int ALPHA_NUM_SETS_LENGTH = 36;
static const jint FLAG_GET_SIGNATURES = 0x00000040;
static const char *DIGEST_TYPE = "SHA256";
static const char *VALID_APPLICATION_IDS[1] = {"com.mobileconnected.myapplicationcxxx"};
static const char *VALID_SIGNATUERS[1] = {"1d61c80b43423636fbd533f99a7943782ce0d55aab661e5f5362d343df30454b"};
static const char *runningApplicationId;
static bool isValidApplicationID = false;
static const char *runningSignature;
static bool isValidSignature = false;

bool isApplicationIdValid(JNIEnv *env, jobject context) {
    if (runningApplicationId == nullptr) {
        jclass clazz = env->GetObjectClass(context);
        jmethodID app_id_method_id = env->GetMethodID(clazz, "getPackageName", "()Ljava/lang/String;");
        auto applicationId = reinterpret_cast<jstring>(env->CallObjectMethod(context, app_id_method_id));
        runningApplicationId = env->GetStringUTFChars(applicationId, nullptr);
        for (const auto &item: VALID_APPLICATION_IDS) {
            if (strcmp(runningApplicationId, item) == 0) {
                isValidApplicationID = true;
                break;
            }
        }
    }
    return isValidApplicationID;
}

bool isSignatureValid(JNIEnv *env, jobject context) {
    if (runningSignature == nullptr) {
        jclass context_clazz = env->GetObjectClass(context);
        jmethodID pm_mid = env->GetMethodID(context_clazz, "getPackageManager",
                                            "()Landroid/content/pm/PackageManager;");
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
        runningSignature = env->GetStringUTFChars(hexSignatureString, nullptr);
        for (const auto &item: VALID_SIGNATUERS) {
            if (strcmp(runningSignature, item) == 0) {
                isValidSignature = true;
                break;
            }
        }
    }
    return isValidSignature;
}

jstring generateAlphanumericString(int length, JNIEnv *env) {
    string randomKey;
    for (int i = 0; i < length; ++i) {
        int randomIndex = random() % ALPHA_NUM_SETS_LENGTH;
        if (i == 0) {
            randomKey = ALPHA_NUM_SETS[randomIndex];
        } else {
            randomKey += ALPHA_NUM_SETS[randomIndex];
        }
    }
    return env->NewStringUTF(randomKey.c_str());
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_mobileconnected_c_1encrypt_EncryptJNI_appIDFromJNI(JNIEnv *env, jobject, jobject context) {
    if (isApplicationIdValid(env, context)) {//如果相等
        return env->NewStringUTF(runningApplicationId);
    } else {
        return nullptr;
    }
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_mobileconnected_c_1encrypt_EncryptJNI_signatureFromJNI(JNIEnv *env, jobject, jobject context) {
    if (isSignatureValid(env, context)) {
        return env->NewStringUTF(runningSignature);
    }
    return nullptr;
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

//char *substring(char *source, int pos, int length) {
//    //定义字符指针 指向传递进来的ch地址
//    char *pch = source;
//    //通过calloc来分配一个length长度的字符数组，返回的是字符指针。
//    char *subch = (char *) calloc(sizeof(char), length + 1);
//    int i;
//    //只有在C99下for循环中才可以声明变量，这里写在外面，提高兼容性。
//    pch = pch + pos;
////是pch指针指向pos位置。
//    for (i = 0; i < length; i++) {
//        subch[i] = *(pch++);
////循环遍历赋值数组。
//    }
//    subch[length] = '\0';//加上字符串结束符。
//    return subch;       //返回分配的字符数组地址。
//}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_mobileconnected_c_1encrypt_EncryptJNI_generateLoginNonce(JNIEnv *env, jobject thiz, jobject context,
                                                            jstring source) {
    if (!isApplicationIdValid(env, context)) return nullptr;
    if (!isSignatureValid(env, context)) return nullptr;

    jstring privateKey = generateAlphanumericString(16, env);
    jstring iv = generateAlphanumericString(16, env);
    jstring randomValue = generateAlphanumericString(8, env);


    jclass aesClass = env->FindClass("cn/hutool/crypto/symmetric/AES");
    jmethodID aes_mid = env->GetMethodID(aesClass, "<init>", "(Ljava/lang/String;Ljava/lang/String;[B[B)V");

    jclass stringClass = env->GetObjectClass(privateKey);
    jmethodID getBytes_mid = env->GetMethodID(stringClass, "getBytes", "()[B");

    jobject aes = env->NewObject(aesClass, aes_mid,
                                 env->NewStringUTF("CBC"),
                                 env->NewStringUTF("PKCS7Padding"),
                                 env->CallObjectMethod(privateKey, getBytes_mid),
                                 env->CallObjectMethod(iv, getBytes_mid));

    jclass dateTimeFormatterClass = env->FindClass("java/time/format/DateTimeFormatter");
    jmethodID ofPattern_mid = env->GetStaticMethodID(dateTimeFormatterClass, "ofPattern",
                                                     "(Ljava/lang/String;)Ljava/time/format/DateTimeFormatter;");
    jobject dateTimeFormatter = env->CallStaticObjectMethod(dateTimeFormatterClass, ofPattern_mid,
                                                            env->NewStringUTF("yyyy-MM-dd hh:mm:ss.SSSSSS"));

    jclass zoneIdClass = env->FindClass("java/time/ZoneId");
    jmethodID of_zone_mid = env->GetStaticMethodID(zoneIdClass, "of", "(Ljava/lang/String;)Ljava/time/ZoneId;");
    jobject utcZoneId = env->CallStaticObjectMethod(zoneIdClass, of_zone_mid, env->NewStringUTF("Z"));

    jclass localDateTimeClass = env->FindClass("java/time/LocalDateTime");
    jmethodID now_localDateTime_mid = env->GetStaticMethodID(localDateTimeClass, "now",
                                                             "(Ljava/time/ZoneId;)Ljava/time/LocalDateTime;");
    jobject now = env->CallStaticObjectMethod(localDateTimeClass, now_localDateTime_mid, utcZoneId);

    jmethodID format_mid = env->GetMethodID(dateTimeFormatterClass, "format",
                                            "(Ljava/time/temporal/TemporalAccessor;)Ljava/lang/String;");
    auto formatNow = reinterpret_cast<jstring>(env->CallObjectMethod(dateTimeFormatter, format_mid, now));

    string sourceString = env->GetStringUTFChars(source, nullptr);
    string formatNowString = env->GetStringUTFChars(formatNow, nullptr);
    string randomValueString = env->GetStringUTFChars(randomValue, nullptr);

    string nonce = sourceString + "&" + formatNowString + "Z" + "&" + randomValueString;

    jmethodID encryptHex_mid = env->GetMethodID(aesClass, "encryptHex", "(Ljava/lang/String;)Ljava/lang/String;");
    string encodeNonce = env->GetStringUTFChars(reinterpret_cast<jstring>(env->CallObjectMethod(aes, encryptHex_mid,
                                                                                                env->NewStringUTF(
                                                                                                        nonce.c_str()))),
                                                nullptr);
    string privateKeyString = env->GetStringUTFChars(privateKey, nullptr);
    string privateKey1 = privateKeyString.substr(0, 8);
    string privateKey2 = privateKeyString.substr(8);
    string ivString = env->GetStringUTFChars(iv, nullptr);
    string iv1 = ivString.substr(0, 8);
    string iv2 = ivString.substr(8);
    string p1 = privateKey1 + iv1;
    string p2 = privateKey2 + iv2;
    string p = p1 + p2;

    jclass secureUtilClass = env->FindClass("cn/hutool/crypto/SecureUtil");
    jmethodID sha256_mid = env->GetStaticMethodID(secureUtilClass, "sha256", "(Ljava/lang/String;)Ljava/lang/String;");
    string encodePrivateKey = env->GetStringUTFChars(
            reinterpret_cast<jstring>(env->CallStaticObjectMethod(secureUtilClass, sha256_mid,
                                                                  env->NewStringUTF(p.c_str()))),
            nullptr);
    string encodePrivateKey1 = encodePrivateKey.substr(0, 32);
    string encodePrivateKey2 = encodePrivateKey.substr(32);
    string loginNonce = privateKey1 + iv1 + encodePrivateKey1 + encodeNonce + privateKey2 + iv2 + encodePrivateKey2;
//    string loginNonce =
//            privateKey1 + "\n" + iv1 + "\n" + encodePrivateKey1 + "\n" + encodeNonce + "\n" + privateKey2 + "\n" + iv2 +
//            "\n" + encodePrivateKey2;

    return env->NewStringUTF(loginNonce.c_str());
}