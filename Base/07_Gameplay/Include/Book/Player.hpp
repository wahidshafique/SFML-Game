#ifndef BOOK_PLAYER_HPP
#define BOOK_PLAYER_HPP

#include <Book/Command.hpp>

#include <SFML/Window/Event.hpp>

#include <map>

class CommandQueue;

class Player
{
	public:
		enum Action
		{
			MoveLeft,
			MoveRight,
			MoveUp,
			MoveDown,

			Fire,
			LaunchMissile,
			LaunchEnergy,
			ActionCount,
			
			SeekTarget,
			StopSeek			
		};

		enum MissionStatus
		{
			MissionRunning,
			MissionSuccess,
			MissionFailure,
			MissionWin
		};


	public:
								Player();

		void					handleEvent(const sf::Event& event, CommandQueue& commands);
		void					handleRealtimeInput(CommandQueue& commands);

		void					assignButton(Action action, sf::Mouse::Button button);
		void					assignKey(Action action, sf::Keyboard::Key key);
		sf::Mouse::Button		getAssignedButton(Action action) const;
		sf::Keyboard::Key		getAssignedKey(Action action) const;


		void 					setMissionStatus(MissionStatus status);
		MissionStatus 			getMissionStatus() const;

		bool					isMouse();
		void					setMouse(bool b);

	private:
		void					initializeActions();
		static bool				isRealtimeAction(Action action);

	private:
		bool									isMouseControlled;
		std::map<sf::Keyboard::Key, Action>		mKeyBinding;
		std::map<sf::Mouse::Button, Action>     mMouseBinding;
		std::map<Action, Command>				mActionBinding;
		MissionStatus 							mCurrentMissionStatus;
};

#endif // BOOK_PLAYER_HPP
