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
	auto& input = InputManager::GetInstance();

	input.AddCommand(ControllerButton::LeftShoulder, new Shoot, KeyState::PRESSED, tronGo.get(), 0);
	input.AddCommand(ControllerButton::RightThumb, new Aim, KeyState::DOWN, tronGo.get(), 0);


	scene.Add(tronGo);
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
		transform.SetPosition(i * blockWidth + 8, 40.f, 0.f);
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
		transform.SetPosition(i * blockWidth + 8, 448.f, 0.f);

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
			transform.SetPosition(8.f, ((i+2) * blockWidth) + 16, 0.f);

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
			transform.SetPosition(608.f, (i + 2) * blockWidth + 16, 0.f);

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
	float blockWidthOffset = 32.f; // Offset with the level edges (which are at scale 1.5)
	float blockHeightOffset = 62.f;//Offset with the level edges (which are offset 50 + their height = 50 + 12 = 62)
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
	GameManager::GetInstance().GetGridBlock(Float2{ 10.f,50.f });
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