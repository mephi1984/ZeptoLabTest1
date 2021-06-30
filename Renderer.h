#pragma once

#include "OpenGlExtensions.h"

#include "Math.h"

#include "ShaderManager.h"

namespace ZL {

	constexpr size_t CONST_MATRIX_STACK_SIZE = 64;

	class VBOHolder {
		GLuint Buffer;

	public:
		VBOHolder();

		VBOHolder(const VBOHolder& v) = delete;

		VBOHolder& operator=(const VBOHolder& v) = delete;

		~VBOHolder();

		GLuint getBuffer();
	};

	struct VertexDataStruct
	{
		std::vector<Vector3f> PositionData;
		std::vector<Vector2f> TexCoordData;

		std::shared_ptr<VBOHolder> positionVBO;
		std::shared_ptr<VBOHolder> texCoordVBO;

		void RefreshVBO();
	};

	VertexDataStruct CreateRect2D(Vector2f center, Vector2f halfWidthHeight, float zLevel);
	VertexDataStruct CreateRectHorizontalSections2D(Vector2f center, Vector2f halfWidthHeight, float zLevel, size_t sectionCount);


	class Renderer
	{
	protected:
		std::stack<Matrix4f> ProjectionMatrixStack;
		std::stack<Matrix4f> ModelviewMatrixStack;

		Matrix4f ProjectionModelViewMatrix;

	public:

		ShaderManager shaderManager;

		void InitOpenGL();

		void PushProjectionMatrix(float width, float height, float zNear = 0.f, float zFar = 1.f);
		void PopProjectionMatrix();

		void PushMatrix();
		void LoadIdentity();
		void TranslateMatrix(const Vector3f& p);
		void ScaleMatrix(float scale);
		void ScaleMatrix(const Vector3f& scale);
		void RotateMatrix(const Vector4f& q);
		void PushSpecialMatrix(const Matrix4f& m);
		void PopMatrix();


		void SetMatrix();



		void EnableVertexAttribArray(const std::string& attribName);

		void DisableVertexAttribArray(const std::string& attribName);


		void RenderUniformMatrix4fv(const std::string& uniformName, bool transpose, const float* value);
		void RenderUniform1i(const std::string& uniformName, const int value);

		void VertexAttribPointer2fv(const std::string& attribName, int stride, const char* pointer);

		void VertexAttribPointer3fv(const std::string& attribName, int stride, const char* pointer);

		void DrawVertexDataStruct(const VertexDataStruct& vertexDataStruct);
	};

	

};