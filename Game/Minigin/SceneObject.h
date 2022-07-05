#pragma once
namespace dae
{
	class Scene;
	class SceneObject
	{
	public:
		virtual void Update(float deltaTime) = 0;
		virtual void FixedUpdate(float deltaTime) = 0;
		virtual void Render() const = 0;
		virtual void Initialize(Scene& /*scene*/) {};
		SceneObject() = default;
		virtual ~SceneObject() = default;
		SceneObject(const SceneObject& other) = delete;
		SceneObject(SceneObject&& other) = delete;
		SceneObject& operator=(const SceneObject& other) = delete;
		SceneObject& operator=(SceneObject&& other) = delete;

		void MarkForDelete() { m_MarkForDelete = true;}
		bool GetMarkForDelete() const { return m_MarkForDelete; }

	protected:
		bool m_MarkForDelete = false;
	};
}
