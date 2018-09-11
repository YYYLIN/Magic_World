/*
* Copyright (C) 2012 Alec Thomas <alec@swapoff.org>
* All rights reserved.
*
* This software is licensed as described in the file COPYING, which
* you should have received as part of this distribution.
*
* Author: Alec Thomas <alec@swapoff.org>
*/

namespace EntityX
{
	template <typename C>
	BaseComponent::Family Component<C>::family()
	{
		static Family family = family_counter_++;
		//assert(family < MAX_COMPONENTS);
		return family;
	}

	template <typename C, typename ... Args>
	ComponentHandle<C> Entity::assign(Args && ... args)
	{
		assert(valid());
		return pManager->assign<C>(m_id, std::forward<Args>(args) ...);
	}

	template <typename C>
	ComponentHandle<C> Entity::assign_from_copy(const C &_component)
	{
		assert(valid());
		return pManager->assign<C>(m_id, std::forward<const C &>(_component));
	}

	template <typename C, typename ... Args>
	ComponentHandle<C> Entity::replace(Args && ... args)
	{
		assert(valid());
		auto handle = component<C>();
		if (handle)
		{
			*(handle.get()) = C(std::forward<Args>(args) ...);
		}
		else
		{
			handle = pManager->assign<C>(m_id, std::forward<Args>(args) ...);
		}
		return handle;
	}

	template <typename C>
	void Entity::remove()
	{
		assert(valid() && has_component<C>());
		pManager->remove<C>(m_id);
	}

	template <typename C, typename>
	ComponentHandle<C> Entity::GetComponent()
	{
		assert(valid());
		return pManager->GetComponent<C>(m_id);
	}

	template <typename C, typename>
	const ComponentHandle<C, const EntityManager> Entity::GetComponent() const
	{
		assert(valid());
		return const_cast<const EntityManager*>(pManager)->GetComponent<const C>(m_id);
	}

	template <typename ... Components>
	std::tuple<ComponentHandle<Components>...> Entity::GetComponents()
	{
		assert(valid());
		return pManager->GetComponents<Components...>(m_id);
	}

	template <typename ... Components>
	std::tuple<ComponentHandle<const Components, const EntityManager>...> Entity::GetComponents() const
	{
		assert(valid());
		return const_cast<const EntityManager*>(pManager)->GetComponents<const Components...>(m_id);
	}


	template <typename C>
	bool Entity::has_component() const
	{
		assert(valid());
		return pManager->has_component<C>(m_id);
	}

	template <typename A, typename ... Args>
	void Entity::unpack(ComponentHandle<A> &a, ComponentHandle<Args> & ... args)
	{
		assert(valid());
		pManager->unpack(m_id, a, args ...);
	}

	inline bool Entity::valid() const
	{
		return pManager && pManager->valid(m_id);
	}

	inline std::ostream &operator << (std::ostream &out, const Entity::Id &id)
	{
		out << "Entity::Id(" << id.index() << "." << id.version() << ")";
		return out;
	}


	inline std::ostream &operator << (std::ostream &out, const Entity &entity)
	{
		out << "Entity(" << entity.id() << ")";
		return out;
	}


	template <typename C, typename EM>
	inline ComponentHandle<C, EM>::operator bool() const
	{
		return valid();
	}

	template <typename C, typename EM>
	inline bool ComponentHandle<C, EM>::valid() const
	{
		return pManager && pManager->valid(m_id) && pManager->template has_component<C>(m_id);
	}

	template <typename C, typename EM>
	inline C *ComponentHandle<C, EM>::operator -> ()
	{
		assert(valid());
		return pManager->template Get_Component_Ptr<C>(m_id);
	}

	template <typename C, typename EM>
	inline const C *ComponentHandle<C, EM>::operator -> () const
	{
		assert(valid());
		return pManager->template Get_Component_Ptr<C>(m_id);
	}

	template <typename C, typename EM>
	inline C &ComponentHandle<C, EM>::operator * ()
	{
		assert(valid());
		return *pManager->template Get_Component_Ptr<C>(m_id);
	}

	template <typename C, typename EM>
	inline const C &ComponentHandle<C, EM>::operator * () const
	{
		assert(valid());
		return *pManager->template Get_Component_Ptr<C>(m_id);
	}

	template <typename C, typename EM>
	inline C *ComponentHandle<C, EM>::get()
	{
		assert(valid());
		return pManager->template Get_Component_Ptr<C>(m_id);
	}

	template <typename C, typename EM>
	inline const C *ComponentHandle<C, EM>::get() const
	{
		assert(valid());
		return pManager->template Get_Component_Ptr<C>(m_id);
	}

	template <typename C, typename EM>
	inline void ComponentHandle<C, EM>::remove()
	{
		assert(valid());
		pManager->template remove<C>(m_id);
	}

	template <typename C, typename EM>
	inline Entity ComponentHandle<C, EM>::entity()
	{
		assert(valid());
		return pManager->get(m_id);
	}

	template <typename C>
	int32_t EntityManager::component_family()
	{
		return ComponentRealFamily(Component<typename std::remove_const<C>::type>::family());
	}

	template <typename C>
	const int32_t EntityManager::component_family() const
	{
		return ComponentRealFamily(Component<typename std::remove_const<C>::type>::family());
	}
}