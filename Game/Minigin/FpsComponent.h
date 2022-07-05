#pragma once
#pragma once
#include "GameObject.h"
#include "Transform.h"
#include "BaseComponent.h"
namespace dae
{
	class Font;
	class Texture2D;

	class FpsComponent final : public BaseComponent
	{
	public:
		virtual void Update(float deltaTime) override;
		virtual void FixedUpdate(float /*deltaTime*/) override {}

		virtual void Render() const {};
		explicit FpsComponent() = default;
		virtual ~FpsComponent() = default;
		FpsComponent(const FpsComponent& other) = delete;
		FpsComponent(FpsComponent&& other) = delete;
		FpsComponent& operator=(const FpsComponent& other) = delete;
		FpsComponent& operator=(FpsComponent&& other) = delete;
	private:
		int m_FPS;
	};
}
