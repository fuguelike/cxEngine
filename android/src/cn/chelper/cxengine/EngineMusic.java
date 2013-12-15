

package cn.chelper.cxengine;
import java.io.FileInputStream;
import android.content.Context;
import android.content.res.AssetFileDescriptor;
import android.media.MediaPlayer;
import android.util.Log;

public class EngineMusic {
	private final Context mContext;
	private MediaPlayer mBackgroundMediaPlayer;
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
			if (this.mBackgroundMediaPlayer != null) {
				this.mBackgroundMediaPlayer.release();
			}
			this.mBackgroundMediaPlayer = this.createMediaplayer(pPath);
			this.mCurrentPath = pPath;
		}
	}

	public void playBackgroundMusic(final String pPath, final boolean isLoop) {
		if (this.mCurrentPath == null) {
			this.mBackgroundMediaPlayer = this.createMediaplayer(pPath);
			this.mCurrentPath = pPath;
		} else {
			if (!this.mCurrentPath.equals(pPath)) {
				if (this.mBackgroundMediaPlayer != null) {
					this.mBackgroundMediaPlayer.release();
				}
				this.mBackgroundMediaPlayer = this.createMediaplayer(pPath);
				this.mCurrentPath = pPath;
			}
		}

		if (this.mBackgroundMediaPlayer == null) {
			Log.e("EngineMusic", "playBackgroundMusic: background media player is null");
		} else {
			this.mBackgroundMediaPlayer.stop();
			this.mBackgroundMediaPlayer.setLooping(isLoop);
			try {
				this.mBackgroundMediaPlayer.prepare();
				this.mBackgroundMediaPlayer.seekTo(0);
				this.mBackgroundMediaPlayer.start();

				this.mPaused = false;
			} catch (final Exception e) {
				Log.e("EngineMusic", "playBackgroundMusic: error state");
			}
		}
	}

	public void stopBackgroundMusic() {
		if (this.mBackgroundMediaPlayer != null) {
			this.mBackgroundMediaPlayer.stop();
			this.mPaused = false;
		}
	}

	public void pauseBackgroundMusic() {
		if (this.mBackgroundMediaPlayer != null && this.mBackgroundMediaPlayer.isPlaying()) {
			this.mBackgroundMediaPlayer.pause();
			this.mPaused = true;
		}
	}

	public void resumeBackgroundMusic() {
		if (this.mBackgroundMediaPlayer != null && this.mPaused) {
			this.mBackgroundMediaPlayer.start();
			this.mPaused = false;
		}
	}

	public void rewindBackgroundMusic() {
		if (this.mBackgroundMediaPlayer != null) {
			this.mBackgroundMediaPlayer.stop();

			try {
				this.mBackgroundMediaPlayer.prepare();
				this.mBackgroundMediaPlayer.seekTo(0);
				this.mBackgroundMediaPlayer.start();

				this.mPaused = false;
			} catch (final Exception e) {
				Log.e("EngineMusic", "rewindBackgroundMusic: error state");
			}
		}
	}

	public boolean isBackgroundMusicPlaying() {
		boolean ret = false;

		if (this.mBackgroundMediaPlayer == null) {
			ret = false;
		} else {
			ret = this.mBackgroundMediaPlayer.isPlaying();
		}

		return ret;
	}

	public void end() {
		if (this.mBackgroundMediaPlayer != null) {
			this.mBackgroundMediaPlayer.release();
		}

		this.initData();
	}

	public float getBackgroundVolume() {
		if (this.mBackgroundMediaPlayer != null) {
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
		if (this.mBackgroundMediaPlayer != null) {
			this.mBackgroundMediaPlayer.setVolume(this.mLeftVolume, this.mRightVolume);
		}
	}

	private void initData() {
		this.mLeftVolume = 0.5f;
		this.mRightVolume = 0.5f;
		this.mBackgroundMediaPlayer = null;
		this.mPaused = false;
		this.mCurrentPath = null;
	}

	private MediaPlayer createMediaplayer(final String pPath) {
		MediaPlayer mediaPlayer = new MediaPlayer();
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
