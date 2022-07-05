#include "BurgerTime.h"
#include <vector>
#include "BurgerTimeStructs.h"
#include "LevelParser.h"
#include "PeterPepper.h"
#include "SpriteComponent.h"
#include "Animation.h"
#include "ResourceManager.h"
#include "LivesDisplayComponent.h"
#include "TextComponent.h"
#include "PointsDisplayComponent.h"
#include "InputManager.h"
#include "PeterPepperCommand.h"
#include "Scene.h"
#include "PlatformComponent.h"
#include "LadderComponent.h"
#include "Renderer.h" 
#include "Sound.h"
#include "BurgerComponent.h"
#include "Enemy.h"
#include "TrayComponent.h"
#include "EnemySpawner.h"
#include "GameManager.h"
#include "BurgerSpawner.h"
#include <algorithm>
void dae::BurgerTime::Initialize()
{
	GameManager::GetInstance().SetBurgerTimeGame(this);
}

void dae::BurgerTime::LoadGame()
{
	auto& menuScene = SceneManager::GetInstance().CreateScene("MainMenu");
	SceneManager::GetInstance().SetActiveScene(&menuScene);
	Physics::GetInstance().SetSceneNr(0);
	CreateMenu(menuScene);
}

void dae::BurgerTime::CreateMenuButton(Scene& scene, Float2 position, GameMode gameMode, const  std::string& text) const
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
		if (dynamic_cast<GameObject*>(sceneObjects[i].get())->GetComponent<PeterPepperComponent>("PeterPepper").get())
		{
			dynamic_cast<GameObject*>(sceneObjects[i].get())->GetComponent<PeterPepperComponent>("PeterPepper")->AddObserver(buttonComp.get());
		}
	}
	buttonGo->SetTransform(position.x, position.y, -1);

	scene.Add(buttonGo);
}
void dae::BurgerTime::CreateHighScoreDisplay(Scene& scene) const
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
void dae::BurgerTime::CreateMenu(Scene& scene) const
{
	auto titleGo = std::make_shared<GameObject>();
	auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);

	auto textComp = std::make_shared<TextComponent>("BURGER TIME!!!!!!!", font);
	titleGo->AddComponent(textComp, "TextComp");
	titleGo->GetTransform().SetPosition(50, 0, 0);
	scene.Add(titleGo);
	CreatePeterPepperAndHUD(Transform(), scene, 0, false);

	CreateMenuButton(scene, Float2{ 150, 100 }, GameMode::SINGLE, "SinglePlayer");
	CreateMenuButton(scene, Float2{ 300, 150 }, GameMode::COOP, "Co-op");
	CreateMenuButton(scene, Float2{ 150, 200 }, GameMode::VERSUS, "Versus");
	CreateHighScoreDisplay(scene);

}


void dae::BurgerTime::CreateEvilPepper(Transform /*spawnPos*/, Scene& scene, int playerNr) const
{
	const float animationScale = 1.75f;
	auto evilPepper = std::make_shared<GameObject>();
	evilPepper->SetTag("EvilPeterPepper");
	std::shared_ptr<PeterPepperComponent> peterPepper = std::make_shared<PeterPepperComponent>(Float2{ 100,8 }, true);
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
	enemySprite->SetGameObject(evilPepper.get());
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
	evilPepper->AddComponent(enemySprite, "Sprite");

	evilPepper->AddComponent(peterPepper, "EvilPeterPepper");

	evilPepper->AddComponent(pRigidBody, "RigidBody");


	peterPepper->SetGameObject(evilPepper.get());
	peterPepper->SetOverlapEvent();
	peterPepper->SetOnTriggerExitEvent();

	peterPepper->AddObserver(enemySprite.get());
	pRigidBody->SetOffset(Float2{ 0.f,-3.f });

	auto& input = InputManager::GetInstance();

	input.AddCommand(ControllerButton::DPadRight, new MoveRight, KeyState::DOWN, evilPepper.get(), playerNr);
	input.AddCommand(ControllerButton::DPadLeft, new MoveLeft, KeyState::DOWN, evilPepper.get(), playerNr);
	input.AddCommand(ControllerButton::DPadDown, new MoveDown, KeyState::DOWN, evilPepper.get(), playerNr);
	input.AddCommand(ControllerButton::DPadUp, new MoveUp, KeyState::DOWN, evilPepper.get(), playerNr);
	evilPepper->SetTransform(300, 8, 0);
	peterPepper->SetInMenu(false);

	scene.Add(evilPepper);
}


void dae::BurgerTime::CreatePeterPepperAndHUD(Transform spawnPos, Scene& scene, int playerNr, bool andHUD) const
{
	float hudX, hudY;
	hudX = 0;
	hudY = 440;
	if (playerNr == 1)
	{
		hudX = 400;
	}

	//Create gameobject and components
	auto peterPepperGo = std::make_shared<GameObject>();
	peterPepperGo->SetTag("PeterPepper");
	Float2 level1Pos = { 100,8 };
	Float2 level2Pos = { 195,40 };
	Float2 level3Pos = { 100,360 };

	peterPepperGo->SetTransform(100, 8, 0);
	std::shared_ptr<PeterPepperComponent> peterPepper = nullptr;
	if (SceneManager::GetInstance().GetActiveSceneName() == "Level2")
	{
		peterPepperGo->SetTransform(100, 40, 0);
		peterPepper = std::make_shared<PeterPepperComponent>(level2Pos, false);
	}
	else
	{
		peterPepper = std::make_shared<PeterPepperComponent>(level1Pos, false);
	}


	peterPepperGo->SetTransform(spawnPos);

	auto peterPSprite = std::make_shared<SpriteComponent>();

	float animationScale = 1.75f;
	//Create animations 
	// 	//Down
	auto peterPAnimationDown = std::make_shared<Animation>(3, 3);
	peterPAnimationDown->SetTexture("PeterPepper/ClimbDownSprite.png");
	peterPAnimationDown->SetScale(animationScale);
	//Right
	auto peterPAnimationRight = std::make_shared<Animation>(3, 3);
	peterPAnimationRight->SetTexture("PeterPepper/LeftRightSprite.png");
	peterPAnimationRight->SetScale(animationScale);

	//Left
	auto peterPAnimationLeft = std::make_shared<Animation>(3, 3);
	peterPAnimationLeft->SetTexture("PeterPepper/LeftRightSprite.png");
	peterPAnimationLeft->SetScale(animationScale);
	//Up
	auto peterPAnimationUp = std::make_shared<Animation>(3, 3);
	peterPAnimationUp->SetTexture("PeterPepper/ClimbUpSprite.png");
	peterPAnimationUp->SetScale(animationScale);
	//Idle
	auto peterPAnimationIdle = std::make_shared<Animation>(1, 1);
	peterPAnimationIdle->SetTexture("PeterPepper/IdleSprite.png");
	peterPAnimationIdle->SetScale(animationScale);
	//Victory
	auto peterPAnimationVictory = std::make_shared<Animation>(2, 2);
	peterPAnimationVictory->SetTexture("PeterPepper/VictorySprite.png");
	peterPAnimationVictory->SetScale(animationScale);
	//Death
	auto peterPAnimationDeath = std::make_shared<Animation>(5, 5);
	peterPAnimationDeath->SetTexture("PeterPepper/DeathSprite.png");
	peterPAnimationDeath->SetScale(animationScale);
	//Add animation to sprite
	peterPSprite->AddAnimation(peterPAnimationRight, "RunRight");
	peterPAnimationRight->SetReversed(true);
	peterPSprite->AddAnimation(peterPAnimationLeft, "RunLeft");
	peterPSprite->AddAnimation(peterPAnimationUp, "Climb");
	peterPSprite->AddAnimation(peterPAnimationDown, "Descend");
	peterPSprite->AddAnimation(peterPAnimationIdle, "Idle");
	peterPSprite->AddAnimation(peterPAnimationVictory, "Victory");
	peterPSprite->AddAnimation(peterPAnimationDeath, "Death");

	peterPSprite->SetGameObject(peterPepperGo.get());
	peterPSprite->SetActiveAnimation("RunRight");

	//RigidbodyComponent
	auto pRigidBody = std::make_shared<RigidBodyComponent>(peterPSprite->GetAnimation().GetScaledWidth(),
		peterPSprite->GetAnimation().GetScaledHeight(),
		true);
	pRigidBody->SetGameObject(peterPepperGo.get());
	pRigidBody->SetOffset(Float2{ 0.f,-3.f });


	//Add everything to scene
	peterPepperGo->AddComponent(peterPSprite, "Sprite");

	peterPepperGo->AddComponent(peterPepper, "PeterPepper");

	peterPepperGo->AddComponent(pRigidBody, "RigidBody");
	//peterPepperGo->AddComponent(pMovement, "Movement");

	peterPepper->SetGameObject(peterPepperGo.get());
	peterPepper->SetOverlapEvent();
	peterPepper->SetOnTriggerExitEvent();

	scene.Add(peterPepperGo);

	if (andHUD && playerNr == 0)
	{

		auto livesDisplay = std::make_shared<GameObject>();
		auto smallFont = ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
		auto lifeComp = std::make_shared<LivesDisplayComponent>();
		auto textComp = std::make_shared<TextComponent>("Lives: 3", smallFont);
		livesDisplay->AddComponent(lifeComp, "LifeComponent");
		livesDisplay->AddComponent(textComp, "TextComponent");
		textComp->SetPosition(hudX, hudY);
		scene.Add(livesDisplay);

		auto pointsDisplay = std::make_shared<GameObject>();
		auto pointComp = std::make_shared<PointsDisplayComponent>();
		auto textCompPts = std::make_shared<TextComponent>("Points: 0", smallFont);
		pointsDisplay->AddComponent(pointComp, "PointsComponent");
		pointsDisplay->AddComponent(textCompPts, "TextComponent");
		textCompPts->SetPosition(hudX, hudY + 20);
		scene.Add(pointsDisplay);

		GameManager::GetInstance().AddObserver(lifeComp.get());
		GameManager::GetInstance().AddObserver(pointComp.get());
	}

	peterPepper->AddObserver(peterPSprite.get());
	//MakeCommands(playerNr,peterPepperGo.get(), andHUD);
	auto& input = InputManager::GetInstance();

	input.AddCommand(ControllerButton::DPadRight, new MoveRight, KeyState::DOWN, peterPepperGo.get(), playerNr);
	input.AddCommand(ControllerButton::DPadLeft, new MoveLeft, KeyState::DOWN, peterPepperGo.get(), playerNr);
	input.AddCommand(ControllerButton::DPadDown, new MoveDown, KeyState::DOWN, peterPepperGo.get(), playerNr);
	input.AddCommand(ControllerButton::DPadUp, new MoveUp, KeyState::DOWN, peterPepperGo.get(), playerNr);

	input.AddCommand(ControllerButton::Nothing, new Idle, KeyState::NOTHING, peterPepperGo.get(), playerNr);

	if (!andHUD)
	{
		input.AddCommand((ControllerButton::ButtonA), new Select, KeyState::RELEASED, peterPepperGo.get(), playerNr);

	}
	else
	{
		input.AddCommand(ControllerButton::ButtonB, new Pepper, KeyState::PRESSED, peterPepperGo.get(), playerNr);
		peterPepper->SetInMenu(false);
	}



	if (SceneManager::GetInstance().GetActiveSceneName() == "Level2")
	{
		peterPepperGo->SetTransform(level2Pos.x, level2Pos.y, 0);
	}
	else if (SceneManager::GetInstance().GetActiveSceneName() == "Level1")
	{
		peterPepperGo->SetTransform(level1Pos.x, level1Pos.y, 0);

	}
	else
	{
		peterPepperGo->SetTransform(level3Pos.x, level3Pos.y, 0);

	}
	peterPepper->Initialize(scene);

}

void dae::BurgerTime::MakeEnemySpawner(std::vector<Float2> spawnPositions) const
{
	auto spawnerGo = std::make_shared<GameObject>();
	spawnerGo->SetTag("Spawner");
	std::shared_ptr<EnemySpawner> spawner = nullptr;
	if (SceneManager::GetInstance().GetActiveSceneName() == "Level3")
	{
		 spawner = std::make_shared<EnemySpawner>(Difficulty::HARD);

	}
	else
		 spawner = std::make_shared<EnemySpawner>(Difficulty::EASY);

	spawner->SetSpawnPositions(spawnPositions);
	spawnerGo->AddComponent(spawner, "Spawner");

	SceneManager::GetInstance().GetActiveScene().Add(spawnerGo);
}

void dae::BurgerTime::MakeBurgerSpawner(std::vector<Float2> spawnPositions) const
{
	auto spawnerGo = std::make_shared<GameObject>();

	auto spawner = std::make_shared<BurgerSpawner>();

	spawner->SetSpawnPositions(spawnPositions);
	spawnerGo->AddComponent(spawner, "Spawner");

	SceneManager::GetInstance().GetActiveScene().Add(spawnerGo);
}
std::vector<dae::Float2> dae::BurgerTime::ParseLevel(Scene& scene, int sceneNr, const std::string& levelName) const
{

	std::vector<Platform> platforms;
	std::vector<Ladder> ladders;
	std::vector<Float2> spawnPositions;
	std::vector<Burger> burgers;
	std::string fullName = "../Data/Level/";
	fullName.append(levelName);
	fullName.append(".txt");
	ParseLevelFile(fullName, platforms, ladders, spawnPositions, burgers);

	//int levelScale = 2;
	//Is half 
	//int scalingIncrease = 8 * levelScale;
	//float platformWidth = 16.f * levelScale;
	auto enemySpawnPositions = MakeLaddersAndPlatforms(scene, ladders, platforms, sceneNr);
	MakeBurgers(scene, burgers, sceneNr);
	//for now just one spawn position

	return enemySpawnPositions;
}

std::vector<dae::Float2> dae::BurgerTime::MakeLaddersAndPlatforms(Scene& scene, const std::vector<Ladder>& ladders, const std::vector<Platform>& platforms, int /*sceneNr*/) const
{

	//Every two platforms, the platfors get pushed 16 pixels further away because of the bug platform.
	//This scalingIncrease goes +16 every 2 platforms
	//And it gets reset when the rowNumber changes
	//Same goes for the ladders

	int levelScale = 2;
	//Is half 
	int scalingIncrease = 8 * levelScale;
	float platformWidth = 16.f * levelScale;

	std::vector<Float2> enemySpawnPositions;
	for (size_t i{}; i < platforms.size(); ++i)
	{

		auto platform = std::make_shared<GameObject>();
		auto platformSprite = std::make_shared<SpriteComponent>();
		auto platformAnimation = std::make_shared<Animation>(1, 1);
		platform->SetTag("Platform");
		platformSprite->SetGameObject(platform.get());
		Transform transform{};
		if (platforms[i].column % 2 == 0)
		{
			platformAnimation->SetTexture("Level/SmallPlatform.png");
			transform.SetPosition((platforms[i].column) * platformWidth + scalingIncrease * platforms[i].column, (platforms[i].row + 1) * platformWidth, 0.f);
		}
		else
		{
			platformAnimation->SetTexture("Level/BigPlatform.png");
			transform.SetPosition((platforms[i].column) * platformWidth + scalingIncrease * (platforms[i].column - 1), (platforms[i].row + 1) * platformWidth, 0.f);
		}
		platformSprite->AddAnimation(platformAnimation, "Platform");
		platformSprite->SetActiveAnimation("Platform");
		platformAnimation->SetScale((float)levelScale);

		platform->SetTransform(transform);

		//RigidbodyComponent
		auto pRigidBody = std::make_shared<RigidBodyComponent>(platformSprite->GetAnimation().GetScaledWidth(),
			platformSprite->GetAnimation().GetScaledHeight(),
			false);
		pRigidBody->SetGameObject(platform.get());

		//Platform component
		auto pPlatform = std::make_shared<PlatformComponent>();
		//If this platform has one in the column left to it, set it's previous platform to true
		if (platforms[i].column != 0 && platforms.size() > 1 && i != 0)
		{
			//If the row is the same
			if (platforms[i - 1].row == platforms[i].row && platforms[i - 1].column == platforms[i].column - 1)
			{
				pPlatform->SetHasPrevious(true);
			}
		}
		//Kind of redundant since it gets initialized to false anyway, but whatever
		else
		{
			pPlatform->SetHasPrevious(false);
		}
		//Same for the right and next platform
		if (platforms[i].column != 11 && i != platforms.size() - 1)
		{
			if (platforms[i].row == platforms[i + 1].row && platforms[i].column + 1 == platforms[i + 1].column)
			{
				pPlatform->SetHasNext(true);
			}
		}
		else
		{
			pPlatform->SetHasNext(false);
		}
		platform->AddComponent(pPlatform, "PlatformComp");
		platform->AddComponent(platformSprite, "PlatformSprite");
		platform->AddComponent(pRigidBody, "RigidBody");

		scene.Add(platform);

		if (platforms[i].column == 3)
		{
			enemySpawnPositions.push_back(Float2{ 0, (platforms[i].row + 1) * platformWidth - 28 });
		}
		else if (platforms[i].column == 9)
		{
			enemySpawnPositions.push_back(Float2{ 608, (platforms[i].row + 1) * platformWidth - 28 });
		}
	}
	//Depending on the column, the ladder will have to be shifted forwards or backward in order to center it
	//In the files, this is a 2 pixel shift
	int ladderShift = 2 * levelScale;


	//New ladder position calculation
	for (size_t i{}; i < ladders.size(); ++i)
	{
		auto ladder = std::make_shared<GameObject>();
		std::shared_ptr<SpriteComponent>ladderSprite = std::make_shared<SpriteComponent>();
		ladder->SetTag("Ladder");
		auto ladderAnimation = std::make_shared<Animation>(1, 1);
		ladderAnimation->SetTexture("Level/Ladder.png");
		ladderAnimation->SetScale((float)levelScale);

		ladderSprite->SetGameObject(ladder.get());

		ladderSprite->AddAnimation(ladderAnimation, "Ladder");
		ladderSprite->SetActiveAnimation("Ladder");

		Transform transform{};
		//+4 because the ladders are otherwise off center
		transform.SetPosition(ladders[i].column * platformWidth + ((platformWidth * ladders[i].column / 2)) + ladderShift, (ladders[i].row + 1) * platformWidth, 0.f);
		ladder->SetTransform(transform);
		ladder->AddComponent(ladderSprite, "Ladder");

		//RigidbodyComponent
		auto pRigidBody = std::make_shared<RigidBodyComponent>(ladderSprite->GetAnimation().GetScaledWidth(),
			ladderSprite->GetAnimation().GetScaledHeight(),
			false);
		pRigidBody->SetGameObject(ladder.get());

		ladder->AddComponent(pRigidBody, "RigidBody");


		//ladder component
		auto pLadder = std::make_shared<LadderComponent>();

		for (size_t k{}; k < ladders.size(); ++k)
		{
			for (size_t j{}; j < ladders.size(); ++j)
			{
				//If they're not the same ladder
				if (&ladders[j] != &ladders[k])
				{
					//Check for ladders higher
					if (i != 0)
					{
						if (ladders[i].column == ladders[j].column)
						{
							if (ladders[i].row - 1 == ladders[j].row)
							{
								pLadder->SetHasUp(true);
							}
						}
					}
					if (j != 0)
					{
						if (ladders[j].column == ladders[i].column)
						{
							if (ladders[j].row + 1 == ladders[i].row)
							{
								pLadder->SetHasUp(true);
							}
						}
					}
				}
			}
		}
		ladder->AddComponent(pLadder, "LadderComp");
		scene.Add(ladder);
	}
	return enemySpawnPositions;
}

void dae::BurgerTime::MakeBurgers(Scene& scene, const std::vector<Burger>& burgers, int sceneNr) const
{
	int levelScale = 2;
	//Is half 
	int scalingIncrease = 8 * levelScale;
	float platformWidth = 16.f * levelScale;
	//int nextPlatforms = 0;
	int column;
	int prevColumn = -1;
	int nrBurgers = 0;

	std::vector<Float2> burgerSpawnPositions;
	for (size_t i{}; i < burgers.size(); ++i)
	{
		if (burgers[i].partName != "")
		{
			column = burgers[i].column;
			if (i > 0)
			{
				prevColumn = burgers[i - 1].column;
			}
			if (column != prevColumn)
			{
				CreateTray(scene, sceneNr, Float2((burgers[i].column) * platformWidth + scalingIncrease * (burgers[i].column - 1) + 1, 440));
				burgerSpawnPositions.push_back(Float2((burgers[i].column) * platformWidth + scalingIncrease * (burgers[i].column - 1) + 1, -50.f));
			}
			if (GameManager::GetInstance().GetGameMode() != GameMode::VERSUS)
			{
				auto burger = std::make_shared<GameObject>();
				auto burgerSprite = std::make_shared<SpriteComponent>();
				auto burgerAnimation = std::make_shared<Animation>(1, 1);
				burger->SetTag("Burger");
				burgerSprite->SetGameObject(burger.get());
				Transform transform{};

				std::string burgerPart = "Hamburger/";
				burgerPart.append(burgers[i].partName);
				burgerPart += ".png";
				burgerAnimation->SetTexture(burgerPart);
				transform.SetPosition((burgers[i].column) * platformWidth + scalingIncrease * (burgers[i].column - 1) + 1, (((burgers[i].row + 1) * platformWidth) - 7 * levelScale), 0.f);
				//transform.SetPosition(burgers[i].column * platformWidth + (((platformWidth/2) * burgers[i].column / 2)), ((burgers[i].row + 1) * platformWidth * 1.5f) - 7 * levelScale, 0.f);

				if (burgers[i].partName == "Top_bun")
				{
					burgerSprite->AddAnimation(burgerAnimation, "Top_bun");

					burgerSprite->SetActiveAnimation("Top_bun");
				}
				else
				{

					burgerSprite->AddAnimation(burgerAnimation, "Burger");
					burgerSprite->SetActiveAnimation("Burger");
				}
				burgerAnimation->SetScale((float)levelScale);

				burger->SetTransform(transform);

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

				scene.Add(burger);

				if (burgers[i].partName == "Top_bun")
				{
					++nrBurgers;
				}
			}

		}
	}
	GameManager::GetInstance().SetNrOfBurgers(nrBurgers);

	if (GameManager::GetInstance().GetGameMode() == GameMode::VERSUS)
	{
		MakeBurgerSpawner(burgerSpawnPositions);
	}
}

void dae::BurgerTime::CreateTray(Scene& scene, int /*sceneNr*/, Float2 position) const
{
	float levelScale = 2.f;
	auto trayGo = std::make_shared<GameObject>();
	trayGo->SetTransform(position.x, position.y, 0.f);
	trayGo->SetTag("Tray");
	auto animation = std::make_shared<Animation>(1, 1);
	animation->SetTexture("Hamburger/Tray.png");
	animation->SetScale(levelScale);
	auto sprite = std::make_shared<SpriteComponent>();

	sprite->SetGameObject(trayGo.get());
	sprite->AddAnimation(animation, "Tray");
	sprite->SetActiveAnimation("Tray");

	trayGo->AddComponent(sprite, "Sprite");
	auto rigidBody = std::make_shared<RigidBodyComponent>(sprite->GetAnimation().GetScaledWidth(),
		sprite->GetAnimation().GetScaledHeight(),
		true);
	rigidBody->SetGameObject(trayGo.get());
	trayGo->AddComponent(rigidBody, "RigidBody");

	auto trayComp = std::make_shared<TrayComponent>();
	trayComp->SetGameObject(trayGo.get());
	trayComp->SetOverlapEvent();
	trayGo->AddComponent(trayComp, "TrayComp");
	scene.Add(trayGo);
}

void dae::BurgerTime::LoadLevel1(GameMode gameMode, const std::string& levelName) const
{
	if (levelName == "MainMenu")
	{
		auto& menuScene = SceneManager::GetInstance().CreateScene("MainMenu");
		SceneManager::GetInstance().SetActiveScene(&menuScene);
		CreateMenu(menuScene);
		return;
	}
	auto& levelScene = SceneManager::GetInstance().CreateScene(levelName);
	SceneManager::GetInstance().SetActiveScene(&levelScene);

	std::vector<Float2> enemySpawnPositions = ParseLevel(levelScene, 0, levelName);

	CreatePeterPepperAndHUD(Transform(), levelScene, 0, true);
	if (gameMode == GameMode::COOP)
	{
		CreatePeterPepperAndHUD(Transform(), levelScene, 1, true);
	}
	else if (gameMode == GameMode::VERSUS)
	{
		CreateEvilPepper(Transform(), levelScene, 1);
	}
	MakeEnemySpawner(enemySpawnPositions);
	levelScene.Initialize();

}


void dae::BurgerTime::Run()
{

	// tell the resource manager where he can find the game data
	ResourceManager::GetInstance().Init("../Data/");
	m_Minigin.Initialize();
	Initialize();
	LoadGame();

	{
		m_Minigin.Run();
	}
	Cleanup();
}

void dae::BurgerTime::Cleanup()
{
}