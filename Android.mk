
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE   := cxEngine

LOCAL_ARM_MODE := arm

LOCAL_CFLAGS   := \
    -std=gnu99 -O2 -DANDROID \
    -I$(LOCAL_PATH) \
    -I$(LOCAL_PATH)/libs/android/include

LOCAL_SRC_FILES := \
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
    textures/cxTextureXML.c \
    textures/cxTexturePVR.c \
    textures/cxTextureJPG.c

LOCAL_SRC_FILES += \
    actions/cxActionSet.c \
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
    actions/cxFollow.c

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
    views/cxAtlasSet.c \
    views/cxPolygon.c

LOCAL_SRC_FILES += \
    socket/cxEventBase.c \
    socket/cxHttp.c \
    socket/cxHttpConn.c \
    socket/cxListen.c \
    socket/cxClient.c

LOCAL_SRC_FILES += \
    core/cxConfig.c \
    core/cxObject.c \
    core/cxMD5.c \
    core/cxAllocator.c \
    core/cxArray.c \
    core/cxAutoPool.c \
    core/cxBase.c \
    core/cxEventArg.c \
    core/cxHash.c \
    core/cxIconv.c \
    core/cxJson.c \
    core/cxBMPFont.c \
    core/cxActionRoot.c \
    core/cxViewRoot.c \
    core/cxList.c \
    core/cxHashRoot.c \
    core/cxNumber.c \
    core/cxUtil.c \
    core/cxRegex.c \
    core/cxMessage.c \
    core/cxStack.c \
    core/cxFreeType.c \
    core/cxString.c \
    core/cxTypes.c \
    core/cxUrlPath.c \
    core/cxXMLScript.c \
    core/cxOpenGL.c \
    core/cxShader.c \
    core/cxStream.c \
    core/cxEngine.c \
    core/cxTexture.c \
    core/cxView.c \
    core/cxAction.c \
    core/cxInitType.c \
    core/cxGlobal.c
    
LOCAL_SRC_FILES += \
    algorithm/cxAStar.c

LOCAL_SRC_FILES += \
    android/jni/cxAndroid.c \
    android/jni/cxAssetsStream.c \
    android/jni/cxPlayer.c

include $(BUILD_STATIC_LIBRARY)

