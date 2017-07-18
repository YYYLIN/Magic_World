#include "MagicWorld.h"



MagicWorld::MagicWorld()
{
}


MagicWorld::~MagicWorld()
{
}

bool MagicWorld::Initialize()
{
	bool result;

	result = m_MagicModelObject.Initialize(1);
	if (!result)
		return false;
	result = m_MagicModelObject.LoadObject(0, "resources/模型/脸.obj", "resources/模型/脸.png");
	if (!result)
		return false;

	AddModelObject(&m_MagicModelObject);
	pLightShader = (*MagicEngineContext::pMagicEngineContext->GetPen())["Light"];

	return true;
}

void MagicWorld::Shutdown()
{

}

void MagicWorld::AddModelObject(MagicModelObject* _pMagicModelObject)
{
	G_MagicModelObject.push_back(_pMagicModelObject);
}

void MagicWorld::Updata()
{
}

void MagicWorld::Render(glm::mat4 CameraMatrix)
{
	pLightShader->Use();
	glm::mat4 projectionMatrix;
	CameraMatrix = glm::lookAt(glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f, 0.0f, -100.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	projectionMatrix = glm::perspective(70.0f, (float)1024 / (float)768, 0.1f, 10000.0f);

	glUniformMatrix4fv((*pLightShader)("CameraMatrix"), 1, GL_FALSE, &CameraMatrix[0][0]);
	glUniformMatrix4fv((*pLightShader)("projectionMatrix"), 1, GL_FALSE, &projectionMatrix[0][0]);

	for (vector<MagicModelObject*>::iterator _piterator = G_MagicModelObject.begin();_piterator != G_MagicModelObject.end(); _piterator++)
	{
		RenderLightModel(CameraMatrix, *_piterator);
	}
	pLightShader->UnUse();
}

void MagicWorld::RenderLightModel(glm::mat4 CameraMatrix, MagicModelObject* _pMagicModelObject)
{

	glm::mat4 worldMatrix;
	worldMatrix[3].y = -10.0f;
	worldMatrix[3].z = -100.0f;
	glUniformMatrix4fv((*pLightShader)("worldMatrix"), 1, GL_FALSE, &worldMatrix[0][0]);

	glUniform4f((*pLightShader)("AmbientColor"), 1.0f, 1.0f, 1.0f, 1.0f);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);

	glDisable(GL_CULL_FACE);
	//   glEnable(GL_CULL_FACE);

	float _pData[] =
	{
		0.0f,0.0f,0.0f,
		30.0f,30.0f,0.0f,
		-30.0f,30.0f,0.0f,

		0.0f,0.0f,0.0f,
		30.0f,-30.0f,0.0f,
		-30.0f,-30.0f,0.0f
	};

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, _pData);

	//glDrawArrays(GL_TRIANGLES, 0, 6);

	for (int a = 0;a < _pMagicModelObject->GetNumber();a++)
	{
	//	glBindTexture(GL_TEXTURE_2D, _pMagicModelObject->GetTextrue(a));
		glBindVertexArray(_pMagicModelObject->GetVAO(a));
		//glDrawElements(GL_TRIANGLES, _pMagicModelObject->GetIndexNumber(a), GL_UNSIGNED_SHORT, (GLvoid*)NULL);
		glDrawArrays(GL_TRIANGLES, 0, _pMagicModelObject->GetVertexNumber(a));
	}

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
}