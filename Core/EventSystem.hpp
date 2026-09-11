#pragma once
#include "Engine/Core/NamedProperties.hpp"
#include <vector>
#include <map>
#include <string>
#include <mutex>
// -----------------------------------------------------------------------------
class DevConsole;
// -----------------------------------------------------------------------------
typedef NamedProperties EventArgs;
typedef bool (*EventCallbackFunction)(EventArgs& args);
// -----------------------------------------------------------------------------
struct EventSystemConfig
{
};
// -----------------------------------------------------------------------------
struct EventSubscriptionBase
{
	virtual ~EventSubscriptionBase() = default;
	virtual bool Execute(EventArgs& args) = 0;
	virtual bool IsAMethodSubscription() const { return false; }
	virtual void* GetObjectPointer()     const { return nullptr; }
};
// -----------------------------------------------------------------------------
struct EventSubscription : EventSubscriptionBase
{
public:
	EventSubscription(EventCallbackFunction functionPtr);
	virtual bool Execute(EventArgs& args) override;

public:
	EventCallbackFunction m_callbackFunction = nullptr;
};
// -----------------------------------------------------------------------------
template <typename T>
struct EventSubscriptionMethod : EventSubscriptionBase
{
public:
	typedef bool (T::* EventCallbackMethodPtr)(EventArgs&);

public:
	EventSubscriptionMethod(T* objectPtr, EventCallbackMethodPtr methodPtr)
		:m_objectPtr(objectPtr),
		 m_methodPtr(methodPtr)
	{
	}
	virtual bool Execute(EventArgs& args) override
	{
		return (m_objectPtr->*m_methodPtr)(args);
	}

	virtual bool IsAMethodSubscription() const override { return true; }
	virtual void* GetObjectPointer() const override { return m_objectPtr; }

	bool Matches(T* objectPtr, EventCallbackMethodPtr methodPtr) const
	{
		return m_objectPtr == objectPtr && m_methodPtr == methodPtr;
	}

public:
	T* m_objectPtr = nullptr;
	EventCallbackMethodPtr m_methodPtr = nullptr;
};
// -----------------------------------------------------------------------------
typedef std::vector<EventSubscriptionBase*> SubscriptionList;
// -----------------------------------------------------------------------------
class EventSystem
{
public:
	EventSystem(EventSystemConfig const& config);
	~EventSystem();
	void Startup();
	void Shutdown();
	void BeginFrame();
	void EndFrame();

	template <typename T>
	void SubscribeEventCallbackMethod(std::string const& eventName, T* objectPtr, bool (T::* methodPtr)(EventArgs&))
	{
		std::scoped_lock<std::mutex> lock(m_eventSystemMutex);
		SubscriptionList& subList = m_subscriptionListsByEventName[eventName];
		subList.push_back(new EventSubscriptionMethod<T>(objectPtr, methodPtr));
	}

	template <typename T>
	void UnSubscribeEventCallbackMethod(std::string const& eventName, T* objectPtr, bool (T::* methodPtr)(EventArgs&))
	{
		std::scoped_lock<std::mutex> lock(m_eventSystemMutex);

		auto found = m_subscriptionListsByEventName.find(eventName);
		if (found == m_subscriptionListsByEventName.end())
		{
			return;
		}

		SubscriptionList& subList = found->second;

		for (int subIndex = 0; subIndex < static_cast<int>(subList.size()); ++subIndex)
		{
			auto* subscriber = dynamic_cast<EventSubscriptionMethod<T>*>(subList[subIndex]);
			if (subscriber && subscriber->Matches(objectPtr, methodPtr))
			{
				delete subList[subIndex];
				subList.erase(subList.begin() + subIndex);
				break;
			}
		}
	}

	void SubscribeEventCallbackFunction(std::string const& eventName, EventCallbackFunction functionPtr);
	void UnsubscribeEventCallbackFunction(std::string const& eventName, EventCallbackFunction functionPtr);
	void UnsubscribeObjectFromAllEvents(void* objectPtr);
	void FireEvent(std::string const& eventName, EventArgs& args);
	void FireEvent(std::string const& eventName);

	std::vector<std::string> GetAllRegisteredCommands() const;

protected:
	EventSystemConfig m_config;
	std::map<HashedCaseInsensitiveString, SubscriptionList> m_subscriptionListsByEventName;
	
	// EventSystem's stored internal mutex
	mutable std::mutex m_eventSystemMutex;
};

// -----------------------------------------------------------------------------
// Standalone global-namespace helper functions; these forward to "the" event system, if it exists
//
void SubscribeEventCallbackFunction(std::string const& eventName, EventCallbackFunction functionPtr);
void UnsubscribeEventCallbackFunction(std::string const& eventName, EventCallbackFunction functionPtr);
void FireEvent(std::string const& eventName, EventArgs& args);
void FireEvent(std::string const& eventName);
// -----------------------------------------------------------------------------
class EventRecipient
{
public:
	virtual ~EventRecipient();
};