package com.example.myapplicationc

import android.annotation.SuppressLint
import android.os.Bundle
import androidx.appcompat.app.AppCompatActivity
import com.example.myapplicationc.databinding.ActivityMainBinding

class MainActivity : AppCompatActivity() {

  private lateinit var binding: ActivityMainBinding

  @SuppressLint("PackageManagerGetSignatures")
  override fun onCreate(savedInstanceState: Bundle?) {
    super.onCreate(savedInstanceState)

    binding = ActivityMainBinding.inflate(layoutInflater)
    setContentView(binding.root)

    // Example of a call to a native method
    binding.sampleText.text = appIDFromJNI()


//    val packageInfo = packageManager.getPackageInfo(packageName, PackageManager.GET_SIGNATURES)
//    val signingInfo = packageInfo.signatures
//    binding.sampleText2.text = signingInfo.firstOrNull()?.toCharsString()
    binding.sampleText2.text = signatureFromJNI()

    val packageInfo = packageManager.getPackageInfo(packageName, 0)

//    binding.sampleText3.text = packageInfo.versionName
    binding.sampleText3.text = versionNameFromJNI()
  }

  /**
   * A native method that is implemented by the 'myapplicationc' native library,
   * which is packaged with this application.
   */
  private external fun appIDFromJNI(): String

  private external fun signatureFromJNI(): String

  private external fun versionNameFromJNI(): String

  /**
   * A native method that is implemented by the 'myapplicationc' native library,
   * which is packaged with this application.
   */

  companion object {
    // Used to load the 'myapplicationc' library on application startup.
    init {
      System.loadLibrary("myapplicationc")
    }
  }
}