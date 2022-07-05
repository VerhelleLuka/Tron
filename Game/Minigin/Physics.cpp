#include "MiniginPCH.h"
#include "Physics.h"
#include "SceneManager.h"
#include "Scene.h"

void dae::Physics::FixedUpdate(float /*deltaTime*/)
{
	if (!SceneManager::GetInstance().GetActiveScene().GetMarkedForDestroy())
	{
		m_SceneNr = SceneManager::GetInstance().GetActiveScene().GetIndex();
	}
	else
	{
		m_SceneNr = 0;
	}
	CheckOverlap();
}
void dae::Physics::AddRigidBodyComponent(RigidBodyComponent* rigidBody)
{
	if (int(m_pRigidBodies.size() - 1) < m_SceneNr)
	{
		std::vector<RigidBodyComponent*> newSceneVector;
		newSceneVector.push_back(rigidBody);
		m_pRigidBodies.push_back(newSceneVector);
		return;
	}
	auto it = std::find(m_pRigidBodies[m_SceneNr].begin(), m_pRigidBodies[m_SceneNr].end(), rigidBody);
	if (it == m_pRigidBodies[m_SceneNr].end())
		m_pRigidBodies[m_SceneNr].push_back(rigidBody);
}

void dae::Physics::RemoveRigidBodyComponent(RigidBodyComponent* rigidBody)
{
	for (size_t i{}; i < m_pRigidBodies.size(); ++i)
	{
		for (size_t j{}; j < m_pRigidBodies[i].size(); ++j)
		{
			if (m_pRigidBodies[i][j] == rigidBody)
			{
				m_pRigidBodies[i].erase(std::remove(m_pRigidBodies[i].begin(), m_pRigidBodies[i].end(), m_pRigidBodies[i][j]), m_pRigidBodies[i].end());
			}
		}
	}
}
void dae::Physics::DeleteScene(int index)
{
	while (size_t(index) >= m_pRigidBodies.size())
	{
		--index;
	}
	m_pRigidBodies[index].clear();
	m_pRigidBodies.erase(std::remove(m_pRigidBodies.begin(), m_pRigidBodies.end(), m_pRigidBodies[index]), m_pRigidBodies.end());
}
void dae::Physics::SetSceneNr(int sceneNr)
{
	m_SceneNr = sceneNr;
}
void dae::Physics::CheckOverlap()
{
	while ((size_t)m_SceneNr >= m_pRigidBodies.size())
	{
		--m_SceneNr;
	}
	//Overlap check for players/AI on ladders
	for (size_t i{}; i < m_pRigidBodies[m_SceneNr].size(); ++i)
	{
		for (auto& rigidBody : m_pRigidBodies[m_SceneNr])
		{
			//if (i >= m_pRigidBodies[m_SceneNr].size())
			//	break;
			if (rigidBody != m_pRigidBodies[m_SceneNr][i])
			{
				//If at least one of the rigidbodies is a trigger
				if (rigidBody->GetTrigger() || m_pRigidBodies[m_SceneNr][i]->GetTrigger())
				{
					//This is super specific code for PeterPepper/Enemy overlap with stairs

					//Every sprite is 32 pixels wide (16px source *  2 scale)
					//The overlap with the ladder should be the center of the ladder (16px) with,
					//the utmost left position they should be able to climb up on is 10 and utmost right 22
					//so
					Float2 posA = { rigidBody->GetTransform().GetPosition().x ,
						rigidBody->GetTransform().GetPosition().y };
					Float2 posB = { m_pRigidBodies[m_SceneNr][i]->GetTransform().GetPosition().x,
						m_pRigidBodies[m_SceneNr][i]->GetTransform().GetPosition().y };
					//float widthA = rigidBody->GetWidth();
					float widthB = m_pRigidBodies[m_SceneNr][i]->GetWidth();
					float heightA = rigidBody->GetHeight();
					float heightB = m_pRigidBodies[m_SceneNr][i]->GetHeight();

					//check overlap

					//check widths
					bool isOverlapping = false;
					if (posA.x <= posB.x + widthB && posA.x >= posB.x)
					{
						//check heights
						if (posA.y > posB.y + heightB || posB.y > posA.y + heightA)
						{
							continue;
						}
						else
						{
							isOverlapping = true;
							if (m_pRigidBodies[m_SceneNr][i]->GetTrigger())
							{
								m_pRigidBodies[m_SceneNr][i]->AddOverlappingBody(rigidBody);
								m_pRigidBodies[m_SceneNr][i]->OnOverlap(rigidBody);
							}
							if (rigidBody->GetTrigger())
							{
								rigidBody->AddOverlappingBody(m_pRigidBodies[m_SceneNr][i]);
								rigidBody->OnOverlap(m_pRigidBodies[m_SceneNr][i]);
							}
						}
					}
					if (!isOverlapping)
					{
						if (m_pRigidBodies[m_SceneNr][i]->GetTrigger())
						{
							m_pRigidBodies[m_SceneNr][i]->RemoveOverlappingBody(rigidBody);
						}
						if (rigidBody->GetTrigger())
						{
							rigidBody->RemoveOverlappingBody(m_pRigidBodies[m_SceneNr][i]);
						}
					}

				}
			}
		}
	}
}