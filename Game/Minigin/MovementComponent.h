//#pragma once
//#pragma once
//#pragma once
//#include "GameObject.h"
//#include "Transform.h"
//#include "BaseComponent.h"
//namespace dae
//{
//	class MovementComponent final : public BaseComponent, public Observer
//	{
//	public:
//		virtual void Update(float /*deltaTime*/) override {};
//		virtual void FixedUpdate(float /*deltaTime*/) override {};
//		virtual void Render() const override {};
//
//		virtual void SetGameObject(GameObject* go) { m_pParent = go; };
//		MovementComponent() {};
//		virtual ~MovementComponent() {};
//
//		bool GetMovementUp() const { return m_CanMoveUp; }
//		bool GetMovementDown() const { return m_CanMoveDown; }
//		bool GetMovementLeft() const { return m_CanMoveLeft; }
//		bool GetMovementRight() const { return m_CanMoveRight; }
//
//		void SetMovementUp(bool canMoveUp) { m_CanMoveUp = canMoveUp; }
//		void SetMovementDown(bool canMoveDown) { m_CanMoveDown = canMoveDown; }
//		void SetMovementLeft(bool canMoveLeft) { m_CanMoveLeft = canMoveLeft; }
//		void SetMovementRight(bool canMoveRight) { m_CanMoveRight = canMoveRight; }
//	protected:
//		GameObject* m_pParent{};
//
//		bool m_CanMoveLeft{ false };
//		bool m_CanMoveRight{ false };	
//		bool m_CanMoveUp{false};
//		bool m_CanMoveDown{ false };
//	};
//}