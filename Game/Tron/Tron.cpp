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
#include "Teleporter.h"
#include "Enemy.h"
#include <TextComponent.h>
#include "ButtonComponent.h"
#include <algorithm>
void dae::Tron::Initialize()
{
	GameManager::GetInstance().SetTronGame(this);
}

void dae::Tron::LoadGame()
{
	auto& menuScene = SceneManager::GetInstance().CreateScene("MainMenu");
	SceneManager::GetInstance().SetActiveScene(&menuScene);
	Physics::GetInstance().SetSceneNr(0);
	CreateMenu(menuScene);
}
void dae::Tron::CreateHighScoreDisplay(Scene& scene) const
{
	auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 24);

	std::ifstream file("../Data/Menu/HighScores.txt");
	std::string sCommand;
	std::vector<int> highScores;
	if (file)
	{
		while (!file.eof())
		{
			file >> sCommand;
			highScores.push_back(std::stoi(sCommand));
		}
		std::sort(highScores.begin(), highScores.end(), std::greater<int>());
	}
	Float2 position{ 400, 50 };
	for (size_t i{}; i < highScores.size(); ++i)
	{
		if (i > 4)
			break;

		auto highScore = std::make_shared<GameObject>();
		auto textComp = std::make_shared<TextComponent>(std::to_string(highScores[i]), font);
		highScore->AddComponent(textComp, "Text");
		textComp->SetPosition(position.x, position.y);
		position.y += 30;
		scene.Add(highScore);
	}


}
void dae::Tron::CreateMenu(Scene& scene) const
{
	auto titleGo = std::make_shared<GameObject>();
	auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);

	auto textComp = std::make_shared<TextComponent>("TRON TIME!!!!!!!", font);
	titleGo->AddComponent(textComp, "TextComp");
	titleGo->GetTransform().SetPosition(50, 0, 0);
	scene.Add(titleGo);
	CreateTronAndHUD(scene, 0, false);

	CreateMenuButton(scene, Float2{ 150, 100 }, GameMode::SINGLE, "SinglePlayer");
	CreateMenuButton(scene, Float2{ 300, 150 }, GameMode::COOP, "Co-op");
	CreateMenuButton(scene, Float2{ 150, 200 }, GameMode::VERSUS, "Versus");
	CreateHighScoreDisplay(scene);

}

void dae::Tron::MakeEnemy(Scene& scene, EnemyType enemyType) const
{
	const float animationScale = 1.5f;
	auto enemyGo = std::make_shared<GameObject>();
	enemyGo->SetTag("Enemy");
	auto descendAnim = std::make_shared<Animation>(2, 2);
	auto walkLeftAnim = std::make_shared<Animation>(2, 2);
	auto climbAnim = std::make_shared<Animation>(2, 2);
	auto walkRightAnim = std::make_shared<Animation>(2, 2);
	auto deathAnim = std::make_shared<Animation>(4, 4);
	auto enemySprite = std::make_shared<SpriteComponent>();
	enemySprite->SetGameObject(enemyGo.get());
	enemySprite->AddAnimation(climbAnim, "Climb");
	enemySprite->AddAnimation(descendAnim, "Descend");
	enemySprite->AddAnimation(walkRightAnim, "WalkRight");
	enemySprite->AddAnimation(deathAnim, "Death");
	enemySprite->AddAnimation(walkLeftAnim, "WalkLeft");
	enemyGo->AddComponent(enemySprite, "Sprite");

	climbAnim->SetScale(animationScale);
	descendAnim->SetScale(animationScale);
	walkLeftAnim->SetScale(animationScale);
	walkRightAnim->SetReversed(true);
	walkRightAnim->SetScale(animationScale);
	deathAnim->SetScale(animationScale);

	climbAnim->SetTexture("Enemies/Sausage_Climb.png");
	descendAnim->SetTexture("Enemies/Sausage_Descend.png");
	walkLeftAnim->SetTexture("Enemies/Sausage_Walk.png");
	walkRightAnim->SetTexture("Enemies/Sausage_Walk.png");
	deathAnim->SetTexture("Enemies/Sausage_Kill.png");

	if (enemyType == EnemyType::RECOGNIZER)
	{

		climbAnim->SetTexture("Enemies/Egg_Climb.png");
		descendAnim->SetTexture("Enemies/Egg_Descend.png");
		walkLeftAnim->SetTexture("Enemies/Egg_Walk.png");
		walkRightAnim->SetTexture("Enemies/Egg_Walk.png");
		deathAnim->SetTexture("Enemies/Egg_Kill.png");
	}
	enemySprite->SetActiveAnimation("WalkRight");
	auto pRigidBody = std::make_shared<RigidBodyComponent>(enemySprite->GetAnimation().GetScaledWidth(),
		enemySprite->GetAnimation().GetScaledHeight(),
		true);
	enemyGo->AddComponent(pRigidBody, "RigidBody");

	//EnemyType enemyType = EnemyType::TANK;

	auto enemy = std::make_shared<Enemy>(enemyType);
	enemy->SetGameObject(enemyGo.get());
	enemy->SetOverlapEvent();
	enemy->AddObserver(enemySprite.get());
	enemyGo->AddComponent(enemy, "Enemy");
	enemyGo->SetTransform(296.f, 191.f, 0.f);
	scene.Add(enemyGo);
	enemy->AddObserver(enemySprite.get());
	GameManager::GetInstance().AddEnemy();
}

void dae::Tron::CreateMenuButton(Scene& scene, Float2 position, GameMode gameMode, const  std::string& text) const
{
	auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 16);
	auto buttonGo = std::make_shared<GameObject>();
	buttonGo->SetTag("Button");
	auto buttonAnim = std::make_shared<Animation>(1, 1);
	buttonAnim->SetTexture("Menu/Button.png");

	auto buttonTextComp = std::make_shared<TextComponent>(text, font);
	buttonGo->AddComponent(buttonTextComp, "TextComp");
	buttonTextComp->SetGameObject(buttonGo.get());
	buttonTextComp->SetPosition(position.x, position.y);

	auto buttonSprite = std::make_shared<SpriteComponent>();
	buttonSprite->SetGameObject(buttonGo.get());
	buttonSprite->AddAnimation(buttonAnim, "button");
	buttonSprite->SetActiveAnimation("button");
	buttonGo->AddComponent(buttonSprite, "ButtonSprite");

	auto buttonComp = std::make_shared<ButtonComponent>("Level1", gameMode);
	buttonGo->AddComponent(buttonComp, "ButtonComp");

	auto buttonRigidBody = std::make_shared<RigidBodyComponent>(buttonAnim->GetScaledWidth(), buttonAnim->GetScaledHeight(), false);
	buttonGo->AddComponent(buttonRigidBody, "RigidBody");
	//buttonRigidBody->SetGameObject(buttonGo.get());
	auto sceneObjects = scene.GetSceneObjects();
	for (size_t i = 0; i < sceneObjects.size(); ++i)
	{
		if (dynamic_cast<GameObject*>(sceneObjects[i].get())->GetComponent<PlayerComponent>("PlayerComp").get())
		{
			dynamic_cast<GameObject*>(sceneObjects[i].get())->GetComponent<PlayerComponent>("PlayerComp")->AddObserver(buttonComp.get());
		}
	}
	buttonGo->SetTransform(position.x, position.y, -1);

	scene.Add(buttonGo);
}

void dae::Tron::CreateEvilTron(Scene& scene, int playerNr) const
{
	const float animationScale = 1.75f;
	auto evilTron = std::make_shared<GameObject>();
	evilTron->SetTag("EvilPlayer");
	evilTron->SetTransform(300.f, 20.f, 0);

	std::shared_ptr<PlayerComponent> player = std::make_shared<PlayerComponent>(true, playerNr);
	auto climbAnim = std::make_shared<Animation>(2, 2);
	climbAnim->SetTexture("Enemies/Sausage_Climb.png");
	climbAnim->SetScale(animationScale);

	auto descendAnim = std::make_shared<Animation>(2, 2);
	descendAnim->SetTexture("Enemies/Sausage_Descend.png");
	descendAnim->SetScale(animationScale);

	auto walkLeftAnim = std::make_shared<Animation>(2, 2);
	walkLeftAnim->SetTexture("Enemies/Sausage_Walk.png");
	walkLeftAnim->SetScale(animationScale);

	auto walkRightAnim = std::make_shared<Animation>(2, 2);
	walkRightAnim->SetTexture("Enemies/Sausage_Walk.png");
	walkRightAnim->SetScale(animationScale);
	walkRightAnim->SetReversed(true);

	auto deathAnim = std::make_shared<Animation>(4, 4);
	deathAnim->SetTexture("Enemies/Sausage_Kill.png");
	deathAnim->SetScale(animationScale);

	auto stunAnim = std::make_shared<Animation>(2, 2);
	stunAnim->SetTexture("Enemies/Sausage_Peppered.png");
	stunAnim->SetScale(animationScale);

	auto enemySprite = std::make_shared<SpriteComponent>();
	enemySprite->SetGameObject(evilTron.get());
	enemySprite->AddAnimation(climbAnim, "Climb");
	enemySprite->AddAnimation(descendAnim, "Descend");
	enemySprite->AddAnimation(walkRightAnim, "RunRight");
	enemySprite->AddAnimation(deathAnim, "Death");
	enemySprite->AddAnimation(walkLeftAnim, "RunLeft");
	enemySprite->AddAnimation(stunAnim, "Stunned");
	enemySprite->SetActiveAnimation("RunLeft");


	//RigidbodyComponent
	auto pRigidBody = std::make_shared<RigidBodyComponent>(enemySprite->GetAnimation().GetScaledWidth(),
		enemySprite->GetAnimation().GetScaledHeight(),
		true);



	//Add everything to scene
	evilTron->AddComponent(enemySprite, "Sprite");
	player->SetGameObject(evilTron.get());
	evilTron->AddComponent(player, "PlayerComp");

	evilTron->AddComponent(pRigidBody, "RigidBody");


	player->SetGameObject(evilTron.get());
	player->SetOverlapEvent();
	player->SetOnTriggerExitEvent();

	player->AddObserver(enemySprite.get());
	pRigidBody->SetOffset(Float2{ 0.f,-3.f });

	auto& input = InputManager::GetInstance();

	input.AddCommand(ControllerButton::DPadDown, new Move(MovementDirection::DOWN), KeyState::DOWN, evilTron.get(), playerNr, '5');
	input.AddCommand(ControllerButton::DPadLeft, new Move(MovementDirection::LEFT), KeyState::DOWN, evilTron.get(), playerNr, '4');
	input.AddCommand(ControllerButton::DPadRight, new Move(MovementDirection::RIGHT), KeyState::DOWN, evilTron.get(), playerNr, '6');
	input.AddCommand(ControllerButton::DPadUp, new Move(MovementDirection::UP), KeyState::DOWN, evilTron.get(), playerNr, '8');
	input.AddCommand(ControllerButton::ButtonX, new Die, KeyState::PRESSED, evilTron.get(), playerNr, '-');
	input.AddCommand(ControllerButton::LeftShoulder, new Shoot, KeyState::PRESSED, evilTron.get(), playerNr, '+');
	player->Initialize();

	scene.Add(evilTron);
}
void dae::Tron::CreateTronAndHUD(Scene& scene, int playerNr, bool andHUD) const
{
	auto tronGo = std::make_shared<GameObject>();
	auto playerComponent = std::make_shared<PlayerComponent>(false, playerNr);
	tronGo->SetTag("Player");
	tronGo->AddComponent(playerComponent, "PlayerComp");

	tronGo->SetTransform(300.f, 145.f, 0.f);

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
	tronAnimationDeath->SetFramesSec(0.3f);
	tronAnimationVictory->SetFramesSec(0.4f);
	tronSprite->SetGameObject(tronGo.get());
	tronSprite->SetActiveAnimation("RunRight");

	tronGo->AddComponent(tronSprite, "Sprite");

	auto pRigidBody = std::make_shared<RigidBodyComponent>(tronSprite->GetAnimation().GetScaledWidth(),
		tronSprite->GetAnimation().GetScaledHeight(),
		true);
	tronGo->AddComponent(pRigidBody, "RigidBody");
	scene.Add(tronGo);
	playerComponent->SetOverlapEvent();
	playerComponent->SetOnTriggerExitEvent();
	playerComponent->AddObserver(tronSprite.get());

	auto& input = InputManager::GetInstance();
	playerComponent->Initialize();

	input.AddCommand(ControllerButton::DPadDown, new Move(MovementDirection::DOWN), KeyState::DOWN, tronGo.get(), playerNr, 'S');
	input.AddCommand(ControllerButton::DPadLeft, new Move(MovementDirection::LEFT), KeyState::DOWN, tronGo.get(), playerNr, 'Q');
	input.AddCommand(ControllerButton::DPadRight, new Move(MovementDirection::RIGHT), KeyState::DOWN, tronGo.get(), playerNr, 'D');
	input.AddCommand(ControllerButton::DPadUp, new Move(MovementDirection::UP), KeyState::DOWN, tronGo.get(), playerNr, 'Z');
	if (!andHUD)
	{
		input.AddCommand(ControllerButton::ButtonA, new Select, KeyState::PRESSED, tronGo.get(), playerNr, ' ');
	}
	else
	{
		input.AddCommand(ControllerButton::LeftShoulder, new Shoot, KeyState::PRESSED, tronGo.get(), playerNr, ' ');
		input.AddCommand(ControllerButton::Start, new SkipLevel, KeyState::PRESSED, tronGo.get(), playerNr, 'P');
		input.AddCommand(ControllerButton::ButtonX, new Die, KeyState::PRESSED, tronGo.get(), playerNr, 'X');
	}
}
void dae::Tron::CreateTeleporter(Scene& scene) const
{
	auto tpGo = std::make_shared<GameObject>();

	auto sprite = std::make_shared<SpriteComponent>();

	float animationScale = 1.f;
	auto animation = std::make_shared<Animation>(1, 1);
	animation->SetTexture("Hamburger/Hamburger.png");
	animation->SetScale(animationScale);
	sprite->SetGameObject(tpGo.get());
	sprite->AddAnimation(animation, "Burger");
	sprite->SetActiveAnimation("Burger");
	tpGo->AddComponent(sprite, "Sprite");

	auto tp = std::make_shared<TeleporterComponent>();

	tpGo->AddComponent(tp, "Teleporter");
	auto pRigidBody = std::make_shared<RigidBodyComponent>(sprite->GetAnimation().GetScaledWidth(),
		sprite->GetAnimation().GetScaledHeight(),
		true);
	pRigidBody->SetGameObject(tpGo.get());
	tpGo->AddComponent(pRigidBody, "RigidBody");
	tpGo->SetTransform(296.f, 191.f, 0.f);
	tp->SetOverlapEvent();
	scene.Add(tpGo);
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
		transform.SetPosition(8.f, ((i + 1) * blockWidth), 0.f);

		blockSprite->AddAnimation(blockAnimation, "Block");
		blockSprite->SetActiveAnimation("Block");
		blockAnimation->SetScale((float)levelScale);

		block->SetTransform(transform);
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

void dae::Tron::LoadLevel(GameMode gameMode, const std::string& levelName) const
{
	//InputManager::GetInstance().ClearCommands();
	auto& newScene = SceneManager::GetInstance().CreateScene(levelName);
	SceneManager::GetInstance().SetActiveScene(&newScene);
	Physics::GetInstance().SetSceneNr(0);
	if (levelName == "MainMenu")
	{
		InputManager().GetInstance().SetPlayer(nullptr, 1);
		CreateMenu(newScene);
		return;
	}
	SceneManager::GetInstance().SetActiveScene(&newScene);
	Physics::GetInstance().SetSceneNr(0);
	//CreateMenu(menuScene);
	ParseLevel(newScene, 0, levelName);
	CreateTronAndHUD(newScene, 0, true);
	CreateTeleporter(newScene);
	MakeEnemy(newScene, EnemyType::TANK);
	MakeEnemy(newScene, EnemyType::TANK);
	MakeEnemy(newScene, EnemyType::RECOGNIZER);

	if (gameMode == GameMode::COOP)
	{
		CreateTronAndHUD(newScene, 1, true);
	}
	else if (gameMode == GameMode::VERSUS)
	{
		CreateEvilTron(newScene, 1);
	}
}