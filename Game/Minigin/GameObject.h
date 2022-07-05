#pragma once
#include "Transform.h"
#include "SceneObject.h"
#include <memory>
#include <map>
#include <vector>
#include <string>
namespace dae
{
	class Scene;
	class BaseComponent;
	class GameObject final : public SceneObject, std::enable_shared_from_this<GameObject>
	{
	public:

		void Update(float deltaTime) override;
		void FixedUpdate(float deltaTime) override;
		void Render() const override;
		virtual void Initialize(Scene& scene) override;

		void AddComponent(std::shared_ptr<BaseComponent> pComponent, const std::string& name);
		template <typename T> std::shared_ptr<T> GetComponent(const std::string& name) const
		{
			if (m_pComponents.size() > 100 || m_pComponents.size() < 1)
				return nullptr;
			if (m_pComponents.find(name) != m_pComponents.end())
			{
				return std::dynamic_pointer_cast<T>(m_pComponents.find(name)->second);
			}
			return nullptr;
		};
		void RemoveComponent(const std::string& name);

		void SetParent(std::shared_ptr<GameObject> parent);
		std::weak_ptr<GameObject> GetParent() const;

		size_t GetChildCount() const;
		std::shared_ptr<GameObject> GetChildAt(int index) const;
		void RemoveChild(std::shared_ptr<GameObject> objToDelete);
		void AddChild(std::shared_ptr<GameObject> go);

		Transform& GetTransform() { return m_Transform; }
		void SetTransform(Transform transform);
		void SetTransform(float x, float y, float z);
		GameObject() :m_Tag("---") {};
		virtual ~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

		void SetTag(const std::string& tag) { m_Tag = tag; }
		std::string GetTag() const { if (m_Tag == "---") return "NULL"; else return m_Tag; }

	private:
		std::map<std::string, std::shared_ptr<BaseComponent>> m_pComponents;
		std::vector<std::shared_ptr<GameObject>> m_pGameObjects;
		std::weak_ptr<GameObject> m_pParent;

		Transform m_Transform;
		std::string m_Tag;
	};
}
