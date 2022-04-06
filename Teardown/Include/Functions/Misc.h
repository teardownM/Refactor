#pragma once

#include "small_vector.h"
#include "Entities.h"
#include "Scene.h"
#include "Util/Math.h"

struct SRaycastFilter
{
	int m_Mask;
	int m_Layers;
	char field_8[4];
	bool m_IgnoreTransparent;
	small_vector<class CBody*> m_IgnoredBodies;
	char field_1D[32];
	small_vector<class CEntity*> m_IgnoredShapes;
};

namespace Teardown {
	typedef bool (*tQueryRaycast) (CScene* pScene, Vector3* vOrigin, Vector3* vDirection, float fMaxDist, void* pFilter, float* fDistOut, Vector3* vHitPos, CShape** pHitShape, void* pUnk);
	inline tQueryRaycast QueryRaycast;

	typedef void (*tDebugPrint) (void* m_Debug, small_string* ssMessage);
	inline tDebugPrint DebugPrint;

	typedef void (*tDrawLine) (void* pRenderer, Vector3* vPoint0, Vector3* vPoint1, Vector4* Colour, Vector4* Colour2, bool bDepthCheck);
	inline tDrawLine DrawLine;

	typedef void (*tSpawnParticle) (void* pParticleSystem, SParticleInfo* pInfo, Vector3* vPos, Vector3* vVelocity, float fLifeTime);
	inline tSpawnParticle SpawnParticle;

	typedef SSoundInfo* (*tLoadSound) (void* pSound, small_string* ssFilePath, small_string* ssType);
	inline tLoadSound LoadSound;

	typedef void (*tPlaySound) (SSoundInfo* pSoundInfo, Vector3* vPosition, float fVolume, float fSpeed);
	inline tPlaySound PlaySound;

	typedef SLoopInfo* (*tLoadLoop) (void* pSound, small_string* ssFilePath, small_string* ssType);
	inline tLoadLoop LoadLoop;

	typedef void (*tPlayLoop) (SLoopInfo* pSoundInfo, Vector3* vPosition, float fVolume, bool bSomething);
	inline tPlayLoop PlayLoop;

	typedef void (*tSpotLight) (void* pRenderer, Vector3* StartPos, Vector3* vColor, float fRayDispertion, float a5, float fRadius, float fFog);
	inline tSpotLight SpotLight;

	typedef void (*tCreateExplosion) (CScene* pScene, Vector3* vPosition, float fIntensity);
	inline tCreateExplosion CreateExplosion;

	typedef void (*tMakeHole) (void* pUnknown, void* pUnknown2, Vector3* vPosition, float fSoftRadius, float fMidRadius, float fHardRadius, bool bSilent, void* pUnknown3);
	inline tMakeHole MakeHole;
}