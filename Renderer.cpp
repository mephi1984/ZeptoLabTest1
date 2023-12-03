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
		glGenVertexArrays(1, &vao);
	}

	VAOHolder::~VAOHolder()
	{
		glDeleteVertexArray(1, &vao);
		//glDeleteBuffers(1, &Buffer);
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

		result.RefreshVBO();

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


		result.RefreshVBO();

		return result;

	}

	void VertexDataStruct::RefreshVBO()
	{
		//Check if main thread, check if data is not empty...

		if (!vao)
		{
			vao = std::make_shared<VAOHolder>();
		}

		glBindVertexArray(vao->getBuffer());
		
		if (!positionVBO)
		{
			positionVBO = std::make_shared<VBOHolder>();
		}

		glBindBuffer(GL_ARRAY_BUFFER, positionVBO->getBuffer());

		glBufferData(GL_ARRAY_BUFFER, PositionData.size() * 12, &PositionData[0], GL_STATIC_DRAW);

		if (!texCoordVBO)
		{
			texCoordVBO = std::make_shared<VBOHolder>();
		}

		glBindBuffer(GL_ARRAY_BUFFER, texCoordVBO->getBuffer());

		glBufferData(GL_ARRAY_BUFFER, TexCoordData.size() * 8, &TexCoordData[0], GL_STATIC_DRAW);

	}

	void Renderer::InitOpenGL()
	{
		ModelviewMatrixStack.push(Matrix4f::Identity());
		ProjectionMatrixStack.push(Matrix4f::Identity());

		glEnable(GL_DEPTH_TEST);
	    glEnable(GL_BLEND);

		glActiveTexture(GL_TEXTURE0);

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

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
			throw std::exception("Projection matrix stack overflow!!!!");
		}
	}


	void Renderer::PopProjectionMatrix()
	{
		if (ProjectionMatrixStack.size() == 0)
		{
			throw std::exception("Projection matrix stack underflow!!!!");
		}
		ProjectionMatrixStack.pop();
		SetMatrix();
	}


	void Renderer::SetMatrix()
	{
		if (ProjectionMatrixStack.size() <= 0)
		{
			throw std::exception("Projection matrix stack out!");
		}

		if (ModelviewMatrixStack.size() <= 0)
		{
			throw std::exception("Modelview matrix stack out!");
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
			throw std::exception("Modelview matrix stack underflow!!!!");
		}

		ModelviewMatrixStack.push(ModelviewMatrixStack.top());

		if (ModelviewMatrixStack.size() > CONST_MATRIX_STACK_SIZE)
		{
			throw std::exception("Modelview matrix stack overflow!!!!");
		}
	}

	void Renderer::LoadIdentity()
	{
		if (ModelviewMatrixStack.size() == 0)
		{
			throw std::exception("Modelview matrix stack underflow!!!!");
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
			throw std::exception("Modelview matrix stack underflow!!!!");
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
			throw std::exception("Modelview matrix stack underflow!!!!");
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
			throw std::exception("Modelview matrix stack underflow!!!!");
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
			throw std::exception("Modelview matrix stack underflow!!!!");
		}

		ModelviewMatrixStack.pop();
		ModelviewMatrixStack.push(m);
		

		SetMatrix();
	}

	void Renderer::PushSpecialMatrix(const Matrix4f& m)
	{
		if (ModelviewMatrixStack.size() > 64)
		{
			throw std::exception("Modelview matrix stack overflow!!!!");
		}
		ModelviewMatrixStack.push(m);
		SetMatrix();
	}


	void Renderer::PopMatrix()
	{
		if (ModelviewMatrixStack.size() == 0)
		{
			throw std::exception("Modelview matrix stack underflow!!!!");
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

	void Renderer::DrawVertexDataStruct(const VertexDataStruct& vertexDataStruct)
	{
		static const std::string vTexCoord("vTexCoord");
		static const std::string vPosition("vPosition");

		//Check if main thread, check if data is not empty...
		glBindBuffer(GL_ARRAY_BUFFER, vertexDataStruct.texCoordVBO->getBuffer());
		VertexAttribPointer2fv(vTexCoord, 0, NULL);

		glBindBuffer(GL_ARRAY_BUFFER, vertexDataStruct.positionVBO->getBuffer());
		VertexAttribPointer3fv(vPosition, 0, NULL);

		glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(vertexDataStruct.PositionData.size()));

	}

}

