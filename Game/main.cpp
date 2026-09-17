#include <atomic>
#include <mutex>
#include <optional>
#include <thread>
#include <variant>
#include <raylib/raylib.h>


struct KeyEvent
{
	int key;
	int state;
};

using Event = std::variant < KeyEvent >;

std::atomic_bool windowOpen = true;
std::optional<Event> event;
std::mutex mtx;

void EventThread()
{
	while (windowOpen.load())
	{
		std::optional<Event> localEvent = std::nullopt;

		{
			std::scoped_lock lock(mtx);
			if (event.has_value())
			{
				localEvent = event;
				event.reset(); // Safely clear it for the next frame
			}
		}

		if (localEvent.has_value())
		{
			if (std::holds_alternative<KeyEvent>(*localEvent))
			{
				KeyEvent keyEvent = std::get<KeyEvent>(*localEvent);
				if (keyEvent.key == KEY_ESCAPE && keyEvent.state == 1)
				{
					windowOpen.store(false);
				}
			}
		}
	}
}

void RenderThread()
{
	InitWindow(800, 600, "Test");
	SetExitKey(KEY_NULL);

	while (windowOpen.load())
	{
		BeginDrawing();
		ClearBackground(RAYWHITE);

		int key;
		while ((key = GetKeyPressed()) > 0)
		{
			std::scoped_lock lock(mtx);
			KeyEvent evt = { key, 1 };

			event = evt;
		}

		EndDrawing();

		if (WindowShouldClose())
		{
			windowOpen.store(false);
		}

		{
			std::scoped_lock lock(mtx);
			event.reset();
		}
	}

	CloseWindow();
}

int main()
{
	std::thread renderThread(&RenderThread);
	std::thread tickThread(&EventThread);

	while (windowOpen.load())
	{
		
	}

	renderThread.join();
	tickThread.join();

	return 0;
}