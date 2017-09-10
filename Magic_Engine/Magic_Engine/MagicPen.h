#pragma once

#include "glm.hpp"

#include <gtc/matrix_transform.hpp>
#include <gtx/transform2.hpp>
#include "MagicShader.h"
#include "MagicTexture.h"
#include "MagicVertex.h"

#include "vector"


namespace Magic
{
	class Pen_Common
	{
	public:
		virtual void RenderStart() = 0;
		virtual void RenderEnd() = 0;
	};

	class Pen_Basis :public Pen_Common
	{
		struct PICTRUE_VERTEX
		{
			glm::vec3 Position;
			glm::vec2 UV;
			Magic::Color4 Color;
		};

		struct PICTRUE_MESSAGE
		{
			unsigned int DrawNumber;
			bool AlphaState;
			MagicTexture* pTexture;
			PICTRUE_MESSAGE();
			PICTRUE_MESSAGE(const unsigned int& _drawnumber);
		};

		struct PICTRUE_DRAW
		{
			unsigned int DrawNumber;
			Magic::Color4 NowColor;
			glm::vec2 NowUV[4];
			std::vector<PICTRUE_VERTEX> V_Vertex;
			std::vector<PICTRUE_MESSAGE> V_Message;
			PICTRUE_DRAW();
			void Clear();
		};
		struct DRAW_BOX
		{
			glm::mat4 CameraMatrix;
			glm::mat4 orthoMatrix;

			PICTRUE_DRAW Pictrue_Draw;
			void Clear();
		};
	public:
		enum DRAW_TYPE {
			PICTRUE,
			LINE
		};

		Pen_Basis();
		~Pen_Basis();

		bool Initialize();

		void DrawPicture(float _x, float _y, float _w, float _h);

		void BindTextrue(DRAW_TYPE _type, MagicTexture* _textrue);

		void EnableAlpha(DRAW_TYPE _type);

		void DisableAlpha(DRAW_TYPE _type);

		void SetStartPosSize(int _x, int _y, int _width, int _height);

		void SetColor(DRAW_TYPE _type, const Magic::Color4& Color4);

		void AddSpecialEffects(MagicShader* _shader, const std::string& _Text);

		MagicShader* operator[](const std::string& _Text);

		virtual void RenderStart();
		virtual void RenderEnd();

	private:
		void AddPictrueMessage();
	private:
		unsigned short m_NowDepth;

		unsigned int m_Draw_Box_Number;
		std::vector<DRAW_BOX> V_Draw_Box;

		MagicShader m_Picture2D;


		MagicShader m_LightShader;
		MagicShader m_DepthShader;
		std::map<std::string, MagicShader*> Map_SpecialEffectsPanel;
	};

}

