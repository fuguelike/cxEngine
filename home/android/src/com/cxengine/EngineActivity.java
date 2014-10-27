
package com.cxengine;
import android.app.Activity;
import android.os.Bundle;
import android.view.KeyEvent;
import android.view.MotionEvent;

public class EngineActivity extends Activity {
	
    static {
        System.loadLibrary("Home");
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
    	int action = event.getAction() & MotionEvent.ACTION_MASK;
    	if(action == MotionEvent.ACTION_POINTER_DOWN){
			int index = event.getAction() >> MotionEvent.ACTION_POINTER_INDEX_SHIFT;
			int[] ids = new int[1];
	    	float[] xs = new float[1];
	    	float[] ys = new float[1];
			ids[0] = event.getPointerId(index);;
			xs[0] = event.getX(index);
			ys[0] = event.getY(index);
			action = MotionEvent.ACTION_DOWN;
			glView.cxEngineAsyncFireTouch(action, ids, xs, ys);
    	}else if(action == MotionEvent.ACTION_POINTER_UP){
    		int index = event.getAction() >> MotionEvent.ACTION_POINTER_INDEX_SHIFT;
			int[] ids = new int[1];
			float[] xs = new float[1];
			float[] ys = new float[1];
			ids[0] = event.getPointerId(index);;
			xs[0] = event.getX(index);
			ys[0] = event.getY(index);
			action = MotionEvent.ACTION_UP;
			glView.cxEngineAsyncFireTouch(action, ids, xs, ys);
    	}else if(action == MotionEvent.ACTION_MOVE || action == MotionEvent.ACTION_CANCEL){
    		int number = event.getPointerCount();
    		int[] ids = new int[number];
        	float[] xs = new float[number];
    		float[] ys = new float[number];
			for (int i = 0; i < number; i++) {
				ids[i] = event.getPointerId(i);
				xs[i] = event.getX(i);
				ys[i] = event.getY(i);
			}
			glView.cxEngineAsyncFireTouch(action, ids, xs, ys);
    	}else if(action == MotionEvent.ACTION_DOWN){
			int[] ids = new int[1];
			float[] xs = new float[1];
			float[] ys = new float[1];
			ids[0] = event.getPointerId(0);;
			xs[0] = event.getX(0);
			ys[0] = event.getY(0);
			glView.cxEngineAsyncFireTouch(action, ids, xs, ys);
    	}else if(action == MotionEvent.ACTION_UP){
			int[] ids = new int[1];
			float[] xs = new float[1];
			float[] ys = new float[1];
			ids[0] = event.getPointerId(0);;
			xs[0] = event.getX(0);
			ys[0] = event.getY(0);
			glView.cxEngineAsyncFireTouch(action, ids, xs, ys);
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

