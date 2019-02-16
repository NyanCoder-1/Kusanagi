#pragma once

#include <list>
#include <map>
#include <string>

#include "utils/EventListener.h"

namespace Kusanagi::Utils
{
	class EventManager
	{
	private:
		std::map<std::string, std::list<Kusanagi::Utils::EventListener*>> events;
		std::list<Kusanagi::Utils::EventListener*> deletable;

	public:
		virtual ~EventManager()
		{
			for (Kusanagi::Utils::EventListener* listener : deletable)
				delete listener;
		}

		void AddEventListener(std::string type, Kusanagi::Utils::EventListener *listener, bool deleteHere = true)
		{
			// prevent repeating
			bool find = false;
			for (Kusanagi::Utils::EventListener *lstnr : events[type])
				if (lstnr == listener)
					find = true;

			// if exist, do nothing
			if (find)
				return;
			// adding listener
			events[type].push_back(listener);

			// set delete check
			if (deleteHere)
				deletable.push_back(listener);
		}
		void RemoveEventListener(std::string type, Kusanagi::Utils::EventListener *listener)
		{
			auto listenerList = events.find(type);
			if (listenerList != events.end())
			{
				// search listener
				for (auto iter = listenerList->second.begin(); iter != listenerList->second.end();)
					if (*iter == listener)
					{
						// check if deletable
						for (auto iter1 = deletable.begin(); iter1 != deletable.end();)
							if (*iter1 == listener)
							{
								delete listener; // delete
								iter1 = deletable.erase(iter1);
							}
							else
								iter1++;
						// remove from list
						iter = listenerList->second.erase(iter);
					}
					else
						iter++;

				// remove list if empty
				if (listenerList->second.empty())
					events.erase(listenerList);
			}
		}

		template <typename ...Args>
		void Event(std::string type, Args ...args)
		{
			auto listenerList = events.find(type);
			if (listenerList != events.end())
				for (Kusanagi::Utils::EventListener *listener : listenerList->second)
					listener->Run(this, args...);
		}
	};
}