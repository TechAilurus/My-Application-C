package com.mobileconnected.c_encrypt

import android.annotation.SuppressLint
import android.content.Context
import android.content.pm.PackageManager
import android.os.Bundle
import androidx.appcompat.app.AppCompatActivity
import cn.hutool.core.util.HexUtil
import com.mobileconnected.c_encrypt.databinding.ActivityMainBinding
import java.security.MessageDigest

const val TAG = "EncryptJNI"

class EncryptJNI : AppCompatActivity() {

  private lateinit var binding: ActivityMainBinding

  @SuppressLint("PackageManagerGetSignatures")
  override fun onCreate(savedInstanceState: Bundle?) {
    super.onCreate(savedInstanceState)

    binding = ActivityMainBinding.inflate(layoutInflater)
    setContentView(binding.root)

    // Example of a call to a native method
    binding.sampleText.text = appIDFromJNI(this) ?: "null"
    binding.sampleText.text = appIDFromJNI(this) ?: "null"

    val packageInfo = packageManager.getPackageInfo(packageName, PackageManager.GET_SIGNATURES)
    val signingInfo = packageInfo.signatures
    val certArray = signingInfo[0].toByteArray()
    val md = MessageDigest.getInstance("SHA256")
    val digest = md.digest(certArray)

//    binding.sampleText2.text = Hex.encodeHexString(digest)
    binding.sampleText2.text = signatureFromJNI(this)

//    val packageInfo = packageManager.getPackageInfo(packageName, 0)

//    binding.sampleText3.text = packageInfo.versionName
    binding.sampleText3.text = versionNameFromJNI(this)

    binding.sampleButton1.setOnClickListener {
      if (binding.sampleEditText1.text.isNullOrBlank()) return@setOnClickListener
      binding.sampleEditText1.setText(encryptFromJNI(this, binding.sampleEditText1.text.toString()))
    }

    binding.sampleButton2.setOnClickListener {
      if (binding.sampleEditText1.text.isNullOrBlank()) return@setOnClickListener
      binding.sampleEditText1.setText(decryptFromJNI(this, binding.sampleEditText1.text.toString()))
    }
  }

  /**
   * A native method that is implemented by the 'myapplicationc' native library,
   * which is packaged with this application.
   */
  private external fun appIDFromJNI(context: Context): String?

  private external fun signatureFromJNI(context: Context): String?

  private external fun versionNameFromJNI(context: Context): String

  private external fun decryptFromJNI(context: Context, content: String): String

  private external fun encryptFromJNI(context: Context, content: String): String


  /**
   * A native method that is implemented by the 'myapplicationc' native library,
   * which is packaged with this application.
   */

  companion object {
    // Used to load the 'cencrypt' library on application startup.
    init {
      System.loadLibrary("cencrypt")
    }
  }
}