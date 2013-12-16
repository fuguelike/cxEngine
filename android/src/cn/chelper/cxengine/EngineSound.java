
package cn.chelper.cxengine;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;
import java.util.Map.Entry;
import java.util.concurrent.Semaphore;
import android.content.Context;
import android.media.AudioManager;
import android.media.SoundPool;
import android.util.Log;

public class EngineSound {
	private final Context mContext;
	private SoundPool mSoundPool;
	private float mLeftVolume;
	private float mRightVolume;

	private final HashMap<String, ArrayList<Integer>> mPathStreamIDsMap = new HashMap<String, ArrayList<Integer>>();
	private final HashMap<String, Integer> mPathSoundIDMap = new HashMap<String, Integer>();
	private final ArrayList<SoundInfoForLoadedCompleted> mEffecToPlayWhenLoadedArray = new ArrayList<SoundInfoForLoadedCompleted>();

	private int mStreamIdSyn;
	private Semaphore mSemaphore;

	private static final int MAX_SIMULTANEOUS_STREAMS_DEFAULT = 10;
	private static final float SOUND_RATE = 1.0f;
	private static final int SOUND_PRIORITY = 1;
	private static final int SOUND_QUALITY = 5;

	private final static int INVALID_SOUND_ID = -1;
	private final static int INVALID_STREAM_ID = -1;

	public EngineSound(final Context pContext) {
		this.mContext = pContext;
		this.initData();
	}

	private void initData() {
		this.mSoundPool = new SoundPool(EngineSound.MAX_SIMULTANEOUS_STREAMS_DEFAULT, AudioManager.STREAM_MUSIC, EngineSound.SOUND_QUALITY);
		this.mSoundPool.setOnLoadCompleteListener(new OnLoadCompletedListener());
		this.mLeftVolume = 0.5f;
		this.mRightVolume = 0.5f;
		this.mSemaphore = new Semaphore(0, true);
	}
	
	public int preloadEffect(final String pPath) {
		Integer soundID = this.mPathSoundIDMap.get(pPath);
		if (soundID == null) {
			soundID = this.createSoundIDFromAsset(pPath);
			if (soundID != EngineSound.INVALID_SOUND_ID) {
				this.mPathSoundIDMap.put(pPath, soundID);
			}
		}
		return soundID;
	}

	public void unloadEffect(final String pPath) {
		final ArrayList<Integer> streamIDs = this.mPathStreamIDsMap.get(pPath);
		if (streamIDs != null) {
			for (final Integer pStreamID : streamIDs) {
				this.mSoundPool.stop(pStreamID);
			}
		}
		this.mPathStreamIDsMap.remove(pPath);
		final Integer soundID = this.mPathSoundIDMap.get(pPath);
		if(soundID != null){
			this.mSoundPool.unload(soundID);
			this.mPathSoundIDMap.remove(pPath);
		}
	}

	public int playEffect(final String pPath, final boolean pLoop, float pitch, float pan, float gain){
		Integer soundID = this.mPathSoundIDMap.get(pPath);
		int streamID = EngineSound.INVALID_STREAM_ID;

		if (soundID != null) {
			streamID = this.doPlayEffect(pPath, soundID.intValue(), pLoop, pitch, pan, gain);
		} else {
			soundID = this.preloadEffect(pPath);
			if (soundID == EngineSound.INVALID_SOUND_ID) {
				return EngineSound.INVALID_SOUND_ID;
			}
			synchronized(this.mSoundPool) {
				SoundInfoForLoadedCompleted sound = new SoundInfoForLoadedCompleted(
						pPath, 
						soundID.intValue(), 
						pLoop,
						pitch, 
						pan, 
						gain);
				mEffecToPlayWhenLoadedArray.add(sound);
				try {
					this.mSemaphore.acquire();
					streamID = this.mStreamIdSyn;
				} catch(Exception e) {
					return EngineSound.INVALID_SOUND_ID;
				}
			}
		}

		return streamID;
	}

	public void stopEffect(final int pStreamID) {
		this.mSoundPool.stop(pStreamID);
		for (final String pPath : this.mPathStreamIDsMap.keySet()) {
			if (this.mPathStreamIDsMap.get(pPath).contains(pStreamID)) {
				this.mPathStreamIDsMap.get(pPath).remove(this.mPathStreamIDsMap.get(pPath).indexOf(pStreamID));
				break;
			}
		}
	}

	public void pauseEffect(final int pStreamID) {
		this.mSoundPool.pause(pStreamID);
	}

	public void resumeEffect(final int pStreamID) {
		this.mSoundPool.resume(pStreamID);
	}

	public void pauseAllEffects() {
		this.mSoundPool.autoPause();
	}

	public void resumeAllEffects() {
		if (!this.mPathStreamIDsMap.isEmpty()) {
			final Iterator<Entry<String, ArrayList<Integer>>> iter = this.mPathStreamIDsMap.entrySet().iterator();
			while (iter.hasNext()) {
				final Entry<String, ArrayList<Integer>> entry = iter.next();
				for (final int pStreamID : entry.getValue()) {
					this.mSoundPool.resume(pStreamID);
				}
			}
		}
	}

	@SuppressWarnings("unchecked")
	public void stopAllEffects() {
		if (this.mPathStreamIDsMap.isEmpty()) {
			return;
		}
		final Iterator<?> iter = this.mPathStreamIDsMap.entrySet().iterator();
		while (iter.hasNext()) {
			final Map.Entry<String, ArrayList<Integer>> entry = (Map.Entry<String, ArrayList<Integer>>) iter.next();
			for (final int pStreamID : entry.getValue()) {
				this.mSoundPool.stop(pStreamID);
			}
		}
		this.mPathStreamIDsMap.clear();
	}

	public float getEffectsVolume() {
		return (this.mLeftVolume + this.mRightVolume) / 2;
	}

	public void setEffectsVolume(float pVolume) {
		if (pVolume < 0) {
			pVolume = 0;
		}
		if (pVolume > 1) {
			pVolume = 1;
		}
		this.mLeftVolume = this.mRightVolume = pVolume;
		if (this.mPathStreamIDsMap.isEmpty()) {
			return;
		}
		final Iterator<Entry<String, ArrayList<Integer>>> iter = this.mPathStreamIDsMap.entrySet().iterator();
		while (iter.hasNext()) {
			final Entry<String, ArrayList<Integer>> entry = iter.next();
			for (final int pStreamID : entry.getValue()) {
				this.mSoundPool.setVolume(pStreamID, this.mLeftVolume, this.mRightVolume);
			}
		}
	}

	public void end() {
		this.mSoundPool.release();
		this.mPathStreamIDsMap.clear();
		this.mPathSoundIDMap.clear();
		this.mEffecToPlayWhenLoadedArray.clear();
		this.mLeftVolume = 0.5f;
		this.mRightVolume = 0.5f;
		this.initData();
	}

	public int createSoundIDFromAsset(final String pPath) {
		int soundID = EngineSound.INVALID_SOUND_ID;
		try {
			if (pPath.startsWith("/")) {
				soundID = this.mSoundPool.load(pPath, 0);
			} else {
				soundID = this.mSoundPool.load(this.mContext.getAssets().openFd(pPath), 0);
			}
		} catch (final Exception e) {
			soundID = EngineSound.INVALID_SOUND_ID;
			Log.e("EngineSound", "error: " + e.getMessage(), e);
		}
		if (soundID == 0) {
			soundID = EngineSound.INVALID_SOUND_ID;
		}
		return soundID;
	}

	private float clamp(float value, float min, float max) {
		return Math.max(min, (Math.min(value, max)));
	}

	private int doPlayEffect(final String pPath, final int soundId, final boolean pLoop, float pitch, float pan, float gain) {
		float leftVolume = this.mLeftVolume * gain * (1.0f - this.clamp(pan, 0.0f, 1.0f));
		float rightVolume = this.mRightVolume * gain * (1.0f - this.clamp(-pan, 0.0f, 1.0f));
		float soundRate = this.clamp(SOUND_RATE * pitch, 0.5f, 2.0f);
		int streamID = this.mSoundPool.play(soundId, this.clamp(leftVolume, 0.0f, 1.0f), this.clamp(rightVolume, 0.0f, 1.0f), EngineSound.SOUND_PRIORITY, pLoop ? -1 : 0, soundRate);
		ArrayList<Integer> streamIDs = this.mPathStreamIDsMap.get(pPath);
		if (streamIDs == null) {
			streamIDs = new ArrayList<Integer>();
			this.mPathStreamIDsMap.put(pPath, streamIDs);
		}
		streamIDs.add(streamID);
		return streamID;
	}
	
	public class SoundInfoForLoadedCompleted {
		public int soundID;
		public boolean isLoop;
		public float pitch;
		public float pan;
		public float gain;
		public String path;
		public SoundInfoForLoadedCompleted(String path, int soundId, boolean isLoop,float pitch, float pan, float gain) {
			this.path = path;
			this.soundID = soundId;
			this.isLoop = isLoop;
			this.pitch = pitch;
			this.pan = pan;
			this.gain = gain;
		}
	}

	public class OnLoadCompletedListener implements SoundPool.OnLoadCompleteListener {
		@Override
		public void onLoadComplete(SoundPool soundPool, int sampleId, int status) {
			if (status == 0) {
				for ( SoundInfoForLoadedCompleted info : mEffecToPlayWhenLoadedArray) {
					if (sampleId == info.soundID) {
						mStreamIdSyn = doPlayEffect(info.path, info.soundID, info.isLoop, info.pitch, info.pan, info.gain);
						mEffecToPlayWhenLoadedArray.remove(info);
						break;
					}
				}
			} else {
				mStreamIdSyn = EngineSound.INVALID_SOUND_ID;
			}
			mSemaphore.release();
		}
	}
}
