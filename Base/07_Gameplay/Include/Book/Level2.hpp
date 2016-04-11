#ifndef BOOK_Level2_HPP
#define BOOK_Level2_HPP

#include <Book/ResourceHolder.hpp>
#include <Book/ResourceIdentifiers.hpp>
#include <Book/SceneNode.hpp>
#include <Book/SpriteNode.hpp>
#include <Book/Aircraft.hpp>
#include <Book/CommandQueue.hpp>
#include <Book/Command.hpp>
#include <Book/SoundPlayer.hpp>

#include <SFML/System/NonCopyable.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <array>
#include <queue>


// Forward declaration
namespace sf
{
	class RenderWindow;
}

class Level2 : private sf::NonCopyable
{
	public:
		explicit							Level2(sf::RenderWindow& window, FontHolder& fonts,
												SoundPlayer& sounds);
		void								update(sf::Time dt);
		void								draw();
		
		CommandQueue&						getCommandQueue();

		bool 								hasAlivePlayer() const;
		bool 								hasPlayerReachedEnd() const;
		void								initialize();
		void								clearLevel();

	private:
		void								loadTextures();
		void								adaptPlayerPosition();
		void								adaptPlayerVelocity(float deltaTime);
		void								handleCollisions();
		void								updateSounds();
		
		void								buildScene();
		void								addEnemies();
		void								addEnemy(Aircraft::Type type, float relX, float relY);
		void								spawnEnemies();
		void								destroyEntitiesOutsideView();
		void								guideMissiles();
		sf::FloatRect						getViewBounds() const;
		sf::FloatRect						getBattlefieldBounds() const;
		bool								matchesCategories(SceneNode::Pair& colliders, Category::Type type1, Category::Type type2);

	private:
		enum Layer
		{
			Background,
			Air,
			LayerCount
		};

		struct SpawnPoint 
		{
			SpawnPoint(Aircraft::Type type, float x, float y)
			: type(type)
			, x(x)
			, y(y)
			{
			}

			Aircraft::Type type;
			float x;
			float y;
		};


	private:
		sf::RenderWindow&					mWindow;
		sf::View							mWorldView;
		TextureHolder						mTextures;
		FontHolder&							mFonts;
		SoundPlayer&						mSounds;
		
		SceneNode							mSceneGraph;
		std::array<SceneNode*, LayerCount>	mSceneLayers;
		CommandQueue						mCommandQueue;

		sf::FloatRect						mWorldBounds;
		sf::Vector2f						mSpawnPosition;
		float								mScrollSpeed;
		Aircraft*							mPlayerAircraft;

		std::vector<SpawnPoint>				mEnemySpawnPoints;
		std::vector<Aircraft*>				mActiveEnemies;
		unsigned int						enemyCount;
};

#endif // BOOK_WORLD_HPP
