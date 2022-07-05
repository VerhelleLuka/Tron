#pragma once
#include <string>
#include "BaseComponent.h"

namespace dae
{
	class Texture2D;
	class TextureComponent final : public BaseComponent
	{
	public:
		void SetTexture(const std::string& filename);
		virtual void Update(float deltaTime) override;
		virtual void FixedUpdate(float /*deltaTime*/) override {}

		virtual void Render() const override;

		void SetPosition(float x, float y);

	private:
		std::shared_ptr<Texture2D> m_Texture{};
		Transform m_Position;
	};
}
