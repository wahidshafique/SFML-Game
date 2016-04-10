#ifndef BOOK_AIRCRAFT_HPP
#define BOOK_AIRCRAFT_HPP

#include <Book/Entity.hpp>
#include <Book/Command.hpp>
#include <Book/ResourceIdentifiers.hpp>
#include <Book/Projectile.hpp>
#include <Book/TextNode.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML\Window\Event.hpp>

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/View.hpp>

namespace sf
{
	class RenderWindow;
}

class Aircraft : public Entity
{
	public:
		enum Type
		{
			Eagle,
			Raptor,
			Avenger,
			TypeCount
		};

		struct Seek
		{
			sf::Vector2i target;
			bool isSeek;
		};

	public:
								Aircraft(Type type, const TextureHolder& textures, const FontHolder& fonts, sf::RenderWindow& window);

		virtual unsigned int	getCategory() const;
		virtual sf::FloatRect	getBoundingRect() const;
		virtual bool 			isMarkedForRemoval() const;
		bool					isAllied() const;
		float					getMaxSpeed() const;

		void					increaseFireRate();
		void					increaseSpread();
		void					collectMissiles(unsigned int count);
		void					collectEnergy(unsigned int count);

		void 					fire();
		void					launchMissile();
		void					launchEnergy();
		
		void					setSeek();
		void					seekTarget(sf::Vector2f pos, sf::Vector2f targetPos);
		void					stopSeek();
		bool					isSeek() const;
		bool					checkSeekBounds(sf::Vector2f pos, sf::Vector2f targetPos) const;
		sf::Vector2i			getTarget() const;

		void					moveToStick();

	private:
		virtual void			drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
		virtual void 			updateCurrent(sf::Time dt, CommandQueue& commands);
		void					updateMovementPattern(sf::Time dt);
		void					checkPickupDrop(CommandQueue& commands);
		void					checkProjectileLaunch(sf::Time dt, CommandQueue& commands);

		void					createBullets(SceneNode& node, const TextureHolder& textures) const;
		void					createProjectile(SceneNode& node, Projectile::Type type, float xOffset, float yOffset, const TextureHolder& textures) const;
		void					createPickup(SceneNode& node, const TextureHolder& textures) const;

		void					updateTexts();


	private:
		Type					mType;
		Seek					mSeek;
		sf::Sprite				mSprite;
		Command 				mFireCommand;
		Command					mMissileCommand;
		Command					mEnergyCommand;
		sf::Time				mFireCountdown;
		bool 					mIsFiring;
		bool					mIsLaunchingMissile;
		bool					mIsLaunchingEnergy;
		bool 					mIsMarkedForRemoval;

		int						mFireRateLevel;
		int						mSpreadLevel;
		int						mMissileAmmo;
		int						mEnergy;

		Command 				mDropPickupCommand;
		float					mTravelledDistance;
		std::size_t				mDirectionIndex;
		TextNode*				mHealthDisplay;
		TextNode*				mMissileDisplay;
		TextNode*				mEnergyDisplay;

		float					mSeekRadius;

		sf::RenderWindow&		mWindow;
};

#endif // BOOK_AIRCRAFT_HPP
