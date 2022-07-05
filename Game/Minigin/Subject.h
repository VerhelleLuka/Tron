#pragma once
#include "GameObject.h"
#include "EventStructs.h"
#include <iostream>
#include <vector>

namespace dae
{
	struct EventArgs;
	class Observer;

	class Subject
	{
	public:
		Subject(){}
		void AddObserver(Observer* pObserver);
		void RemoveObserver(Observer* pObserver);
		void ClearObservers() { m_pObservers.clear(); };
		void Notify(EventType event_, std::shared_ptr<EventArgs> args);
	protected:
		std::vector<Observer*> m_pObservers;

	};
}
