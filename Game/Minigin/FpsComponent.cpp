#include "MiniginPCH.h"
#include <SDL_ttf.h>
#include "FpsComponent.h"
#include "Renderer.h"
#include "Font.h"
#include "Texture2D.h"
#include "TextComponent.h"


void dae::FpsComponent::Update(float deltaTime)
{
	m_FPS = int(1.f / deltaTime);
	auto textComponent = m_pParent->GetComponent<TextComponent>("text");

	textComponent->SetText("Fps: " + std::to_string(m_FPS));
}



