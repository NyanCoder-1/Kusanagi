#pragma once

#include <functional>

namespace Kusanagi::Utils
{
	class EventListener
	{
	public:
		EventListener() {}
		virtual ~EventListener() {};

		virtual void _cdecl Run(...) = 0;
		virtual inline void _cdecl operator()() = 0;
	};

	template <typename ...Args>
	class EventListenerTemplate
	{
	private:
		std::function<void(Args...)> listener;

	public:
		EventListenerTemplate(std::function<void(Args...)> listener) {
			EventListenerTemplate::listener = listener;
		}

		void _cdecl Run(Args... args) override
		{
			listener(args...);
		}
		void _cdecl Run() override
		{

		}
		inline void _cdecl operator()(Args... args) override
		{
			Run(args);
		}
	};
}