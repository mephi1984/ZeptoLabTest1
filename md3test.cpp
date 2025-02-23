#include "md3test.h"
#include "Renderer.h"
#include "AnimatedModel.h"

void abc(const char* str)
{
	/*
	  FILE *fp = fopen("output.txt", "a+");
	  fprintf(fp, "%s\n", str);
	  fclose(fp);
	  */
}


/*
void DbgTriangle(IDirect3DDevice8* pDevice,
	D3DXVECTOR3& a, D3DXVECTOR3& b, D3DXVECTOR3& c,
	DWORD colour)
{
	struct TLVERTEX
	{
		float x, y, z;
		DWORD col;
		enum { FVF_TLVERTEX = D3DFVF_XYZ | D3DFVF_DIFFUSE };
	};

	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);

#if(0) // no lighting

	pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
#else
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
#endif

	TLVERTEX Vertex[4] =
	{
		//   x	  y		z	colour
		{    a.x, a.y, a.z, colour  },
		{    b.x, b.y, b.z, colour  },
		{    c.x, c.y, c.z, colour  }
	};


#if(DIRECT3D_VERSION >= 0x0900)
	pDevice->SetFVF(TLVERTEX::FVF_TLVERTEX);
#else
	pDevice->SetVertexShader(TLVERTEX::FVF_TLVERTEX);
#endif // DIRECT3D_VERSION
	pDevice->SetTexture(0, NULL);

	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	pDevice->SetRenderState(D3DRS_FILLMODE,
		D3DFILL_WIREFRAME);

	HRESULT hr = pDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 1, Vertex, sizeof(TLVERTEX));
	//g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}*/

//---------------------------------------------------------------------

/*
extern void TexSquareA(IDirect3DDevice8* pDevice,
	D3DXVECTOR3 pos,
	float w,
	float h
);*/


class CMD3
{
public:

	stM3DModel  m_lower;
	stM3DModel  m_upper;
	stM3DModel  m_head;

	stM3DModel  m_gun;

	stAnim		m_Anim[26];


	CMD3()
	{
		//m_startFrame = 0;   
		//m_endFrame = 0;
		//m_currentframe = 0;
		//poll = 0;
		//m_lastUpdate = 0;
	}


	void Create(/* file names */)
	{
		/*for (int i = 0; i < MAX_TEXTURES; i++)
		{
			pTextures[i] = NULL;
		}*/
		InitNormals();

		LoadModel(&m_lower, SZ_MD3_LOWER_FILE);
		LoadSkin(&m_lower, SZ_MD3_LOWER_SKIN_FILE);

		LoadModel(&m_upper, SZ_MD3_UPPER_FILE);
		LoadSkin(&m_upper, SZ_MD3_UPPER_SKIN_FILE);

		LoadModel(&m_head, SZ_MD3_HEAD_FILE);
		LoadSkin(&m_head, SZ_MD3_HEAD_SKIN_FILE);

		LoadAnim(SZ_MD3_ANIM_FILE);

		LoadModel(&m_gun, "model\\railgun\\railgun.md3");
		LoadSkin(&m_gun, "model\\railgun\\railgun.skin");

		SetAnim(TORSO_STAND);
		SetAnim(LEGS_WALK);

		memset(m_head.m_pLinks, 0, sizeof(m_head.m_pLinks));
		memset(m_upper.m_pLinks, 0, sizeof(m_upper.m_pLinks));
		memset(m_lower.m_pLinks, 0, sizeof(m_lower.m_pLinks));

		LinkModel(&m_lower, "tag_torso", &m_upper);
		LinkModel(&m_upper, "tag_head", &m_head);
		LinkModel(&m_upper, "tag_weapon", &m_gun);

		m_upper.m_currentframe = m_upper.m_startFrame;
		m_head.m_currentframe = m_head.m_startFrame;
		m_lower.m_currentframe = m_lower.m_startFrame;

		m_upper.m_poll = 0.0f;
		m_head.m_poll = 0.0f;
		m_lower.m_poll = 0.0f;
	}
	void Release()
	{
		ReleaseModel(&m_lower);
		ReleaseModel(&m_upper);
		ReleaseModel(&m_head);

		ReleaseModel(&m_gun);
		/*
		for (int i = 0; i < MAX_TEXTURES; i++)
		{
			if (pTextures[i])
			{
				pTextures[i]->Release();
				pTextures[i] = NULL;
			}
		}*/
	}
	void Update(float time)
	{
		UpdateFrame(&m_lower, time);
		UpdateFrame(&m_upper, time);
		UpdateFrame(&m_head, time);

	}
	/*
	void Render()
	{
		D3DXMATRIX matCur, matNext;
		D3DXMatrixIdentity(&matCur);
		matNext = matCur;
		DrawSkeleton(&m_lower, &matCur, &matNext);

		DrawModel(&m_lower, &matCur, &matCur);
	}*/

	ZL::AnimatedModel convertToAnimatedModel();

protected:


	void LinkModel(stM3DModel* from, const char* tagnameTo, stM3DModel* modelTo)
	{
		int numTags = from->m_md3Header.numTags;
		stTag* pTags = from->m_pTags;

		for (int i = 0; i < numTags; i++)
		{
			if (strcmp(pTags[i].Name, tagnameTo) == 0)
			{
				from->m_pLinks[i] = modelTo;
			}
		}
	}


	void UpdateFrame(stM3DModel* pMod, float time)
	{
		if (pMod->m_FPS > 0)
		{
			/*
			float deltaFPS = time - (1.0f/pMod->m_FPS);
			if ( deltaFPS < 0.0f )
			{
				_asm
				{
					int 13
				};
			}
			pMod->m_poll += time + (time - 1.0f/pMod->m_FPS);
			*/
			pMod->m_poll += time;
		}
		else
		{
			pMod->m_poll += time;
		}


		if (pMod->m_poll > 1.0f)
		{
			pMod->m_poll = 0.0f;

			pMod->m_currentframe = pMod->m_nextFrame;
			pMod->m_nextFrame++;
			if (pMod->m_nextFrame > pMod->m_endFrame)
			{
				pMod->m_nextFrame = pMod->m_startFrame;
			}
		}
	}



	/*
	void DrawSkeleton(stM3DModel* pMod, D3DXMATRIX* matCur, D3DXMATRIX* matNext)
	{
		DrawModel(pMod, matCur, matNext);

		stM3DModel** pLinks = pMod->m_pLinks;
		stMD3Header* pModHeader = &pMod->m_md3Header;
		stTag* pTags = pMod->m_pTags;
		int currentFrame = pMod->m_currentframe;
		int nextFrame = pMod->m_nextFrame;
		//int startFrame			=  pMod->m_startFrame;

		for (int i = 0; i < pModHeader->numTags; i++)
		{

			if (pLinks[i] == NULL)
			{
				continue;
			}

			stM3DModel* childMod = pLinks[i];
			//stMD3Header* pHeader	= &childMod->m_md3Header;
			//stTag* pTags			=  childMod->m_pTags;


			float (*Rotation)[3];

			Rotation = pTags[currentFrame * pModHeader->numTags + i].Rotation; //3x3 mat
			float* Position = pTags[currentFrame * pModHeader->numTags + i].Position; //vector3
			char* tagName = pTags[currentFrame * pModHeader->numTags + i].Name;


			D3DXMATRIX m;
			m(0, 0) = Rotation[0][0];
			m(0, 1) = Rotation[0][1];
			m(0, 2) = Rotation[0][2];
			m(0, 3) = 0;
			m(1, 0) = Rotation[1][0];
			m(1, 1) = Rotation[1][1];
			m(1, 2) = Rotation[1][2];
			m(1, 3) = 0;
			m(2, 0) = Rotation[2][0];
			m(2, 1) = Rotation[2][1];
			m(2, 2) = Rotation[2][2];
			m(2, 3) = 0;
			m(3, 0) = Position[0];
			m(3, 1) = Position[1];
			m(3, 2) = Position[2];
			m(3, 3) = 1;

			float (*RotationNext)[3] = pTags[nextFrame * pModHeader->numTags + i].Rotation; //3x3 mat
			float* PositionNext = pTags[nextFrame * pModHeader->numTags + i].Position; //vector3
			char* tagNameNext = pTags[nextFrame * pModHeader->numTags + i].Name;

			D3DXMATRIX mNext;
			mNext(0, 0) = RotationNext[0][0];
			mNext(0, 1) = RotationNext[0][1];
			mNext(0, 2) = RotationNext[0][2];
			mNext(0, 3) = 0;
			mNext(1, 0) = RotationNext[1][0];
			mNext(1, 1) = RotationNext[1][1];
			mNext(1, 2) = RotationNext[1][2];
			mNext(1, 3) = 0;
			mNext(2, 0) = RotationNext[2][0];
			mNext(2, 1) = RotationNext[2][1];
			mNext(2, 2) = RotationNext[2][2];
			mNext(2, 3) = 0;
			mNext(3, 0) = PositionNext[0];
			mNext(3, 1) = PositionNext[1];
			mNext(3, 2) = PositionNext[2];
			mNext(3, 3) = 1;


			//D3DXMATRIX world;
			//g_pD3DDevice->GetTransform(D3DTS_WORLD, &world);
			//D3DXMATRIX wrld = m * world;
			//g_pD3DDevice->SetTransform(D3DTS_WORLD, &wrld);

			m = m * (*matCur);
			mNext = mNext * (*matNext);
			DrawSkeleton(childMod, &m, &mNext);

			//g_pD3DDevice->SetTransform(D3DTS_WORLD, &world);

		}
	}

	void DrawModel(stM3DModel* pMod, D3DXMATRIX* matCur, D3DXMATRIX* matNext)
	{
		DrawModelInt(pMod, pMod->m_currentframe, pMod->m_nextFrame, matCur, matNext, pMod->m_poll);
	}

	IDirect3DVertexBuffer8* pVertexBuffer;*/

	float aNorms[256][256][3];

	void InitNormals()
	{
		for (int i = 0; i < 255; i++)
		{
			for (int j = 0; j < 255; j++)
			{
				float alpha = 2.0f * i * M_PI / 255;
				float beta = 2.0f * j * M_PI / 255;
				aNorms[i][j][0] = cosf(beta) * sinf(alpha);
				aNorms[i][j][1] = sinf(beta) * sinf(alpha);
				aNorms[i][j][2] = cosf(alpha);
			}
		}
	}

	/*
	void DrawModelInt(stM3DModel* pMod, const int currentFrame, const int nexFrame, D3DXMATRIX* matCur, D3DXMATRIX* matNext, float pol)
	{
		stMD3Header* pHeader = &pMod->m_md3Header;
		stMesh* pMeshes = pMod->m_pMeshes;

		struct TVERTEX
		{
			float x, y, z;
			float nx, ny, nz;
			float u, v;
			enum { FVF_TVERTEX = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1 };
		};


#if(0) // no lighting

		g_pD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
		g_pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
#else
		g_pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		g_pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
		g_pD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
#endif

		g_pD3DDevice->SetTexture(0, NULL);



		for (int k = 0; k < pHeader->numMeshes; k++)
		{
			stMesh* currentMesh = &pMeshes[k];
			//char* meshName = currentMesh->MeshHeader.Name;

			if (currentMesh->texID >= 0)
			{
				g_pD3DDevice->SetTexture(0, pTextures[currentMesh->texID]);
			}

			int currentOffsetVertex = currentFrame * currentMesh->MeshHeader.numVertexes;
			//interpolation
			int nextCurrentOffsetVertex = nexFrame * currentMesh->MeshHeader.numVertexes;

			int TriangleNum = currentMesh->MeshHeader.numTriangles;


			//TVERTEX * pVertices = new TVERTEX[TriangleNum * 3];


			pVertexBuffer = NULL;
			g_pD3DDevice->CreateVertexBuffer(TriangleNum * 3 * sizeof(TVERTEX), D3DUSAGE_WRITEONLY, TVERTEX::FVF_TVERTEX, D3DPOOL_DEFAULT, &pVertexBuffer);
			TVERTEX* pVertices;
			pVertexBuffer->Lock(0, TriangleNum * 3 * sizeof(TVERTEX), (BYTE**)&pVertices, 0);


			int indx = 0;
			for (int i = 0; i < TriangleNum; i++)
			{
				//D3DXVECTOR3 tri[3];
				for (int j = 0; j < 3; j++)
				{
					int currentVertex = currentMesh->pTriangle[i].Vertex[j];

					D3DXVECTOR3 vA;
					vA.x = (currentMesh->pVertex[currentOffsetVertex + currentVertex].Vertex[0] / 64.0f);
					vA.y = (currentMesh->pVertex[currentOffsetVertex + currentVertex].Vertex[1] / 64.0f);
					vA.z = (currentMesh->pVertex[currentOffsetVertex + currentVertex].Vertex[2] / 64.0f);

					D3DXVECTOR3 nextVA;
					nextVA.x = (currentMesh->pVertex[nextCurrentOffsetVertex + currentVertex].Vertex[0] / 64.0f);
					nextVA.y = (currentMesh->pVertex[nextCurrentOffsetVertex + currentVertex].Vertex[1] / 64.0f);
					nextVA.z = (currentMesh->pVertex[nextCurrentOffsetVertex + currentVertex].Vertex[2] / 64.0f);

					D3DXVec3TransformCoord(&vA, &vA, matCur);
					D3DXVec3TransformCoord(&nextVA, &nextVA, matNext);


					int normU, normV;
					normU = currentMesh->pVertex[currentOffsetVertex + currentVertex].Normal[0];
					normV = currentMesh->pVertex[currentOffsetVertex + currentVertex].Normal[1];
					D3DXVECTOR3 norm;
					norm[0] = aNorms[normU][normV][0];
					norm[1] = aNorms[normU][normV][1];
					norm[2] = aNorms[normU][normV][2];

					normU = currentMesh->pVertex[nextCurrentOffsetVertex + currentVertex].Normal[0];
					normV = currentMesh->pVertex[nextCurrentOffsetVertex + currentVertex].Normal[1];
					D3DXVECTOR3 normNext;
					normNext[0] = aNorms[normU][normV][0];
					normNext[1] = aNorms[normU][normV][1];
					normNext[2] = aNorms[normU][normV][2];


					float u = currentMesh->pTexCoord[currentVertex].Coord[0];
					float v = currentMesh->pTexCoord[currentVertex].Coord[1];

					// Interplated value
					float nx = norm[0] + pol * (normNext[0] - norm[0]);
					float ny = norm[1] + pol * (normNext[1] - norm[1]);
					float nz = norm[2] + pol * (normNext[2] - norm[2]);

					float pA0 = vA[0] + pol * (nextVA[0] - vA[0]);
					float pA1 = vA[1] + pol * (nextVA[1] - vA[1]);
					float pA2 = vA[2] + pol * (nextVA[2] - vA[2]);

					//tri[j].x = pA0;
					//tri[j].y = pA1;
					//tri[j].z = pA2;


					pVertices[indx].x = pA0;
					pVertices[indx].y = pA1;
					pVertices[indx].z = pA2;
					pVertices[indx].nx = nx;
					pVertices[indx].ny = ny;
					pVertices[indx].nz = nz;
					pVertices[indx].u = u;
					pVertices[indx].v = v;
					indx++;
				}

				//DbgTriangle(g_pD3DDevice, tri[0], tri[1], tri[2], 0xff00ffff );
			}


			pVertexBuffer->Unlock();
			g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
			g_pD3DDevice->SetVertexShader(TVERTEX::FVF_TVERTEX);
			g_pD3DDevice->SetStreamSource(0, pVertexBuffer, sizeof(TVERTEX));
			g_pD3DDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, TriangleNum);
			pVertexBuffer->Release();
			pVertexBuffer = NULL;


		}
	}*/


	bool SetAnim(int ani)
	{
		if (ani >= 0 && ani <= 5)
		{
			m_lower.m_FPS = m_Anim[ani].FPS;
			m_upper.m_FPS = m_Anim[ani].FPS;

			m_lower.m_startFrame = m_Anim[ani].FirstFrame;
			m_upper.m_startFrame = m_Anim[ani].FirstFrame;

			//m_upper.m_currentframe = m_upper.m_startFrame;
			//m_lower.m_currentframe = m_lower.m_startFrame;

			m_lower.m_endFrame = m_Anim[ani].FirstFrame + m_Anim[ani].numFrames;
			m_upper.m_endFrame = m_Anim[ani].FirstFrame + m_Anim[ani].numFrames;

			m_lower.m_anim = ani;
			m_upper.m_anim = ani;
		}
		else if (ani >= 6 && ani <= 12)
		{
			m_upper.m_FPS = m_Anim[ani].FPS;
			m_upper.m_nextFrame = m_Anim[ani].FirstFrame;
			m_upper.m_startFrame = m_Anim[ani].FirstFrame;
			m_upper.m_endFrame = m_Anim[ani].FirstFrame + m_Anim[ani].numFrames;
			//m_upper.m_currentframe = m_upper.m_startFrame;

			m_upper.m_anim = ani;
		}
		else if (ani >= 13 && ani <= 24)
		{
			m_lower.m_FPS = m_Anim[ani].FPS;
			m_lower.m_nextFrame = m_Anim[ani].FirstFrame;
			m_lower.m_startFrame = m_Anim[ani].FirstFrame;
			m_lower.m_endFrame = m_Anim[ani].FirstFrame + m_Anim[ani].numFrames;
			//m_lower.m_currentframe = m_lower.m_startFrame;

			m_lower.m_anim = ani;
		}
		else
		{
			throw std::runtime_error("lol");
			/*_asm
			{
				int 13;
			}*/
		}

		return true;
	}


	bool LoadSkin(stM3DModel* pMod, const char* filename)
	{
		char buf[256];

		char  MeshName[256] = { 0 };
		char  ImageName[256] = { 0 };

		FILE* fp = fopen(filename, "r");

		if (fp == NULL)
		{
			abc("unable to open file");
			return false;
		}

		//char firstWord[256];

		char* val = NULL;
		int i = 0;

		do
		{
			memset(MeshName, 0, sizeof(MeshName));

			val = fgets(buf, 256, fp);
			if (val == NULL)
			{
				break;
			}
			//sscanf(buf, "%s", firstWord);

			if (buf[strlen(buf) - 1] == '\n')
			{
				buf[strlen(buf) - 1] = NULL;
			}

			if (buf[strlen(buf) - 1] == ',')
			{
				strcpy(MeshName, buf);
				MeshName[strlen(MeshName) - 1] = NULL;
			}

			if (strncmp(buf, "tag_", 4) == 0)  // tags dont have skins
			{
				continue;
			}

			char* pMeshNameEnd = strchr(buf, ',');
			int meshNameLen = (int)(pMeshNameEnd - buf);
			strncpy(MeshName, buf, meshNameLen);

			//char* pImageName = strstr(MeshName, ","); // get the full image and path name
			//strcpy(ImageName, pImageName);			  // get name from last / (i.e only filename)
			char* pName = strrchr(buf, '/');
			strcpy(ImageName, pName + 1);



			for (int i = 0; i < pMod->m_md3Header.numMeshes; i++)
			{
				stMesh* pMesh = &pMod->m_pMeshes[i];
				if (strcmp(pMesh->MeshHeader.Name, MeshName) == 0)
				{
					char TextureName[256];
					strcpy(TextureName, SZ_MD3_TEXTURE_PATH);
					strcat(TextureName, ImageName);

					//D3DXCreateTextureFromFile(g_pD3DDevice, TextureName, &pTextures[g_NumLoadedTextures]);
					pMesh->texID = g_NumLoadedTextures;
					g_NumLoadedTextures++;
				}
			}
			// lose the starting /
		} while (val);


		return true;
	}



	bool LoadAnim(const char* filename)
	{
		char buf[256];

		FILE* fp = fopen(filename, "r");

		if (fp == NULL)
		{
			abc("unable to open file");
			return false;
		}

		char firstWord[256];

		char* val = NULL;
		int i = 0;

		do
		{
			strcpy(firstWord, "//");

			val = fgets(buf, 256, fp);
			if (val == NULL)
			{
				break;
			}
			sscanf(buf, "%s", firstWord);


			if (strcmp("sex", firstWord) == NULL)
			{
			}
			else if (strcmp("headoffset", firstWord) == NULL)
			{
			}
			else if (strcmp("footsteps", firstWord) == NULL)
			{
			}
			else if (strcmp("", firstWord) == NULL)
			{
			}
			else if (strcmp("//", firstWord) == NULL)
			{
			}
			else
			{
				char words[4][256];

				sscanf(buf, "%s %s %s %s", &words[0], &words[1], &words[2], &words[3]);
				// Extract the values of FirstFrame, numFrames, LoopingFrames, FPS from the String
				int FirstFrame = atoi(words[0]);
				int numFrames = atoi(words[1]);
				int loopingFrames = atoi(words[2]);
				int FPS = atoi(words[3]);

				m_Anim[i].FirstFrame = FirstFrame;
				m_Anim[i].numFrames = numFrames;
				m_Anim[i].LoopingFrames = loopingFrames;
				m_Anim[i].FPS = FPS;

				i++;
			}

		} while (val);

		int skip = m_Anim[LEGS_WALKCR].FirstFrame - m_Anim[TORSO_GESTURE].FirstFrame;

		for (int i = LEGS_WALKCR; i < MAX_ANIMATIONS; i++)
		{
			m_Anim[i].FirstFrame = m_Anim[i].FirstFrame - skip;
		}


		for (int i = 0; i < MAX_ANIMATIONS; i++)
		{
			if (m_Anim[i].numFrames > 0)
			{
				m_Anim[i].numFrames = m_Anim[i].numFrames - 1;
			}
		}



		fclose(fp);

		return true;
	}

	//-----------------------------------------------------------------------
	//
	//  Loads model from a .md3 file
	//
	//-----------------------------------------------------------------------
	bool LoadModel(stM3DModel* pMod, const char* filename)
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
		strcpy(pMod->m_md3FileName, filename);
		sprintf(buf, "MD3 FileName: %s", pMod->m_md3FileName);
		abc(buf);

		sprintf(buf, "FileSize: %d", md3filesize);
		abc(buf);

		abc("\n~~MD3 Header~~\n");

		stMD3Header* pHeader = &pMod->m_md3Header;

		// read header
		fread(pHeader, 1, sizeof(stMD3Header), fp);

		/*
		// log debug information to file
		sprintf(buf, "ID %c%c%c%c", pHeader->ID[0], pHeader->ID[1], pHeader->ID[2], pHeader->ID[3]);
		abc(buf);

		sprintf(buf, "Version: %d", pHeader->Version);
		abc(buf);

		sprintf(buf, "FileName: %s", pHeader->Filename);
		abc(buf);

		sprintf(buf, "numBoneFrames: %d", pHeader->numBoneFrames);
		abc(buf);

		sprintf(buf, "numTags: %d", pHeader->numTags);
		abc(buf);

		sprintf(buf, "numMeshes: %d", pHeader->numMeshes);
		abc(buf);

		sprintf(buf, "numMaxSkins: %d", pHeader->numMaxSkins);
		abc(buf);

		sprintf(buf, "ofsFrames: %d", pHeader->ofsFrames);
		abc(buf);

		sprintf(buf, "ofsTagStart: %d", pHeader->ofsTagStart);
		abc(buf);

		sprintf(buf, "ofMeshSurfaces: %d", pHeader->ofMeshSurfaces);
		abc(buf);

		sprintf(buf, "ofEndOfFile (Filesize): %d", pHeader->ofEndOfFile);
		abc(buf);
		*/

		if (strcmp("IDP3", pHeader->ID) == NULL)
		{
			sprintf(buf, "Incorrect File Format 'Incorrect ID' ie. ('IDP3')");
			abc(buf);
		}


		// Allocate memory for all or bones/tags/etc
		pMod->m_pBoneFrame = new stBoneFrame[pHeader->numBoneFrames];
		pMod->m_pTags = new stTag[pHeader->numBoneFrames * pHeader->numTags];
		pMod->m_pMeshes = new stMesh[pHeader->numMeshes];

		stBoneFrame* pBoneFrame = pMod->m_pBoneFrame;
		stTag* pTags = pMod->m_pTags;
		stMesh* pMeshes = pMod->m_pMeshes;

		// Lets seek to the start of the bone frames & read boneframe
		fseek(fp, pHeader->ofsFrames, SEEK_SET);
		fread(pBoneFrame, 1, pHeader->numBoneFrames * sizeof(stBoneFrame), fp);

		/*
		sprintf(buf, "\n~~~~BoneFrames: %d~~~~~~", pHeader->numBoneFrames);
		abc(buf);
		for (int i=0; i<pHeader->numBoneFrames; i++)
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
		*/


		// Seek to start of tags and read them all in
		fseek(fp, pHeader->ofsTagStart, SEEK_SET);
		fread(pTags, 1, pHeader->numBoneFrames * pHeader->numTags * sizeof(stTag), fp);

		/*
		sprintf(buf, "\n~~~~Tags: %d~~~~~~", pHeader->numTags);
		abc(buf);
		for (int i=0; i<m_md3Header.numTags; i++)
		{
			abc("#");
			sprintf(buf, "Name[%s]", m_pTags[i].Name);
			abc(buf);
			sprintf(buf, "Position[%.1f,%.1f,%.1f]", m_pTags[i].Position[0], m_pTags[i].Position[1], m_pTags[i].Position[2]);
			abc(buf);
			sprintf(buf, "Rotation[%.1f,%.1f,%.1f][...][...]", m_pTags[i].Rotation[0][0], m_pTags[i].Rotation[0][1], m_pTags[i].Rotation[0][2]);
			abc(buf);
		}
		*/

		int meshOFS = pHeader->ofMeshSurfaces;

		for (int j = 0; j < pHeader->numMeshes; j++)
		{
			stMesh* pMesh = &pMeshes[j];
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
				pMesh->pVertex = new stVertex[pMeshHeader->numVertexes * pMeshHeader->numMeshFrames];
				fread(pMesh->pVertex, 1, pMeshHeader->numMeshFrames * pMeshHeader->numVertexes * sizeof(stVertex), fp);

				pMesh->texID = -1;
			}

			meshOFS += pMeshHeader->ofsEndMeshSize;

		}//End for meshes


		/*
		sprintf(buf, "\n~~~~Mesh Surfaces: %d~~~~~~", m_md3Header.numMeshes);
		abc(buf);
		for (int j=0; j<m_md3Header.numMeshes; j++)
		{
			abc("#");
			stMesh * pMesh = &m_pMeshes[j];
			stMeshHeader * pMeshHeader = &(pMesh->MeshHeader);

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
			for (int i=0; i<pMeshHeader->numTriangles; i++)
			{
				stTriangle * pTri = &(pMesh->pTriangle[i]);
				sprintf(buf, "Triangle [%d,%d,%d]", pTri->Vertex[0], pTri->Vertex[1], pTri->Vertex[2]);
				abc(buf);
			}

			// Mesh Skins
			for (int i=0; i<pMeshHeader->numSkins; i++)
			{
				stSkin * pSkin = &(pMesh->pSkins[i]);
				sprintf(buf, "Skin:Name [%s]", pSkin->Name);
				abc(buf);
				sprintf(buf, "Skin:Index [%d]", pSkin->index);
				abc(buf);
			}

			for (int i=0; i<pMeshHeader->numVertexes; i++)
			{
				stTexCoord * pTex = &(pMesh->pTexCoord[i]);
				sprintf(buf, "TexCoord:Index [%.1f,%.1f]", pTex->Coord[0], pTex->Coord[1]);
				abc(buf);
			}


			for (int i=0; i<pMeshHeader->numVertexes; i++)
			{
				stVertex* pVert = &(pMesh->pVertex[i]);
				sprintf(buf, "Vertice:Vertex [%d,%d,%d]", pVert->Vertex[0], pVert->Vertex[1], pVert->Vertex[2]);
				abc(buf);

			}
		}
		*/


		fclose(fp);

		/*
		for (int j=0; j<m_md3Header.numMeshes; j++)
		{
			stMesh * pMesh = &m_pMeshes[j];

			delete[] pMesh->pSkins;
			delete[] pMesh->pTexCoord;
			delete[] pMesh->pTriangle;
			delete[] pMesh->pVertex;

			pMesh->pSkins		= NULL;
			pMesh->pTexCoord	= NULL;
			pMesh->pTriangle	= NULL;
			pMesh->pVertex		= NULL;
		}

		delete[] m_pBoneFrame;
		delete[] m_pTags;
		delete[] m_pMeshes;

		m_pBoneFrame = NULL;
		m_pTags		 = NULL;
		m_pMeshes	 = NULL;
		*/
		return true;
	}// End LoadModel(..)


	bool ReleaseModel(stM3DModel* pMod)
	{
		stMD3Header* pHeader = &pMod->m_md3Header;
		stMesh* pMeshes = pMod->m_pMeshes;

		for (int j = 0; j < pHeader->numMeshes; j++)
		{
			stMesh* pMesh = &pMeshes[j];

			delete[] pMesh->pSkins;
			delete[] pMesh->pTexCoord;
			delete[] pMesh->pTriangle;
			delete[] pMesh->pVertex;

			pMesh->pSkins = NULL;
			pMesh->pTexCoord = NULL;
			pMesh->pTriangle = NULL;
			pMesh->pVertex = NULL;
		}

		stBoneFrame* pBoneFrame = pMod->m_pBoneFrame;
		stTag* pTags = pMod->m_pTags;
		stMesh* pMshes = pMod->m_pMeshes;

		delete[] pBoneFrame;
		delete[] pTags;
		delete[] pMshes;

		pBoneFrame = NULL;
		pTags = NULL;
		pMshes = NULL;

		return true;
	}
};

ZL::AnimatedModel CMD3::convertToAnimatedModel()
{
	ZL::AnimatedModel result;

	result.parts.resize(3);

	std::array<stM3DModel*, 3> parts = { &this->m_head, &this->m_upper, &this->m_lower };
	std::array<std::vector<std::string>, 3> textureNames = {
		//std::vector<std::string>{"model/sarge/cigar.png", "model/sarge/band.png"},
		std::vector<std::string>{"model/sarge/band.png"},
		std::vector<std::string>{"model/sarge/band.png"},
		std::vector<std::string>{"model/sarge/band.png"}
	};

	for (int m = 0; m < 3; m++)
	{
		int numMeshes = parts[m]->m_md3Header.numMeshes;

		result.parts[m].meshes.resize(numMeshes);

		for (int i = 0; i < textureNames[m].size(); i++)
		{
			std::string texName = textureNames[m][i];
			result.parts[m].textures.push_back(std::make_shared<ZL::Texture>(ZL::CreateTextureDataFromPng(texName)));
		}

		for (int n = 0; n < numMeshes; n++)
		{
			auto& pMesh = parts[m]->m_pMeshes[n];
			auto& pMeshHeader = parts[m]->m_pMeshes[n].MeshHeader;
			

			
			for (int i = 0; i < pMeshHeader.numTriangles; i++)
			{
				stTriangle* pTri = &(pMesh.pTriangle[i]);

				stVertex* pVert0 = &(pMesh.pVertex[pTri->Vertex[0]]);
				stVertex* pVert1 = &(pMesh.pVertex[pTri->Vertex[1]]);
				stVertex* pVert2 = &(pMesh.pVertex[pTri->Vertex[2]]);
				ZL::Vector3f v0;

				v0.v[0] = pVert0->Vertex[0];
				v0.v[1] = pVert0->Vertex[1];
				v0.v[2] = pVert0->Vertex[2];

				result.parts[m].meshes[n].PositionData.push_back(v0);

				ZL::Vector3f v1;

				v1.v[0] = pVert1->Vertex[0];
				v1.v[1] = pVert1->Vertex[1];
				v1.v[2] = pVert1->Vertex[2];

				result.parts[m].meshes[n].PositionData.push_back(v1);

				ZL::Vector3f v2;

				v2.v[0] = pVert2->Vertex[0];
				v2.v[1] = pVert2->Vertex[1];
				v2.v[2] = pVert2->Vertex[2];

				result.parts[m].meshes[n].PositionData.push_back(v2);

				stTexCoord* pTex0 = &(pMesh.pTexCoord[pTri->Vertex[0]]);
				stTexCoord* pTex1 = &(pMesh.pTexCoord[pTri->Vertex[1]]);
				stTexCoord* pTex2 = &(pMesh.pTexCoord[pTri->Vertex[2]]);



				ZL::Vector2f t0;

				t0.v[0] = pTex0->Coord[0];
				t0.v[1] = pTex0->Coord[1];

				result.parts[m].meshes[n].TexCoordData.push_back(t0);

				ZL::Vector2f t1;

				t1.v[0] = pTex1->Coord[0];
				t1.v[1] = pTex1->Coord[1];

				result.parts[m].meshes[n].TexCoordData.push_back(t1);

				ZL::Vector2f t2;

				t2.v[0] = pTex2->Coord[0];
				t2.v[1] = pTex2->Coord[1];

				result.parts[m].meshes[n].TexCoordData.push_back(t2);
			}
		}
	}

	return result;
}


ZL::AnimatedModel testLoadModel()
{
	CMD3 md3;

	md3.Create();

	ZL::AnimatedModel r = md3.convertToAnimatedModel();

	r.RefreshRenderMeshes();

	return r;

}