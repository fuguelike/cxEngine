
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := cxEngine

LOCAL_CFLAGS := \
    -std=gnu99 -O2 -DANDROID \
    -I$(LOCAL_PATH) \
    -I$(LOCAL_PATH)/libs/android/include

LOCAL_SRC_FILES += \
    cxcore/cxNumber.c \
	cxcore/cxMessage.c \
	cxcore/cxMD5.c \
	cxcore/cxString.c \
	cxcore/cxArray.c \
	cxcore/cxCore.c \
	cxcore/cxHash.c \
	cxcore/cxList.c \
	cxcore/cxMemPool.c \
	cxcore/cxProperty.c \
	cxcore/cxStack.c \
	cxcore/cxType.c \
	cxcore/cxMemory.c \
    cxcore/cxAnyArray.c

LOCAL_SRC_FILES += \
    kazmath/aabb.c \
    kazmath/mat3.c \
    kazmath/mat4.c \
    kazmath/mat4stack.c \
    kazmath/matrix.c \
    kazmath/plane.c \
    kazmath/quaternion.c \
    kazmath/ray2.c \
    kazmath/ray3.c \
    kazmath/utility.c \
    kazmath/vec2.c \
    kazmath/vec3.c \
    kazmath/vec4.c

LOCAL_SRC_FILES += \
    shaders/cxShaderAlpha.c \
    shaders/cxShaderClipping.c \
    shaders/cxShaderDefault.c \
    shaders/cxShaderPositionColor.c

LOCAL_SRC_FILES += \
    streams/cxMemStream.c \
    streams/cxFileStream.c

LOCAL_SRC_FILES += \
    textures/cxTextureFactory.c \
    textures/cxTexturePKM.c \
    textures/cxTexturePNG.c \
    textures/cxTextureTXT.c \
    textures/cxTextureJSON.c \
    textures/cxTexturePVR.c \
    textures/cxTextureJPG.c

LOCAL_SRC_FILES += \
    actions/cxMultiple.c \
    actions/cxCurve.c \
    actions/cxFade.c \
    actions/cxJump.c \
    actions/cxMove.c \
    actions/cxRotate.c \
    actions/cxScale.c \
    actions/cxTimer.c \
    actions/cxTint.c \
    actions/cxRunner.c \
    actions/cxAnimate.c \
    actions/cxSpline.c \
    actions/cxParticle.c \
    actions/cxParabola.c \
    actions/cxFollow.c \
    actions/cxTimeLine.c \
    actions/cxBezier.c

LOCAL_SRC_FILES += \
    views/cxAtlas.c \
    views/cxButton.c \
    views/cxClipping.c \
    views/cxLabelTTF.c \
    views/cxSprite.c \
    views/cxTable.c \
    views/cxWindow.c \
    views/cxScroll.c \
    views/cxLoading.c \
    views/cxLabelBMP.c \
    views/cxPolygon.c \
    views/cxAlert.c

LOCAL_SRC_FILES += \
    socket/cxEventBase.c \
    socket/cxHttp.c \
    socket/cxHttpConn.c \
    socket/cxListen.c \
    socket/cxClient.c

LOCAL_SRC_FILES += \
    engine/cxAction.c \
	engine/cxBMPFont.c \
	engine/cxViewLoader.c \
	engine/cxEngine.c \
	engine/cxGlobal.c \
	engine/cxIconv.c \
	engine/cxJson.c \
	engine/cxMath.c \
	engine/cxOpenGL.c \
	engine/cxRegex.c \
	engine/cxShader.c \
	engine/cxStream.c \
	engine/cxTexture.c \
	engine/cxUrlPath.c \
	engine/cxUtil.c \
	engine/cxView.c
    
LOCAL_SRC_FILES += \
    algorithm/cxTile.c \
    algorithm/cxAStar.c \
    algorithm/aes_cbc.c \
 	algorithm/aes_cfb.c \
 	algorithm/aes_core.c \
 	algorithm/aes_ctr.c \
 	algorithm/aes_ecb.c \
 	algorithm/aes_ige.c \
 	algorithm/aes_ofb.c

LOCAL_SRC_FILES += \
    android/jni/cxAndroid.c \
    android/jni/cxAssetsStream.c \
    android/jni/cxPlayer.c
    
LOCAL_SRC_FILES += \
    jansson/dump.c \
    jansson/error.c \
    jansson/hashtable.c \
    jansson/hashtable_seed.c \
    jansson/load.c \
    jansson/memory.c \
    jansson/pack_unpack.c \
    jansson/strbuffer.c \
    jansson/strconv.c \
    jansson/utf.c \
    jansson/value.c

include $(BUILD_STATIC_LIBRARY)

