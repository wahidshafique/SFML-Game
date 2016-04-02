#ifndef BOOK_GAMESTATE_HPP
#define BOOK_GAMESTATE_HPP

#include <Book/State.hpp>
#include <Book/Level1.hpp>
#include <Book/Level2.hpp>
#include <Book/Level3.hpp>
#include <Book/Player.hpp>

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>


class GameState : public State
{
	public:
							GameState(StateStack& stack, Context context);

		virtual void		draw();
		virtual bool		update(sf::Time dt);
		virtual bool		handleEvent(const sf::Event& event);

		enum CurrentLevel
		{
			LVL_1, LVL_2, LVL_3, TOTAL_LEVELS
		};

	private:
		Level1				level1;
		Level2				level2;
		Level3				level3;
		Player&				mPlayer;

		CurrentLevel		level;
};

#endif // BOOK_GAMESTATE_HPP