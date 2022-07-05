#include "PepperComponent.h"
#include "BaseComponent.h"
#include <SpriteComponent.h>
#include "Animation.h"
void dae::PepperComponent::Update(float)
{
	if (m_pParent->GetComponent<SpriteComponent>("Sprite")->GetAnimation().GetFrameNr() >= m_pParent->GetComponent<SpriteComponent>("Sprite")->GetAnimation().GetNrFrames() -1)
	{
		m_pParent->MarkForDelete();
	}
}