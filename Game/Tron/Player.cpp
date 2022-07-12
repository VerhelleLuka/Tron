#include "Player.h"
#include <Animation.h>
#include <SpriteComponent.h>
#include "SceneManager.h"
#include "Scene.h"
#include "Bullet.h"
dae::PlayerComponent::PlayerComponent(bool /*isEvil*/)
	:m_AimDirection(0.f,0.f)
{

}

dae::PlayerComponent::~PlayerComponent()
{}

void dae::PlayerComponent::Update(float /*elapsedTime*/)
{
}
void dae::PlayerComponent::Shoot()
{
	auto bullet = std::make_shared<GameObject>();

	auto bulletSprite = std::make_shared<SpriteComponent>();
	auto bulletAnimation = std::make_shared<Animation>(1, 1);

	Transform transform{};
	transform.SetPosition(150, 150.f, 0.f);
	bullet->SetTransform(transform);

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