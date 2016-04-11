#include <Book/Button.hpp>
#include <Book/Utility.hpp>
#include <Book/SoundPlayer.hpp>
#include <Book/ResourceHolder.hpp>

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include <iostream>
namespace GUI
{

Button::Button(const FontHolder& fonts, const TextureHolder& textures, const sf::RenderWindow& window, State::Context context)
: mCallback()
, mNormalTexture(textures.get(Textures::ButtonNormal))
, mSelectedTexture(textures.get(Textures::ButtonSelected))
, mPressedTexture(textures.get(Textures::ButtonPressed))
, mSprite()
, mText("", fonts.get(Fonts::Main), 16)
, mWindow(window)
, mIsToggle(false)
, mIsHiding(false)
, mSounds(*context.sounds)
{
	mSprite.setTexture(mNormalTexture);

	bounds = mSprite.getLocalBounds();
	mText.setPosition(bounds.width / 2.f, bounds.height / 2.f);	
}

void Button::setCallback(Callback callback)
{
	mCallback = std::move(callback);
}

void Button::setText(const std::string& text)
{
	mText.setString(text);
	centerOrigin(mText);
}

void Button::setToggle(bool flag)
{
	mIsToggle = flag;
}

bool Button::isSelectable() const
{
    if (mIsHiding)
		return false;

	return true;
}

void Button::setHide(bool flag)
{
	mIsHiding = flag;
	
	sf::Color color = mSprite.getColor();
	
	if (flag)
		color.a = 100.f;
	else
		color.a = 255.f;

	mSprite.setColor(color);
}

void Button::select()
{
	Component::select();

	mSprite.setTexture(mSelectedTexture);
}

void Button::deselect()
{
	Component::deselect();

	mSprite.setTexture(mNormalTexture);
}

void Button::activate()
{
	Component::activate();

    // If we are toggle then we should show that the button is pressed and thus "toggled".
	if (mIsToggle)
		mSprite.setTexture(mPressedTexture);

	if (mCallback)
		mCallback();

    // If we are not a toggle then deactivate the button since we are just momentarily activated.
	if (!mIsToggle)
		deactivate();
	
	mSounds.play(SoundEffect::Button);
}

void Button::deactivate()
{
	Component::deactivate();

	if (mIsToggle)
	{
        // Reset texture to right one depending on if we are selected or not.
		if (isSelected())
			mSprite.setTexture(mSelectedTexture);
		else
			mSprite.setTexture(mNormalTexture);
	}
}

bool Button::checkWorldBounds()
{
	bounds.left = this->getPosition().x;
	bounds.top = this->getPosition().y;

	if (bounds.contains(sf::Mouse::getPosition(mWindow).x, sf::Mouse::getPosition(mWindow).y))
	{
		return true;
	}
	
	return false;
}

void Button::handleEvent(const sf::Event&)
{
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(mSprite, states);
	target.draw(mText, states);
}

}
