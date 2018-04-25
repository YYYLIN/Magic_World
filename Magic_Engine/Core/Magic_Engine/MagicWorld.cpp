#include "Include/MagicWorld.h"
#include "Include/MagicCamera.h"


MagicWorld::MagicWorld()
{
}


MagicWorld::~MagicWorld()
{
}

bool MagicWorld::Initialize()
{
	//bool result;

	/*glDisable(GL_CULL_FACE);
	//glEnable(GL_CULL_FACE);

	glm::mat4 worldMatrix;

	result = m_MagicModelObject.CreateObject(4);
	if (!result)
	return false;
	result = m_MagicModelObject.LoadObject(0, "resources/模型/脸.obj", "resources/模型/脸.png");
	if (!result)
	return false;
	result = m_MagicModelObject.LoadObject(1, "resources/模型/头发.obj", "resources/模型/头发_1.png");
	if (!result)
	return false;
	result = m_MagicModelObject.LoadObject(2, "resources/模型/眼睛.obj", "resources/模型/眼睛.png");
	if (!result)
	return false;
	result = m_MagicModelObject.LoadObject(3, "resources/模型/眼球.obj", "resources/模型/其他.png");
	if (!result)
	return false;
	worldMatrix = glm::rotate(90.0f, 0.0f, 1.0f, 0.0f);
	worldMatrix[3].y = -10.0f;
	m_MagicModelObject.SetWorldMatrix(worldMatrix);
	AddModelObject(&m_MagicModelObject);


	result = m_GroundModelObject.CreateObject(1);
	if (!result)
	return false;
	result = m_GroundModelObject.LoadObject(0, "resources/模型/地面贴图.obj", "resources/模型/bilibili.png");
	if (!result)
	return false;
	worldMatrix = glm::rotate(-90.0f, 1.0f, 0.0f, 0.0f);
	m_GroundModelObject.SetWorldMatrix(worldMatrix);
	AddModelObject(&m_GroundModelObject);*/

	/*
		result = m_BallModelObject.CreateObject(1);
		if (!result)
		return false;
		result = m_BallModelObject.LoadObject(0, "resources/模型/圆心球.obj", "resources/模型/bilibili.png");
		if (!result)
		return false;
		worldMatrix = glm::mat4();
		worldMatrix[3].y = 10.0f;
		worldMatrix[3].x = 20.0f;
		m_BallModelObject.SetWorldMatrix(worldMatrix);
		AddModelObject(&m_BallModelObject);*/

	m_DepthFBO.Initialize(1024, 1024, MagicFBOTexture::DEPTH);

	//	pLightShader = (*MagicEngineContext::pMagicEngineContext->GetPen())["Light"];
	//	pDepthShader = (*MagicEngineContext::pMagicEngineContext->GetPen())["Depth"];

	m_CameraMatrix = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	m_CameraMatrix *= glm::rotate(45.0f, 1.0f, 0.0f, 0.0f);
	m_CameraMatrix[3].z = -70.0f;

	m_LightMatrix = m_CameraMatrix;

	return true;
}

void MagicWorld::Shutdown()
{

}

int MagicWorld::MessageHandle(unsigned int _MessageType, unsigned int _Message)
{
	static float _Look = -70.0f;
	static float _rotate = 0.0f;
	static float _RightRotate = 45.0f;
	switch (_MessageType)
	{
	case VK_UP:
		_Look += 1.0f;
		break;
	case VK_DOWN:
		_Look -= 1.0f;
		break;
	case VK_LEFT:
		_rotate += 1.0f;
		break;
	case VK_RIGHT:
		_rotate -= 1.0f;
		break;
	case VK_NUMPAD8:
		_RightRotate += 1.0f;
		break;
	case VK_NUMPAD2:
		_RightRotate -= 1.0f;
		break;
	}
	m_CameraMatrix = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	m_CameraMatrix *= glm::rotate(_RightRotate, 1.0f, 0.0f, 0.0f);
	m_CameraMatrix *= glm::rotate(_rotate, 0.0f, 1.0f, 0.0f);
	m_CameraMatrix[3].z = _Look;

	return MAGIC_MESSAGE_RETURN_OK;
}

void MagicWorld::AddModelObject(MagicModelObject* _pMagicModelObject)
{
	G_MagicModelObject.push_back(_pMagicModelObject);
}

void MagicWorld::OnUpdata()
{
}

void MagicWorld::Render(glm::vec2 _DrawPos)
{
	/*
		glEnable(GL_DEPTH_TEST);
		//GL_LEQUAL小于等于 ，GL_LESS小鱼
		glDepthFunc(GL_LEQUAL);

		glm::mat4 projectionMatrix, lightOrtho;

		projectionMatrix = glm::perspective(70.0f, (float)1024 / (float)768, 0.1f, 10000.0f);
		lightOrtho = glm::ortho(
		-50.0f, 50.0f,
		-50.0f, 50.0f, 0.1f, 100.0f);

		m_DepthFBO.Use();
		glClear(GL_DEPTH_BUFFER_BIT);
		pDepthShader->Use();

		glUniformMatrix4fv((*pDepthShader)("CameraMatrix"), 1, GL_FALSE, &m_LightMatrix[0][0]);
		glUniformMatrix4fv((*pDepthShader)("projectionMatrix"), 1, GL_FALSE, &lightOrtho[0][0]);

		for (std::vector<MagicModelObject*>::iterator _piterator = G_MagicModelObject.begin(); _piterator != G_MagicModelObject.end(); _piterator++)
		{
		RenderLightModel(pDepthShader, *_piterator);
		}

		m_DepthFBO.UnUse();

		pLightShader->Use();

		glUniformMatrix4fv((*pLightShader)("CameraMatrix"), 1, GL_FALSE, &m_CameraMatrix[0][0]);
		glUniformMatrix4fv((*pLightShader)("projectionMatrix"), 1, GL_FALSE, &projectionMatrix[0][0]);
		glUniformMatrix4fv((*pLightShader)("LightprojectionMatrix"), 1, GL_FALSE, &lightOrtho[0][0]);
		glUniformMatrix4fv((*pLightShader)("LightMatrix"), 1, GL_FALSE, &m_LightMatrix[0][0]);
		glUniform4f((*pLightShader)("AmbientColor"), 1.0f, 1.0f, 1.0f, 1.0f);


		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_TEXTURE_2D);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, m_DepthFBO.GetTextrue());
		glActiveTexture(GL_TEXTURE0);

		for (std::vector<MagicModelObject*>::iterator _piterator = G_MagicModelObject.begin(); _piterator != G_MagicModelObject.end(); _piterator++)
		{
		RenderLightModel(pLightShader, *_piterator);
		}

		glDisable(GL_TEXTURE_2D);
		glDisable(GL_BLEND);

		pLightShader->UnUse();

		glDisable(GL_DEPTH_TEST);*/

	/*
		MagicPen* pPen = MagicEngineContext::pMagicEngineContext->GetPen();
		pPen->DrawPicture(CONST_CAMERA, glm::mat4(), m_DepthFBO.GetTextrue(), glm::vec2(0, 0), glm::vec2(256, 256));*/
}

void MagicWorld::RenderLightModel(MagicShader* _pShader, MagicModelObject* _pMagicModelObject)
{
	/*
		glUniformMatrix4fv((*_pShader)("worldMatrix"), 1, GL_FALSE, &_pMagicModelObject->GetWorldMatrix()[0][0]);

		for (int a = 0; a < _pMagicModelObject->GetNumber(); a++)
		{
		glBindTexture(GL_TEXTURE_2D, _pMagicModelObject->GetTextrue(a));
		glBindVertexArray(_pMagicModelObject->GetVAO(a));
		glDrawElements(GL_TRIANGLES, _pMagicModelObject->GetIndexNumber(a), GL_UNSIGNED_SHORT, (GLvoid*)NULL);
		}*/
}