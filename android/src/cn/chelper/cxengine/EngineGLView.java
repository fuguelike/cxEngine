
package cn.chelper.cxengine;

import android.content.Context;
import android.content.res.AssetManager;
import android.opengl.GLSurfaceView;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

class EngineGLView extends GLSurfaceView {
	
	private static EngineActivity glActivity = null;
	private static EngineSound engineSound = null;
	private static EngineMusic engineMusic = null;
	static {
        System.loadLibrary("cxEngineTest");
    }
	
	private static EngineGLView glView = null;
	
	public static native void cxEngineFireTouch(int action,float x,float y);
	
    public static native void cxEngineLayout(int width, int height);
    
    public static native void cxEngineInit();
    
    public static native void cxEngineDraw();
    
    public static native void cxEnginePause();

    public static native void cxEngineResume();

    public static native void cxEngineMemory();
    
    public static native void cxEngineExit();
    
    //sound method
    public static int cxEnginePlayEffect(String file,boolean loop) {
    	return engineSound.playEffect(file, loop, 1.0f, 0.0f, 1.0f);
    }
    
    public static void cxEnginePauseEffect(int id){
    	engineSound.pauseEffect(id);
    }
    
    public static void cxEngineResumeEffect(int id){
    	engineSound.resumeEffect(id);
    }
    
    public static void cxEngineStopEffect(int id){
    	engineSound.stopEffect(id);
    }
    
    public static void cxEnginePlayMusic(final String file,final boolean loop){
    	engineMusic.playBackgroundMusic(file, loop);
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
    
    //each invoke method
    public static void cxEngineRecvJson(String json) {
    	
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
    
    private static String TAG = "EngineGLView";
  
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
        	cxEngineInit();
        }
    }
}
