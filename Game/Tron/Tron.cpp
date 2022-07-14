#include "Tron.h"
#include "GameManager.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "TronStructs.h"
#include <LevelParser.h>
#include <SpriteComponent.h>
#include "Animation.h"
#include "Scene.h"
#include "InputManager.h"
#include "TronCommand.h"
void dae::Tron::Initialize()
{
	GameManager::GetInstance().SetTronGame(this);
}

void dae::Tron::LoadGame()
{
	auto& menuScene = SceneManager::GetInstance().CreateScene("MainMenu");
	SceneManager::GetInstance().SetActiveScene(&menuScene);
	Physics::GetInstance().SetSceneNr(0);
	//CreateMenu(menuScene);
	ParseLevel(menuScene, 0, "Level1");
	CreateTronAndHUD(menuScene, 0, 0);
}

void dae::Tron::CreateTronAndHUD(Scene& scene, int /*playerNr*/, bool /*andHUD*/) const
{
	auto tronGo = std::make_shared<GameObject>();
	auto playerComponent = std::make_shared<PlayerComponent>(false);

	tronGo->AddComponent(playerComponent, "PlayerComp");


	tronGo->SetTransform(300.f,150.f,0.f);

	auto tronSprite = std::make_shared<SpriteComponent>();

	float animationScale = 1.5f;
	//Create animations 
	// 	//Down
	auto tronAnimationDown = std::make_shared<Animation>(3, 3);
	tronAnimationDown->SetTexture("PeterPepper/ClimbDownSprite.png");
	tronAnimationDown->SetScale(animationScale);
	//Right
	auto tronAnimationRight = std::make_shared<Animation>(3, 3);
	tronAnimationRight->SetTexture("PeterPepper/LeftRightSprite.png");
	tronAnimationRight->SetScale(animationScale);

	//Left
	auto tronAnimationLeft = std::make_shared<Animation>(3, 3);
	tronAnimationLeft->SetTexture("PeterPepper/LeftRightSprite.png");
	tronAnimationLeft->SetScale(animationScale);
	//Up
	auto tronAnimationUp = std::make_shared<Animation>(3, 3);
	tronAnimationUp->SetTexture("PeterPepper/ClimbUpSprite.png");
	tronAnimationUp->SetScale(animationScale);
	//Idle
	auto tronAnimationIdle = std::make_shared<Animation>(1, 1);
	tronAnimationIdle->SetTexture("PeterPepper/IdleSprite.png");
	tronAnimationIdle->SetScale(animationScale);
	//Victory
	auto tronAnimationVictory = std::make_shared<Animation>(2, 2);
	tronAnimationVictory->SetTexture("PeterPepper/VictorySprite.png");
	tronAnimationVictory->SetScale(animationScale);
	//Death
	auto tronAnimationDeath = std::make_shared<Animation>(5, 5);
	tronAnimationDeath->SetTexture("PeterPepper/DeathSprite.png");
	tronAnimationDeath->SetScale(animationScale);
	//Add animation to sprite
	tronSprite->AddAnimation(tronAnimationRight, "RunRight");
	tronAnimationRight->SetReversed(true);
	tronSprite->AddAnimation(tronAnimationLeft, "RunLeft");
	tronSprite->AddAnimation(tronAnimationUp, "Climb");
	tronSprite->AddAnimation(tronAnimationDown, "Descend");
	tronSprite->AddAnimation(tronAnimationIdle, "Idle");
	tronSprite->AddAnimation(tronAnimationVictory, "Victory");
	tronSprite->AddAnimation(tronAnimationDeath, "Death");

	tronSprite->SetGameObject(tronGo.get());
	tronSprite->SetActiveAnimation("RunRight");

	tronGo->AddComponent(tronSprite, "Sprite");

	auto pRigidBody = std::make_shared<RigidBodyComponent>(tronSprite->GetAnimation().GetScaledWidth(),
		tronSprite->GetAnimation().GetScaledHeight(),
		true);
	pRigidBody->SetGameObject(tronGo.get());
	tronGo->AddComponent(pRigidBody, "RigidBody");
	scene.Add(tronGo);


	auto& input = InputManager::GetInstance();

	input.AddCommand(ControllerButton::LeftShoulder, new Shoot, KeyState::PRESSED, tronGo.get(), 0);
	input.AddCommand(ControllerButton::RightThumb, new Aim, KeyState::DOWN, tronGo.get(), 0);
	input.AddCommand(ControllerButton::DPadDown, new Move(MovementDirection::DOWN),KeyState::DOWN, tronGo.get(), 0);
	input.AddCommand(ControllerButton::DPadLeft, new Move(MovementDirection::LEFT),KeyState::DOWN, tronGo.get(), 0);
	input.AddCommand(ControllerButton::DPadRight, new Move(MovementDirection::RIGHT),KeyState::DOWN, tronGo.get(), 0);
	input.AddCommand(ControllerButton::DPadUp, new Move(MovementDirection::UP),KeyState::DOWN, tronGo.get(), 0);
}

std::vector<dae::Float2> dae::Tron::ParseLevel(Scene& scene, int sceneNr, const std::string& levelName) const
{
	std::vector<Block> blocks;

	std::vector<Float2> spawnPositions;
	std::string fullName = "../Data/Level/";
	fullName.append(levelName);
	fullName.append(".txt");
	ParseLevelFile(fullName, blocks, spawnPositions);

	CreateFixedBlocks(scene, sceneNr);
	CreateBlocks(scene, sceneNr, blocks);
	return spawnPositions;
}

void dae::Tron::CreateFixedBlocks(Scene& scene, int /*sceneNr*/) const
{
	float levelScale = 1.5f;
	float blockWidth = 16.f * levelScale;
	//Horizontal blocks top
	for (int i = 0; i < 26; ++i)
	{
		auto block = std::make_shared<GameObject>();
		auto blockSprite = std::make_shared<SpriteComponent>();
		auto blockAnimation = std::make_shared<Animation>(1, 1);
		Transform transform{};
		block->SetTag("Block");

		block->AddComponent(blockSprite, "BlockSprite");

		blockAnimation->SetTexture("Level/Ladder.png");
		transform.SetPosition(i * blockWidth + 8, 0.f, 0.f);
		blockSprite->AddAnimation(blockAnimation, "Block");
		blockSprite->SetActiveAnimation("Block");
		blockAnimation->SetScale((float)levelScale);

		block->SetTransform(transform);

		//RigidBodyComponent
		auto pRigidBody = std::make_shared<RigidBodyComponent>(blockSprite->GetAnimation().GetScaledWidth(),
			blockSprite->GetAnimation().GetScaledHeight(),
			false);
		pRigidBody->SetGameObject(block.get());
		
		block->AddComponent(pRigidBody, "RigidBody");

		scene.Add(block);
	}
	//bottom
		for (int i = 0; i < 26; ++i)
	{
		auto block = std::make_shared<GameObject>();
		auto blockSprite = std::make_shared<SpriteComponent>();
		auto blockAnimation = std::make_shared<Animation>(1, 1);
		Transform transform{};

		block->AddComponent(blockSprite, "BlockSprite");
		block->SetTag("Block");

		blockAnimation->SetTexture("Level/Ladder.png");
		transform.SetPosition(i * blockWidth + 8, 408.f, 0.f);

		blockSprite->AddAnimation(blockAnimation, "Block");
		blockSprite->SetActiveAnimation("Block");
		blockAnimation->SetScale((float)levelScale);

		block->SetTransform(transform);

		//RigidBodyComponent
		auto pRigidBody = std::make_shared<RigidBodyComponent>(blockSprite->GetAnimation().GetScaledWidth(),
			blockSprite->GetAnimation().GetScaledHeight(),
			false);
		pRigidBody->SetGameObject(block.get());

		block->AddComponent(pRigidBody, "RigidBody");

		scene.Add(block);
	}
	//Vertical blocks left
		for (int i = 0; i < 16; ++i)
		{
			auto block = std::make_shared<GameObject>();
			auto blockSprite = std::make_shared<SpriteComponent>();
			auto blockAnimation = std::make_shared<Animation>(1, 1);
			Transform transform{};

			block->AddComponent(blockSprite, "BlockSprite");
			block->SetTag("Block");

			blockAnimation->SetTexture("Level/Ladder.png");
			transform.SetPosition(8.f, ((i+1) * blockWidth), 0.f);

			blockSprite->AddAnimation(blockAnimation, "Block");
			blockSprite->SetActiveAnimation("Block");
			blockAnimation->SetScale((float)levelScale);

			block->SetTransform(transform);

			//RigidBodyComponent
			auto pRigidBody = std::make_shared<RigidBodyComponent>(blockSprite->GetAnimation().GetScaledWidth(),
				blockSprite->GetAnimation().GetScaledHeight(),
				false);
			pRigidBody->SetGameObject(block.get());

			block->AddComponent(pRigidBody, "RigidBody");

			scene.Add(block);
		}
		//right
		for (int i = 0; i < 16; ++i)
		{
			auto block = std::make_shared<GameObject>();
			auto blockSprite = std::make_shared<SpriteComponent>();
			auto blockAnimation = std::make_shared<Animation>(1, 1);
			Transform transform{};
			block->SetTag("Block");

			block->AddComponent(blockSprite, "BlockSprite");

			blockAnimation->SetTexture("Level/Ladder.png");
			transform.SetPosition(608.f, (i + 1) * blockWidth, 0.f);

			blockSprite->AddAnimation(blockAnimation, "Block");
			blockSprite->SetActiveAnimation("Block");
			blockAnimation->SetScale((float)levelScale);

			block->SetTransform(transform);

			//RigidBodyComponent
			auto pRigidBody = std::make_shared<RigidBodyComponent>(blockSprite->GetAnimation().GetScaledWidth(),
				blockSprite->GetAnimation().GetScaledHeight(),
				false);
			pRigidBody->SetGameObject(block.get());

			block->AddComponent(pRigidBody, "RigidBody");

			scene.Add(block);
		}
}

void dae::Tron::CreateBlocks(Scene& scene, int /*sceneNr*/, std::vector<Block>& blocks) const
{
	float levelScale = 1.5;
	float blockWidth = 16.f * levelScale;
	float blockWidthOffset = 8.f; // Offset with the level edges (which are at scale 1.5)
	float blockHeightOffset = 0.f;//Offset with the level edges (which are offset 50 + their height = 50 + 12 = 62)
	for (size_t i{}; i < blocks.size(); ++i)
	{

		auto block = std::make_shared<GameObject>();
		auto blockSprite = std::make_shared<SpriteComponent>();
		auto blockAnimation = std::make_shared<Animation>(1, 1);
		Transform transform{};
		block->SetTag("Block");
		block->AddComponent(blockSprite, "BlockSprite");

		blockAnimation->SetTexture("Level/Ladder.png");
		transform.SetPosition(blocks[i].column * blockWidth + blockWidthOffset, blocks[i].row * blockWidth + blockHeightOffset, 0.f);
		GameManager::GetInstance().SetGridBlock(blocks[i].column, blocks[i].row);
		blockSprite->AddAnimation(blockAnimation, "Block");
		blockSprite->SetActiveAnimation("Block");
		blockAnimation->SetScale((float)levelScale);

		block->SetTransform(transform);

		//RigidBodyComponent
		auto pRigidBody = std::make_shared<RigidBodyComponent>(blockSprite->GetAnimation().GetScaledWidth(),
			blockSprite->GetAnimation().GetScaledHeight(),
			false);
		pRigidBody->SetGameObject(block.get());

		block->AddComponent(pRigidBody, "RigidBody");

		scene.Add(block);
	}
	GameManager::GetInstance().GetGridBlock(Float2{ 30.f,50.f });
}
void dae::Tron::Run()
{
	// tell the resource manager where he can find the game data
	ResourceManager::GetInstance().Init("../Data/");
	m_Minigin.Initialize();
	Initialize();
	LoadGame();

	{
		m_Minigin.Run();
	}
	//Cleanup();
}

void dae::Tron::LoadLevel(GameMode /*gameMode*/, const std::string& /*levelName*/) const
{

}