#pragma once


#include "Renderer.h"
#include "TextureManager.h"

namespace ZL
{

	struct MeshGroup
	{
		std::vector<std::shared_ptr<Texture>> textures;
		std::vector<VertexDataStruct> meshes;
		std::vector<VertexRenderStruct> renderMeshes;
	};

	struct AnimatedModel
	{
		std::vector<MeshGroup> parts;

		void RefreshRenderMeshes()
		{
			for (int i = 0; i < parts.size(); i++)
			{
				parts[i].renderMeshes.resize(parts[i].meshes.size());

				for (int j = 0; j < parts[i].meshes.size(); j++)
				{
					parts[i].renderMeshes[j].AssignFrom(parts[i].meshes[j]);
					parts[i].renderMeshes[j].RefreshVBO();
				}

				
			}

	
		}
	};




}