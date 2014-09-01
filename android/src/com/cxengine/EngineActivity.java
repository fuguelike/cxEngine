
package com.cxengine;
import android.app.Activity;
import android.os.Bundle;
import android.view.KeyEvent;
import android.view.MotionEvent;

public class EngineActivity extends Activity {
	
    static {
        System.loadLibrary("cxEngineTest");
    }
	
	private EngineGLView glView = null;
	
	@Override
	protected void onCreate(Bundle state) {
		super.onCreate(state);
		glView = new EngineGLView(this);
		setContentView(glView);
	}
	
	public void onRecvJson(String json){
		
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
		final float[] xs = new float[pointerNumber];
		final float[] ys = new float[pointerNumber];
		for (int i = 0; i < pointerNumber; i++) {
			xs[i] = event.getX(i);
			ys[i] = event.getY(i);
		}
		if(action == MotionEvent.ACTION_POINTER_DOWN){
			int idx = event.getAction() >> MotionEvent.ACTION_POINTER_INDEX_SHIFT;
			float x = event.getX(idx);
			float y = event.getY(idx);
			glView.cxEngineAsyncFireTouch(MotionEvent.ACTION_DOWN, x, y);
		}else if(action == MotionEvent.ACTION_POINTER_UP){
			int idx = event.getAction() >> MotionEvent.ACTION_POINTER_INDEX_SHIFT;
			float x = event.getX(idx);
			float y = event.getY(idx);
			glView.cxEngineAsyncFireTouch(MotionEvent.ACTION_UP, x, y);
		}else if(pointerNumber == 1){
			glView.cxEngineAsyncFireTouch(action, xs[0], ys[0]);
		}else{
			glView.cxEngineAsyncFireGesture(action, xs, ys);
		}
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

