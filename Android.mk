
LOCAL_PATH := $(call my-dir)

NDK_PATH := /Users/xuhua/android/android-ndk

include $(CLEAR_VARS)

LOCAL_MODULE := cxengine

LOCAL_CFLAGS := \
    -std=gnu99 -O2 -DANDROID \
    -I$(LOCAL_PATH) \
    -I$(LOCAL_PATH)/libs/android/include \
    -I$(NDK_PATH)/sources/android

LOCAL_SRC_FILES += \
    lzma/LzFind.c \
    lzma/LzmaDec.c \
    lzma/LzmaEnc.c \
    lzma/LzmaLib.c \
    lzma/lzma.c \
    lzma/cxLzma.c

LOCAL_SRC_FILES += \
	chipmunk/chipmunk.c \
	chipmunk/cpDampedRotarySpring.c \
	chipmunk/cpPinJoint.c \
	chipmunk/cpSimpleMotor.c \
	chipmunk/cpSpaceStep.c \
	chipmunk/cpArbiter.c \
	chipmunk/cpDampedSpring.c \
	chipmunk/cpPivotJoint.c \
	chipmunk/cpSlideJoint.c \
	chipmunk/cpSpatialIndex.c \
	chipmunk/cpArray.c \
	chipmunk/cpGearJoint.c \
	chipmunk/cpPolyShape.c \
	chipmunk/cpSpace.c \
	chipmunk/cpSweep1D.c \
	chipmunk/cpBBTree.c \
	chipmunk/cpGrooveJoint.c \
	chipmunk/cpPolyline.c \
	chipmunk/cpSpaceComponent.c \
	chipmunk/cpBody.c \
	chipmunk/cpHashSet.c \
	chipmunk/cpRatchetJoint.c \
	chipmunk/cpSpaceDebug.c \
	chipmunk/cpCollision.c \
	chipmunk/cpHastySpace.c \
	chipmunk/cpRotaryLimitJoint.c \
	chipmunk/cpSpaceHash.c \
	chipmunk/cpConstraint.c \
	chipmunk/cpMarch.c \
	chipmunk/cpShape.c \
	chipmunk/cpSpaceQuery.c

LOCAL_SRC_FILES += \
    cxcore/cxNumber.c \
	cxcore/cxMessage.c \
	cxcore/cxMD5.c \
	cxcore/cxStr.c \
	cxcore/cxArray.c \
	cxcore/cxCore.c \
	cxcore/cxHash.c \
	cxcore/cxList.c \
	cxcore/cxMemPool.c \
	cxcore/cxStack.c \
	cxcore/cxMemory.c \
    cxcore/cxAnyArray.c \
    cxcore/cxJson.c \
    cxcore/cxMath.c \
    cxcore/cxLoader.c \
    cxcore/cxPath.c \
    cxcore/cxStream.c \
    cxcore/cxRegex.c \
    cxcore/xxtea.c \
    cxcore/utf8.c \
    cxcore/xxhash.c

LOCAL_SRC_FILES += \
    kazmath/aabb2.c \
    kazmath/aabb3.c \
    kazmath/mat3.c \
    kazmath/mat4.c \
    kazmath/GL/mat4stack.c \
    kazmath/GL/matrix.c \
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
    shaders/cxShaderColor.c \
    shaders/cxShaderGray.c

LOCAL_SRC_FILES += \
    streams/cxMemStream.c \
    streams/cxFileStream.c \
    streams/cxStrStream.c

LOCAL_SRC_FILES += \
    textures/cxTextureCache.c \
    textures/cxTexturePKM.c \
    textures/cxTexturePNG.c \
    textures/cxTextureTXT.c \
    textures/cxTextureJSON.c \
    textures/cxTexturePVR.c \
    textures/cxTextureJPG.c \
    textures/cxTextureXTF.c \
    textures/cxTextureMTF.c

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
    actions/cxParabola.c \
    actions/cxFollow.c \
    actions/cxTimeLine.c \
    actions/cxBezier.c \
    actions/cxParticle.c \
    actions/cxSpline.c \
    actions/cxValue.c

LOCAL_SRC_FILES += \
    views/cxAtlas.c \
    views/cxButton.c \
    views/cxClipping.c \
    views/cxLabel.c \
    views/cxSprite.c \
    views/cxTable.c \
    views/cxWindow.c \
    views/cxScroll.c \
    views/cxLoading.c \
    views/cxAlert.c \
    views/cxProgress.c \
    views/cxCircle.c

LOCAL_SRC_FILES += \
    net/cxTCP.c \
    net/http_parser.c \
    net/cxHTTP.c

LOCAL_SRC_FILES += \
    engine/cxAction.c \
	engine/cxBMPFont.c \
	engine/cxEngine.c \
	engine/cxGlobal.c \
	engine/cxOpenGL.c \
	engine/cxShader.c \
	engine/cxTexture.c \
	engine/cxUtil.c \
	engine/cxView.c \
	engine/cxInput.c \
	engine/cxGroup.c \
    engine/cxSpatial.c \
    engine/cxAsync.c \
    engine/cxFrames.c \
    engine/cxLooper.c
    
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
    android/jni/cxPlayer.c \
    android/jni/cxMMapStream.c

LOCAL_SRC_FILES += \
	effect/WaterShader.c

include $(BUILD_STATIC_LIBRARY)

