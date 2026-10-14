#include "Logger.h"

template<typename T>
T* Entity::AddComponent()
{
	static_assert(std::is_base_of<IComponent, T>(), "Given component Type doesn't inherit from IComponent");

	if (GetComponent<T>() != nullptr)
	{
		Logger::Warning(std::string("Cannot add 2 component of type %s to a single Entity", typeid(T).name()));
		return nullptr;
	}
	T* component = new T(*this);
	Components.push_back(component);

	if (IDrawableComponent* drawableComponent = dynamic_cast<IDrawableComponent*>(component))
	{
		DrawableComponents.push_back(drawableComponent);
	}

	return component;
}

template<typename T>
T* Entity::GetComponent() const
{
	for (IComponent* component : Components)
	{
		if (T* c = dynamic_cast<T*>(component))
		{
			return c;
		}
	}
	return nullptr;
}