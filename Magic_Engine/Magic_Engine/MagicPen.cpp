#include "MagicPen.h"
#pragma comment(lib,"glew32.lib")
#include <GL/glew.h>  

namespace Magic
{
	Pen_Basis::PICTRUE_MESSAGE::PICTRUE_MESSAGE()
	{
		DrawNumber = 0;
		AlphaState = 0;
		pTexture = 0;
	}

	Pen_Basis::PICTRUE_MESSAGE::PICTRUE_MESSAGE(const unsigned int& _drawnumber)
	{
		DrawNumber = _drawnumber;
	}

	Pen_Basis::PICTRUE_DRAW::PICTRUE_DRAW()
	{
		DrawNumber = 0;
	}

	void Pen_Basis::PICTRUE_DRAW::Clear()
	{
		DrawNumber = 0;
		V_Vertex.clear();
		V_Message.clear();
	}

	void Pen_Basis::DRAW_BOX::Clear()
	{
		Pictrue_Draw.Clear();
	}

	Pen_Basis::Pen_Basis()
	{
		m_Draw_Box_Number = -1;
		m_NowDepth = 0xFFFF;
	}


	Pen_Basis::~Pen_Basis()
	{
	}

	bool Pen_Basis::Initialize()
	{
		bool result;

		//-----------------------------------------------------------------

		result = m_Picture2D.LoadFromFile(GL_VERTEX_SHADER, "shader/Picture2D.vertshader");
		if (!result)
			return false;
		result = m_Picture2D.LoadFromFile(GL_FRAGMENT_SHADER, "shader/Picture2D.fragshader");
		if (!result)
			return false;
		result = m_Picture2D.CreateAndLinkProgram();
		if (!result)
			return false;

		m_Picture2D.Use();

		m_Picture2D.AddUniform("CameraMatrix");
		m_Picture2D.AddUniform("projectionMatrix");

		m_Picture2D.AddUniform("sampler0");
		glUniform1i((m_Picture2D)("sampler0"), 0);

		m_Picture2D.UnUse();

		AddSpecialEffects(&m_Picture2D, "Picture2D");

		//-------------------------------------------------------------------------
		result = m_LightShader.LoadFromFile(GL_VERTEX_SHADER, "shader/Light.vertshader");
		if (!result)
			return false;
		result = m_LightShader.LoadFromFile(GL_FRAGMENT_SHADER, "shader/Light.fragshader");
		if (!result)
			return false;
		result = m_LightShader.CreateAndLinkProgram();
		if (!result)
			return false;

		m_LightShader.Use();

		m_LightShader.AddUniform("worldMatrix");
		m_LightShader.AddUniform("CameraMatrix");
		m_LightShader.AddUniform("projectionMatrix");
		m_LightShader.AddUniform("LightprojectionMatrix");
		m_LightShader.AddUniform("LightMatrix");

		m_LightShader.AddUniform("sampler0");
		m_LightShader.AddUniform("shadowMap");
		m_LightShader.AddUniform("AmbientColor");

		glUniform1i((m_LightShader)("sampler0"), 0);
		glUniform1i((m_LightShader)("shadowMap"), 1);

		m_LightShader.UnUse();

		AddSpecialEffects(&m_LightShader, "Light");

		//-------------------------------------------------------------------------
		result = m_DepthShader.LoadFromFile(GL_VERTEX_SHADER, "shader/Depth.vertshader");
		if (!result)
			return false;
		result = m_DepthShader.LoadFromFile(GL_FRAGMENT_SHADER, "shader/Depth.fragshader");
		if (!result)
			return false;
		result = m_DepthShader.CreateAndLinkProgram();
		if (!result)
			return false;

		m_DepthShader.Use();

		m_DepthShader.AddUniform("worldMatrix");
		m_DepthShader.AddUniform("CameraMatrix");
		m_DepthShader.AddUniform("projectionMatrix");

		m_DepthShader.UnUse();

		AddSpecialEffects(&m_DepthShader, "Depth");

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glDisable(GL_BLEND);

		glEnableVertexAttribArray(0);

		return true;
	}

	void Pen_Basis::DrawPicture(float _x, float _y, float _w, float _h)
	{
		PICTRUE_DRAW* _pPICTRUE_DRAW = &V_Draw_Box[m_Draw_Box_Number].Pictrue_Draw;
		std::vector<PICTRUE_VERTEX>* _pVertex = &V_Draw_Box[m_Draw_Box_Number].Pictrue_Draw.V_Vertex;

		PICTRUE_VERTEX _Vertex;
		_Vertex.Color = _pPICTRUE_DRAW->NowColor;
		_Vertex.Position.z = -m_NowDepth;

		_Vertex.Position.x = _x;
		_Vertex.Position.y = _y;
		_Vertex.UV.x = _pPICTRUE_DRAW->NowUV[0].x;
		_Vertex.UV.y = _pPICTRUE_DRAW->NowUV[0].y;
		_pVertex->push_back(_Vertex);

		_Vertex.Position.x = _x + _w;
		_Vertex.Position.y = _y;
		_Vertex.UV.x = _pPICTRUE_DRAW->NowUV[1].x;
		_Vertex.UV.y = _pPICTRUE_DRAW->NowUV[1].y;
		_pVertex->push_back(_Vertex);

		_Vertex.Position.x = _x + _w;
		_Vertex.Position.y = _y + _h;
		_Vertex.UV.x = _pPICTRUE_DRAW->NowUV[2].x;
		_Vertex.UV.y = _pPICTRUE_DRAW->NowUV[2].y;
		_pVertex->push_back(_Vertex);

		_Vertex.Position.x = _x;
		_Vertex.Position.y = _y + _h;
		_Vertex.UV.x = _pPICTRUE_DRAW->NowUV[3].x;
		_Vertex.UV.y = _pPICTRUE_DRAW->NowUV[3].y;
		_pVertex->push_back(_Vertex);
		m_NowDepth--;
	}

	void Pen_Basis::BindTextrue(DRAW_TYPE _type, MagicTexture* _textrue)
	{
		switch (_type)
		{
		case Magic::Pen_Basis::PICTRUE:
			AddPictrueMessage();
			V_Draw_Box[m_Draw_Box_Number].Pictrue_Draw.V_Message.back().pTexture = _textrue;
			break;
		case Magic::Pen_Basis::LINE:
			break;
		}
	}

	void Pen_Basis::EnableAlpha(DRAW_TYPE _type)
	{
		switch (_type)
		{
		case Magic::Pen_Basis::PICTRUE:
			AddPictrueMessage();
			V_Draw_Box[m_Draw_Box_Number].Pictrue_Draw.V_Message.back().AlphaState = true;
			break;
		case Magic::Pen_Basis::LINE:
			break;
		}
	}

	void Pen_Basis::DisableAlpha(DRAW_TYPE _type)
	{
		switch (_type)
		{
		case Magic::Pen_Basis::PICTRUE:
			AddPictrueMessage();
			V_Draw_Box[m_Draw_Box_Number].Pictrue_Draw.V_Message.back().AlphaState = false;
			break;
		case Magic::Pen_Basis::LINE:
			break;
		}
	}


	void Pen_Basis::SetStartPosSize(int _x, int _y, int _width, int _height)
	{
		DRAW_BOX* _pDRAW_BOX = &V_Draw_Box[m_Draw_Box_Number];

		_pDRAW_BOX->CameraMatrix[3].x = _x;
		_pDRAW_BOX->CameraMatrix[3].y = _y;
		//左下角x坐标，右上角x坐标，左下角y坐标，右上角y坐标 坐标全相对于窗口左下角 原点
		_pDRAW_BOX->orthoMatrix = glm::ortho(
			0.0f, (float)_width,
			0.0f, (float)_height, 1.0f, (float)0xFFFF);
		//projectionMatrix = glm::perspective(-70.0f, -(float)_width / (float)_height, 0.1f, 1000000.0f);
	}


	void Pen_Basis::SetColor(DRAW_TYPE _type, const Magic::Color4& Color4)
	{
		switch (_type)
		{
		case Magic::Pen_Basis::PICTRUE:
			V_Draw_Box[m_Draw_Box_Number].Pictrue_Draw.NowColor = Color4;
			break;
		case Magic::Pen_Basis::LINE:
			break;
		}
	}

	void Pen_Basis::AddSpecialEffects(MagicShader* _shader, const std::string& _Text)
	{
		Map_SpecialEffectsPanel[_Text] = _shader;
	}

	MagicShader* Pen_Basis::operator[](const std::string& _Text)
	{
		return Map_SpecialEffectsPanel[_Text];
	}

	void Pen_Basis::RenderStart()
	{
		V_Draw_Box.push_back(DRAW_BOX());
		m_Draw_Box_Number++;
	}

	void Pen_Basis::RenderEnd()
	{
		if (V_Draw_Box[m_Draw_Box_Number].Pictrue_Draw.DrawNumber)
		{
			glClear(GL_DEPTH_BUFFER_BIT);
			glEnable(GL_DEPTH_TEST);
			//GL_LEQUAL小于等于 ，GL_LESS小鱼
			glDepthFunc(GL_LEQUAL);

			m_Picture2D.Use();

			DRAW_BOX* _pDRAW_BOX = &V_Draw_Box[m_Draw_Box_Number];
			auto _Message = &_pDRAW_BOX->Pictrue_Draw.V_Message;
			glUniformMatrix4fv((m_Picture2D)("CameraMatrix"), 1, GL_FALSE, &_pDRAW_BOX->CameraMatrix[0][0]);
			glUniformMatrix4fv((m_Picture2D)("projectionMatrix"), 1, GL_FALSE, &_pDRAW_BOX->orthoMatrix[0][0]);

			glDisable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glEnable(GL_TEXTURE_2D);
			glActiveTexture(GL_TEXTURE0);

			glBindVertexArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			glBufferSubData(GL_ARRAY_BUFFER, 0, _pDRAW_BOX->Pictrue_Draw.V_Vertex.size() * sizeof(PICTRUE_VERTEX), &_pDRAW_BOX->Pictrue_Draw.V_Vertex[0]);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (GLvoid*)(sizeof(glm::vec3)));
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Magic::Color4), (GLvoid*)(sizeof(glm::vec3) + sizeof(glm::vec2)));

			PICTRUE_MESSAGE _Message_Buffer;
			for (auto _auto : *_Message)
			{
				if (_auto.AlphaState != _Message_Buffer.AlphaState)
				{
					_Message_Buffer.AlphaState = _auto.AlphaState;
					if (_Message_Buffer.AlphaState)
						glEnable(GL_BLEND);
					else
						glDisable(GL_BLEND);
				}
				if (_auto.pTexture != _Message_Buffer.pTexture)
				{
					_Message_Buffer.pTexture = _auto.pTexture;
					glBindTexture(GL_TEXTURE_2D, _Message_Buffer.pTexture->GetTextrue());
				}

				unsigned int _DrawNumber = _auto.DrawNumber - _Message_Buffer.DrawNumber;
				if (_DrawNumber)
				{
					glDrawArrays(GL_QUADS, _Message_Buffer.DrawNumber * 4, _DrawNumber * 4);
					_Message_Buffer.DrawNumber = _auto.DrawNumber;
				}
			}

			unsigned int _DrawNumber = _pDRAW_BOX->Pictrue_Draw.DrawNumber - _Message_Buffer.DrawNumber;
			if (_DrawNumber)
				glDrawArrays(GL_QUADS, _Message_Buffer.DrawNumber * 4, _DrawNumber * 4);

			glDisable(GL_TEXTURE_2D);
			glDisable(GL_DEPTH_TEST);

			m_Picture2D.UnUse();
		}

		V_Draw_Box[m_Draw_Box_Number].Clear();
		if (--m_Draw_Box_Number == -1)
			m_NowDepth = 0xFFFF;
	}

	void Pen_Basis::AddPictrueMessage()
	{
		PICTRUE_DRAW* _pPICTRUE_DRAW = &V_Draw_Box[m_Draw_Box_Number].Pictrue_Draw;
		if (_pPICTRUE_DRAW->V_Message.size())
		{
			if (_pPICTRUE_DRAW->DrawNumber != _pPICTRUE_DRAW->V_Message.back().DrawNumber)
				_pPICTRUE_DRAW->V_Message.push_back(PICTRUE_MESSAGE(_pPICTRUE_DRAW->DrawNumber));
		}
		else
			_pPICTRUE_DRAW->V_Message.push_back(PICTRUE_MESSAGE(0));
	}
}