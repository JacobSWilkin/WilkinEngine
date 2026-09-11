#include "Engine/Core/EventSystem.hpp"
#include "Engine/Core/DevConsole.hpp"
#include "Engine/Core/EngineCommon.h"
EventSystem* g_theEventSystem = nullptr;

EventSystem::EventSystem(EventSystemConfig const& config)
	:m_config(config)
{
}

EventSystem::~EventSystem()
{
}

void EventSystem::Startup()
{
}

void EventSystem::Shutdown()
{
	for (auto& pair : m_subscriptionListsByEventName)
	{
		for (EventSubscriptionBase* subscriber : pair.second)
		{
			delete subscriber;
		}
	}
	m_subscriptionListsByEventName.clear();
}

void EventSystem::BeginFrame()
{
}

void EventSystem::EndFrame()
{
}

void EventSystem::SubscribeEventCallbackFunction(std::string const& eventName, EventCallbackFunction functionPtr)
{
	std::scoped_lock<std::mutex> lock(m_eventSystemMutex);

	SubscriptionList& subscriptionList = m_subscriptionListsByEventName[eventName];
	EventSubscription newSubscription = { functionPtr };
	subscriptionList.push_back(new EventSubscription(functionPtr));
}

void EventSystem::UnsubscribeEventCallbackFunction(std::string const& eventName, EventCallbackFunction functionPtr)
{
	std::scoped_lock<std::mutex> lock(m_eventSystemMutex);

	std::map<HashedCaseInsensitiveString, SubscriptionList>::iterator found = m_subscriptionListsByEventName.find(eventName);
	if (found == m_subscriptionListsByEventName.end())
	{
		return;
	}
	SubscriptionList& subscriptionList = found->second;
	for (int subIndex = 0; subIndex < (int)subscriptionList.size(); ++subIndex)
	{
		EventSubscription* subscriber = dynamic_cast<EventSubscription*>(subscriptionList[subIndex]);
		if (subscriber && subscriber->m_callbackFunction == functionPtr)
		{
			delete subscriptionList[subIndex];
			subscriptionList.erase(subscriptionList.begin() + subIndex);
			break;
		}
	}
}

void EventSystem::UnsubscribeObjectFromAllEvents(void* objectPtr)
{
	std::scoped_lock<std::mutex> lock(m_eventSystemMutex);

	for (auto& pair : m_subscriptionListsByEventName)
	{
		SubscriptionList& subList = pair.second;
		for (int subIndex = 0; subIndex < static_cast<int>(subList.size()); ++subIndex)
		{
			if (subList[subIndex]->IsAMethodSubscription() && subList[subIndex]->GetObjectPointer() == objectPtr)
			{
				delete subList[subIndex];
				subList.erase(subList.begin() + subIndex);
			}
			else
			{
				++subIndex;
			}
		}
	}
}

void EventSystem::FireEvent(std::string const& eventName, EventArgs& args)
{
	SubscriptionList subListCopy;

	{
		std::scoped_lock<std::mutex> lock(m_eventSystemMutex);

		std::map<HashedCaseInsensitiveString, SubscriptionList>::iterator found = m_subscriptionListsByEventName.find(eventName);
		if (found == m_subscriptionListsByEventName.end())
		{
			if (g_theDevConsole != nullptr)
			{
				g_theDevConsole->AddLine(DevConsole::ERROR_MAJOR, "Unknown Command: " + eventName);
			}
			return;
		}
		subListCopy = found->second;
	}	

	for (EventSubscriptionBase* subscriber : subListCopy)
	{
		if (subscriber->Execute(args))
		{
			break;
		}
	}
}

void EventSystem::FireEvent(std::string const& eventName)
{
	EventArgs args;
	FireEvent(eventName, args);
}

std::vector<std::string> EventSystem::GetAllRegisteredCommands() const
{
	std::vector<std::string> registeredCommands;
	for (auto found = m_subscriptionListsByEventName.begin(); found != m_subscriptionListsByEventName.end(); ++found)
	{
		registeredCommands.push_back(found->first.GetOriginalString());
	}
	return registeredCommands;
}

void SubscribeEventCallbackFunction(std::string const& eventName, EventCallbackFunction functionPtr)
{
	if (g_theEventSystem)
	{
		g_theEventSystem->SubscribeEventCallbackFunction(eventName, functionPtr);
	}
}

void UnsubscribeEventCallbackFunction(std::string const& eventName, EventCallbackFunction functionPtr)
{
	if (g_theEventSystem)
	{
		g_theEventSystem->UnsubscribeEventCallbackFunction(eventName, functionPtr);
	}
}

void FireEvent(std::string const& eventName, EventArgs& args)
{
	if (g_theEventSystem)
	{
		g_theEventSystem->FireEvent(eventName, args);
	}
}

void FireEvent(std::string const& eventName)
{
	if (g_theEventSystem)
	{
		g_theEventSystem->FireEvent(eventName);
	}
}
// -----------------------------------------------------------------------------
EventSubscription::EventSubscription(EventCallbackFunction functionPtr)
	:m_callbackFunction(functionPtr)
{
}

bool EventSubscription::Execute(EventArgs& args)
{
	return m_callbackFunction(args);
}
// -----------------------------------------------------------------------------
EventRecipient::~EventRecipient()
{
	if (g_theEventSystem)
	{
		g_theEventSystem->UnsubscribeObjectFromAllEvents(this);
	}
}
