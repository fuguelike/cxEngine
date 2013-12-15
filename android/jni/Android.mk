
LOCAL_PATH := $(call my-dir)

LIBS_DIR    := ../../libs/android/lib
CX_ENGINE_DIR	:= ../..

include $(CLEAR_VARS)
LOCAL_MODULE := libxml2
LOCAL_SRC_FILES := $(LIBS_DIR)/libxml2.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := db
LOCAL_SRC_FILES := $(LIBS_DIR)/libdb.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := iconv
LOCAL_SRC_FILES := $(LIBS_DIR)/libiconv.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := charset
LOCAL_SRC_FILES := $(LIBS_DIR)/libcharset.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := pcre
LOCAL_SRC_FILES := $(LIBS_DIR)/libpcre.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := event
LOCAL_SRC_FILES := $(LIBS_DIR)/libevent.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := jpeg
LOCAL_SRC_FILES := $(LIBS_DIR)/libjpeg.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := mpg123
LOCAL_SRC_FILES := $(LIBS_DIR)/libmpg123.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := png
LOCAL_SRC_FILES := $(LIBS_DIR)/libpng.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := freetype
LOCAL_SRC_FILES := $(LIBS_DIR)/libfreetype.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := jansson
LOCAL_SRC_FILES := $(LIBS_DIR)/libjansson.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)

LOCAL_MODULE    := cxEngine

LOCAL_ARM_MODE := arm

LOCAL_CFLAGS    := -std=gnu99 -O2 -DANDROID -I$(CX_ENGINE_DIR) -I$(CX_ENGINE_DIR)/libs/android/include

LOCAL_SRC_FILES := \
$(CX_ENGINE_DIR)/kazmath/aabb.c \
$(CX_ENGINE_DIR)/kazmath/mat3.c \
$(CX_ENGINE_DIR)/kazmath/mat4.c \
$(CX_ENGINE_DIR)/kazmath/mat4stack.c \
$(CX_ENGINE_DIR)/kazmath/matrix.c \
$(CX_ENGINE_DIR)/kazmath/plane.c \
$(CX_ENGINE_DIR)/kazmath/quaternion.c \
$(CX_ENGINE_DIR)/kazmath/ray2.c \
$(CX_ENGINE_DIR)/kazmath/ray3.c \
$(CX_ENGINE_DIR)/kazmath/utility.c \
$(CX_ENGINE_DIR)/kazmath/vec2.c \
$(CX_ENGINE_DIR)/kazmath/vec3.c \
$(CX_ENGINE_DIR)/kazmath/vec4.c

LOCAL_SRC_FILES += \
$(CX_ENGINE_DIR)/chipmunk/chipmunk.c \
$(CX_ENGINE_DIR)/chipmunk/cpArbiter.c \
$(CX_ENGINE_DIR)/chipmunk/cpArray.c \
$(CX_ENGINE_DIR)/chipmunk/cpBB.c \
$(CX_ENGINE_DIR)/chipmunk/cpBBTree.c \
$(CX_ENGINE_DIR)/chipmunk/cpBody.c \
$(CX_ENGINE_DIR)/chipmunk/cpCollision.c \
$(CX_ENGINE_DIR)/chipmunk/cpHashSet.c \
$(CX_ENGINE_DIR)/chipmunk/cpPolyShape.c \
$(CX_ENGINE_DIR)/chipmunk/cpShape.c \
$(CX_ENGINE_DIR)/chipmunk/cpSpace.c \
$(CX_ENGINE_DIR)/chipmunk/cpSpaceComponent.c \
$(CX_ENGINE_DIR)/chipmunk/cpSpaceHash.c \
$(CX_ENGINE_DIR)/chipmunk/cpSpaceQuery.c \
$(CX_ENGINE_DIR)/chipmunk/cpSpaceStep.c \
$(CX_ENGINE_DIR)/chipmunk/cpSpatialIndex.c \
$(CX_ENGINE_DIR)/chipmunk/cpSweep1D.c \
$(CX_ENGINE_DIR)/chipmunk/cpVect.c \
$(CX_ENGINE_DIR)/chipmunk/constraints/cpConstraint.c \
$(CX_ENGINE_DIR)/chipmunk/constraints/cpDampedRotarySpring.c \
$(CX_ENGINE_DIR)/chipmunk/constraints/cpDampedSpring.c \
$(CX_ENGINE_DIR)/chipmunk/constraints/cpGearJoint.c \
$(CX_ENGINE_DIR)/chipmunk/constraints/cpGrooveJoint.c \
$(CX_ENGINE_DIR)/chipmunk/constraints/cpPinJoint.c \
$(CX_ENGINE_DIR)/chipmunk/constraints/cpPivotJoint.c \
$(CX_ENGINE_DIR)/chipmunk/constraints/cpRatchetJoint.c \
$(CX_ENGINE_DIR)/chipmunk/constraints/cpRotaryLimitJoint.c \
$(CX_ENGINE_DIR)/chipmunk/constraints/cpSimpleMotor.c \
$(CX_ENGINE_DIR)/chipmunk/constraints/cpSlideJoint.c

LOCAL_SRC_FILES += \
$(CX_ENGINE_DIR)/shaders/cxShaderAlpha.c \
$(CX_ENGINE_DIR)/shaders/cxShaderClipping.c \
$(CX_ENGINE_DIR)/shaders/cxShaderDefault.c \
$(CX_ENGINE_DIR)/shaders/cxShaderPositionColor.c

LOCAL_SRC_FILES += \
cxAssetsStream.c \
$(CX_ENGINE_DIR)/streams/cxMemStream.c

LOCAL_SRC_FILES += \
$(CX_ENGINE_DIR)/textures/cxTextureFactory.c \
$(CX_ENGINE_DIR)/textures/cxTexturePKM.c \
$(CX_ENGINE_DIR)/textures/cxTexturePNG.c \
$(CX_ENGINE_DIR)/textures/cxTextureTXT.c \
$(CX_ENGINE_DIR)/textures/cxTextureXML.c \
$(CX_ENGINE_DIR)/textures/cxTexturePVR.c \
$(CX_ENGINE_DIR)/textures/cxTextureJPG.c

LOCAL_SRC_FILES += \
$(CX_ENGINE_DIR)/actions/cxActionSet.c \
$(CX_ENGINE_DIR)/actions/cxCurve.c \
$(CX_ENGINE_DIR)/actions/cxFade.c \
$(CX_ENGINE_DIR)/actions/cxJump.c \
$(CX_ENGINE_DIR)/actions/cxMove.c \
$(CX_ENGINE_DIR)/actions/cxRotate.c \
$(CX_ENGINE_DIR)/actions/cxScale.c \
$(CX_ENGINE_DIR)/actions/cxTimer.c \
$(CX_ENGINE_DIR)/actions/cxTint.c \
$(CX_ENGINE_DIR)/actions/cxRunner.c \
$(CX_ENGINE_DIR)/actions/cxAnimate.c \
$(CX_ENGINE_DIR)/actions/cxSpline.c

LOCAL_SRC_FILES += \
$(CX_ENGINE_DIR)/views/cxAtlas.c \
$(CX_ENGINE_DIR)/views/cxButton.c \
$(CX_ENGINE_DIR)/views/cxClipping.c \
$(CX_ENGINE_DIR)/views/cxLabelTTF.c \
$(CX_ENGINE_DIR)/views/cxParticle.c \
$(CX_ENGINE_DIR)/views/cxSprite.c \
$(CX_ENGINE_DIR)/views/cxTable.c \
$(CX_ENGINE_DIR)/views/cxWindow.c \
$(CX_ENGINE_DIR)/views/cxScroll.c \
$(CX_ENGINE_DIR)/views/cxLoading.c \
$(CX_ENGINE_DIR)/views/cxChipmunk.c \
$(CX_ENGINE_DIR)/views/cxLabelBMP.c

LOCAL_SRC_FILES += \
$(CX_ENGINE_DIR)/socket/cxEventBase.c \
$(CX_ENGINE_DIR)/socket/cxHttp.c \
$(CX_ENGINE_DIR)/socket/cxHttpConn.c \
$(CX_ENGINE_DIR)/socket/cxListen.c \
$(CX_ENGINE_DIR)/socket/cxClient.c

LOCAL_SRC_FILES += \
$(CX_ENGINE_DIR)/core/cxConfig.c    \
$(CX_ENGINE_DIR)/core/cxMD5.c    \
$(CX_ENGINE_DIR)/core/cxAllocator.c \
$(CX_ENGINE_DIR)/core/cxArray.c  \
$(CX_ENGINE_DIR)/core/cxAutoPool.c  \
$(CX_ENGINE_DIR)/core/cxBase.c  \
$(CX_ENGINE_DIR)/core/cxEventArg.c  \
$(CX_ENGINE_DIR)/core/cxEventItem.c  \
$(CX_ENGINE_DIR)/core/cxHash.c  \
$(CX_ENGINE_DIR)/core/cxIconv.c  \
$(CX_ENGINE_DIR)/core/cxJson.c  \
$(CX_ENGINE_DIR)/core/cxBMPFont.c  \
$(CX_ENGINE_DIR)/core/cxActionRoot.c  \
$(CX_ENGINE_DIR)/core/cxViewRoot.c  \
$(CX_ENGINE_DIR)/core/cxList.c  \
$(CX_ENGINE_DIR)/core/cxDB.c  \
$(CX_ENGINE_DIR)/core/cxHashRoot.c  \
$(CX_ENGINE_DIR)/core/cxNumber.c  \
$(CX_ENGINE_DIR)/core/cxUtil.c  \
$(CX_ENGINE_DIR)/core/cxRegex.c  \
$(CX_ENGINE_DIR)/core/cxMessage.c  \
$(CX_ENGINE_DIR)/core/cxStack.c  \
$(CX_ENGINE_DIR)/core/cxFreeType.c  \
$(CX_ENGINE_DIR)/core/cxString.c  \
$(CX_ENGINE_DIR)/core/cxTypes.c  \
$(CX_ENGINE_DIR)/core/cxUrlPath.c  \
$(CX_ENGINE_DIR)/core/cxXMLScript.c \
$(CX_ENGINE_DIR)/core/cxOpenGL.c \
$(CX_ENGINE_DIR)/core/cxShader.c \
$(CX_ENGINE_DIR)/core/cxStream.c \
$(CX_ENGINE_DIR)/core/cxEngine.c \
$(CX_ENGINE_DIR)/core/cxTexture.c \
$(CX_ENGINE_DIR)/core/cxView.c \
$(CX_ENGINE_DIR)/core/cxAction.c

LOCAL_SRC_FILES += cxAndroid.c cxMain.c

LOCAL_STATIC_LIBRARIES := db pcre event jpeg mpg123 png freetype charset iconv libxml2 jansson

LOCAL_LDLIBS := -llog -lGLESv2 -lz -lEGL -landroid -lOpenSLES

include $(BUILD_SHARED_LIBRARY)

