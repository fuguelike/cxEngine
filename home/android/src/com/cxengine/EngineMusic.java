

package com.cxengine;
import java.io.FileInputStream;
import android.content.Context;
import android.content.res.AssetFileDescriptor;
import android.media.MediaPlayer;
import android.util.Log;

public class EngineMusic {
	private final Context mContext;
	private MediaPlayer mediaPlayer;
	private float mLeftVolume;
	private float mRightVolume;
	private boolean mPaused;
	private String mCurrentPath;
	
	public EngineMusic(final Context pContext) {
		this.mContext = pContext;
		this.initData();
	}
	
	public void preloadBackgroundMusic(final String pPath) {
		if ((this.mCurrentPath == null) || (!this.mCurrentPath.equals(pPath))) {
			if (this.mediaPlayer != null) {
				this.mediaPlayer.release();
			}
			this.mediaPlayer = this.createMediaplayer(pPath);
			this.mCurrentPath = pPath;
		}
	}

	public void playBackgroundMusic(final String pPath, final boolean isLoop) {
		if (this.mCurrentPath == null) {
			this.mediaPlayer = this.createMediaplayer(pPath);
			this.mCurrentPath = pPath;
		} else {
			if (!this.mCurrentPath.equals(pPath)) {
				if (this.mediaPlayer != null) {
					this.mediaPlayer.release();
				}
				this.mediaPlayer = this.createMediaplayer(pPath);
				this.mCurrentPath = pPath;
			}
		}

		if (this.mediaPlayer == null) {
			Log.e("EngineMusic", "playBackgroundMusic: background media player is null");
		} else {
			this.mediaPlayer.stop();
			this.mediaPlayer.setLooping(isLoop);
			try {
				this.mediaPlayer.prepare();
				this.mediaPlayer.seekTo(0);
				this.mediaPlayer.start();
				this.mPaused = false;
			} catch (final Exception e) {
				Log.e("EngineMusic", "playBackgroundMusic: error state");
			}
		}
	}

	public void stopBackgroundMusic() {
		if (this.mediaPlayer != null) {
			this.mediaPlayer.stop();
			this.mPaused = false;
		}
	}

	public void pauseBackgroundMusic() {
		if (this.mediaPlayer != null && this.mediaPlayer.isPlaying()) {
			this.mediaPlayer.pause();
			this.mPaused = true;
		}
	}

	public void resumeBackgroundMusic() {
		if (this.mediaPlayer != null && this.mPaused) {
			this.mediaPlayer.start();
			this.mPaused = false;
		}
	}

	public void rewindBackgroundMusic() {
		if (this.mediaPlayer != null) {
			this.mediaPlayer.stop();
			try {
				this.mediaPlayer.prepare();
				this.mediaPlayer.seekTo(0);
				this.mediaPlayer.start();
				this.mPaused = false;
			} catch (final Exception e) {
				Log.e("EngineMusic", "rewindBackgroundMusic: error state");
			}
		}
	}

	public boolean isBackgroundMusicPlaying() {
		boolean ret = false;
		if (this.mediaPlayer == null) {
			ret = false;
		} else {
			ret = this.mediaPlayer.isPlaying();
		}
		return ret;
	}

	public void end() {
		if (this.mediaPlayer != null) {
			this.mediaPlayer.stop();
			this.mediaPlayer.release();
		}
		this.initData();
	}

	public float getBackgroundVolume() {
		if (this.mediaPlayer != null) {
			return (this.mLeftVolume + this.mRightVolume) / 2;
		} else {
			return 0.0f;
		}
	}

	public void setBackgroundVolume(float pVolume) {
		if (pVolume < 0.0f) {
			pVolume = 0.0f;
		}
		if (pVolume > 1.0f) {
			pVolume = 1.0f;
		}
		this.mLeftVolume = this.mRightVolume = pVolume;
		if (this.mediaPlayer != null) {
			this.mediaPlayer.setVolume(this.mLeftVolume, this.mRightVolume);
		}
	}

	private void initData() {
		this.mLeftVolume = 0.5f;
		this.mRightVolume = 0.5f;
		this.mediaPlayer = null;
		this.mPaused = false;
		this.mCurrentPath = null;
	}

	private MediaPlayer createMediaplayer(final String pPath) {
		MediaPlayer mediaPlayer = new MediaPlayer();
		mediaPlayer.setOnCompletionListener(new MediaPlayer.OnCompletionListener(){
			@Override
			public void onCompletion(MediaPlayer mp) {
				Log.e("cxEngine", pPath + " play completed");				
			}
		});
		try {
			if (pPath.startsWith("/")) {
				final FileInputStream fis = new FileInputStream(pPath);
				mediaPlayer.setDataSource(fis.getFD());
				fis.close();
			} else {
				final AssetFileDescriptor assetFileDescritor = this.mContext.getAssets().openFd(pPath);
				mediaPlayer.setDataSource(assetFileDescritor.getFileDescriptor(), assetFileDescritor.getStartOffset(), assetFileDescritor.getLength());
			}
			mediaPlayer.prepare();
			mediaPlayer.setVolume(this.mLeftVolume, this.mRightVolume);
		} catch (final Exception e) {
			mediaPlayer = null;
			Log.e("EngineMusic", "error: " + e.getMessage(), e);
		}
		return mediaPlayer;
	}
}
