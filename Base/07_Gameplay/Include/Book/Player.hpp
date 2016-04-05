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
			SeekTarget,
			ActionCount
		};

		enum MissionStatus
		{
			MissionRunning,
			MissionSuccess,
			MissionFailure,
			MissionWin
		};

		struct Seek
		{
			sf::Vector2f target;
			sf::Vector2f direction;
			bool isSeek;
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

	private:
		void					initializeActions();
		static bool				isRealtimeAction(Action action);
		sf::Vector2f			getDirectionFromMousePosition();
		bool					CanSeek();

	private:
		Seek									mSeek;								
		std::map<sf::Keyboard::Key, Action>		mKeyBinding;
		std::map<sf::Mouse::Button, Action>     mMouseBinding;
		std::map<Action, Command>				mActionBinding;
		MissionStatus 							mCurrentMissionStatus;
};

#endif // BOOK_PLAYER_HPP
