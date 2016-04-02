#include <Book/GameState.hpp>


GameState::GameState(StateStack& stack, Context context)
: State(stack, context)
, level1(*context.window, *context.fonts)
, level2(*context.window, *context.fonts)
, level3(*context.window, *context.fonts)
, level(CurrentLevel::LVL_1)
, mPlayer(*context.player)
{
	level1.initialize();
	mPlayer.setMissionStatus(Player::MissionRunning);
}

void GameState::draw()
{
	switch(level)
	{
	case 0:
		level1.draw();
		break;
	case 1:
		level2.draw();
		break;
	case 2:
		level3.draw();
		break;
	}
}

bool GameState::update(sf::Time dt)
{
	{
		switch(level)
		{
		case 0:
			level1.update(dt);
			if(!level1.hasAlivePlayer())
			{
				mPlayer.setMissionStatus(Player::MissionFailure);
				requestStackPush(States::GameOver);
			}
			else if(level1.hasPlayerReachedEnd())
			{
				mPlayer.setMissionStatus(Player::MissionSuccess);
				level = CurrentLevel::LVL_2;
				level1.clearLevel();
				level2.initialize();
			}
			mPlayer.handleRealtimeInput(level1.getCommandQueue());
			break;
		case 1:
			level2.update(dt);
			if(!level2.hasAlivePlayer())
			{
				mPlayer.setMissionStatus(Player::MissionFailure);
				requestStackPush(States::GameOver);
			}
			else if(level2.hasPlayerReachedEnd())
			{
				mPlayer.setMissionStatus(Player::MissionSuccess);
				level = CurrentLevel::LVL_3;
				level2.clearLevel();
				level3.initialize();
			}
			mPlayer.handleRealtimeInput(level2.getCommandQueue());
			break;
		case 2:
			level3.update(dt);
			if(!level3.hasAlivePlayer())
			{
				mPlayer.setMissionStatus(Player::MissionFailure);
				requestStackPush(States::GameOver);
			}
			else if(level3.hasPlayerReachedEnd())
			{
				mPlayer.setMissionStatus(Player::MissionSuccess);
				requestStackPush(States::GameOver);
			}
			mPlayer.handleRealtimeInput(level3.getCommandQueue());
			break;
		}
	}

	return true;
}

bool GameState::handleEvent(const sf::Event& event)
{
	// Game input handling
	switch (level)
	{
	case 0:
		mPlayer.handleEvent(event, level1.getCommandQueue());
		break;
	case 1:
		mPlayer.handleEvent(event, level2.getCommandQueue());
		break;
	case 2:
		mPlayer.handleEvent(event, level3.getCommandQueue());
		break;
	}

	// Escape pressed, trigger the pause screen
	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
		requestStackPush(States::Pause);

	return true;
}