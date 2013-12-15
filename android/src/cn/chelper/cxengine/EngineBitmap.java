
package cn.chelper.cxengine;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.util.LinkedList;
import android.annotation.SuppressLint;
import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Paint.Align;
import android.graphics.Paint.FontMetricsInt;
import android.graphics.Rect;
import android.graphics.Typeface;
import android.text.TextPaint;
import android.text.TextUtils;
import android.util.FloatMath;
import android.util.Log;

@SuppressLint("FloatMath")
public class EngineBitmap {

	private static final int HORIZONTALALIGN_LEFT = 1;
	private static final int HORIZONTALALIGN_RIGHT = 2;
	private static final int HORIZONTALALIGN_CENTER = 3;
	private static final int VERTICALALIGN_TOP = 1;
	private static final int VERTICALALIGN_BOTTOM = 2;
	private static final int VERTICALALIGN_CENTER = 3;

	private static Context _context;

	public static void setContext(final Context context) {
		EngineBitmap._context = context;
	}

	private static native void nativeInitBitmapDC(final int width,
			final int height, final byte[] pixels);

	
	public static void createTextBitmap(String string, final String fontName,
			final int fontSize, final int alignment, final int width,
			final int height) {
		
		//
		createTextBitmapShadowStroke( string, fontName, fontSize, 1.0f, 1.0f, 1.0f,   	// text font and color
									  alignment, width, height,							// alignment and size
									  false, 0.0f, 0.0f, 0.0f, 0.0f,           				// no shadow
									  false, 1.0f, 1.0f, 1.0f, 1.0f);						// no stroke
									 
	}

	@SuppressLint("FloatMath")
	public static boolean createTextBitmapShadowStroke(String string,  final String fontName, final int fontSize,
													final float fontTintR, final float fontTintG, final float fontTintB,
													final int alignment, final int width, final int height, final boolean shadow,
													final float shadowDX, final float shadowDY, final float shadowBlur, final float shadowOpacity, final boolean stroke,
													final float strokeR, final float strokeG, final float strokeB, final float strokeSize) {

		
		final int horizontalAlignment = alignment & 0x0F;
		final int verticalAlignment   = (alignment >> 4) & 0x0F;
		string = EngineBitmap.refactorString(string);
		final Paint paint = EngineBitmap.newPaint(fontName, fontSize, horizontalAlignment);
		if(0 != width) {
			final int firstWordWidth = (int) FloatMath.ceil(paint.measureText(string, 0,1));
			if ( firstWordWidth > width)
			{
				Log.w("createTextBitmapShadowStroke warning:","the input width is less than the width of the pString's first word\n");
				return false;
			}
		}
		paint.setARGB(255, (int)(255.0 * fontTintR), (int)(255.0 * fontTintG), (int)(255.0 * fontTintB));
		final TextProperty textProperty = EngineBitmap.computeTextProperty(string, width, height, paint);
		final int bitmapTotalHeight = (height == 0 ? textProperty.mTotalHeight: height);
		float bitmapPaddingX   = 0.0f;
		float bitmapPaddingY   = 0.0f;
		float renderTextDeltaX = 0.0f;
		float renderTextDeltaY = 0.0f;
		if ( shadow ) {
			int shadowColor = ((int)(255 * shadowOpacity) & 0xff) << 24;
			paint.setShadowLayer(shadowBlur, shadowDX, shadowDY, shadowColor);
			bitmapPaddingX = Math.abs(shadowDX);
			bitmapPaddingY = Math.abs(shadowDY);
			if ( shadowDX < 0.0 ) {
				renderTextDeltaX = bitmapPaddingX;
			}
			if ( shadowDY < 0.0 ) {
				renderTextDeltaY = 	bitmapPaddingY;
			}
		}

		if (0 == textProperty.mMaxWidth || 0 == bitmapTotalHeight) {
			Log.w("createTextBitmapShadowStroke warning:","textProperty MaxWidth is 0 or bitMapTotalHeight is 0\n");
			return false;
		}
		final Bitmap bitmap = Bitmap.createBitmap(textProperty.mMaxWidth + (int)bitmapPaddingX,
				bitmapTotalHeight + (int)bitmapPaddingY, Bitmap.Config.ARGB_8888);
		
		final Canvas canvas = new Canvas(bitmap);
		/* Draw string. */
		final FontMetricsInt fontMetricsInt = paint.getFontMetricsInt();
		int x = 0;
		int y = EngineBitmap.computeY(fontMetricsInt, height, textProperty.mTotalHeight, verticalAlignment);
		final String[] lines = textProperty.mLines;
		for (final String line : lines) {
			x = EngineBitmap.computeX(line, textProperty.mMaxWidth, horizontalAlignment);
			canvas.drawText(line, x + renderTextDeltaX, y + renderTextDeltaY, paint);
			y += textProperty.mHeightPerLine;
		} 
		if ( stroke ) {
			final Paint paintStroke = EngineBitmap.newPaint(fontName, fontSize, horizontalAlignment);
			paintStroke.setStyle(Paint.Style.STROKE);
			paintStroke.setStrokeWidth(strokeSize * 0.5f);
			paintStroke.setARGB(255, (int) (strokeR * 255), (int) (strokeG * 255), (int) (strokeB * 255));
			x = 0;
			y = EngineBitmap.computeY(fontMetricsInt, height, textProperty.mTotalHeight, verticalAlignment);
			final String[] lines2 = textProperty.mLines;
			for (final String line : lines2) {
				x = EngineBitmap.computeX(line, textProperty.mMaxWidth, horizontalAlignment);
				canvas.drawText(line, x + renderTextDeltaX, y + renderTextDeltaY, paintStroke);
				y += textProperty.mHeightPerLine;
			}
		}
		EngineBitmap.initNativeObject(bitmap);
		return true;
	}

	private static Paint newPaint(final String fontName, final int fontSize,
			final int horizontalAlignment) {
		final Paint paint = new Paint();
		paint.setColor(Color.WHITE);
		paint.setTextSize(fontSize); 
		paint.setAntiAlias(true);
		if (fontName.endsWith(".ttf")) {
			try {
				final Typeface typeFace = EngineTypefaces.get(EngineBitmap._context, fontName);
				paint.setTypeface(typeFace);
			} catch (final Exception e) {
				Log.e("Cocos2dxBitmap", "error to create ttf type face: "+ fontName);
				paint.setTypeface(Typeface.create(fontName, Typeface.NORMAL));
			}
		} else {
			paint.setTypeface(Typeface.create(fontName, Typeface.NORMAL));
		}
		switch (horizontalAlignment) {
		case HORIZONTALALIGN_CENTER:
			paint.setTextAlign(Align.CENTER);
			break;
		case HORIZONTALALIGN_RIGHT:
			paint.setTextAlign(Align.RIGHT);
			break;
		case HORIZONTALALIGN_LEFT:
		default:
			paint.setTextAlign(Align.LEFT);
			break;
		}
		return paint;
	}
	
	@SuppressLint("FloatMath")
	private static TextProperty computeTextProperty(final String string,
			final int width, final int height, final Paint paint) {
		final FontMetricsInt fm = paint.getFontMetricsInt();
		final int h = (int) Math.ceil(fm.bottom - fm.top);
		int maxContentWidth = 0;
		final String[] lines = EngineBitmap.splitString(string, width, height, paint);
		if (width != 0) {
			maxContentWidth = width;
		} else {
			int temp = 0;
			for (final String line : lines) {
				temp = (int) FloatMath.ceil(paint.measureText(line, 0, line.length()));
				if (temp > maxContentWidth) {
					maxContentWidth = temp;
				}
			}
		}
		return new TextProperty(maxContentWidth, h, lines);
	}

	private static int computeX(final String text, final int maxWidth,
			final int horizontalAlignment) {
		int ret = 0;
		switch (horizontalAlignment) {
		case HORIZONTALALIGN_CENTER:
			ret = maxWidth / 2;
			break;
		case HORIZONTALALIGN_RIGHT:
			ret = maxWidth;
			break;
		case HORIZONTALALIGN_LEFT:
		default:
			break;
		}

		return ret;
	}

	private static int computeY(final FontMetricsInt fontMetricsInt,
			final int constrainHeight, final int totalHeight,
			final int verticalAlignment) {
		int y = -fontMetricsInt.top;
		if (constrainHeight > totalHeight) {
			switch (verticalAlignment) {
			case VERTICALALIGN_TOP:
				y = -fontMetricsInt.top;
				break;
			case VERTICALALIGN_CENTER:
				y = -fontMetricsInt.top + (constrainHeight - totalHeight) / 2;
				break;
			case VERTICALALIGN_BOTTOM:
				y = -fontMetricsInt.top + (constrainHeight - totalHeight);
				break;
			default:
				break;
			}
		}
		return y;
	}

	private static String[] splitString(final String string,
			final int maxWidth, final int maxHeight, final Paint paint) {
		final String[] lines = string.split("\\n");
		String[] ret = null;
		final FontMetricsInt fm = paint.getFontMetricsInt();
		final int heightPerLine = (int) Math.ceil(fm.bottom - fm.top);
		final int maxLines = maxHeight / heightPerLine;
		if (maxWidth != 0) {
			final LinkedList<String> strList = new LinkedList<String>();
			for (final String line : lines) {
				final int lineWidth = (int) FloatMath.ceil(paint
						.measureText(line));
				if (lineWidth > maxWidth) {
					strList.addAll(EngineBitmap.divideStringWithMaxWidth(
							line, maxWidth, paint));
				} else {
					strList.add(line);
				}
				if (maxLines > 0 && strList.size() >= maxLines) {
					break;
				}
			}
			if (maxLines > 0 && strList.size() > maxLines) {
				while (strList.size() > maxLines) {
					strList.removeLast();
				}
			}
			ret = new String[strList.size()];
			strList.toArray(ret);
		} else if (maxHeight != 0 && lines.length > maxLines) {
			final LinkedList<String> strList = new LinkedList<String>();
			for (int i = 0; i < maxLines; i++) {
				strList.add(lines[i]);
			}
			ret = new String[strList.size()];
			strList.toArray(ret);
		} else {
			ret = lines;
		}
		return ret;
	}

	@SuppressLint("FloatMath")
	private static LinkedList<String> divideStringWithMaxWidth(
			final String string, final int maxWidth, final Paint paint) {
		final int charLength = string.length();
		int start = 0;
		int tempWidth = 0;
		final LinkedList<String> strList = new LinkedList<String>();
		for (int i = 1; i <= charLength; ++i) {
			tempWidth = (int) FloatMath.ceil(paint.measureText(string, start, i));
			if (tempWidth >= maxWidth) {
				final int lastIndexOfSpace = string.substring(0, i).lastIndexOf(" ");
				if (lastIndexOfSpace != -1 && lastIndexOfSpace > start) {
					strList.add(string.substring(start, lastIndexOfSpace));
					i = lastIndexOfSpace + 1; // skip space
				} else {
					if (tempWidth > maxWidth) {
						strList.add(string.substring(start, i - 1));
						--i;
					} else {
						strList.add(string.substring(start, i));
					}
				}
				while (i < charLength && string.charAt(i) == ' ') {
					++i;
				}
				start = i;
			}
		}
		if (start < charLength) {
			strList.add(string.substring(start));
		}
		return strList;
	}

	private static String refactorString(final String string) {
		if (string.compareTo("") == 0) {
			return " ";
		}
		final StringBuilder strBuilder = new StringBuilder(string);
		int start = 0;
		int index = strBuilder.indexOf("\n");
		while (index != -1) {
			if (index == 0 || strBuilder.charAt(index - 1) == '\n') {
				strBuilder.insert(start, " ");
				start = index + 2;
			} else {
				start = index + 1;
			}

			if (start > strBuilder.length() || index == strBuilder.length()) {
				break;
			}

			index = strBuilder.indexOf("\n", start);
		}
		return strBuilder.toString();
	}

	private static void initNativeObject(final Bitmap bitmap) {
		final byte[] pixels = EngineBitmap.getPixels(bitmap);
		if (pixels == null) {
			return;
		}

		EngineBitmap.nativeInitBitmapDC(bitmap.getWidth(),
				bitmap.getHeight(), pixels);
	}

	private static byte[] getPixels(final Bitmap bitmap) {
		if (bitmap != null) {
			final byte[] pixels = new byte[bitmap.getWidth()
					* bitmap.getHeight() * 4];
			final ByteBuffer buf = ByteBuffer.wrap(pixels);
			buf.order(ByteOrder.nativeOrder());
			bitmap.copyPixelsToBuffer(buf);
			return pixels;
		}

		return null;
	}

	private static int getFontSizeAccordingHeight(int height) {
		Paint paint = new Paint();
		Rect bounds = new Rect();

		paint.setTypeface(Typeface.DEFAULT);
		int incr_text_size = 1;
		boolean found_desired_size = false;

		while (!found_desired_size) {

			paint.setTextSize(incr_text_size);
			String text = "SghMNy";
			paint.getTextBounds(text, 0, text.length(), bounds);

			incr_text_size++;

			if (height - bounds.height() <= 2) {
				found_desired_size = true;
			}
			Log.d("font size", "incr size:" + incr_text_size);
		}
		return incr_text_size;
	}

	private static String getStringWithEllipsis(String string, float width,
			float fontSize) {
		if (TextUtils.isEmpty(string)) {
			return "";
		}

		TextPaint paint = new TextPaint();
		paint.setTypeface(Typeface.DEFAULT);
		paint.setTextSize(fontSize);

		return TextUtils.ellipsize(string, paint, width,
				TextUtils.TruncateAt.END).toString();
	}
	private static class TextProperty {
		/** The max width of lines. */
		private final int mMaxWidth;
		/** The height of all lines. */
		private final int mTotalHeight;
		private final int mHeightPerLine;
		private final String[] mLines;
		
		TextProperty(final int maxWidth, final int heightPerLine,
				final String[] lines) {
			this.mMaxWidth = maxWidth;
			this.mHeightPerLine = heightPerLine;
			this.mTotalHeight = heightPerLine * lines.length;
			this.mLines = lines;
		}
	}
}
