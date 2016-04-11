
#ifndef BOOK_BUTTON_HPP
#define BOOK_BUTTON_HPP

#include <Book/Component.hpp>
#include <Book/ResourceIdentifiers.hpp>
#include <Book/ResourceHolder.hpp>
#include <Book/State.hpp>

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Mouse.hpp>

#include <vector>
#include <string>
#include <memory>
#include <functional>

class SoundPlayer;

namespace GUI
{

class Button : public Component
{
    public:
        typedef std::shared_ptr<Button>		Ptr;
        typedef std::function<void()>		Callback;


	public:
		Button(const FontHolder& fonts, const TextureHolder& textures, const sf::RenderWindow& window, State::Context context);

        void					setCallback(Callback callback);
        void					setText(const std::string& text);
        void					setToggle(bool flag);
		void					setHide(bool flag);

        virtual bool			isSelectable() const;
        virtual void			select();
        virtual void			deselect();

        virtual void			activate();
        virtual void			deactivate();
		bool					checkWorldBounds();

        virtual void			handleEvent(const sf::Event& event);


    private:
        virtual void			draw(sf::RenderTarget& target, sf::RenderStates states) const;


    private:
        const sf::RenderWindow& mWindow;
		sf::FloatRect			bounds;
		Callback				mCallback;
        const sf::Texture&		mNormalTexture;
        const sf::Texture&		mSelectedTexture;
        const sf::Texture&		mPressedTexture;
        sf::Sprite				mSprite;
        sf::Text				mText;
        bool					mIsToggle;
		bool					mIsHiding;
		SoundPlayer&			mSounds;
};

}

#endif // BOOK_BUTTON_HPP
