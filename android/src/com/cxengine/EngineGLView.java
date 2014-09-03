
package com.cxengine;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.util.Locale;

import android.content.Context;
import android.content.res.AssetManager;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Typeface;
import android.graphics.Paint.FontMetricsInt;
import android.opengl.GLSurfaceView;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class EngineGLView extends GLSurfaceView {
	
	private static final int HORIZONTALALIGN_LEFT = 1;
	private static final int HORIZONTALALIGN_RIGHT = 2;
	private static final int HORIZONTALALIGN_CENTER = 3;
	private static final int VERTICALALIGN_TOP = 1;
	private static final int VERTICALALIGN_BOTTOM = 2;
	private static final int VERTICALALIGN_CENTER = 3;
	
	private static String TAG = "EngineGLView";
	private static EngineActivity glActivity = null;
	private static EngineSound engineSound = null;
	private static EngineMusic engineMusic = null;
	private static EngineGLView glView = null;
	//
	public static native void cxEngineFireTouch(int action,int id[],float xs[],float ys[]);
	public void cxEngineAsyncFireTouch(final int action,final int id[],final float xs[],final float ys[]){
		this.queueEvent(new Runnable(){
			@Override
			public void run(){
				cxEngineFireTouch(action,id,xs,ys);
			}
		});
	}
	public static native void cxEngineFireKey(int type,int code);
	public void cxEngineAsyncFireKey(final int type,final int code){
		this.queueEvent(new Runnable(){
			@Override
			public void run(){
				cxEngineFireKey(type,code);
			}
		});
	}
    public static native void cxEngineLayout(int width, int height);
    public static native void cxEngineBegin();
    public static native void cxEngineDraw();
    public static native void cxEnginePause();
    public static native void cxEngineResume();
    public static native void cxEngineMemory();
    
    //completed exit android app
    public static void cxEngineTerminate() {
    	glActivity.runOnUiThread(new Runnable(){
    		@Override
    		public void run(){
    			glView.onDestroy();
    			glActivity.finish();
    			terminateProcess();
    		}
    	});
    }
    //sound method
    public static int cxEnginePlayEffect(final String file,final boolean loop) {
    	return engineSound.playEffect(file, loop, 1.0f, 0.0f, 1.0f);
    }
    public static void cxEnginePauseEffect(final int id){
    	glActivity.runOnUiThread(new Runnable(){
    		@Override
    		public void run(){
    			engineSound.pauseEffect(id);
    		}
    	});
    }
    public static void cxEngineResumeEffect(final int id){
    	glActivity.runOnUiThread(new Runnable(){
    		@Override
    		public void run(){
    			engineSound.resumeEffect(id);
    		}
    	});
    }
    public static void cxEngineStopEffect(final int id){
    	glActivity.runOnUiThread(new Runnable(){
    		@Override
    		public void run(){
    			engineSound.stopEffect(id);
    		}
    	});
    }
    public static void cxEnginePlayMusic(final String file,final boolean loop){
    	glActivity.runOnUiThread(new Runnable(){
    		@Override
    		public void run(){
    			engineMusic.playBackgroundMusic(file, loop);
    		}
    	});
    }
    public static void cxEngineStopMusic(){
    	glActivity.runOnUiThread(new Runnable(){
    		@Override
    		public void run(){
    			engineMusic.stopBackgroundMusic();
    		}
    	});
    }
    public static void cxEnginePauseMusic(){
    	glActivity.runOnUiThread(new Runnable(){
    		@Override
    		public void run(){
    			engineMusic.pauseBackgroundMusic();
    		}
    	});	
    }
    public static void cxEngineResumeMusic(){
    	glActivity.runOnUiThread(new Runnable(){
    		@Override
    		public void run(){
    			engineMusic.resumeBackgroundMusic();
    		}
    	});	
    }
    //get localized lang
    public static String cxEngineLocalized(){
    	return Locale.getDefault().getLanguage();
    }
    //json invoke method
    public static void cxEngineRecvJson(final String json) {
    	glActivity.runOnUiThread(new Runnable(){
    		@Override
    		public void run(){
    			glActivity.onRecvJson(json);
    		}
    	});
    }
    //send json -> cxEngine c runtime
    public static native void cxEngineSendJsonImp(String json);
    public static void cxEngineSendJson(final String json){
    	glView.queueEvent(new Runnable(){
    		@Override
    		public void run(){
    			cxEngineSendJsonImp(json);
    		}
    	});
    }
    //assert mehtod
    public static AssetManager cxEngineAssertManager(){
    	return glActivity.getAssets();
    }
  //get getdocument path
    public static String cxEngineDocumentPath(String file){
    	String path = glActivity.getFilesDir().getAbsolutePath();
    	if(file != null){
    		return path + "/" + file;
    	}
    	return path;
    }
    //force exit
	public static void terminateProcess() {
		android.os.Process.killProcess(android.os.Process.myPid());
	}
    //create bitmap method
	private Paint newPaint(String pFontName, int pFontSize) {
		Paint paint = new Paint();
		paint.setColor(Color.WHITE);
		paint.setTextSize(pFontSize);
		paint.setAntiAlias(true);
		if (pFontName != null) {
			paint.setTypeface(Typeface.create(pFontName, Typeface.NORMAL));
		} else {
			paint.setTypeface(Typeface.DEFAULT);
		}
		return paint;
	}
	private byte[] packInt(int v) {
		byte[] wb = new byte[4];
		wb[0] = (byte) (v & 0xFF);
		wb[1] = (byte) ((v >> 8) & 0xFF);
		wb[2] = (byte) ((v >> 16) & 0xFF);
		wb[3] = (byte) ((v >> 24) & 0xFF);
		return wb;
	}
	private byte[] getPixels(Bitmap pBitmap) {
		int width = pBitmap.getWidth();
		int height = pBitmap.getHeight();
		int isize = width * height * 4;
		byte[] wb = packInt(width);
		byte[] hb = packInt(height);
		byte[] pixels = new byte[isize + 8];
		ByteBuffer buf = ByteBuffer.wrap(pixels);
		buf.order(ByteOrder.nativeOrder());
		pBitmap.copyPixelsToBuffer(buf);
		System.arraycopy(wb, 0, pixels, isize, 4);
		System.arraycopy(hb, 0, pixels, isize + 4, 4);
		return pixels;
	}
	//create ttf image
	public byte[] createTextBitmapImp(String pString, String pFontName, int fontSize,int w,int h,int align,int stroke) {
		Paint paint = newPaint(pFontName, fontSize);
		FontMetricsInt fm = paint.getFontMetricsInt();
		int height = (int) Math.ceil(fm.bottom - fm.top);
		int width = (int) Math.ceil(paint.measureText(pString, 0, pString.length()));
		Bitmap bitmap = Bitmap.createBitmap(width, height, Bitmap.Config.ARGB_8888);
		Canvas canvas = new Canvas(bitmap);
		canvas.drawText(pString, 0, -fm.top, paint);
		return getPixels(bitmap);
	}
	
	public static byte[] createTextBitmap(String pString, String pFontName, int fontSize,int w,int h,int align,int stroke) {
		return glView.createTextBitmapImp(pString, pFontName, fontSize, w, h,align, stroke);
	}
    public EngineGLView(Context context) {
        super(context);
        glView = this;
        glActivity = (EngineActivity)context;
        engineSound = new EngineSound(context);
    	engineMusic = new EngineMusic(context);
    	setEGLContextClientVersion(2);
        setFocusable(true);
        setRenderer(new Renderer());
    }
    public void  onDestroy(){
    	engineSound.end();
    	engineMusic.end();
    }
    private static class Renderer implements GLSurfaceView.Renderer {
        public void onDrawFrame(GL10 gl) {
        	cxEngineDraw();
        }
        public void onSurfaceChanged(GL10 gl, int width, int height) {
        	cxEngineLayout(width,height);
        }
        public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        	cxEngineBegin();
        }
    }
}
