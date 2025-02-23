#include "md3test.h"
#include "Renderer.h"

//Saving debug information to a log file
void abc(const char* str)
{
    /*
    FILE* fp = fopen("output.txt", "a+");
    fprintf(fp, "%s\n", str);
    fclose(fp);*/
}


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


class CMD3
{
public:
    char        m_md3FileName[MAX_FILENAME_LENGTH];
    stMD3Header m_md3Header;


    stBoneFrame* m_pBoneFrame;
    stTag* m_pTags;
    stMesh* m_pMeshes;

    std::vector<ZL::VertexDataStruct> modelData;

    //-----------------------------------------------------------------------
    //
    //  Loads model from a .md3 file
    //
    //-----------------------------------------------------------------------
    bool LoadModel(const char* filename)
    {
        char buf[256];

        FILE* fp = fopen(filename, "rb");

        if (fp == NULL)
        {
            abc("unable to open file");
            return false;
        }

        // Lets get the size of this md3 file
        int md3filesize = filesize(fp);
        fseek(fp, 0L, SEEK_SET);

        if (strlen(filename) > 255)
        {
            sprintf(buf, "filename is longer than %d", MAX_FILENAME_LENGTH);
            abc(buf);
            return false;
        }
        // copy name
        strcpy(m_md3FileName, filename);
        sprintf(buf, "MD3 FileName: %s", m_md3FileName);
        abc(buf);

        sprintf(buf, "FileSize: %d", md3filesize);
        abc(buf);

        abc("\n~~MD3 Header~~\n");

        // read header
        fread(&m_md3Header, 1, sizeof(stMD3Header), fp);

        // log debug information to file
        sprintf(buf, "ID %c%c%c%c", m_md3Header.ID[0], m_md3Header.ID[1], m_md3Header.ID[2], m_md3Header.ID[3]);
        abc(buf);

        sprintf(buf, "Version: %d", m_md3Header.Version);
        abc(buf);

        sprintf(buf, "FileName: %s", m_md3Header.Filename);
        abc(buf);

        sprintf(buf, "numBoneFrames: %d", m_md3Header.numBoneFrames);
        abc(buf);

        sprintf(buf, "numTags: %d", m_md3Header.numTags);
        abc(buf);

        sprintf(buf, "numMeshes: %d", m_md3Header.numMeshes);
        abc(buf);

        sprintf(buf, "numMaxSkins: %d", m_md3Header.numMaxSkins);
        abc(buf);

        sprintf(buf, "ofsFrames: %d", m_md3Header.ofsFrames);
        abc(buf);

        sprintf(buf, "ofsTagStart: %d", m_md3Header.ofsTagStart);
        abc(buf);

        sprintf(buf, "ofMeshSurfaces: %d", m_md3Header.ofMeshSurfaces);
        abc(buf);

        sprintf(buf, "ofEndOfFile (Filesize): %d", m_md3Header.ofEndOfFile);
        abc(buf);


        if (strcmp("IDP3", m_md3Header.ID) == NULL)
        {
            sprintf(buf, "Incorrect File Format 'Incorrect ID' ie. ('IDP3')");
            abc(buf);
        }

        // Allocate memory for all or bones/tags/etc
        m_pBoneFrame = new stBoneFrame[m_md3Header.numBoneFrames];
        m_pTags = new stTag[m_md3Header.numBoneFrames * m_md3Header.numTags];
        m_pMeshes = new stMesh[m_md3Header.numMeshes];


        // Lets seek to the start of the bone frames & read boneframe
        fseek(fp, m_md3Header.ofsFrames, SEEK_SET);
        fread(m_pBoneFrame, 1, m_md3Header.numBoneFrames * sizeof(stBoneFrame), fp);

        sprintf(buf, "\n~~~~BoneFrames: %d~~~~~~", m_md3Header.numBoneFrames);
        abc(buf);
        for (int i = 0; i < m_md3Header.numBoneFrames; i++)
        {
            abc("#");
            sprintf(buf, "mins[%.1f,%.1f,%.1f]", m_pBoneFrame[i].mins[0], m_pBoneFrame[i].mins[1], m_pBoneFrame[i].mins[2]);
            abc(buf);
            sprintf(buf, "maxs[%.1f,%.1f,%.1f]", m_pBoneFrame[i].maxs[0], m_pBoneFrame[i].maxs[1], m_pBoneFrame[i].maxs[2]);
            abc(buf);
            sprintf(buf, "Position[%.1f,%.1f,%.1f]", m_pBoneFrame[i].Position[0], m_pBoneFrame[i].Position[1], m_pBoneFrame[i].Position[2]);
            abc(buf);
            sprintf(buf, "Scale[%.1f]", m_pBoneFrame[i].Scale);
            abc(buf);
            sprintf(buf, "Creator[%s]", m_pBoneFrame[i].Creator);
            abc(buf);
        }


        // Seek to start of tags and read them all in
        fseek(fp, m_md3Header.ofsTagStart, SEEK_SET);
        fread(m_pTags, 1, m_md3Header.numBoneFrames * m_md3Header.numTags * sizeof(stTag), fp);

        sprintf(buf, "\n~~~~Tags: %d~~~~~~", m_md3Header.numTags);
        abc(buf);
        for (int i = 0; i < m_md3Header.numTags; i++)
        {
            abc("#");
            sprintf(buf, "Name[%s]", m_pTags[i].Name);
            abc(buf);
            sprintf(buf, "Position[%.1f,%.1f,%.1f]", m_pTags[i].Position[0], m_pTags[i].Position[1], m_pTags[i].Position[2]);
            abc(buf);
            sprintf(buf, "Rotation[%.1f,%.1f,%.1f][...][...]", m_pTags[i].Rotation[0][0], m_pTags[i].Rotation[0][1], m_pTags[i].Rotation[0][2]);
            abc(buf);
        }


        int meshOFS = m_md3Header.ofMeshSurfaces;

        

        for (int j = 0; j < m_md3Header.numMeshes; j++)
        {
            stMesh* pMesh = &m_pMeshes[j];
            stMeshHeader* pMeshHeader = &(pMesh->MeshHeader);

            fseek(fp, meshOFS, SEEK_SET);

            // Seek to the start of the mesh data and read it all in
            fread(pMeshHeader, 1, sizeof(stMeshHeader), fp);

            // Read in all the sub parts of the mesh data
            {
                fseek(fp, meshOFS + pMeshHeader->ofsTriangles, SEEK_SET);
                pMesh->pTriangle = new stTriangle[pMeshHeader->numTriangles];
                fread(pMesh->pTriangle, 1, pMeshHeader->numTriangles * sizeof(stTriangle), fp);

                fseek(fp, meshOFS + pMeshHeader->ofsSkins, SEEK_SET);
                pMesh->pSkins = new stSkin[pMeshHeader->numSkins];
                fread(pMesh->pSkins, 1, pMeshHeader->numSkins * sizeof(stSkin), fp);

                fseek(fp, meshOFS + pMeshHeader->ofsTexVector, SEEK_SET);
                pMesh->pTexCoord = new stTexCoord[pMeshHeader->numVertexes];
                fread(pMesh->pTexCoord, 1, pMeshHeader->numVertexes * sizeof(stTexCoord), fp);

                fseek(fp, meshOFS + pMeshHeader->ofsVertex, SEEK_SET);
                pMesh->pVertex = new stVertex[pMeshHeader->numVertexes];
                fread(pMesh->pVertex, 1, pMeshHeader->numVertexes * sizeof(stVertex), fp);
            }

            meshOFS += pMeshHeader->ofsEndMeshSize;

        }//End for meshes



        modelData.resize(m_md3Header.numMeshes); // !!! Load the mesh actually


        sprintf(buf, "\n~~~~Mesh Surfaces: %d~~~~~~", m_md3Header.numMeshes);
        abc(buf);
        for (int j = 0; j < m_md3Header.numMeshes; j++)
        {
            abc("#");
            stMesh* pMesh = &m_pMeshes[j];
            stMeshHeader* pMeshHeader = &(pMesh->MeshHeader);

            sprintf(buf, "ID [%c%c%c%c]", pMeshHeader->ID[0], pMeshHeader->ID[1], pMeshHeader->ID[2], pMeshHeader->ID[3]);
            abc(buf);
            sprintf(buf, "Name [%s]", pMeshHeader->Name);
            abc(buf);
            sprintf(buf, "flags [0x%.2X]", pMeshHeader->flags);
            abc(buf);
            sprintf(buf, "numMeshFrames [%d]", pMeshHeader->numMeshFrames);
            abc(buf);
            sprintf(buf, "numSkins [%d]", pMeshHeader->numSkins);
            abc(buf);
            sprintf(buf, "numVertexes [%d]", pMeshHeader->numVertexes);
            abc(buf);
            sprintf(buf, "numVertexes [%d]", pMeshHeader->numVertexes);
            abc(buf);
            sprintf(buf, "ofsTriangles [%d]", pMeshHeader->ofsTriangles);
            abc(buf);
            sprintf(buf, "ofsSkins [%d]", pMeshHeader->ofsSkins);
            abc(buf);
            sprintf(buf, "ofsTexVector [%d]", pMeshHeader->ofsTexVector);
            abc(buf);
            sprintf(buf, "ofsVertex [%d]", pMeshHeader->ofsVertex);
            abc(buf);
            sprintf(buf, "ofsEndMeshSize [%d]", pMeshHeader->ofsEndMeshSize);
            abc(buf);


            // Mesh Triangles
            for (int i = 0; i < pMeshHeader->numTriangles; i++)
            {
                stTriangle* pTri = &(pMesh->pTriangle[i]);
                sprintf(buf, "Triangle [%d,%d,%d]", pTri->Vertex[0], pTri->Vertex[1], pTri->Vertex[2]);
                abc(buf);
            }

            // Mesh Skins
            for (int i = 0; i < pMeshHeader->numSkins; i++)
            {
                stSkin* pSkin = &(pMesh->pSkins[i]);
                sprintf(buf, "Skin:Name [%s]", pSkin->Name);
                abc(buf);
                sprintf(buf, "Skin:Index [%d]", pSkin->index);
                abc(buf);
            }

            for (int i = 0; i < pMeshHeader->numVertexes; i++)
            {
                stTexCoord* pTex = &(pMesh->pTexCoord[i]);
                sprintf(buf, "TexCoord:Index [%.1f,%.1f]", pTex->Coord[0], pTex->Coord[1]);
                abc(buf);
            }


            for (int i = 0; i < pMeshHeader->numVertexes; i++)
            {
                stVertex* pVert = &(pMesh->pVertex[i]);
                sprintf(buf, "Vertice:Vertex [%d,%d,%d]", pVert->Vertex[0], pVert->Vertex[1], pVert->Vertex[2]);
                abc(buf);

            }

            //!!! Load the mesh actually:
            for (int i = 0; i < pMeshHeader->numTriangles; i++)
            {
                stTriangle* pTri = &(pMesh->pTriangle[i]);

                stVertex* pVert0 = &(pMesh->pVertex[pTri->Vertex[0]]);
                stVertex* pVert1 = &(pMesh->pVertex[pTri->Vertex[1]]);
                stVertex* pVert2 = &(pMesh->pVertex[pTri->Vertex[2]]);
                ZL::Vector3f v0;

                v0.v[0] = pVert0->Vertex[0];
                v0.v[1] = pVert0->Vertex[1];
                v0.v[2] = pVert0->Vertex[2];

                modelData[j].PositionData.push_back(v0);

                ZL::Vector3f v1;

                v1.v[0] = pVert1->Vertex[0];
                v1.v[1] = pVert1->Vertex[1];
                v1.v[2] = pVert1->Vertex[2];

                modelData[j].PositionData.push_back(v1);

                ZL::Vector3f v2;

                v2.v[0] = pVert2->Vertex[0];
                v2.v[1] = pVert2->Vertex[1];
                v2.v[2] = pVert2->Vertex[2];

                modelData[j].PositionData.push_back(v2);

                stTexCoord* pTex0 = &(pMesh->pTexCoord[pTri->Vertex[0]]);
                stTexCoord* pTex1 = &(pMesh->pTexCoord[pTri->Vertex[1]]);
                stTexCoord* pTex2 = &(pMesh->pTexCoord[pTri->Vertex[2]]);



                ZL::Vector2f t0;

                t0.v[0] = pTex0->Coord[0];
                t0.v[1] = pTex0->Coord[1];

                modelData[j].TexCoordData.push_back(t0);

                ZL::Vector2f t1;

                t1.v[0] = pTex1->Coord[0];
                t1.v[1] = pTex1->Coord[1];

                modelData[j].TexCoordData.push_back(t1);

                ZL::Vector2f t2;

                t2.v[0] = pTex2->Coord[0];
                t2.v[1] = pTex2->Coord[1];

                modelData[j].TexCoordData.push_back(t2);
            }


        }


        fclose(fp);

        for (int j = 0; j < m_md3Header.numMeshes; j++)
        {
            stMesh* pMesh = &m_pMeshes[j];

            delete[] pMesh->pSkins;
            delete[] pMesh->pTexCoord;
            delete[] pMesh->pTriangle;
            delete[] pMesh->pVertex;

            pMesh->pSkins = NULL;
            pMesh->pTexCoord = NULL;
            pMesh->pTriangle = NULL;
            pMesh->pVertex = NULL;
        }

        delete[] m_pBoneFrame;
        delete[] m_pTags;
        delete[] m_pMeshes;

        m_pBoneFrame = NULL;
        m_pTags = NULL;
        m_pMeshes = NULL;

        return true;
    }// End LoadModel(..)

};


std::vector<ZL::VertexDataStruct> testLoadModel()
{
    static CMD3 md3;

    md3.LoadModel(SZ_MD3_FILE);

    return md3.modelData;
}