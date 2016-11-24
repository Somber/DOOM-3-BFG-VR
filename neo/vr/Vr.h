/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.

This file is part of the Doom 3 BFG Edition GPL Source Code ("Doom 3 BFG Edition Source Code").

Doom 3 BFG Edition Source Code is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Doom 3 BFG Edition Source Code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Doom 3 BFG Edition Source Code.  If not, see <http://www.gnu.org/licenses/>.

In addition, the Doom 3 BFG Edition Source Code is also subject to certain additional terms. You should have received a copy of these additional terms immediately following the terms and conditions of the GNU General Public License which accompanied the Doom 3 BFG Edition Source Code.  If not, please request a copy in writing from id Software at the address below.

If you have questions concerning this license or the applicable additional terms, you may contact in writing id Software LLC, c/o ZeniMax Media Inc., Suite 120, Rockville, Maryland 20850 USA.

===========================================================================
*/

#include "precompiled.h"
#pragma hdrstop

#include "vr_hmd.h"
#include "vr_sixense.h"
#include "..\renderer\Framebuffer.h"
#include "..\libs\OpenVR\headers\openvr.h"


#ifndef __VR_H__
#define __VR_H__

// hand == 0 == right, 1 == left 
#define HAND_RIGHT 0
#define HAND_LEFT 1

typedef enum
{
	MOTION_NONE,
	MOTION_HYDRA,
	MOTION_STEAMVR
} vr_motionControl_t;

typedef enum 
{
	VR_AA_NONE,
	VR_AA_MSAA,
	VR_AA_FXAA,
	NUM_VR_AA
} vr_aa_t;

typedef enum
{
	VR_HUD_NONE,
	VR_HUD_FULL,
	VR_HUD_LOOK_DOWN,
} vr_hud_t;

typedef enum
{
	RENDERING_NORMAL,
	RENDERING_PDA,
	RENDERING_HUD
} vr_swf_render_t;

typedef enum
{
	FLASH_BODY,
	FLASH_HEAD,
	FLASH_GUN,
	FLASH_HAND
} vr_flashlight_mode_t;

class idClipModel;

class iVr
{
public:
	
	iVr();
	
	void				HMDInit( void );
	void				HMDShutdown( void );
	void				HMDInitializeDistortion( void );
	void				HMDGetOrientation( idAngles &hmdAngles, idVec3 &headPositionDelta, idVec3 &bodyPositionDelta, idVec3 &absolutePosition, bool resetTrackingOffset );
	void				HMDGetOrientationAbsolute( idAngles &hmdAngles, idVec3 &positoin );
	void				HMDRender( idImage *leftCurrent, idImage *rightCurrent );
	void				HMDTrackStatic();
	void				HUDRender( idImage *image0, idImage *image1 );
	void				HMDResetTrackingOriginOffset();
	
	void				FrameStart( void );

	void				HydraInit( void );
	void				HydraSetRotationOffset( void );
	void				HydraSetOffset( void );
	void				HydraSetLeftOffset( hydraData hydraOffset );
	void				HydraSetRightOffset( hydraData hydraOffset );
	void				HydraGetLeftOffset( hydraData &hydraOffset );
	void				HydraGetRightOffset( hydraData &hydraOffset );
	void				HydraGetLeft( hydraData &leftHydra );
	void				HydraGetRight( hydraData &rightHydra );
	void				HydraGetLeftWithOffset( hydraData &leftOffsetHydra );
	void				HydraGetRightWithOffset( hydraData &rightOffsetHydra );

	void				OpenVrGetRight( idVec3 &position, idQuat &rotation );
	void				OpenVrGetLeft( idVec3 &position, idQuat &rotation );

	
	void				MotionControlSetRotationOffset();
	void				MotionControlSetOffset();
	void				MotionControlGetHand( int hand, idVec3 &position, idQuat &rotation );
	void				MotionControlGetLeftHand( idVec3 &position, idQuat &rotation );
	void				MotionControlGetRightHand( idVec3 &position, idQuat &rotation );
	void				MotionControlGetOpenVrController( vr::TrackedDeviceIndex_t deviceNum, idVec3 &position, idQuat &rotation );
	void				MotionControllerSetHaptic( int hand, unsigned short value );
	
	void				MSAAResolve( void );
	void				FXAAResolve( idImage * leftCurrent, idImage * rightCurrent );
	void				FXAASetUniforms( Framebuffer FBO );

	void				CalcAimMove( float &yawDelta, float &pitchDelta );
	

	//------------------


	bool				VR_GAME_PAUSED;
	
	bool				PDAforcetoggle;
	bool				PDAforced;
	bool				PDArising;
	bool				gameSaving;

	int					swfRenderMode;
	bool				PDAclipModelSet;
	int					pdaToggleTime;
	bool				forceLeftStick;
	
	int					currentFlashlightPosition;

	bool				handInGui;


	bool				vrIsBackgroundSaving;

	int					vrFrameNumber;
			
	idVec3				lastViewOrigin;
	idMat3				lastViewAxis;
	
	idVec3				lastCenterEyeOrigin;
	idMat3				lastCenterEyeAxis;

	float				handRoll[2];


	float				bodyYawOffset;
	float				lastHMDYaw;
	float				lastHMDPitch;
	float				lastHMDRoll;
	idVec3				lastHMDViewOrigin;
	idMat3				lastHMDViewAxis;
	idVec3				lastAbsolutePosition;
		
	bool				isWalking;
	
	float				angles[3];
	
	uint32_t			hmdWidth;
	uint32_t			hmdHeight;
	int					hmdHz;
	
	bool				useFBO;
	int					primaryFBOWidth;
	int					primaryFBOHeight;
	
	int					VR_AAmode;
	
	int					VR_USE_MOTION_CONTROLS;
	int					weaponHand;
		
	bool				hasHMD;
	bool				hasOculusRift;

	bool				m_bDebugOpenGL;
	bool				m_bVerbose;
	bool				m_bPerf;
	bool				m_bVblank;
	bool				m_bGlFinishHack;

	vr::IVRSystem			*m_pHMD;
	vr::IVRCompositor		*m_pCompositor;
	vr::IVRRenderModels		*m_pRenderModels;
	vr::TrackedDevicePose_t	m_rTrackedDevicePose[vr::k_unMaxTrackedDeviceCount];
	vr::TrackedDevicePose_t	m1_rTrackedDevicePose[vr::k_unMaxTrackedDeviceCount];
	
	idStr				m_strDriver;
	idStr				m_strDisplay;
	
	char				m_rDevClassChar[vr::k_unMaxTrackedDeviceCount];
	idMat4				m_rmat4DevicePose[vr::k_unMaxTrackedDeviceCount];
	bool				m_rbShowTrackedDevice[vr::k_unMaxTrackedDeviceCount];

	idMat4				m_mat4ProjectionLeft;
	idMat4				m_mat4ProjectionRight;
	idMat4				m_mat4eyePosLeft;
	idMat4				m_mat4eyePosRight;

	float				hmdFovX;
	float				hmdFovY;
	float				hmdAspect;
	hmdEye_t			hmdEye[2];
	
	
	float				openVrIPD;
	float				openVrHeight;
	
	float				manualIPD;
	float				manualHeight;

	idImage*			hmdEyeImage[2];
	idImage*			hmdCurrentRender[2];
	
	int					mirrorW;
	int					mirrorH;

	idImage*			primaryFBOimage;
	idImage*			resolveFBOimage;
	idImage*			fullscreenFBOimage;
						
	double				hmdFrameTime;
	bool				hmdPositionTracked;
	int					currentEye;



	idVec3				trackingOriginOffset;
	float				trackingOriginYawOffset;
	bool				chestDefaultDefined;
	idVec3				hmdBodyTranslation;
	
	float				independentWeaponYaw;
	float				independentWeaponPitch;
	
	float				playerDead;

	bool				isLoading;

	int					lastRead;
	int					currentRead;
	bool				updateScreen;

	idAngles			bodyMoveAng;


	vr_motionControl_t	motionControlType;
	
	// wip stuff
	int					wipNumSteps;
	int					wipStepState;
	int					wipLastPeriod;
	float				wipCurrentDelta;
	float				wipCurrentVelocity;
	float				wipPeriodVel;
	float				wipTotalDelta;
	float				wipLastAcces;
	float				wipAvgPeriod;
	float				wipTotalDeltaAvg;

	// clip stuff
	idClipModel*		bodyClip;
	idClipModel*		headClip;
			
	//---------------------------
private:
	
	void				HydraGetData( int hydraIndex, hydraData &hydraData );
	
	int					hydraLeftIndex ;						
	int					hydraRightIndex ;
	
	hydraData			hydraLeftOffset;
	hydraData			hydraRightOffset;

	
};

#endif

//koz g_gun cvars allow tweaking of gun position when aiming with hydra
extern idCVar	vr_hydraOffsetForward;
extern idCVar	vr_hydraOffsetHorizontal;
extern idCVar	vr_hydraOffsetVertical;
extern idCVar	vr_hydraPitchOffset;

extern idCVar	vr_vignette;
extern idCVar	vr_scale;
extern idCVar	vr_useOculusProfile;
extern idCVar	vr_manualIPDEnable;
extern idCVar	vr_manualIPD;
extern idCVar	vr_manualHeight;

extern idCVar	vr_showBody;
extern idCVar	vr_viewModelArms;
extern idCVar	vr_wristStatMon;
extern idCVar	vr_disableWeaponAnimation;
extern idCVar	vr_headKick;

extern idCVar	vr_hydraEnable;
extern idCVar	vr_hydraForceDetect;

extern idCVar	vr_armIKenable;
extern idCVar	vr_weaponHand;

extern idCVar	vr_flashlightMode;

extern idCVar	vr_flashlightBodyPosX;
extern idCVar	vr_flashlightBodyPosY;
extern idCVar	vr_flashlightBodyPosZ;

extern idCVar	vr_flashlightHelmetPosX;
extern idCVar	vr_flashlightHelmetPosY;
extern idCVar	vr_flashlightHelmetPosZ;

extern idCVar	vr_forward_keyhole;

extern idCVar	vr_PDAscale;
extern idCVar	vr_PDAfixLocation;

extern idCVar	vr_weaponPivotOffsetForward;
extern idCVar	vr_weaponPivotOffsetHorizontal;
extern idCVar	vr_weaponPivotOffsetVertical;
extern idCVar	vr_weaponPivotForearmLength;


extern idCVar	vr_guiScale; 
extern idCVar	vr_guiSeparation;

extern idCVar	vr_guiMode;

extern idCVar	vr_hudScale;
extern idCVar	vr_hudPosHor; 
extern idCVar	vr_hudPosVer; 
extern idCVar	vr_hudPosDis;
extern idCVar	vr_hudPosLock;
extern idCVar	vr_hudType;
extern idCVar	vr_hudPosAngle;
extern idCVar	vr_hudRevealAngle;
extern idCVar	vr_hudTransparency;
extern idCVar	vr_hudOcclusion;

extern idCVar	vr_hudHealth;
extern idCVar	vr_hudAmmo;
extern idCVar	vr_hudPickUps;
extern idCVar	vr_hudTips;
extern idCVar	vr_hudLocation;
extern idCVar	vr_hudObjective;
extern idCVar	vr_hudStamina;
extern idCVar	vr_hudPills;
extern idCVar	vr_hudComs;
extern idCVar	vr_hudWeap;
extern idCVar	vr_hudNewItems;
extern idCVar	vr_hudFlashlight;
extern idCVar	vr_hudLowHealth;

extern idCVar	vr_tweakTalkCursor; 

extern idCVar	vr_listMonitorName;

extern idCVar	vr_enable;
extern idCVar	vr_FBOscale;
extern idCVar	vr_hydraEnable;
extern idCVar	vr_joystickMenuMapping;

extern idCVar	vr_trackingPredictionUserDefined;

extern idCVar	vr_clipPositional;

extern idCVar	vr_minLoadScreenTime;

extern idCVar	vr_deadzonePitch;
extern idCVar	vr_deadzoneYaw;
extern idCVar	vr_comfortDelta;

//extern idCVar	vr_interactiveCinematic;

extern idCVar	vr_headingBeamLength;
extern idCVar	vr_headingBeamWidth;
extern idCVar	vr_headingBeamMode;

extern idCVar	vr_weaponSight;
extern idCVar	vr_weaponSightToSurface;

extern idCVar   vr_motionFlashPitchAdj;
extern idCVar	vr_motionWeaponPitchAdj;

extern idCVar	vr_3dgui;
extern idCVar	vr_shakeAmplitude;
extern idCVar	vr_controllerStandard;

extern idCVar	vr_offHandPosX;
extern idCVar	vr_offHandPosY;
extern idCVar	vr_offHandPosZ;

extern idCVar	vr_padDeadzone;
extern idCVar	vr_padToButtonThreshold;
extern idCVar	vr_knockBack;

extern idCVar	vr_mountedWeaponController;
extern idCVar	vr_walkSpeedAdjust;


extern idCVar	vr_movePoint;

extern iVr* commonVr;


