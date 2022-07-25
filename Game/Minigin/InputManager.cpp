#include "MiniginPCH.h"
#include "InputManager.h"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <Xinput.h>
#include <thread>
#pragma comment(lib, "xinput.lib")
#include <backends/imgui_impl_sdl.h>
#include <backends/imgui_impl_opengl2.h>
#include "Sound.h"

#define NrOfPlayers 2
//pImpl try
class dae::InputManager::Impl
{

	XINPUT_STATE m_CurrentState[NrOfPlayers]{};
	XINPUT_STATE m_PreviousState[NrOfPlayers]{};

	unsigned char m_CurrentKeyboardState[NrOfPlayers][256] = {};
	unsigned char m_PreviousKeyboardState[NrOfPlayers][256] = {};
	//The assigned buttons for each player (it'd be better if it was an unordered_set but whatevs)

	std::map<std::pair<std::pair<ControllerButton, unsigned char>, KeyState>, std::unique_ptr<Command>> m_ButtonCommands[NrOfPlayers];

public:

	Impl()
	{
		for (int i{}; i < NrOfPlayers; ++i)
		{
			memset(m_CurrentKeyboardState[i], 0, 256 * sizeof(unsigned char));
			memset(m_PreviousKeyboardState[i], 0, 256 * sizeof(unsigned char));
		}
	};
	~Impl() = default;
	Impl(const Impl& other) = delete;
	Impl(Impl&& other) = delete;
	Impl& operator=(const Impl& other) = delete;
	Impl& operator=(Impl&& other) = delete;
	std::map<std::pair<std::pair<ControllerButton, unsigned char>, KeyState>, std::unique_ptr<Command>>& GetButtonCommands(int idx) { return m_ButtonCommands[idx]; }
	bool ProcessInput()
	{


		SDL_Event e;
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) {
				return false;
			}
			if (e.type == SDL_KEYDOWN) {
				if (e.key.keysym.sym == SDLK_p)
					ServiceLocator::GetSoundSystem().Play(0, 100);
				if (e.key.keysym.sym == SDLK_m)
					ServiceLocator::GetSoundSystem().Play(1, 100);
				if (e.key.keysym.sym == SDLK_l)
					ServiceLocator::GetSoundSystem().Play(2, 100);
			}
			if (e.type == SDL_MOUSEBUTTONDOWN) {

			}

			ImGui_ImplSDL2_ProcessEvent(&e);
		}
		for (int i{}; i < NrOfPlayers; ++i)
		{
			CopyMemory(&m_PreviousState[i], &m_CurrentState[i], sizeof(XINPUT_STATE));
			ZeroMemory(&m_CurrentState[i], sizeof(XINPUT_STATE));
			XInputGetState(i, &m_CurrentState[i]);

			memcpy(m_PreviousKeyboardState[i], m_CurrentKeyboardState[i], 256 * sizeof(BYTE));
			if (!GetKeyboardState(m_CurrentKeyboardState[i])) {
				GetLastError();
			}
		}
		POINT mousePos{};

		GetCursorPos(&mousePos);
		if (&mousePos)
			std::cout << mousePos.x << " " << mousePos.y << "\n";

		return true;
	};
	bool IsPressed(ControllerButton button, int playerIdx) const
	{
		if (((m_PreviousState[playerIdx].Gamepad.wButtons & int(button)) == 0))
		{
			return((m_CurrentState[playerIdx].Gamepad.wButtons & int(button)) != 0);
		}
		return false;
	};

	bool IsPressed(unsigned char key, int playerIdx)
	{
		unsigned char  currentByte = m_CurrentKeyboardState[playerIdx][(unsigned int)key];
		unsigned char  previousByte = m_PreviousKeyboardState[playerIdx][(unsigned int)key];
		currentByte >>= 7;
		previousByte >>= 7;

		if (currentByte == 1 && previousByte != 1) {
			return true;
		}
		return false;
	}
	bool IsReleased(ControllerButton button, int playerIdx) const
	{
		if (((m_PreviousState[playerIdx].Gamepad.wButtons & int(button)) != 0))
		{
			return((m_CurrentState[playerIdx].Gamepad.wButtons & int(button)) == 0);
		}
		return false;
	};
	bool IsReleased(unsigned char key, int playerIdx)
	{
		unsigned char  currentByte = m_CurrentKeyboardState[playerIdx][(unsigned int)key];
		unsigned char  previousByte = m_PreviousKeyboardState[playerIdx][(unsigned int)key];
		currentByte >>= 7;
		previousByte >>= 7;

		if (currentByte != 1 && previousByte == 1) {
			return true;
		}
		return false;
	}
	bool IsDown(ControllerButton button, int playerIdx) const
	{
		return((m_CurrentState[playerIdx].Gamepad.wButtons & int(button)) != 0);
	}
	bool IsDown(unsigned char key, int playerIdx)
	{
		unsigned char currentByte = m_CurrentKeyboardState[playerIdx][(unsigned int)key];
		currentByte >>= 7;
		if (currentByte == 1)
		{
			return true;
		}
		return false;
	}
	Float2 GetRStickValues()
	{
		return Float2{ static_cast<float>(m_CurrentState[0].Gamepad.sThumbRX),static_cast<float>(m_CurrentState[0].Gamepad.sThumbRY) };
	}

};
dae::InputManager::InputManager()
	:pImpl(new Impl())
{

}
dae::InputManager::~InputManager()
{
	delete pImpl;
	pImpl = nullptr;
}

dae::Float2 dae::InputManager::GetRStickValues() const
{
	return pImpl->GetRStickValues();
}
bool dae::InputManager::ProcessInput()
{
	return pImpl->ProcessInput();
}

bool dae::InputManager::IsPressed(ControllerButton button, int playerIdx) const
{
	return pImpl->IsPressed(button, playerIdx);
}
bool dae::InputManager::IsPressed(unsigned char key, int playerIdx) const
{
	return pImpl->IsPressed(key, playerIdx);
}
bool dae::InputManager::IsReleased(ControllerButton button, int playerIdx) const
{
	return pImpl->IsReleased(button, playerIdx);
}
bool dae::InputManager::IsReleased(unsigned char key, int playerIdx) const
{
	return pImpl->IsReleased(key, playerIdx);
}
bool dae::InputManager::IsDown(ControllerButton button, int playerIdx) const
{
	return pImpl->IsDown(button, playerIdx);
}

bool dae::InputManager::IsDown(unsigned char key, int playerIdx) const
{
	return pImpl->IsDown(key, playerIdx);
}
void dae::InputManager::AddCommand(ControllerButton button, Command* commandButton, KeyState keyState, GameObject* gameObject, int playerIdx, unsigned char keyboardKey)
{
	std::pair<std::pair<ControllerButton, unsigned char>, KeyState> key;
	key.first.first = button;
	key.first.second = keyboardKey;
	key.second = keyState;

	pImpl->GetButtonCommands(playerIdx)[key].reset(commandButton);
	pImpl->GetButtonCommands(playerIdx)[key]->SetGameObject(gameObject);

}
void dae::InputManager::Update()
{
	for (int i{}; i < NrOfPlayers; ++i)
	{
		for (const auto& p : pImpl->GetButtonCommands(i))
		{
			//std::map<std::pair<ControllerButton, KeyState>, std::unique_ptr<Command>> m_ButtonCommands[NrOfPlayers];
			switch (p.first.second)
			{
			case KeyState::PRESSED:
				if (IsPressed(p.first.first.first, i) || IsPressed(p.first.first.second, i))
				{
					pImpl->GetButtonCommands(i)[p.first]->Execute();
				}
				break;
			case KeyState::RELEASED:
				if (IsReleased(p.first.first.first, i) || IsReleased(p.first.first.second, i))
				{
					pImpl->GetButtonCommands(i)[p.first]->Execute();
				}
				break;
			case KeyState::DOWN:
				if (IsDown(p.first.first.first, i) || IsDown(p.first.first.second, i))
				{
					pImpl->GetButtonCommands(i)[p.first]->Execute();
				}
			case KeyState::NOTHING:
				if (p.first.first.first == ControllerButton::Nothing)
				{
					pImpl->GetButtonCommands(i)[p.first]->Execute();
				}
				break;
			}
		}

	}

}

void dae::InputManager::SetPlayer(GameObject* pGo, int playerIdx)
{
	auto it = pImpl->GetButtonCommands(playerIdx).begin();
	while (it != pImpl->GetButtonCommands(playerIdx).end())
	{
		it->second->SetGameObject(pGo);
		it++;
	}
}