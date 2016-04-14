#include <Book/Container.hpp>
#include <Book/Foreach.hpp>

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>


namespace GUI
{

Container::Container()
: mChildren()
, mSelectedChild(-1)
, mIsStickMove(false)
, mStickSensitivity(0.25f)
{
}

void Container::pack(Component::Ptr component)
{
	mChildren.push_back(component);

	if (!hasSelection() && component->isSelectable())
		select(mChildren.size() - 1);
}

bool Container::isSelectable() const
{
    return false;
}

void Container::handleEvent(const sf::Event& event)
{
    float dirY = sf::Joystick::getAxisPosition(0, sf::Joystick::Y) / 100; // for the stick ( see bottom of function )
	
	// If we have selected a child then give it events
	if (hasSelection() && mChildren[mSelectedChild]->isActive())
	{
		mChildren[mSelectedChild]->handleEvent(event);
	}
	else if (event.type == sf::Event::KeyReleased)
	{
		if (event.key.code == sf::Keyboard::W || event.key.code == sf::Keyboard::Up)
		{
			selectPrevious();
		}
		else if (event.key.code == sf::Keyboard::S || event.key.code == sf::Keyboard::Down)
		{
			selectNext();
		}
		else if (event.key.code == sf::Keyboard::Return || event.key.code == sf::Keyboard::Space)
		{
			if (hasSelection())
				mChildren[mSelectedChild]->activate();
		}
	}
	// if mouse button is clicked, and an item was selected,
	// then activate the item (button, etc.)
	else if (event.type == sf::Event::MouseButtonPressed)
	{
		if (event.mouseButton.button == sf::Mouse::Left)
		{
			if (selectMouse())
			{
				if (hasSelection())
					mChildren[mSelectedChild]->activate();
			}
		}
	}
	// if mouse button is clicked, and an item was selected,
	// then activate the item (button, etc.)
	else if (event.type == sf::Event::JoystickMoved && !mIsStickMove)
	{
		if (abs(dirY) > mStickSensitivity)
		{
			if (dirY > 0)
				selectNext();
			else if (dirY < 0)
				selectPrevious();

			mIsStickMove = true;
		}
	}
	else if (event.type == sf::Event::JoystickButtonPressed)
	{
		// 'a' button pressed
		if (sf::Joystick::isButtonPressed(0, 0))
			mChildren[mSelectedChild]->activate();
	}

	if (abs(dirY) <= mStickSensitivity)
		mIsStickMove = false;
}

void Container::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();

	FOREACH(const Component::Ptr& child, mChildren)
		target.draw(*child, states);
}

bool Container::hasSelection() const
{
	return mSelectedChild >= 0;
}

void Container::select(std::size_t index)
{
	if (mChildren[index]->isSelectable())
	{
		if (hasSelection())
			mChildren[mSelectedChild]->deselect();

		mChildren[index]->select();
		mSelectedChild = index;
	}
}

// returns true if item is selected,
// using transform position
bool Container::selectMouse()
{
	int index = 0;

	// check mouse position against world bounds of each item
	// and return true if found
	FOREACH(const Component::Ptr& child, mChildren)
	{		
		if (child->checkWorldBounds())
		{
			if (mChildren[index]->isSelectable())
			{
				if (hasSelection())
					mChildren[mSelectedChild]->deselect();

				mChildren[index]->select();
				mSelectedChild = index;				
			}			

			return true;

		}
		
		index++;		
	}

	return false;
}

void Container::selectNext()
{
	if (!hasSelection())
		return;

	// Search next component that is selectable, wrap around if necessary
	int next = mSelectedChild;
	do
		next = (next + 1) % mChildren.size();
	while (!mChildren[next]->isSelectable());

	// Select that component
	select(next);
}

void Container::selectPrevious()
{
	if (!hasSelection())
		return;

	// Search previous component that is selectable, wrap around if necessary
	int prev = mSelectedChild;
	do
		prev = (prev + mChildren.size() - 1) % mChildren.size();
	while (!mChildren[prev]->isSelectable());

	// Select that component
	select(prev);
}

bool Container::checkWorldBounds()
{
	return false;
}

}
