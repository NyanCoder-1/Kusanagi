#pragma once

#include <functional>

namespace Kusanagi::Utils {
	class EventListener {
	public:
		EventListener() {}
		virtual ~EventListener() {};
	};

	template <typename ...Args>
	class EventListenerTemplate : public EventListener {
	private:
		std::function<void(void*, Args...)> action;

	public:
		EventListenerTemplate(std::function<void(void*, Args...)> action) {
			EventListenerTemplate::action = action;
		}

		void _cdecl Run(void *sender, Args... args) {
			action(sender, args...);
		}
		inline void _cdecl operator()(void *sender, Args... args) {
			Run(sender, args...);
		}
	};
	template <>
	class EventListenerTemplate<> : public EventListener
	{
	private:
		std::function<void(void*)> action;

	public:
		EventListenerTemplate(std::function<void(void*)> action) {
			EventListenerTemplate::action = action;
		}

		void _cdecl Run(void *sender) {
			action(sender);
		}
		inline void _cdecl operator()(void *sender) {
			Run(sender);
		}
	};
}