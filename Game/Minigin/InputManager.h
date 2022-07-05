#pragma once
#include <memory>
#include <iostream>
#include "Command.h"
#include <map>
#include "Singleton.h"

namespace dae
{
	enum class KeyState
	{
		DOWN,
		PRESSED,
		RELEASED,
		NOTHING
	};
	enum class ControllerButton
	{
		Nothing = 0x0000,
		ButtonA = 0x1000,
		ButtonB = 0x2000,
		ButtonX = 0x4000,
		ButtonY = 0x8000,
		DPadUp = 0x0001,
		DPadDown = 0x0002,
		DPadLeft = 0x0004,
		DPadRight = 0x0008,
		Start = 0x0010,
		Back = 0x0020,
		LeftThumb = 0x0040,
		RightThumb = 0x0080,
		LeftShoulder = 0x0100,
		RightShoulder = 0x0200
	};

	class InputManager final : public Singleton<InputManager>
	{
	private:

		class Impl;

		Impl* pImpl;

	public:
		InputManager();
		~InputManager();
		InputManager(const InputManager& other) = delete;
		InputManager(InputManager&& other) = delete;
		InputManager& operator=(const InputManager& other) = delete;
		InputManager& operator=(InputManager&& other) = delete;
		bool ProcessInput();
		bool IsDown(ControllerButton button, int playerIdx) const;
		bool IsReleased(ControllerButton button, int playerIdx) const;
		bool IsPressed(ControllerButton button, int playerIdx) const;
		void AddCommand(ControllerButton button, Command* command, KeyState keyState, GameObject* pGameObject, int playerIdx);
		//void SetPlayer(GameObject* pGo, int playerIdx);

		void Update();
	};



}
