#pragma once

struct RwTexture;
struct RwImage;
struct RwRaster;

struct RwGlobals;
struct RsGlobalType;

class rwFunc
{
public:
	static RwGlobals **m_pRwEngineInstance;
	static RsGlobalType *m_RsGlobal;

	static RwImage *(__cdecl *RtPNGImageRead)(const char *filename);
	static int(__cdecl *RwImageDestroy)(RwImage *);
	static RwImage *(__cdecl *RwImageFindRasterFormat)(RwImage *, int, int *, int *, int *, int *);
	static RwRaster *(__cdecl *RwRasterCreate)(int, int, int, int);
	static RwRaster *(__cdecl *RwRasterSetFromImage)(RwRaster *, RwImage *);
	static RwTexture *(__cdecl *RwTextureCreate)(RwRaster *);

	static void Init10U();
	static void InitSteam();
};
