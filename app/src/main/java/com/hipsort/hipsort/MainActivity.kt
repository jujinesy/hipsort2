package com.hipsort.hipsort

import android.graphics.Bitmap
import android.support.v7.app.AppCompatActivity
import android.os.Bundle
import kotlinx.android.synthetic.main.activity_main.*


class MainActivity : AppCompatActivity() {
    val bm: Bitmap = Bitmap.createBitmap(21, 21, Bitmap.Config.ARGB_8888)

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        var x = 0
        Thread(Runnable {
            while (true) {
                runOnUiThread {
                    this@MainActivity.renderQR(bm)
                    val bt = Bitmap.createScaledBitmap(bm, 200, 200, false);
                    when (x) {
                        0 -> {
                            imageView0.setImageBitmap(bt)
                        }
                        1 -> {
                            imageView1.setImageBitmap(bt)
                        }
                        2 -> {
                            imageView2.setImageBitmap(bt)
                        }
                        3 -> {
                            imageView3.setImageBitmap(bt)
                        }
                    }
                    x++
                    if (x == 4) {
                        x = 0
                    }
                }
                Thread.sleep(30)
            }
        }).start()
        imageView0.setImageBitmap(bm)
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    external fun stringFromJNI(): String

    external fun renderQR(bm: Bitmap)

    companion object {
        // Used to load the 'native-lib' library on application startup.
        init {
            System.loadLibrary("native-lib")
        }
    }
}

