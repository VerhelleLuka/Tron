#include "MiniginPCH.h"
#include "TextureComponent.h"
#include "ResourceManager.h"
#include "Renderer.h"
using namespace dae;
void TextureComponent::Render() const
{
    Renderer::GetInstance().RenderTexture(*m_Texture, m_Position.GetPosition().x, m_Position.GetPosition().y);
}

void TextureComponent::SetTexture(const std::string& filename)
{
    m_Texture = ResourceManager::GetInstance().LoadTexture(filename);
}

void TextureComponent::SetPosition(float x, float y)
{
    m_Position.SetPosition(x, y, 0.f);
}

void TextureComponent::Update(float deltaTime)
{
    deltaTime = 0.0f;
}