#pragma once

#include <functional>

namespace Kusanagi::Utils
{
	class EventListener
	{
	public:
		EventListener() {}
		virtual ~EventListener() {};

		virtual void _cdecl Run(void *sender, ...) = 0;
		virtual inline void _cdecl operator()(void *sender, ...) = 0;
	};

	template <typename ...Args>
	class EventListenerTemplate : public EventListener
	{
	private:
		std::function<void(void*, Args...)> action;

	public:
		EventListenerTemplate(std::function<void(void*, Args...)> action) {
			EventListenerTemplate::action = action;
		}

		void _cdecl Run(void *sender, Args... args) override
		{
			action(sender, args...);
		}
		inline void _cdecl operator()(void *sender, Args... args) override
		{
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

		void _cdecl Run(void *sender, ...) override
		{
			action(sender);
		}
		inline void _cdecl operator()(void *sender, ...) override
		{
			Run(sender);
		}
	};
}