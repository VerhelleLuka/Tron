
#include "MiniginPCH.h"
#include "TrayComponent.h"
#include "BurgerComponent.h"
#include <InputManager.h>


void dae::TrayComponent::OnOverlap(RigidBodyComponent* other)
{
	if (other->GetParent()->GetTag() == "Burger")
	{
		if (other->GetParent()->GetComponent<SpriteComponent>("BurgerSprite")->GetAnimationName() == "Top_bun" && !other->GetParent()->GetComponent<BurgerComponent>("BurgerComp")->GetCaught())
		{
			GameManager::GetInstance().BurgerCompleted();
		}
		other->GetParent()->GetComponent<BurgerComponent>("BurgerComp")->SetCaught(true);

	}
}