#include <Book/Player.hpp>
#include <Book/CommandQueue.hpp>
#include <Book/Aircraft.hpp>
#include <Book/Foreach.hpp>

#include <map>
#include <string>
#include <algorithm>

using namespace std::placeholders;

struct AircraftMover
{
	AircraftMover(float vx, float vy)
	: velocity(vx, vy)
	{
	}

	AircraftMover(sf::Vector2f dir)
	: velocity(dir)
	{
	}

	void operator() (Aircraft& aircraft, sf::Time) const
	{
		aircraft.accelerate(velocity * aircraft.getMaxSpeed());
	}

	sf::Vector2f velocity;
};

Player::Player()
: mCurrentMissionStatus(MissionRunning)
, isMouseControlled(false)
{
	// Set initial key bindings
	mKeyBinding[sf::Keyboard::Left] = MoveLeft;
	mKeyBinding[sf::Keyboard::Right] = MoveRight;
	mKeyBinding[sf::Keyboard::Up] = MoveUp;
	mKeyBinding[sf::Keyboard::Down] = MoveDown;
	mKeyBinding[sf::Keyboard::Space] = Fire;
	mKeyBinding[sf::Keyboard::M] = LaunchMissile;
	mKeyBinding[sf::Keyboard::E] = LaunchEnergy;
	mMouseBinding[sf::Mouse::Left] = SeekTarget;

	// Set initial action bindings
	initializeActions();	

	// Assign all categories to player's aircraft
	FOREACH(auto& pair, mActionBinding)
		pair.second.category = Category::PlayerAircraft;
}

void Player::handleEvent(const sf::Event& event, CommandQueue& commands)
{
	if (event.type == sf::Event::KeyPressed)
	{
		// Check if pressed key appears in key binding, trigger command if so
		auto found = mKeyBinding.find(event.key.code);
		if (found != mKeyBinding.end() && !isRealtimeAction(found->second))
		{
			commands.push(mActionBinding[found->second]);
		}
	}
	if (event.type == sf::Event::MouseButtonPressed && isMouse())
	{		
		auto found = mMouseBinding.find(event.mouseButton.button);
		if (found != mMouseBinding.end() && isRealtimeAction(found->second))
		{
			commands.push(mActionBinding[found->second]);
		}
	}
}

void Player::handleRealtimeInput(CommandQueue& commands)
{
	// Traverse all assigned keys and check if they are pressed
	FOREACH(auto pair, mKeyBinding)
	{
		// If key is pressed, lookup action and trigger corresponding command
		if (sf::Keyboard::isKeyPressed(pair.first) && isRealtimeAction(pair.second))
		{
			if (!isMouse())
				commands.push(mActionBinding[pair.second]);
			else if (isMouse() && pair.second != MoveLeft &&
					pair.second != MoveRight && pair.second != MoveUp &&
					pair.second != MoveDown)
				commands.push(mActionBinding[pair.second]);
		}		
	}
}

bool Player::isMouse()
{
	return isMouseControlled;
}

void Player::setMouse(bool b)
{
	isMouseControlled = b;
}

void Player::assignButton(Action action, sf::Mouse::Button button)
{
	// Remove all keys that already map to action
	for (auto itr = mMouseBinding.begin(); itr != mMouseBinding.end();)
	{
		if (itr->second == action)
			mMouseBinding.erase(itr++);
		else
			++itr;
	}

	// Insert new binding
	mMouseBinding[button] = action;
}

sf::Mouse::Button Player::getAssignedButton(Action action) const
{
	FOREACH(auto pair, mMouseBinding)
	{
		if (pair.second == action)
			return pair.first;
	}

	return sf::Mouse::Button::ButtonCount; //check against this
}

void Player::assignKey(Action action, sf::Keyboard::Key key)
{
	// Remove all keys that already map to action
	for (auto itr = mKeyBinding.begin(); itr != mKeyBinding.end(); )
	{
		if (itr->second == action)
			mKeyBinding.erase(itr++);
		else
			++itr;
	}

	// Insert new binding
	mKeyBinding[key] = action;
}

sf::Keyboard::Key Player::getAssignedKey(Action action) const
{
	FOREACH(auto pair, mKeyBinding)
	{
		if (pair.second == action)
			return pair.first;
	}

	return sf::Keyboard::Unknown;
}

void Player::setMissionStatus(MissionStatus status)
{
	mCurrentMissionStatus = status;
}

Player::MissionStatus Player::getMissionStatus() const
{
	return mCurrentMissionStatus;
}

void Player::initializeActions()
{
	mActionBinding[MoveLeft].action      = derivedAction<Aircraft>(AircraftMover(-1,  0));
	mActionBinding[MoveRight].action     = derivedAction<Aircraft>(AircraftMover(+1,  0));
	mActionBinding[MoveUp].action        = derivedAction<Aircraft>(AircraftMover( 0, -1));
	mActionBinding[MoveDown].action      = derivedAction<Aircraft>(AircraftMover( 0, +1));
	mActionBinding[SeekTarget].action	 = derivedAction<Aircraft>([] (Aircraft& a, sf::Time){ a.setSeek(); });
	mActionBinding[StopSeek].action		 = derivedAction<Aircraft>([] (Aircraft& a, sf::Time){ a.stopSeek(); });
	mActionBinding[Fire].action          = derivedAction<Aircraft>([] (Aircraft& a, sf::Time){ a.fire(); });
	mActionBinding[LaunchMissile].action = derivedAction<Aircraft>([] (Aircraft& a, sf::Time){ a.launchMissile(); });
	mActionBinding[LaunchEnergy].action  = derivedAction<Aircraft>([] (Aircraft& a, sf::Time){ a.launchEnergy(); });
}

bool Player::isRealtimeAction(Action action)
{
	switch (action)
	{
		case MoveLeft:
		case MoveRight:
		case MoveDown:
		case MoveUp:
		case Fire:
		case SeekTarget:
		case StopSeek:
			return true;

		default:
			return false;
	}
}
