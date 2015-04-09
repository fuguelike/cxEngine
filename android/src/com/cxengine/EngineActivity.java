package com.cxengine;

import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.util.HashMap;
import java.util.UUID;
import android.app.NativeActivity;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.graphics.Paint.Align;
import android.graphics.Paint.FontMetricsInt;
import android.graphics.Paint.Style;
import android.graphics.Typeface;
import android.os.Bundle;

public class EngineActivity extends NativeActivity {
	private HashMap<String, Typeface> sTypefaceCache = new HashMap<String, Typeface>();
	public void onCreate(Bundle state){
		super.onCreate(state);
	}
	public void onDestroy(){
		super.onDestroy();
	}
	public void OnRecvJson(final String json){
		this.runOnUiThread(new Runnable(){
			public void run(){
				//
			}
		});
	}
	public void EngineTerminate(){
		this.runOnUiThread(new Runnable(){
			public void run(){
				EngineActivity.this.finish();
			}
		});
	}
    public String EngineUUID(){
    	UUID uuid = UUID.randomUUID();
    	return uuid.toString();
    }
    public String EngineDefaultLocalized(){
    	String ret = null;
    	try {
			String[] files = this.getAssets().list("strings");
			if(files.length >= 1){
				ret = "strings/"+files[0];
			}
		} catch (IOException e) {
			e.printStackTrace();
		}
    	return ret;
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
	public synchronized Typeface getTypeface(String pAssetName) {
		if (!sTypefaceCache.containsKey(pAssetName)) {
			Typeface typeface = null;
			if (pAssetName.startsWith("/")){
				typeface = Typeface.createFromFile(pAssetName);
			}else{
				typeface = Typeface.createFromAsset(this.getAssets(), pAssetName);
			}
			sTypefaceCache.put(pAssetName, typeface);
		}
		return sTypefaceCache.get(pAssetName);
	}
	private Paint newPaint(String fontName,int fontSize,boolean bold) {
		Paint paint = new Paint();
		paint.setTextSize(fontSize); 
		paint.setAntiAlias(true);
		if (fontName != null && fontName.endsWith(".ttf")) {
			try {
				Typeface typeFace = getTypeface(fontName);
				paint.setTypeface(typeFace);
			} catch (Exception e) {
				paint.setTypeface(Typeface.create(fontName, bold?Typeface.BOLD:Typeface.NORMAL));
			}
		} else if(fontName != null) {
			paint.setTypeface(Typeface.create(fontName, bold?Typeface.BOLD:Typeface.NORMAL));
		} else {
			paint.setTypeface(bold?Typeface.DEFAULT_BOLD:Typeface.DEFAULT);
		}
		paint.setTextAlign(Align.LEFT);
		return paint;
	}
	public byte[] createTextBitmap(String pString, 
			String pFontName, boolean bold,int fontSize,
			int align,
			float r,float g,float b,float a,
			float sw,float sr,float sg,float sb,float sa,float sx,float sy) {
		if(pString.length() == 0){
			return null;
		}
		//normal
		Paint paint = newPaint(pFontName, fontSize, bold);
		paint.setFakeBoldText(bold);
		paint.setFlags(Paint.ANTI_ALIAS_FLAG);
		paint.setStyle(Style.FILL);
		paint.setARGB((int)(a*255),(int)(r*255), (int)(g*255), (int)(b*255));
		//stroke
		Paint stroke = null;
		if(sw > 0){
			stroke = newPaint(pFontName, fontSize, bold);
			stroke.setFakeBoldText(bold);
			stroke.setFlags(Paint.ANTI_ALIAS_FLAG);
			stroke.setStrokeWidth(sw);
			stroke.setStyle(Style.STROKE);
			stroke.setARGB((int)(sa*255),(int)(sr*255), (int)(sg*255), (int)(sb*255));
		}
		int maxWidth = 0;
		int maxHeight = 0;
		int number = 0;
		FontMetricsInt fm = paint.getFontMetricsInt();
		int h = (int)Math.ceil(fm.bottom - fm.top);
		if(stroke != null){
			h += sy;
		}
		//获取总高度，宽度，每行的高度，宽度
		String[] lines = pString.split("\\n");
		int[] ws = new int[lines.length];
		int[] hs = new int[lines.length];
		for (String line : lines) {
			int w = (int) Math.ceil(paint.measureText(line, 0, line.length()));
			if(stroke != null){
				w += sx;
			}
			if(w > maxWidth){
				maxWidth = w;
			}
			ws[number] = w;
			hs[number] = h;
			number ++;
			maxHeight += h;
		}
		Bitmap bitmap = Bitmap.createBitmap(maxWidth,maxHeight, Bitmap.Config.ARGB_8888);
		Canvas canvas = new Canvas(bitmap);
		int x = 0;
		int y = -paint.getFontMetricsInt().top;
		number = 0;
		for (String line : lines) {
			if(align == 2){
				x = (maxWidth - ws[number])/2;
			}else if(align == 3){
				x = maxWidth - ws[number];
			}else{
				x = 0;
			}
			if(stroke != null){
				canvas.drawText(line, x+sx, y+sy, stroke);
			}
			if(paint != null){
				canvas.drawText(line, x, y, paint);
			}
			y+= hs[number];
			number ++;
		}
		return getPixels(bitmap);
	}
}
