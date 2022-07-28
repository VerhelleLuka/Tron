#include "Player.h"
#include <Animation.h>
#include <SpriteComponent.h>
#include "SceneManager.h"
#include "Scene.h"
#include "Bullet.h"
#include "TronStructs.h"
#include "GameManager.h"
#include "ButtonComponent.h"
#include "Enemy.h"
dae::PlayerComponent::PlayerComponent(bool isEvil, int playerIndex)
	:m_AimDirection(0.f, 0.f),
	m_OverlappingButton(false),
	m_IsEvil(isEvil),
	m_BulletHits(3),
	m_PlayerIndex(playerIndex),
	m_WinAnimationTimer(0.f)
{
	GameManager::GetInstance().AddObserver(this);
}

dae::PlayerComponent::~PlayerComponent()
{
	GameManager::GetInstance().RemoveObserver(this);
}

void dae::PlayerComponent::Update(float /*elapsedTime*/)
{

}

void dae::PlayerComponent::OnNotify(EventType event_, std::shared_ptr<EventArgs> /*args*/)
{
	if (event_ == EventType::LEVELCLEAR)
	{
		Win();
	}
}


void dae::PlayerComponent::FixedUpdate(float elapsedTime)
{
	if (m_BulletHits <= 0)
	{
		GameManager::GetInstance().LoadLevel("MainMenu");
	}
	if (m_PlayerState == PlayerState::Dead)
	{
		if (m_pParent->GetComponent<SpriteComponent>("Sprite")->GetAnimation().GetFrameNr() == m_pParent->GetComponent<SpriteComponent>("Sprite")->GetAnimation().GetNrFrames() - 1)
		{
			GameManager::GetInstance().LoadLevel("Same");
		}
	}
	else if (m_PlayerState == PlayerState::Win)
	{
		m_WinAnimationTimer += elapsedTime;
		if (m_WinAnimationTimer >= m_WinAnimationTime)
			GameManager::GetInstance().LoadLevel("Next");
	}

}
void dae::PlayerComponent::Die()
{
	m_PlayerState = PlayerState::Dead;
	std::shared_ptr<SpriteEventArgs> args = std::make_shared<SpriteEventArgs>();
	args->name = "Death";
	Notify(EventType::LOSTLIFE, args);
	Notify(EventType::STATECHANGED, args);
}

void dae::PlayerComponent::Win()
{
	m_PlayerState = PlayerState::Win;
	std::shared_ptr<SpriteEventArgs> args = std::make_shared<SpriteEventArgs>();
	args->name = "Victory";
	Notify(EventType::STATECHANGED, args);
}
void dae::PlayerComponent::Move(MovementDirection movDir)
{
	if (m_PlayerState == PlayerState::Dead || m_PlayerState == PlayerState::Win)
		return;

	std::shared_ptr<SpriteEventArgs> args = std::make_shared<SpriteEventArgs>();
	switch (movDir)
	{

	case MovementDirection::RIGHT:
		args->name = "RunRight";
		break;
	case MovementDirection::LEFT:
		args->name = "RunLeft";
		break;
	case MovementDirection::UP:
		args->name = "Climb";
		break;
	case MovementDirection::DOWN:
		args->name = "Descend";
		break;
	default:
		args->name = "Idle";
		break;
	}
	Notify(EventType::STATECHANGED, args);
	float offset = 24.f;
	Float2 centerPoint = { m_pParent->GetTransform().GetPosition().x,m_pParent->GetTransform().GetPosition().y };
	float halfWidth = m_pParent->GetComponent<RigidBodyComponent>("RigidBody")->GetWidth() / 2.f;
	float halfHeight = m_pParent->GetComponent<RigidBodyComponent>("RigidBody")->GetHeight() / 2.f;
	centerPoint.x += halfWidth;
	centerPoint.y -= halfHeight;
	Float2 direction = m_pParent->GetComponent<RigidBodyComponent>("RigidBody")->GetDirection();
	auto& gameManager = GameManager::GetInstance();


	if (gameManager.GetGridBlock(Float2{ (centerPoint.x - halfWidth), centerPoint.y + offset }) && movDir == MovementDirection::LEFT)
	{
		return;
	}
	else if (gameManager.GetGridBlock(Float2{ (centerPoint.x + halfWidth), centerPoint.y + offset }) && movDir == MovementDirection::RIGHT)
	{
		return;
	}
	else if (gameManager.GetGridBlock(Float2{ centerPoint.x , (centerPoint.y - halfHeight + offset) }) && movDir == MovementDirection::UP)
	{
		return;
	}
	else if (gameManager.GetGridBlock(Float2{ centerPoint.x , (centerPoint.y + halfHeight + offset) }) && movDir == MovementDirection::DOWN)
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

	Transform transform = m_pParent->GetTransform();
	transform.SetPosition(transform.GetPosition().x + 5, transform.GetPosition().y + 5, transform.GetPosition().z);

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

	auto bulletComp = std::make_shared<BulletComponent>(m_IsEvil);
	bullet->AddComponent(bulletComp, "Bullet");
	bulletComp->SetOverlapEvent();
	SceneManager::GetInstance().GetActiveScene().Add(bullet);
}

void dae::PlayerComponent::OnOverlap(RigidBodyComponent* other)
{
	if (m_PlayerState == PlayerState::Win || m_PlayerState == PlayerState::Dead)
		return;
	if (other->GetParent()->GetTag() == "Button")
	{
		other->GetParent()->GetComponent<ButtonComponent>("ButtonComp")->SetOverlapping(true);
		m_OverlappingButton = true;
		return;
	}
	if (other->GetParent()->GetTag() == "Bullet")
	{
		if (other->GetParent()->GetComponent<BulletComponent>("Bullet")->GetEvil() != m_IsEvil)
		{
			other->GetParent()->MarkForDelete();
			--m_BulletHits;
		}
	}
	if (other->GetParent()->GetTag() == "Enemy")
	{
		if (!other->GetParent()->GetComponent<Enemy>("Enemy")->GetDead())
			Die();
	}
}

void dae::PlayerComponent::ButtonPress()
{
	if (m_OverlappingButton)
	{
		std::shared_ptr<EventArgs> emptyArgs = std::make_shared<EventArgs>();
		Notify(EventType::LOADLEVEL, emptyArgs);
	}
}

void dae::PlayerComponent::OnTriggerExit(RigidBodyComponent* other)
{
	if (other->GetParent()->GetTag() == "Button")
	{
		other->GetParent()->GetComponent<ButtonComponent>("ButtonComp")->SetOverlapping(false);
		m_OverlappingButton = false;
	}
}