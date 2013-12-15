
package cn.chelper.cxengine;

import java.util.HashMap;

import android.content.Context;
import android.graphics.Typeface;

public class EngineTypefaces {
	private static final HashMap<String, Typeface> sTypefaceCache = new HashMap<String, Typeface>();
	public static synchronized Typeface get(final Context pContext, final String pAssetName) {
		if (!EngineTypefaces.sTypefaceCache.containsKey(pAssetName)) {
			Typeface typeface = null;
			if (pAssetName.startsWith("/")) {
				typeface = Typeface.createFromFile(pAssetName);
			}else {
				typeface = Typeface.createFromAsset(pContext.getAssets(), pAssetName);
			}
			EngineTypefaces.sTypefaceCache.put(pAssetName, typeface);
		}
		return EngineTypefaces.sTypefaceCache.get(pAssetName);
	}
}
