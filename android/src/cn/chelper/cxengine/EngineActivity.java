
package cn.chelper.cxengine;
import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import android.view.MotionEvent;

public class EngineActivity extends Activity {
	
	private EngineGLView glView = null;
	
	@Override
	protected void onCreate(Bundle state) {
		super.onCreate(state);
		glView = new EngineGLView(this.getApplicationContext());
		this.setContentView(glView);
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
		final float x = xs[0];
		final float y = ys[0];
		glView.queueEvent(new Runnable(){
			@Override
			public void run(){
				EngineGLView.cxEngineFireTouch(action,x,y);
			}
		});
    	return super.onTouchEvent(event);
    }
	
	@Override
	public void onDestroy(){
		glView.queueEvent(new Runnable(){
			@Override
			public void run(){
				EngineGLView.cxEngineExit();
			}
		});
		Log.e("cxEngine", "cxEngineExit");
		super.onDestroy();
	}
}
