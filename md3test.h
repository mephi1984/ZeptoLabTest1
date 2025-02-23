#pragma once


#define SZ_MD3_FILE "model/sarge/upper.md3"

#define MAX_FILENAME_LENGTH 256


//---------------------------------------------------------------------------

#include <windows.h>
#include <stdio.h>                      //sprintf(...)


//---------------------------------------------------------------------------


typedef unsigned int        uint32;
typedef int                 int32;
typedef unsigned short int  uint16;
typedef short int           int16;
typedef float               float32;



struct stMD3Header
{
    char        ID[4];          //  ID of the file is always "IDP3"
    int32       Version;        //  Version number, usually 15
    char        Filename[68];   //  Filename, sometimes left blank
    int32       numBoneFrames;  //  Number of BoneFrames
    int32       numTags;        //  Number of 'tags' per BoneFrame
    int32       numMeshes;      //  Number of Meshes/Skins in MaxSkin
    int32       numMaxSkins;    //  Maximum number of unique skins
    int32       ofsFrames;      //  Always equal to the length this header
    int32       ofsTagStart;    //  Starting position of tag structures
    int32       ofMeshSurfaces; //  Ending position of tag structure
    int32       ofEndOfFile;    //  Size of file
};



struct stBoneFrame
{
    float32 mins[3];
    float32 maxs[3];
    float32 Position[3];
    float32 Scale;
    char    Creator[16];
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
    char            Name[64];
    float32         Position[3];
    float32         Rotation[3][3];
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
    int16           Vertex[3];
    unsigned char   Normal[2];
};

struct stMeshHeader
{
    char    ID[4];
    char    Name[64];
    int32   flags;
    int32   numMeshFrames;
    int32   numSkins;
    int32   numVertexes;
    int32   numTriangles;
    int32   ofsTriangles;
    int32   ofsSkins;
    int32   ofsTexVector;
    int32   ofsVertex;
    int32   ofsEndMeshSize;
};

struct stMesh
{
    stMeshHeader    MeshHeader;
    stSkin* pSkins;
    stTriangle* pTriangle;
    stTexCoord* pTexCoord;
    stVertex* pVertex;
};

