#include "MiniginPCH.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include <algorithm>
#include <vector>
#include "BaseComponent.h"
#include "RigidBodyComponent.h"
#include "Physics.h"
#include "Scene.h"
void dae::GameObject::Initialize(Scene& scene)
{
	for (auto& component : m_pComponents)
	{
		component.second->Initialize(scene);

	}
	for (auto& child : m_pGameObjects)
	{
		child->Initialize(scene);
	}

}

void dae::GameObject::Update(float deltaTime)
{
	for (auto& component : m_pComponents)
	{
		component.second->Update(deltaTime);
		
	}
	for (auto& child : m_pGameObjects)
	{
		child->Update(deltaTime);
	}
}

void dae::GameObject::FixedUpdate(float deltaTime)
{
	for (auto& component : m_pComponents)
	{
		component.second->FixedUpdate(deltaTime);

	}
	for (auto& child : m_pGameObjects)
	{
		child->FixedUpdate(deltaTime);
	}
}

void dae::GameObject::Render() const
{
	for (auto& component : m_pComponents)
	{
		component.second->Render();
	}
	for (auto& child : m_pGameObjects)
	{

		child->Render();
	}
}



void dae::GameObject::AddComponent(std::shared_ptr<BaseComponent> pComponent, const std::string& name)
{
	pComponent->SetGameObject(this);
	m_pComponents[name] = pComponent;
}

void dae::GameObject::RemoveComponent(const std::string& name)
{
	m_pComponents.erase(name);
}

void dae::GameObject::SetParent(std::shared_ptr<GameObject> parent)
{
	if (m_pParent.lock())
	{
		m_pParent.lock()->RemoveChild(shared_from_this());
	}
	m_pParent = parent;
	m_pParent.lock()->AddChild(shared_from_this());
}
std::weak_ptr<dae::GameObject> dae::GameObject::GetParent() const
{
	return m_pParent;
}

size_t dae::GameObject::GetChildCount() const
{
	return m_pGameObjects.size();
}

std::shared_ptr<dae::GameObject> dae::GameObject::GetChildAt(int index) const
{
	return m_pGameObjects.at(index);
}

void dae::GameObject::RemoveChild(std::shared_ptr<GameObject> objToDelete)
{
	m_pGameObjects.erase(std::find(m_pGameObjects.begin(), m_pGameObjects.end(), objToDelete));
}

void dae::GameObject::AddChild(std::shared_ptr<GameObject> go)
{
	m_pGameObjects.push_back(go);
}

void dae::GameObject::SetTransform(Transform transform)
{
	m_Transform.SetPosition(transform.GetPosition().x, transform.GetPosition().y, transform.GetPosition().z);
}
void dae::GameObject::SetTransform(float x, float y, float z)
{
	m_Transform.SetPosition(x,y,z);

}

dae::GameObject::~GameObject()
{
	for (auto& comp : m_pComponents)
	{
		if (dynamic_cast<RigidBodyComponent*>(comp.second.get()))
		{
			Physics::GetInstance().RemoveRigidBodyComponent(dynamic_cast<RigidBodyComponent*>(comp.second.get()));
		}
	}
}