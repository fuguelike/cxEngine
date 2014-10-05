
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := cxEngine

LOCAL_CFLAGS := \
    -std=gnu99 -O2 -DANDROID \
    -I$(LOCAL_PATH) \
    -I$(LOCAL_PATH)/libs/android/include

LOCAL_SRC_FILES += \
    lzma/LzFind.c \
    lzma/LzmaDec.c \
    lzma/LzmaEnc.c \
    lzma/LzmaLib.c

LOCAL_SRC_FILES += \
    chipmunk/chipmunk.c \
    chipmunk/cpArbiter.c \
    chipmunk/cpArray.c \
    chipmunk/cpBB.c \
    chipmunk/cpBBTree.c \
    chipmunk/cpBody.c \
    chipmunk/cpCollision.c \
    chipmunk/cpHashSet.c \
    chipmunk/cpPolyShape.c \
    chipmunk/cpShape.c \
    chipmunk/cpSpace.c \
    chipmunk/cpSpaceComponent.c \
    chipmunk/cpSpaceHash.c \
    chipmunk/cpSpaceQuery.c \
    chipmunk/cpSpaceStep.c \
    chipmunk/cpSpatialIndex.c \
    chipmunk/cpSweep1D.c \
    chipmunk/cpVect.c \
    chipmunk/constraints/cpConstraint.c \
    chipmunk/constraints/cpDampedRotarySpring.c \
    chipmunk/constraints/cpDampedSpring.c \
    chipmunk/constraints/cpGearJoint.c \
    chipmunk/constraints/cpGrooveJoint.c \
    chipmunk/constraints/cpPinJoint.c \
    chipmunk/constraints/cpPivotJoint.c \
    chipmunk/constraints/cpRatchetJoint.c \
    chipmunk/constraints/cpRotaryLimitJoint.c \
    chipmunk/constraints/cpSimpleMotor.c \
    chipmunk/constraints/cpSlideJoint.c


LOCAL_SRC_FILES += \
	spine/Animation.c \
	spine/AnimationState.c \
	spine/AnimationStateData.c \
	spine/Atlas.c \
	spine/AtlasAttachmentLoader.c \
	spine/Attachment.c \
	spine/AttachmentLoader.c \
	spine/Bone.c \
	spine/BoneData.c \
	spine/BoundingBoxAttachment.c \
	spine/Event.c \
	spine/EventData.c \
	spine/IkConstraint.c \
	spine/IkConstraintData.c \
	spine/Json.c \
	spine/MeshAttachment.c \
	spine/RegionAttachment.c \
	spine/Skeleton.c \
	spine/SkeletonBounds.c \
	spine/SkeletonData.c \
	spine/SkeletonJson.c \
	spine/Skin.c \
	spine/SkinnedMeshAttachment.c \
	spine/Slot.c \
	spine/SlotData.c \
	spine/extension.c

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
    cxcore/cxAnyArray.c \
    cxcore/cxJson.c \
    cxcore/cxMath.c \
    cxcore/cxLoader.c \
    cxcore/cxPath.c \
    cxcore/cxStream.c

LOCAL_SRC_FILES += \
    kazmath/aabb.c \
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
    kazmath/vec4.c \
    kazmath/neon_matrix_impl.c

LOCAL_SRC_FILES += \
    shaders/cxShaderAlpha.c \
    shaders/cxShaderClipping.c \
    shaders/cxShaderDefault.c \
    shaders/cxShaderPositionColor.c \
    shaders/cxShaderMultiple.c

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
    actions/cxBezier.c \
    actions/cxSkeleton.c

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
    views/cxAlert.c \
    views/cxSpine.c \
    views/cxSprites.c

LOCAL_SRC_FILES += \
    socket/cxEventBase.c \
    socket/cxHttp.c \
    socket/cxHttpConn.c \
    socket/cxUDP.c

LOCAL_SRC_FILES += \
    engine/cxAction.c \
	engine/cxBMPFont.c \
	engine/cxEngine.c \
	engine/cxGlobal.c \
	engine/cxIconv.c \
	engine/cxOpenGL.c \
	engine/cxRegex.c \
	engine/cxShader.c \
	engine/cxTexture.c \
	engine/cxUtil.c \
	engine/cxView.c \
	engine/cxInput.c \
	engine/cxGroup.c \
    engine/cxSpatial.c \
    engine/cxValue.c
    
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

