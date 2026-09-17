#include <atomic>
#include <mutex>
#include <optional>
#include <thread>
#include <raylib/raylib.h>

class Event
{
public:
	virtual ~Event() = default;

};

class KeyEvent : public Event
{
public:
	int key;
	int state;

public:
	KeyEvent(int key, int state)
		: key{ key }, state{ state }
	{
		
	}

};

std::atomic_bool windowOpen = true;
std::atomic<std::optional<Event*>> event;
std::mutex mtx;

void EventThread()
{
	while (windowOpen.load())
	{
		std::optional evt = event.load();

		if (evt.has_value())
		{
			KeyEvent* keyEvent = dynamic_cast<KeyEvent*>(evt.value());

			if (keyEvent->key == KEY_ESCAPE && keyEvent->state == 1)
			{
				windowOpen.store(false);
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

			event.store(&evt);
		}

		EndDrawing();

		if (WindowShouldClose())
		{
			windowOpen.store(false);
		}

		event.load().reset();
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