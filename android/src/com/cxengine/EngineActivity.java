
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
		int action = event.getAction() & MotionEvent.ACTION_MASK;
    	float x = event.getX();
    	float y = event.getY();
    	if(action == MotionEvent.ACTION_DOWN || 
    			action == MotionEvent.ACTION_CANCEL || 
    			action == MotionEvent.ACTION_MOVE || 
    			action == MotionEvent.ACTION_UP){
    		glView.cxEngineAsyncFireTouch(action, x, y);
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

