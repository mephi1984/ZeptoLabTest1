#pragma once

#include "OpenGlExtensions.h"
#include "Math.h"
#include <exception>
#include <stdexcept>
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

	class VAOHolder {
		GLuint vao;

	public:
		VAOHolder();

		VAOHolder(const VAOHolder& v) = delete;

		VAOHolder& operator=(const VAOHolder& v) = delete;

		~VAOHolder();

		GLuint getBuffer();
	};

	struct VertexDataStruct
	{
		std::vector<Vector3f> PositionData;
		std::vector<Vector2f> TexCoordData;
		std::vector<Vector3f> NormalData;
		std::vector<Vector3f> TangentData;
		std::vector<Vector3f> BinormalData;
		std::vector<Vector3f> ColorData;

		void RotateByMatrix(Matrix3f m);

		void Scale(float scale);
		void Move(Vector3f diff);
		void SwapZandY();
	};

	struct VertexRenderStruct
	{
		VertexDataStruct data;

		std::shared_ptr<VAOHolder> vao;
		std::shared_ptr<VBOHolder> positionVBO;
		std::shared_ptr<VBOHolder> texCoordVBO;
		std::shared_ptr<VBOHolder> normalVBO;
		std::shared_ptr<VBOHolder> tangentVBO;
		std::shared_ptr<VBOHolder> binormalVBO;
		std::shared_ptr<VBOHolder> colorVBO;
		void RefreshVBO();

		void AssignFrom(const VertexDataStruct& v);
	};

	VertexDataStruct CreateRect2D(Vector2f center, Vector2f halfWidthHeight, float zLevel);
	VertexDataStruct CreateRectHorizontalSections2D(Vector2f center, Vector2f halfWidthHeight, float zLevel, size_t sectionCount);
	VertexDataStruct CreateCube3D(float scale);


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
		void PushPerspectiveProjectionMatrix(float fovY, float aspectRatio, float zNear, float zFar);
		void PopProjectionMatrix();

		void PushMatrix();
		void LoadIdentity();
		void TranslateMatrix(const Vector3f& p);
		void ScaleMatrix(float scale);
		void ScaleMatrix(const Vector3f& scale);
		void RotateMatrix(const Vector4f& q);
		void PushSpecialMatrix(const Matrix4f& m);
		void PopMatrix();


		Matrix4f GetProjectionModelViewMatrix();

		void SetMatrix();



		void EnableVertexAttribArray(const std::string& attribName);

		void DisableVertexAttribArray(const std::string& attribName);


		void RenderUniformMatrix4fv(const std::string& uniformName, bool transpose, const float* value);
		void RenderUniform1i(const std::string& uniformName, const int value);
		void RenderUniform3fv(const std::string& uniformName, const float* value);


		void VertexAttribPointer2fv(const std::string& attribName, int stride, const char* pointer);

		void VertexAttribPointer3fv(const std::string& attribName, int stride, const char* pointer);

		void DrawVertexRenderStruct(const VertexRenderStruct& VertexRenderStruct);
	};

	

};