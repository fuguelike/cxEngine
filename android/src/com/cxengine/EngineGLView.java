
package com.cxengine;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.util.HashMap;
import java.util.Locale;
import android.content.Context;
import android.content.res.AssetManager;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Paint.Align;
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
	
	private static final HashMap<String, Typeface> sTypefaceCache = new HashMap<String, Typeface>();
	
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
    public static native void cxEngineStartup();
    public static native void cxEngineDraw(float dt);
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
	private String refactorString(final String string) {
		if (string.compareTo("") == 0) {
			return " ";
		}
		StringBuilder strBuilder = new StringBuilder(string);
		int start = 0;
		int index = strBuilder.indexOf("\n");
		while (index != -1) {
			if (index == 0 || strBuilder.charAt(index - 1) == '\n') {
				strBuilder.insert(start, " ");
				start = index + 2;
			} else {
				start = index + 1;
			}
			if (start > strBuilder.length() || index == strBuilder.length()) {
				break;
			}
			index = strBuilder.indexOf("\n", start);
		}
		return strBuilder.toString();
	}
	public synchronized Typeface getTypeface(String pAssetName) {
		if (!sTypefaceCache.containsKey(pAssetName)) {
			Typeface typeface = null;
			if (pAssetName.startsWith("/")){
				typeface = Typeface.createFromFile(pAssetName);
			}else{
				typeface = Typeface.createFromAsset(cxEngineAssertManager(), pAssetName);
			}
			sTypefaceCache.put(pAssetName, typeface);
		}

		return sTypefaceCache.get(pAssetName);
	}
	private Paint newPaint(String fontName,int fontSize,int horizontalAlignment) {
		Paint paint = new Paint();
		paint.setColor(Color.WHITE);
		paint.setTextSize(fontSize); 
		paint.setAntiAlias(true);
		if (fontName != null && fontName.endsWith(".ttf")) {
			try {
				Typeface typeFace = getTypeface(fontName);
				paint.setTypeface(typeFace);
			} catch (final Exception e) {
				paint.setTypeface(Typeface.create(fontName, Typeface.NORMAL));
			}
		} else if(fontName != null) {
			paint.setTypeface(Typeface.create(fontName, Typeface.NORMAL));
		} else {
			paint.setTypeface(Typeface.DEFAULT);
		}
		switch (horizontalAlignment) {
		case HORIZONTALALIGN_CENTER:
			paint.setTextAlign(Align.CENTER);
			break;
		case HORIZONTALALIGN_RIGHT:
			paint.setTextAlign(Align.RIGHT);
			break;
		case HORIZONTALALIGN_LEFT:
		default:
			paint.setTextAlign(Align.LEFT);
			break;
		}
		return paint;
	}
	private class TextProperty 
	{
		public int mMaxWidth;
		public int mTotalHeight;
		public int mHeightPerLine;
		public String[] mLines;
	}
	private TextProperty computeTextProperty(final String string,final int width, final int height, final Paint paint) {
		TextProperty rv = new TextProperty();
		FontMetricsInt fm = paint.getFontMetricsInt();
		int h = (int) Math.ceil(fm.bottom - fm.top);
		int maxContentWidth = 0;
		String[] lines = string.split("\\n");
		if (width != 0) {
			maxContentWidth = width;
		} else {
			int temp = 0;
			for (String line : lines) {
				temp = (int) Math.ceil(paint.measureText(line, 0, line.length()));
				if (temp > maxContentWidth) {
					maxContentWidth = temp;
				}
			}
		}
		rv.mMaxWidth = width > maxContentWidth?width : maxContentWidth;
		rv.mHeightPerLine = h;
		rv.mTotalHeight = rv.mHeightPerLine * lines.length;
		rv.mTotalHeight = height > rv.mTotalHeight ? height : rv.mTotalHeight;
		rv.mLines = lines;
		return rv;
	}
	private int computeX(final String text, final int maxWidth, final int horizontalAlignment) {
		int ret = 0;
		switch (horizontalAlignment) {
		case HORIZONTALALIGN_CENTER:
			ret = maxWidth / 2;
			break;
		case HORIZONTALALIGN_RIGHT:
			ret = maxWidth;
			break;
		case HORIZONTALALIGN_LEFT:
		default:
			break;
		}
		return ret;
	}

	private int computeY(final FontMetricsInt fontMetricsInt,final int constrainHeight, final int totalHeight,final int verticalAlignment) {
		int y = -fontMetricsInt.top;
		if (constrainHeight > totalHeight) {
			switch (verticalAlignment) {
			case VERTICALALIGN_TOP:
				y = -fontMetricsInt.top;
				break;
			case VERTICALALIGN_CENTER:
				y = -fontMetricsInt.top + (constrainHeight - totalHeight) / 2;
				break;
			case VERTICALALIGN_BOTTOM:
				y = -fontMetricsInt.top + (constrainHeight - totalHeight);
				break;
			default:
				break;
			}
		}
		return y;
	}
	//create ttf image
	public byte[] createTextBitmapImp(String pString, String pFontName, int fontSize,int align,int w,int h) {
		int hAlign = align & 0x0F;
		int vAlign   = (align >> 4) & 0x0F;
		pString = refactorString(pString);
		Paint paint = newPaint(pFontName, fontSize, hAlign);
		paint.setARGB(255, 255, 255, 255);
		final TextProperty textProperty = computeTextProperty(pString, w, h, paint);
		int totalHeight = (h == 0 ? textProperty.mTotalHeight: h);
		if(h > totalHeight){
			totalHeight = h;
		}
		float bitmapPaddingX   = 0.0f;
		float bitmapPaddingY   = 0.0f;
		float renderTextDeltaX = 0.0f;
		float renderTextDeltaY = 0.0f;
		if (0 == textProperty.mMaxWidth || 0 == totalHeight){
			return null;
		}
		Bitmap bitmap = Bitmap.createBitmap(textProperty.mMaxWidth + (int)bitmapPaddingX,totalHeight + (int)bitmapPaddingY, Bitmap.Config.ARGB_8888);
		Canvas canvas = new Canvas(bitmap);
		FontMetricsInt fontMetricsInt = paint.getFontMetricsInt();
		int x = 0;
		int y = computeY(fontMetricsInt, h, textProperty.mTotalHeight, vAlign);
		String[] lines = textProperty.mLines;
		for (String line : lines) {
			x = computeX(line, textProperty.mMaxWidth, hAlign);
			canvas.drawText(line, x + renderTextDeltaX, y + renderTextDeltaY, paint);
			y += textProperty.mHeightPerLine;
		}
		return getPixels(bitmap);
	}
	
	public static byte[] createTextBitmap(String pString, String pFontName, int fontSize,int align,int w,int h) {
		return glView.createTextBitmapImp(pString, pFontName, fontSize, align, w, h);
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
        	cxEngineDraw(1.0f/60.0f);
        }
        public void onSurfaceChanged(GL10 gl, int width, int height) {
        	cxEngineLayout(width,height);
        }
        public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        	cxEngineStartup();
        }
    }
}
