#pragma once

#include <list>
#include <map>
#include <vector>
#include <string>

#include "utils/EventListener.h"

namespace Kusanagi::Utils
{
	class EventManager
	{
	private:
		std::vector<std::list<Kusanagi::Utils::EventListener*>> events;
		std::map<std::string, long long> keys;
		std::list<Kusanagi::Utils::EventListener*> deletable;

	public:
		EventManager() : EventManager(std::vector<std::string>{}) {}
		EventManager(std::vector<std::string> keys)
		{
			long long i = 0;
			for (std::string &str : keys)
				if (this->keys.find(str) == this->keys.end())
					this->keys[str] = i++;
			events.resize(i);
		}
		virtual ~EventManager()
		{
			for (Kusanagi::Utils::EventListener* listener : deletable)
				delete listener;
		}

	protected:
		long long AddEvent(std::string name)
		{
			if (keys.find(name) == this->keys.end())
			{
				long long id = keys[name] = events.size();
				events.push_back(std::list<Kusanagi::Utils::EventListener*>());
				return id;
			}
			return -1;
		}

	public:
		void AddEventListener(std::string eventName, Kusanagi::Utils::EventListener *listener, bool deleteHere = true)
		{
			// search for key existing
			long long id = GetEventID(eventName);
			// do nothing if key not exist
			if ((id < 0) && (id >= events.size()))
			{
				// and delete listener if it's deletable
				if (deleteHere && listener)
					delete listener;
				return;
			}
			// add listener
			events[id].push_back(listener);

			// set delete check
			if (deleteHere)
				deletable.push_back(listener);
		}
		void RemoveEventListener(std::string eventName, Kusanagi::Utils::EventListener *listener)
		{
			long long id = GetEventID(eventName);
			if ((id >= 0) && (id < events.size()))
			{
				// search listener
				auto &e = events[id];
				for (auto iter = e.begin(); iter != e.end();)
					if (*iter == listener)
					{
						// deletablenes
						for (auto iter1 = deletable.begin(); iter1 != deletable.end();)
							if (*iter1 == listener)
							{
								delete listener; // delete
								iter1 = deletable.erase(iter1);
							}
							else
								iter1++;

						// remove from list
						iter = e.erase(iter);
					}
					else
						iter++;
			}
		}

		long long GetEventID(std::string eventName)
		{
			auto id = keys.find(eventName);
			if ((id == keys.end()) || (id->second < 0) || (id->second >= events.size()))
				return -1;
			return id->second;
		}

		template <typename ...Args>
		void Event(std::string eventName, Args ...args)
		{
			Event(GetEventID(eventName), args...);
		}
		void Event(std::string eventName)
		{
			Event(GetEventID(eventName));
		}
		template <typename ...Args>
		void Event(long long id, Args ...args)
		{
			if ((id < 0) && (id >= events.size()))
				return;
			for (Kusanagi::Utils::EventListener *listener : events[id])
				reinterpret_cast<Kusanagi::Utils::EventListenerTemplate<Args...>*>(listener)->Run(this, args...);
		}
		void Event(long long id)
		{
			if ((id < 0) && (id >= events.size()))
				return;
			for (Kusanagi::Utils::EventListener *listener : events[id])
				reinterpret_cast<Kusanagi::Utils::EventListenerTemplate<>*>(listener)->Run(this);
		}
	};
}