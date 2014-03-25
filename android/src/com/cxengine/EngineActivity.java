
package com.cxengine;
import android.app.Activity;
import android.os.Bundle;
import android.view.KeyEvent;
import android.view.MotionEvent;

public class EngineActivity extends Activity {
	
	private EngineGLView glView = null;
	
	@Override
	protected void onCreate(Bundle state) {
		super.onCreate(state);
		glView = new EngineGLView(this);
		setContentView(glView);
	}
	
	public void onEngineJson(String json){
		
	}
	
	@Override
	public void onLowMemory(){
		glView.queueEvent(new Runnable(){
			@Override
			public void run(){
				EngineGLView.cxEngineMemory();
			}
		});
		super.onLowMemory();
	}
	
	@Override
	public void onPause(){
		glView.queueEvent(new Runnable(){
			@Override
			public void run(){
				EngineGLView.cxEnginePause();
			}
		});
		super.onPause();
	}
	
	@Override
	public void onResume(){
		glView.queueEvent(new Runnable(){
			@Override
			public void run(){
				EngineGLView.cxEngineResume();
			}
		});
		super.onResume();
	}
	
	@Override
    public boolean onTouchEvent(MotionEvent event){
    	final int action = event.getAction() & MotionEvent.ACTION_MASK;
		final int pointerNumber = event.getPointerCount();
		if(pointerNumber == 0){
			return false;
		}
		final int[] ids = new int[pointerNumber];
		final float[] xs = new float[pointerNumber];
		final float[] ys = new float[pointerNumber];
		for (int i = 0; i < pointerNumber; i++) {
			ids[i] = event.getPointerId(i);
			xs[i] = event.getX(i);
			ys[i] = event.getY(i);
		}
		glView.cxEngineAsyncFireTouch(action, xs[0], ys[0]);
    	return super.onTouchEvent(event);
    }
	
	@Override
	public boolean onKeyDown(final int keyCode, KeyEvent event){
		if(keyCode != KeyEvent.KEYCODE_BACK){
			return super.onKeyDown(keyCode, event);
		}
		glView.cxEngineAsyncFireKey(KeyEvent.ACTION_DOWN, keyCode);
		return true;
	}
	
	@Override
	public boolean onKeyUp(final int keyCode, KeyEvent event){
		if(keyCode != KeyEvent.KEYCODE_BACK){
			return super.onKeyUp(keyCode, event);
		}
		glView.cxEngineAsyncFireKey(KeyEvent.ACTION_UP, keyCode);
		return true;
	}
}

