#include "Player.h"
#include <Animation.h>
#include <SpriteComponent.h>
#include "SceneManager.h"
#include "Scene.h"
#include "Bullet.h"
#include "TronStructs.h"
#include "GameManager.h"
dae::PlayerComponent::PlayerComponent(bool /*isEvil*/)
	:m_AimDirection(0.f, 0.f)
{

}

dae::PlayerComponent::~PlayerComponent()
{}

void dae::PlayerComponent::Update(float /*elapsedTime*/)
{
}

void dae::PlayerComponent::Move(MovementDirection movDir) const
{
	float offset = 24.f;
	Float2 centerPoint = { m_pParent->GetTransform().GetPosition().x,m_pParent->GetTransform().GetPosition().y };
	float halfWidth = m_pParent->GetComponent<RigidBodyComponent>("RigidBody")->GetWidth() / 2.f;
	float halfHeight = m_pParent->GetComponent<RigidBodyComponent>("RigidBody")->GetHeight() / 2.f;
	centerPoint.x += halfWidth;
	centerPoint.y -= halfHeight;
	Float2 direction = m_pParent->GetComponent<RigidBodyComponent>("RigidBody")->GetDirection();
	auto& gameManager = GameManager::GetInstance();


	if (gameManager.GetGridBlock(Float2{ (centerPoint.x - halfWidth), centerPoint.y + offset}) && movDir == MovementDirection::LEFT)
	{
		return;
	}
	else if (gameManager.GetGridBlock(Float2{ (centerPoint.x + halfWidth), centerPoint.y+offset }) && movDir == MovementDirection::RIGHT)
	{
		return;
	}
	else if (gameManager.GetGridBlock(Float2{ centerPoint.x , (centerPoint.y - halfHeight + offset) }) && movDir == MovementDirection::UP)
	{
		return;
	}
	else if (gameManager.GetGridBlock(Float2{ centerPoint.x , (centerPoint.y + halfHeight + offset ) }) && movDir == MovementDirection::DOWN)
	{
		return;
	}



	int x{ 0 }, y{ 0 };

	switch (movDir)
	{
	case MovementDirection::DOWN:
		y = 1;
		break;
	case MovementDirection::LEFT:
		x = -1;
		break;
	case MovementDirection::RIGHT:
		x = 1;
		break;
	case MovementDirection::UP:
		y = -1;
		break;
	default:
		break;
	}

	m_pParent->GetComponent<RigidBodyComponent>("RigidBody")->SetDirection(Float2{ m_MoveSpeed * x, m_MoveSpeed * y });

}
void dae::PlayerComponent::Shoot()
{
	auto bullet = std::make_shared<GameObject>();

	auto bulletSprite = std::make_shared<SpriteComponent>();
	auto bulletAnimation = std::make_shared<Animation>(1, 1);

	bullet->SetTransform(m_pParent->GetTransform());

	bullet->SetTag("Bullet");

	bullet->AddComponent(bulletSprite, "BulletSprite");

	bulletAnimation->SetTexture("Hamburger/Cheese.png");

	bulletSprite->AddAnimation(bulletAnimation, "Bullet");
	bulletSprite->SetActiveAnimation("Bullet");
	bulletAnimation->SetScale(2.f);

	//RigidBodyComponent
	auto pRigidBody = std::make_shared<RigidBodyComponent>(bulletSprite->GetAnimation().GetScaledWidth(),
		bulletSprite->GetAnimation().GetScaledHeight(),
		true);
	pRigidBody->SetGameObject(bullet.get());
	pRigidBody->SetVelocityPreservation(true);
	bullet->AddComponent(pRigidBody, "RigidBody");

	float bulletSpeed = 50.f;
	Float2 aimDir = m_AimDirection;
	if (abs(m_AimDirection.x) < 0.001f && abs(m_AimDirection.y) < 0.001f)
	{
		aimDir.x = 100.f;
	}
	pRigidBody->SetDirection(Float2{ aimDir.x * bulletSpeed, aimDir.y * -bulletSpeed });

	auto bulletComp = std::make_shared<BulletComponent>();
	bullet->AddComponent(bulletComp, "Bullet");
	bulletComp->SetOverlapEvent();
	SceneManager::GetInstance().GetActiveScene().Add(bullet);
}

void dae::PlayerComponent::OnOverlap(RigidBodyComponent* /*other*/)
{

}