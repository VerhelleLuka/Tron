#pragma once
#include "GameObject.h"
#include "Transform.h"
#include "BaseComponent.h"
#include "Observer.h"

namespace dae
{
	//Had to do this because otherwise in the cpp i get an error for not referencing args
	class LivesDisplayComponent final: public BaseComponent, public Observer
	{
	public:
		virtual void Update(float /*deltaTime*/) override {};
		virtual void FixedUpdate(float /*deltaTime*/) override{}
		virtual void Render() const override {};
		LivesDisplayComponent() {};
		virtual ~LivesDisplayComponent();
		LivesDisplayComponent(const LivesDisplayComponent& other) = delete;
		LivesDisplayComponent(LivesDisplayComponent&& other) = delete;
		LivesDisplayComponent& operator=(const LivesDisplayComponent& other) = delete;
		LivesDisplayComponent& operator=(LivesDisplayComponent&& other) = delete;
		virtual void OnNotify(EventType event_, std::shared_ptr<EventArgs> args) override;
	};
}
