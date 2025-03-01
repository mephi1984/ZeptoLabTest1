#include "Renderer.h"



namespace ZL {

	VBOHolder::VBOHolder()
	{
		glGenBuffers(1, &Buffer);
	}

	VBOHolder::~VBOHolder()
	{
		glDeleteBuffers(1, &Buffer);
	}

	GLuint VBOHolder::getBuffer()
	{
		return Buffer;
	}

	VAOHolder::VAOHolder()
	{
#ifndef EMSCRIPTEN
		glGenVertexArrays(1, &vao);
#endif
	}

	VAOHolder::~VAOHolder()
	{
#ifndef EMSCRIPTEN

#ifdef __linux__
		glDeleteVertexArrays(1, &vao);
#else
		//Windows
		glDeleteVertexArray(1, &vao);
#endif
#endif
	}

	GLuint VAOHolder::getBuffer()
	{
		return vao;
	}


	VertexDataStruct CreateRect2D(Vector2f center, Vector2f halfWidthHeight, float zLevel)
	{
		Vector2f posFrom = center - halfWidthHeight;

		Vector2f posTo = center + halfWidthHeight;
		
		Vector3f pos1 = { posFrom.v[0], posFrom.v[1], zLevel };
		Vector3f pos2 = { posFrom.v[0], posTo.v[1], zLevel }; 
		Vector3f pos3 = { posTo.v[0], posTo.v[1], zLevel };
		Vector3f pos4 = { posTo.v[0], posFrom.v[1], zLevel };


		Vector2f texCoordPos1 = { 0.0f, 0.0f };
		Vector2f texCoordPos2 = { 0.0f, 1.0f };
		Vector2f texCoordPos3 = { 1.0f, 1.0f };
		Vector2f texCoordPos4 = { 1.0f, 0.0f };

		VertexDataStruct result;

		result.PositionData.push_back(pos1);
		result.PositionData.push_back(pos2);
		result.PositionData.push_back(pos3);
		result.PositionData.push_back(pos3);
		result.PositionData.push_back(pos4);
		result.PositionData.push_back(pos1);

		result.TexCoordData.push_back(texCoordPos1);
		result.TexCoordData.push_back(texCoordPos2);
		result.TexCoordData.push_back(texCoordPos3);
		result.TexCoordData.push_back(texCoordPos3);
		result.TexCoordData.push_back(texCoordPos4);
		result.TexCoordData.push_back(texCoordPos1);

		return result;
	}

	VertexDataStruct CreateRectHorizontalSections2D(Vector2f center, Vector2f halfWidthHeight, float zLevel, size_t sectionCount)
	{
		Vector2f posFrom = center - halfWidthHeight;

		Vector2f posTo = center + halfWidthHeight;

		float sectionWidth = halfWidthHeight.v[0] * 2.f;

		VertexDataStruct result;

		for (size_t i = 0; i < sectionCount; i++)
		{
			Vector3f pos1 = { posFrom.v[0]+sectionWidth*i, posFrom.v[1], zLevel };
			Vector3f pos2 = { posFrom.v[0] + sectionWidth * i, posTo.v[1], zLevel };
			Vector3f pos3 = { posTo.v[0] + sectionWidth * i, posTo.v[1], zLevel };
			Vector3f pos4 = { posTo.v[0] + sectionWidth * i, posFrom.v[1], zLevel };

			result.PositionData.push_back(pos1);
			result.PositionData.push_back(pos2);
			result.PositionData.push_back(pos3);
			result.PositionData.push_back(pos3);
			result.PositionData.push_back(pos4);
			result.PositionData.push_back(pos1);

			Vector2f texCoordPos1 = { 0.0f, 0.0f };
			Vector2f texCoordPos2 = { 0.0f, 1.0f };
			Vector2f texCoordPos3 = { 1.0f, 1.0f };
			Vector2f texCoordPos4 = { 1.0f, 0.0f };

			result.TexCoordData.push_back(texCoordPos1);
			result.TexCoordData.push_back(texCoordPos2);
			result.TexCoordData.push_back(texCoordPos3);
			result.TexCoordData.push_back(texCoordPos3);
			result.TexCoordData.push_back(texCoordPos4);
			result.TexCoordData.push_back(texCoordPos1);
		}


		return result;

	}

	VertexDataStruct CreateCube3D(float scale)
	{

		std::array<std::array<Vector3f, 4>, 6> cubeSides;

		std::array<Vector3f, 6> cubeColors;


		cubeSides[0][0] = { -1, -1, -1 };
		cubeSides[0][1] = { -1,  1, -1 };
		cubeSides[0][2] = {  1,  1, -1 };
		cubeSides[0][3] = {  1, -1, -1 };

		cubeSides[1][0] = { -1, -1, 1 };
		cubeSides[1][1] = { -1,  1, 1 };
		cubeSides[1][2] = { 1,  1,  1 };
		cubeSides[1][3] = { 1, -1,  1 };

		//------------

		cubeSides[2][0] = { -1, -1, -1 };
		cubeSides[2][1] = { -1, -1,  1 };
		cubeSides[2][2] = {  1, -1,  1 };
		cubeSides[2][3] = {  1, -1, -1 };

		cubeSides[3][0] = { -1,  1, -1 };
		cubeSides[3][1] = { -1,  1,  1 };
		cubeSides[3][2] = {  1,  1,  1 };
		cubeSides[3][3] = {  1,  1, -1 };

		//------------
		cubeSides[4][0] = { -1, -1, -1 };
		cubeSides[4][1] = { -1, -1,  1 };
		cubeSides[4][2] = { -1,  1,  1 };
		cubeSides[4][3] = { -1,  1, -1 };

		cubeSides[5][0] = {  1, -1, -1 };
		cubeSides[5][1] = {  1, -1,  1 };
		cubeSides[5][2] = {  1,  1,  1 };
		cubeSides[5][3] = {  1,  1, -1 };

		//-----------

		cubeColors[0] = Vector3f{ 1, 0, 0 };
		cubeColors[1] = Vector3f{ 0, 1, 0 };
		cubeColors[2] = Vector3f{ 0, 0, 1 };
		cubeColors[3] = Vector3f{ 1, 1, 0 };
		cubeColors[4] = Vector3f{ 0, 1, 1 };
		cubeColors[5] = Vector3f{ 1, 0, 1 };

		//-----------

		VertexDataStruct result;

		for (int i = 0; i < 6; i++)
		{
			result.PositionData.push_back(cubeSides[i][0] * scale);
			result.PositionData.push_back(cubeSides[i][1] * scale);
			result.PositionData.push_back(cubeSides[i][2] * scale);
			result.PositionData.push_back(cubeSides[i][2] * scale);
			result.PositionData.push_back(cubeSides[i][3] * scale);
			result.PositionData.push_back(cubeSides[i][0] * scale);

			result.ColorData.push_back(cubeColors[i]);
			result.ColorData.push_back(cubeColors[i]);
			result.ColorData.push_back(cubeColors[i]);
			result.ColorData.push_back(cubeColors[i]);
			result.ColorData.push_back(cubeColors[i]);
			result.ColorData.push_back(cubeColors[i]);
		}

		return result;
	}

	void VertexRenderStruct::RefreshVBO()
	{
		//Check if main thread, check if data is not empty...

#ifndef EMSCRIPTEN
		if (!vao)
		{
			vao = std::make_shared<VAOHolder>();
		}

		glBindVertexArray(vao->getBuffer());
#endif
		if (!positionVBO)
		{
			positionVBO = std::make_shared<VBOHolder>();
		}

		glBindBuffer(GL_ARRAY_BUFFER, positionVBO->getBuffer());

		glBufferData(GL_ARRAY_BUFFER, data.PositionData.size() * 12, &data.PositionData[0], GL_STATIC_DRAW);

		if (data.TexCoordData.size() > 0)
		{
			if (!texCoordVBO)
			{
				texCoordVBO = std::make_shared<VBOHolder>();
			}

			glBindBuffer(GL_ARRAY_BUFFER, texCoordVBO->getBuffer());

			glBufferData(GL_ARRAY_BUFFER, data.TexCoordData.size() * 8, &data.TexCoordData[0], GL_STATIC_DRAW);
		}


		if (data.NormalData.size() > 0)
		{
			if (!normalVBO)
			{
				normalVBO = std::make_shared<VBOHolder>();
			}

			glBindBuffer(GL_ARRAY_BUFFER, normalVBO->getBuffer());

			glBufferData(GL_ARRAY_BUFFER, data.NormalData.size() * 12, &data.NormalData[0], GL_STATIC_DRAW);
		}

		if (data.TangentData.size() > 0)
		{
			if (!tangentVBO)
			{
				tangentVBO = std::make_shared<VBOHolder>();
			}

			glBindBuffer(GL_ARRAY_BUFFER, tangentVBO->getBuffer());

			glBufferData(GL_ARRAY_BUFFER, data.TangentData.size() * 12, &data.TangentData[0], GL_STATIC_DRAW);
		}

		if (data.BinormalData.size() > 0)
		{
			if (!binormalVBO)
			{
				binormalVBO = std::make_shared<VBOHolder>();
			}

			glBindBuffer(GL_ARRAY_BUFFER, binormalVBO->getBuffer());

			glBufferData(GL_ARRAY_BUFFER, data.BinormalData.size() * 12, &data.BinormalData[0], GL_STATIC_DRAW);
		}

		if (data.ColorData.size() > 0)
		{
			if (!colorVBO)
			{
				colorVBO = std::make_shared<VBOHolder>();
			}

			glBindBuffer(GL_ARRAY_BUFFER, colorVBO->getBuffer());

			glBufferData(GL_ARRAY_BUFFER, data.ColorData.size() * 12, &data.ColorData[0], GL_STATIC_DRAW);
		}
	}

	void VertexDataStruct::Scale(float scale)
	{
		for (int i = 0; i < PositionData.size(); i++)
		{
			PositionData[i] = PositionData[i] * scale;
		}
	}

	void VertexDataStruct::SwapZandY()
	{
		for (int i = 0; i < PositionData.size(); i++)
		{
			auto value = PositionData[i].v[1];
			PositionData[i].v[1] = PositionData[i].v[2];
			PositionData[i].v[2] = value;
		}
	}

	void VertexDataStruct::RotateByMatrix(Matrix3f m)
	{

		for (int i = 0; i < PositionData.size(); i++)
		{
			PositionData[i] = MultVectorMatrix(PositionData[i], m);
		}

		for (int i = 0; i < NormalData.size(); i++)
		{
			NormalData[i] = MultVectorMatrix(NormalData[i], m);
		}

		for (int i = 0; i < TangentData.size(); i++)
		{
			TangentData[i] = MultVectorMatrix(TangentData[i], m);
		}

		for (int i = 0; i < BinormalData.size(); i++)
		{
			BinormalData[i] = MultVectorMatrix(BinormalData[i], m);
		}
	}

	void VertexRenderStruct::AssignFrom(const VertexDataStruct& v)
	{
		data = v;
		RefreshVBO();
	}

	void Renderer::InitOpenGL()
	{
		ModelviewMatrixStack.push(Matrix4f::Identity());
		ProjectionMatrixStack.push(Matrix4f::Identity());

		glEnable(GL_DEPTH_TEST);
	    glEnable(GL_BLEND);

		glActiveTexture(GL_TEXTURE0);

#ifndef EMSCRIPTEN
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
#endif
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDepthFunc(GL_LEQUAL);

		CheckGlError();

	}

	void Renderer::PushProjectionMatrix(float width, float height, float zNear, float zFar)
	{
		Matrix4f m = MakeOrthoMatrix(width, height, zNear, zFar);
		ProjectionMatrixStack.push(m);
		SetMatrix();

		if (ProjectionMatrixStack.size() > CONST_MATRIX_STACK_SIZE)
		{
			throw std::runtime_error("Projection matrix stack overflow!!!!");
		}
	}
	
	void Renderer::PushPerspectiveProjectionMatrix(float fovY, float aspectRatio, float zNear, float zFar)
	{
		Matrix4f m = MakePerspectiveMatrix(fovY, aspectRatio, zNear, zFar);
		ProjectionMatrixStack.push(m);
		SetMatrix();

		if (ProjectionMatrixStack.size() > CONST_MATRIX_STACK_SIZE)
		{
			throw std::runtime_error("Projection matrix stack overflow!!!!");
		}
	}


	void Renderer::PopProjectionMatrix()
	{
		if (ProjectionMatrixStack.size() == 0)
		{
			throw std::runtime_error("Projection matrix stack underflow!!!!");
		}
		ProjectionMatrixStack.pop();
		SetMatrix();
	}

	Matrix4f Renderer::GetProjectionModelViewMatrix()
	{
		return ProjectionModelViewMatrix;
	}

	void Renderer::SetMatrix()
	{
		if (ProjectionMatrixStack.size() <= 0)
		{
			throw std::runtime_error("Projection matrix stack out!");
		}

		if (ModelviewMatrixStack.size() <= 0)
		{
			throw std::runtime_error("Modelview matrix stack out!");
		}

		ProjectionModelViewMatrix = ProjectionMatrixStack.top() * ModelviewMatrixStack.top();

		static const std::string ProjectionModelViewMatrixName = "ProjectionModelViewMatrix";
	
		//static const std::string ProjectionMatrixName = "ProjectionMatrix";

		RenderUniformMatrix4fv(ProjectionModelViewMatrixName, false, &ProjectionModelViewMatrix.m[0]);

		//RenderUniformMatrix4fv(ProjectionMatrixName, false, &ProjectionMatrixStack.top().m[0]);

	}

	void Renderer::PushMatrix()
	{
		if (ModelviewMatrixStack.size() == 0)
		{
			throw std::runtime_error("Modelview matrix stack underflow!!!!");
		}

		ModelviewMatrixStack.push(ModelviewMatrixStack.top());

		if (ModelviewMatrixStack.size() > CONST_MATRIX_STACK_SIZE)
		{
			throw std::runtime_error("Modelview matrix stack overflow!!!!");
		}
	}

	void Renderer::LoadIdentity()
	{
		if (ModelviewMatrixStack.size() == 0)
		{
			throw std::runtime_error("Modelview matrix stack underflow!!!!");
		}

		ModelviewMatrixStack.pop();
		ModelviewMatrixStack.push(Matrix4f::Identity());

		SetMatrix();
	}

	void Renderer::TranslateMatrix(const Vector3f& p)
	{

		Matrix4f m = Matrix4f::Identity();
		m.m[12] = p.v[0];
		m.m[13] = p.v[1];
		m.m[14] = p.v[2];

		m = ModelviewMatrixStack.top() * m;

		if (ModelviewMatrixStack.size() == 0)
		{
			throw std::runtime_error("Modelview matrix stack underflow!!!!");
		}

		ModelviewMatrixStack.pop();
		ModelviewMatrixStack.push(m);

		SetMatrix();
	}

	void Renderer::ScaleMatrix(float scale)
	{
		Matrix4f m = Matrix4f::Identity();
		m.m[0] = scale;
		m.m[5] = scale;
		m.m[10] = scale;

		m = ModelviewMatrixStack.top() * m;

		if (ModelviewMatrixStack.size() == 0)
		{
			throw std::runtime_error("Modelview matrix stack underflow!!!!");
		}

		ModelviewMatrixStack.pop();
		ModelviewMatrixStack.push(m);

		SetMatrix();
	}

	void Renderer::ScaleMatrix(const Vector3f& scale)
	{
		Matrix4f m = Matrix4f::Identity();
		m.m[0] = scale.v[0];
		m.m[5] = scale.v[1];
		m.m[10] = scale.v[2];

		m = ModelviewMatrixStack.top() * m;

		if (ModelviewMatrixStack.size() == 0)
		{
			throw std::runtime_error("Modelview matrix stack underflow!!!!");
		}

		ModelviewMatrixStack.pop();
		ModelviewMatrixStack.push(m);

		SetMatrix();
	}

	void Renderer::RotateMatrix(const Vector4f& q)
	{
		
		Matrix3f m3 = QuatToMatrix(q);
		Matrix4f m = Matrix4f::Identity();
		m.m[0] = m3.m[0];
		m.m[1] = m3.m[1];
		m.m[2] = m3.m[2];

		m.m[4] = m3.m[3];
		m.m[5] = m3.m[4];
		m.m[6] = m3.m[5];

		m.m[8] = m3.m[6];
		m.m[9] = m3.m[7];
		m.m[10] = m3.m[8];

		m = ModelviewMatrixStack.top() * m;

		if (ModelviewMatrixStack.size() == 0)
		{
			throw std::runtime_error("Modelview matrix stack underflow!!!!");
		}

		ModelviewMatrixStack.pop();
		ModelviewMatrixStack.push(m);
		

		SetMatrix();
	}

	void Renderer::PushSpecialMatrix(const Matrix4f& m)
	{
		if (ModelviewMatrixStack.size() > 64)
		{
			throw std::runtime_error("Modelview matrix stack overflow!!!!");
		}
		ModelviewMatrixStack.push(m);
		SetMatrix();
	}


	void Renderer::PopMatrix()
	{
		if (ModelviewMatrixStack.size() == 0)
		{
			throw std::runtime_error("Modelview matrix stack underflow!!!!");
		}
		ModelviewMatrixStack.pop();

		SetMatrix();
	}


	void Renderer::EnableVertexAttribArray(const std::string& attribName)
	{

		auto shader = shaderManager.GetCurrentShader();
		if (shader->attribList.find(attribName) != shader->attribList.end())
			glEnableVertexAttribArray(shader->attribList[attribName]);
	}

	void Renderer::DisableVertexAttribArray(const std::string& attribName)
	{
		auto shader = shaderManager.GetCurrentShader();
		if (shader->attribList.find(attribName) != shader->attribList.end())
			glDisableVertexAttribArray(shader->attribList[attribName]);
	}


	void Renderer::RenderUniformMatrix4fv(const std::string& uniformName, bool transpose, const float* value)
	{
		auto shader = shaderManager.GetCurrentShader();
		
		auto uniform = shader->uniformList.find(uniformName);

		if (uniform != shader->uniformList.end())
		{
			glUniformMatrix4fv(uniform->second, 1, transpose, value);
		}
	}

	void Renderer::RenderUniform1i(const std::string& uniformName, const int value)
	{
		auto shader = shaderManager.GetCurrentShader();

		auto uniform = shader->uniformList.find(uniformName);

		if (uniform != shader->uniformList.end())
		{
			glUniform1i(uniform->second, value);
		}

	}



	void Renderer::VertexAttribPointer2fv(const std::string& attribName, int stride, const char* pointer)
	{
		auto shader = shaderManager.GetCurrentShader();
		if (shader->attribList.find(attribName) != shader->attribList.end())
			glVertexAttribPointer(shader->attribList[attribName], 2, GL_FLOAT, GL_FALSE, stride, pointer);
		
	}

	void Renderer::VertexAttribPointer3fv(const std::string& attribName, int stride, const char* pointer)
	{

		auto shader = shaderManager.GetCurrentShader();
		if (shader->attribList.find(attribName) != shader->attribList.end())
			glVertexAttribPointer(shader->attribList[attribName], 3, GL_FLOAT, GL_FALSE, stride, pointer);
	}

	void Renderer::DrawVertexRenderStruct(const VertexRenderStruct& VertexRenderStruct)
	{
		static const std::string vNormal("vNormal");
		static const std::string vTangent("vTangent");
		static const std::string vBinormal("vBinormal");
		static const std::string vColor("vColor");
		static const std::string vTexCoord("vTexCoord");
		static const std::string vPosition("vPosition");
		
		//glBindVertexArray(VertexRenderStruct.vao->getBuffer());

		//Check if main thread, check if data is not empty...
		if (VertexRenderStruct.data.NormalData.size() > 0)
		{
			glBindBuffer(GL_ARRAY_BUFFER, VertexRenderStruct.normalVBO->getBuffer());
			VertexAttribPointer3fv(vNormal, 0, NULL);
		}
		if (VertexRenderStruct.data.TangentData.size() > 0)
		{
			glBindBuffer(GL_ARRAY_BUFFER, VertexRenderStruct.tangentVBO->getBuffer());
			VertexAttribPointer3fv(vTangent, 0, NULL);
		}
		if (VertexRenderStruct.data.BinormalData.size() > 0)
		{
			glBindBuffer(GL_ARRAY_BUFFER, VertexRenderStruct.binormalVBO->getBuffer());
			VertexAttribPointer3fv(vBinormal, 0, NULL);
		}
		if (VertexRenderStruct.data.ColorData.size() > 0)
		{
			glBindBuffer(GL_ARRAY_BUFFER, VertexRenderStruct.colorVBO->getBuffer());
			VertexAttribPointer3fv(vColor, 0, NULL);
		}
		if (VertexRenderStruct.data.TexCoordData.size() > 0)
		{
			glBindBuffer(GL_ARRAY_BUFFER, VertexRenderStruct.texCoordVBO->getBuffer());
			VertexAttribPointer2fv(vTexCoord, 0, NULL);
		}

		glBindBuffer(GL_ARRAY_BUFFER, VertexRenderStruct.positionVBO->getBuffer());
		VertexAttribPointer3fv(vPosition, 0, NULL);

		glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(VertexRenderStruct.data.PositionData.size()));

	}

}

