

#define SZ_MD3_LOWER_FILE "model\\sarge\\lower.md3"
#define SZ_MD3_LOWER_SKIN_FILE "model\\sarge\\lower_default.skin"

#define SZ_MD3_UPPER_FILE "model\\sarge\\upper.md3"
#define SZ_MD3_UPPER_SKIN_FILE "model\\sarge\\upper_default.skin"

#define SZ_MD3_HEAD_FILE "model\\sarge\\head.md3"
#define SZ_MD3_HEAD_SKIN_FILE "model\\sarge\\head_default.skin"

#define SZ_MD3_TEXTURE_PATH "model\\sarge\\" 
#define SZ_MD3_ANIM_FILE "model\\sarge\\animation.cfg"

#define MAX_FILENAME_LENGTH 256
#define MAX_TEXTURES 20

//---------------------------------------------------------------------------

#include <windows.h>
#include <stdio.h>						//sprintf(...)
#include <string.h>



//---------------------------------------------------------------------------


typedef unsigned int		uint32;
typedef int					int32;
typedef unsigned short int	uint16;
typedef short int			int16;
typedef float				float32;



struct stMD3Header
{
	char		ID[4];			//	ID of the file is always "IDP3"
	int32		Version;		//	Version number, usually 15
	char		Filename[68];	//  Filename, sometimes left blank
	int32		numBoneFrames;	//  Number of BoneFrames
	int32		numTags;		//  Number of 'tags' per BoneFrame
	int32		numMeshes;		//  Number of Meshes/Skins in MaxSkin
	int32		numMaxSkins;	//  Maximum number of unique skins
	int32		ofsFrames;		//  Always equal to the length this header
	int32		ofsTagStart;	//  Starting position of tag structures
	int32		ofMeshSurfaces;	//  Ending position of tag structure
	int32		ofEndOfFile;	//  Size of file
};



struct stBoneFrame
{
	float32 mins[3];
	float32 maxs[3];
	float32 Position[3];
	float32 Scale;
	char	Creator[16];
};

struct stAnim
{
	int32 FirstFrame;
	int32 numFrames;
	int32 LoopingFrames;
	int32 FPS;
};

struct stSkin
{
	char Name[64];
	int32 index;
};

struct stTag
{
	char			Name[64];
	float32			Position[3];
	float32			Rotation[3][3];
};

struct stTriangle
{
	int32 Vertex[3];
};

struct stTexCoord
{
	float32 Coord[2];
};

struct stVertex // = Record
{
	int16			Vertex[3];
	unsigned char	Normal[2];
};

struct stMeshHeader
{
	char	ID[4];
	char	Name[64];
	int32	flags;
	int32	numMeshFrames;
	int32	numSkins;
	int32	numVertexes;
	int32	numTriangles;
	int32	ofsTriangles;
	int32	ofsSkins;
	int32	ofsTexVector;
	int32	ofsVertex;
	int32	ofsEndMeshSize;
};

struct stMesh
{
	stMeshHeader	MeshHeader;
	stSkin* pSkins;
	stTriangle* pTriangle;
	stTexCoord* pTexCoord;
	stVertex* pVertex;
	int				texID;
};


struct stM3DModel
{
	char		m_md3FileName[MAX_FILENAME_LENGTH];
	stMD3Header m_md3Header;

	stBoneFrame* m_pBoneFrame;
	stTag* m_pTags;
	stMesh* m_pMeshes;

	int	m_FPS;
	int m_startFrame;
	int m_endFrame;
	int m_nextFrame;
	int m_anim;
	float m_poll;
	//int m_animLower;
	//int m_animUpper;
	stM3DModel* m_pLinks[10];

	int m_currentframe;
};

//IDirect3DTexture8* pTextures[MAX_TEXTURES];
int g_NumLoadedTextures = 0;

//---------------------------------------------------------------------------

long filesize(FILE* stream)
{
	long curpos, length;

	curpos = ftell(stream);
	fseek(stream, 0L, SEEK_END);
	length = ftell(stream);
	fseek(stream, curpos, SEEK_SET);
	return length;
}

//---------------------------------------------------------------------------

enum
{
	BOTH_DEATH1 = 0,
	BOTH_DEAD1 = 1,
	BOTH_DEATH2 = 2,
	BOTH_DEAD2 = 3,
	BOTH_DEATH3 = 4,
	BOTH_DEAD3 = 5,

	TORSO_GESTURE = 6,
	TORSO_ATTACK = 7,
	TORSO_ATTACK2 = 8,
	TORSO_DROP = 9,
	TORSO_RAISE = 10,
	TORSO_STAND = 11,
	TORSO_STAND2 = 12,

	LEGS_WALKCR = 13,
	LEGS_WALK = 14,
	LEGS_RUN = 15,
	LEGS_BACK = 16,
	LEGS_SWIM = 17,
	LEGS_JUMP = 18,
	LEGS_LAND = 19,
	LEGS_JUMPB = 20,
	LEGS_LANDB = 21,
	LEGS_IDLE = 22,
	LEGS_IDLECR = 23,
	LEGS_TURN = 24,

	MAX_ANIMATIONS
};
