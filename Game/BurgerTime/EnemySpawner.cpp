#include "EnemySpawner.h"
#include "Animation.h"
#include "SpriteComponent.h"
#include "RigidBodyComponent.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Enemy.h"
#include "Physics.h"
dae::EnemySpawner::EnemySpawner(Difficulty difficulty)
	: m_Difficulty(difficulty),
	m_NumEnemies(0),
	m_EnemyRespawnTime(0.f),
	m_EggPresent(false)
{
	switch (difficulty)
	{
	case Difficulty::EASY:
		m_MaxEnemies = 5;
		m_EnemyRespawnTimer = 3.5f;
		break;

	case Difficulty::HARD:
		m_MaxEnemies = 7;
		m_EnemyRespawnTimer = 1.5f;
		break;
	}
}
void dae::EnemySpawner::Initialize(Scene&)
{
	switch (m_Difficulty)
	{
	case Difficulty::EASY:
		m_MaxEnemies = 5;
		m_EnemyRespawnTimer = 2.f;
		break;

	case Difficulty::HARD:
		m_MaxEnemies = 7;
		m_EnemyRespawnTimer = 1.5f;
		break;
	}
}



void dae::EnemySpawner::Update(float deltaTime)
{
	m_EnemyRespawnTime += deltaTime;
	if (m_EnemyRespawnTime > m_EnemyRespawnTimer && m_NumEnemies < m_MaxEnemies)
	{
		m_EnemyRespawnTime = 0.f;
		m_NumEnemies++;
		SpawnEnemy();
	}
	else if (m_NumEnemies >= m_MaxEnemies)
	{
		m_EnemyRespawnTime = 0.f;
	}
}

void dae::EnemySpawner::SpawnEnemy()
{
	const float animationScale = 1.75f;
	auto enemyGo = std::make_shared<GameObject>();
	enemyGo->SetTag("Enemy");
	auto descendAnim = std::make_shared<Animation>(2, 2);
	auto walkLeftAnim = std::make_shared<Animation>(2, 2);
	auto climbAnim = std::make_shared<Animation>(2, 2);
	auto walkRightAnim = std::make_shared<Animation>(2, 2);
	auto deathAnim = std::make_shared<Animation>(4, 4);
	auto pepperedAnim = std::make_shared<Animation>(2, 2);
	auto enemySprite = std::make_shared<SpriteComponent>();
	enemySprite->SetGameObject(enemyGo.get());
	enemySprite->AddAnimation(climbAnim, "Climb");
	enemySprite->AddAnimation(descendAnim, "Descend");
	enemySprite->AddAnimation(walkRightAnim, "WalkRight");
	enemySprite->AddAnimation(deathAnim, "Death");
	enemySprite->AddAnimation(walkLeftAnim, "WalkLeft");
	enemySprite->AddAnimation(pepperedAnim, "Peppered");
	enemyGo->AddComponent(enemySprite, "EnemySprite");

	climbAnim->SetScale(animationScale);
	descendAnim->SetScale(animationScale);
	walkLeftAnim->SetScale(animationScale);
	walkRightAnim->SetReversed(true);
	walkRightAnim->SetScale(animationScale);
	pepperedAnim->SetScale(animationScale);
	deathAnim->SetScale(animationScale);
	//Set all animation to ensure RigidBodyComponent can get the scaled width and the update performed in setactive animation 
	//is'nt weird
	climbAnim->SetTexture("Enemies/Sausage_Climb.png");
	descendAnim->SetTexture("Enemies/Sausage_Descend.png");
	walkLeftAnim->SetTexture("Enemies/Sausage_Walk.png");
	walkRightAnim->SetTexture("Enemies/Sausage_Walk.png");
	deathAnim->SetTexture("Enemies/Sausage_Kill.png");
	pepperedAnim->SetTexture("Enemies/Sausage_Peppered.png");

	enemySprite->SetActiveAnimation("WalkLeft");

	//RigidbodyComponent
	auto pRigidBody = std::make_shared<RigidBodyComponent>(enemySprite->GetAnimation().GetScaledWidth(),
		enemySprite->GetAnimation().GetScaledHeight(),
		true);
	pRigidBody->SetOffset(Float2{ 5,0 });
	//pRigidBody->SetGameObject(enemyGo.get());

	enemyGo->AddComponent(pRigidBody, "RigidBody");
	//Get random number between one and 6 -- one, two and three for sausage, four and five for pickle, and six for egg (if there isn't any present)
	EnemyType enemyType = EnemyType::SAUSAGE;

	if (GameManager::GetInstance().GetGameMode() != GameMode::VERSUS)
	{
		int enemyToMake = 1 + (std::rand() % (6));
		if (m_EggPresent)
		{
			if (enemyToMake < 5)
			{
				climbAnim->SetTexture("Enemies/Sausage_Climb.png");
				descendAnim->SetTexture("Enemies/Sausage_Descend.png");
				walkLeftAnim->SetTexture("Enemies/Sausage_Walk.png");
				walkRightAnim->SetTexture("Enemies/Sausage_Walk.png");
				deathAnim->SetTexture("Enemies/Sausage_Kill.png");
				pepperedAnim->SetTexture("Enemies/Sausage_Peppered.png");
				enemyType = EnemyType::SAUSAGE;
			}
			else
			{
				climbAnim->SetTexture("Enemies/Pickle_Climb.png");
				descendAnim->SetTexture("Enemies/Pickle_Descend.png");
				walkLeftAnim->SetTexture("Enemies/Pickle_Walk.png");
				walkRightAnim->SetTexture("Enemies/Pickle_Walk.png");
				deathAnim->SetTexture("Enemies/Pickle_Kill.png");
				pepperedAnim->SetTexture("Enemies/Pickle_Peppered.png");

				enemyType = EnemyType::PICKLE;

			}


		}
		else
		{
			if (enemyToMake < 5)
			{
				climbAnim->SetTexture("Enemies/Sausage_Climb.png");
				descendAnim->SetTexture("Enemies/Sausage_Descend.png");
				walkLeftAnim->SetTexture("Enemies/Sausage_Walk.png");
				walkRightAnim->SetTexture("Enemies/Sausage_Walk.png");
				pepperedAnim->SetTexture("Enemies/Sausage_Peppered.png");
				deathAnim->SetTexture("Enemies/Sausage_Kill.png");
				enemyType = EnemyType::SAUSAGE;

			}
			else if (enemyToMake ==5)
			{
				climbAnim->SetTexture("Enemies/Pickle_Climb.png");
				descendAnim->SetTexture("Enemies/Pickle_Descend.png");
				walkLeftAnim->SetTexture("Enemies/Pickle_Walk.png");
				walkRightAnim->SetTexture("Enemies/Pickle_Walk.png");
				deathAnim->SetTexture("Enemies/Pickle_Kill.png");
				pepperedAnim->SetTexture("Enemies/Pickle_Peppered.png");

				enemyType = EnemyType::PICKLE;
			}
			else
			{
				climbAnim->SetTexture("Enemies/Egg_Climb.png");
				descendAnim->SetTexture("Enemies/Egg_Descend.png");
				walkLeftAnim->SetTexture("Enemies/Egg_Walk.png");
				walkRightAnim->SetTexture("Enemies/Egg_Walk.png");
				pepperedAnim->SetTexture("Enemies/Egg_Peppered.png");

				deathAnim->SetTexture("Enemies/Egg_Kill.png");
				m_EggPresent = true;
				enemyType = EnemyType::EGG;
			}
		}
	}

	auto enemy = std::make_shared<Enemy>(enemyType);
	enemy->SetGameObject(enemyGo.get());
	enemy->SetOverlapEvent();
	enemy->AddObserver(this);
	enemy->SetOnTriggerExitEvent();
	enemyGo->AddComponent(enemy, "Enemy");

	int spawnPos = (std::rand() % (m_SpawnPositions.size()));

	enemyGo->SetTransform(m_SpawnPositions[spawnPos].x, m_SpawnPositions[spawnPos].y, 0);

	enemy->Initialize(SceneManager::GetInstance().GetActiveScene());
	auto& sceneManager = SceneManager::GetInstance();
	sceneManager.GetActiveScene().Add(enemyGo);

}

void dae::EnemySpawner::OnNotify(EventType event_, std::shared_ptr<EventArgs> args)
{
	if (event_ == EventType::ENEMYKILLED)
	{
		m_NumEnemies--;
		if (std::static_pointer_cast<EnemyKilledArgs>(args)->enemyType == EnemyType::EGG)
		{
			m_EggPresent = false;
		}
	}

}