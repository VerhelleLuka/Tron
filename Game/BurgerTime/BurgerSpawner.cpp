#include "BurgerSpawner.h"
#include "SpriteComponent.h"
#include "BurgerComponent.h"
#include "RigidBodyComponent.h"
#include "Animation.h"
#include "SceneManager.h"
#include "Scene.h"
dae::BurgerSpawner::BurgerSpawner()
	:m_MaxBurgers(6),
	m_NumBurgers(0),
	m_BurgerRespawnTime(0.f)

{
	m_BurgerSpriteNames.push_back("Bottom_bun");
	m_BurgerSpriteNames.push_back("Top_bun");
	m_BurgerSpriteNames.push_back("Lettuce");
	m_BurgerSpriteNames.push_back("Patty");
	m_BurgerSpriteNames.push_back("Cheese");
	m_BurgerSpriteNames.push_back("Tomato");
}

void dae::BurgerSpawner::Update(float deltaTime)
{
	m_BurgerRespawnTime += deltaTime;
	if (m_BurgerRespawnTime > m_BurgerRespawnTimer && m_NumBurgers < m_MaxBurgers)
	{
		m_BurgerRespawnTime = 0.f;
		m_NumBurgers++;
		SpawnBurger();
	}
	else if (m_NumBurgers >= m_MaxBurgers)
	{
		m_BurgerRespawnTime = 0.f;
	}
}
void dae::BurgerSpawner::SpawnBurger()
{
	const float animationScale = 1.75f;

	auto burger = std::make_shared<GameObject>();
	auto burgerSprite = std::make_shared<SpriteComponent>();
	auto burgerAnimation = std::make_shared<Animation>(1, 1);
	burger->SetTag("Burger");
	burgerSprite->SetGameObject(burger.get());

	int partName = (std::rand() % (m_BurgerSpriteNames.size()));
	std::string burgerPart = "Hamburger/";
	burgerPart.append(m_BurgerSpriteNames[partName]);
	burgerPart += ".png";
	burgerAnimation->SetTexture(burgerPart);
	burgerSprite->AddAnimation(burgerAnimation, "Burger");
	burgerSprite->SetActiveAnimation("Burger");
	burgerAnimation->SetScale((float)animationScale);

	int spawnPos = (std::rand() % (m_SpawnPositions.size()));
	burger->SetTransform(m_SpawnPositions[spawnPos].x, m_SpawnPositions[spawnPos].y, -1.f);

	//RigidbodyComponent
	auto pRigidBody = std::make_shared<RigidBodyComponent>(burgerSprite->GetAnimation().GetScaledWidth(),
		burgerSprite->GetAnimation().GetScaledHeight(),
		true);
	pRigidBody->SetGameObject(burger.get());
	//Burger component
	auto pBurger = std::make_shared<BurgerComponent>();

	burger->AddComponent(burgerSprite, "BurgerSprite");
	burger->AddComponent(pRigidBody, "RigidBody");
	burger->AddComponent(pBurger, "BurgerComp");
	pBurger->SetGameObject(burger.get());
	pBurger->Initialize();
	pBurger->SetOverlapEvent();

	auto& sceneManager = SceneManager::GetInstance();
	pBurger->ForceFall();
	sceneManager.GetActiveScene().Add(burger);
}

void dae::BurgerSpawner::OnNotify(EventType event_, std::shared_ptr<EventArgs> /*args*/)
{
	if (event_ == EventType::BURGERDROPPED)
	{
		m_NumBurgers--;
	}

}