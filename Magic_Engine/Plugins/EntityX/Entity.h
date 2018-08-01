/*
 * Copyright (C) 2012 Alec Thomas <alec@swapoff.org>
 * All rights reserved.
 *
 * This software is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.
 *
 * Author: Alec Thomas <alec@swapoff.org>
 */

#ifndef _ENTITYX_ENTITY_H_
#define _ENTITYX_ENTITY_H_


#include <cstdint>
#include <tuple>
#include <new>
#include <cstdlib>
#include <algorithm>
#include <bitset>
#include <cassert>
#include <iostream>
#include <iterator>
#include <list>
#include <memory>
#include <set>
#include <string>
#include <utility>
#include <vector>
#include <type_traits>
#include <functional>

#include "help/Pool.h"
#include "config.h"
#include "Event.h"
#include "help/NonCopyable.h"
#include "Define/Magic_Macro.h"

#pragma warning(push)
#pragma warning(disable:4251)

namespace EntityX
{
	typedef std::uint32_t uint32_t;
	typedef std::uint64_t uint64_t;

	class EntityManager;


	template <typename C, typename EM = EntityManager>
	class ComponentHandle;



	/** A convenience handle around an Entity::Id.
	 *
	 * If an entity is destroyed, any copies will be invalidated. Use valid() to
	 * check for validity before using.
	 *
	 * Create entities with `EntityManager`:
	 *
	 *     Entity entity = entity_manager->create();
	 */
	class DLL_MAGIC_ENGINE_OUTPUT_INPUT Entity
	{
	public:
		struct Id
		{
			Id() :m_id(0) {}
			explicit Id(uint64_t id) : m_id(id) {}
			Id(uint32_t index, uint32_t version) : m_id(uint64_t(index) | uint64_t(version) << 32UL) {}

			uint64_t id() const { return m_id; }

			bool operator == (const Id &other) const { return m_id == other.m_id; }
			bool operator != (const Id &other) const { return m_id != other.m_id; }
			bool operator < (const Id &other) const { return m_id < other.m_id; }

			uint32_t index() const { return m_id & 0xffffffffUL; }
			uint32_t version() const { return m_id >> 32; }

		private:
			friend class EntityManager;

			uint64_t m_id;
		};


		/**
		 * Id of an invalid Entity.
		 */
		static const Id INVALID;

		Entity() = default;
		Entity(EntityManager *manager, Entity::Id id) : pManager(manager), m_id(id) {}
		Entity(const Entity &other) = default;
		Entity &operator = (const Entity &other) = default;

		/**
		 * Check if Entity handle is invalid.
		 */
		 //检查实体句柄是否无效。
		operator bool() const {
			return valid();
		}

		bool operator == (const Entity &other) const {
			return other.pManager == pManager && other.m_id == m_id;
		}

		bool operator != (const Entity &other) const {
			return !(other == *this);
		}

		bool operator < (const Entity &other) const {
			return other.m_id < m_id;
		}

		/**
		 * Is this Entity handle valid?
		 *
		 * In older versions of EntityX, there were no guarantees around entity
		 * validity if a previously allocated entity slot was reassigned. That is no
		 * longer the case: if a slot is reassigned, old Entity::Id's will be
		 * invalid.
		 */
		 //检查是否是有效实体,是否存在
		bool valid() const;

		/**
		 * Invalidate Entity handle, disassociating it from an EntityManager and invalidating its ID.
		 *
		 * Note that this does *not* affect the underlying entity and its
		 * components. Use destroy() to destroy the associated Entity and components.
		 */
		 //清楚当前数据,对存放的实体并无影响
		void invalidate();

		Id id() const { return m_id; }

		//分配组件(属性)变长模板参数
		template <typename C, typename ... Args>
		ComponentHandle<C> assign(Args && ... args);

		//拷贝组件(属性)
		template <typename C>
		ComponentHandle<C> assign_from_copy(const C &_component);

		//更改值
		template <typename C, typename ... Args>
		ComponentHandle<C> replace(Args && ... args);

		template <typename C>
		void remove();

		//获取某个组件不为const
		template <typename C, typename = typename std::enable_if<!std::is_const<C>::value>::type>
		ComponentHandle<C> GetComponent();

		//获取某个组件为const
		template <typename C, typename = typename std::enable_if<std::is_const<C>::value>::type>
		const ComponentHandle<C, const EntityManager> GetComponent() const;

		//获取多个组件的集合不为const
		template <typename ... Components>
		std::tuple<ComponentHandle<Components>...> GetComponents();

		//获取多个组件的为const
		template <typename ... Components>
		std::tuple<ComponentHandle<const Components, const EntityManager>...> GetComponents() const;

		//是否存在某个属性
		template <typename C>
		bool has_component() const;

		//同时获取多个属性的值
		template <typename A, typename ... Args>
		void unpack(ComponentHandle<A> &a, ComponentHandle<Args> & ... args);

		/**
		 * Destroy and invalidate this Entity.
		 */
		 //删除这个实体
		void destroy();

		std::bitset<MAX_COMPONENTS> GetComponentMask() const;

	private:
		EntityManager *pManager = nullptr;
		Entity::Id m_id = INVALID;
	};


	/**
	 * A ComponentHandle<C> is a wrapper around an instance of a component.
	 *
	 * It provides safe access to components. The handle will be invalidated under
	 * the following conditions:
	 *
	 * - If a component is removed from its host entity.
	 * - If its host entity is destroyed.
	 */
	template <typename C, typename EM>
	class ComponentHandle
	{
	public:
		typedef C ComponentType;

		ComponentHandle() : pManager(nullptr) {}

		bool valid() const;
		operator bool() const;

		C *operator -> ();
		const C *operator -> () const;

		C &operator * ();
		const C &operator * () const;

		C *get();
		const C *get() const;

		/**
		 * Remove the component from its entity and destroy it.
		 */
		void remove();

		/**
		 * Returns the Entity associated with the component
		 */
		Entity entity();

		bool operator == (const ComponentHandle<C> &other) const {
			return pManager == other.pManager && m_id == other.m_id;
		}

		bool operator != (const ComponentHandle<C> &other) const {
			return !(*this == other);
		}

	private:
		friend class EntityManager;

		ComponentHandle(EM *manager, Entity::Id id) :
			pManager(manager), m_id(id) {}

		EM *pManager;
		Entity::Id m_id;
	};


	/**
	 * Base component class, only used for insertion into collections.
	 *
	 * Family is used for registration.
	 */
	struct BaseComponent
	{
	public:
		typedef size_t Family;

		// NOTE: Component memory is *always* managed by the EntityManager.
		// Use Entity::destroy() instead.
		void operator delete(void *p) { fail(); }
		void operator delete[](void *p) { fail(); }


	protected:
		static void fail()
		{
#if defined(_HAS_EXCEPTIONS) || defined(__EXCEPTIONS)
			throw std::bad_alloc();
#else
			std::abort();
#endif
		}

		static Family family_counter_;
	};


	/**
	 * Component implementations should inherit from this.
	 *
	 * Components MUST provide a no-argument constructor.
	 * Components SHOULD provide convenience constructors for initializing on assignment to an Entity::Id.
	 *
	 * This is a struct to imply that components should be data-only.
	 *
	 * Usage:
	 *
	 *     struct Position : public Component<Position> {
	 *       Position(float x = 0.0f, float y = 0.0f) : x(x), y(y) {}
	 *
	 *       float x, y;
	 *     };
	 *
	 * family() is used for registration.
	 */
	template <typename Derived>
	struct Component : public BaseComponent
	{
	public:
		typedef ComponentHandle<Derived> Handle;
		typedef ComponentHandle<const Derived, const EntityManager> ConstHandle;

	private:
		friend class EntityManager;
		/// Used internally for registration.
		static Family family();
	};


	/**
	 * Emitted when an entity is added to the system.
	 */
	 //将实体添加到系统时发出。
	struct EntityCreatedEvent : public Event<EntityCreatedEvent>
	{
		explicit EntityCreatedEvent(Entity entity) : entity(entity) {}
		virtual ~EntityCreatedEvent();

		Entity entity;
	};


	/**
	 * Called just prior to an entity being destroyed.
	 */
	struct EntityDestroyedEvent : public Event<EntityDestroyedEvent>
	{
		explicit EntityDestroyedEvent(Entity entity) : entity(entity) {}
		virtual ~EntityDestroyedEvent();

		Entity entity;
	};


	/**
	 * Emitted when any component is added to an entity.
	 */
	template <typename C>
	struct ComponentAddedEvent : public Event<ComponentAddedEvent<C>>
	{
		ComponentAddedEvent(Entity entity, ComponentHandle<C> component) :
			entity(entity), component(component) {}

		Entity entity;
		ComponentHandle<C> component;
	};

	/**
	 * Emitted when any component is removed from an entity.
	 */
	template <typename C>
	struct ComponentRemovedEvent : public Event<ComponentRemovedEvent<C>>
	{
		ComponentRemovedEvent(Entity entity, ComponentHandle<C> component) :
			entity(entity), component(component) {}

		Entity entity;
		ComponentHandle<C> component;
	};

	/**
	 * Helper class to perform component operations in a typed manner.
	 */
	class BaseComponentHelper
	{
	public:
		virtual ~BaseComponentHelper() {}
		virtual void remove_component(Entity e) = 0;
		virtual void copy_component_to(Entity source, Entity target) = 0;
	};

	template <typename C>
	class ComponentHelper : public BaseComponentHelper
	{
	public:
		void remove_component(Entity e) override {
			e.remove<C>();
		}
		void copy_component_to(Entity source, Entity target) override 
		{
			target.assign_from_copy<C>(*(source.GetComponent<C>().get()));
		}
	};

	/**
	 * Manages Entity::Id creation and component assignment.
	 */
	class EntityManager : private ::EntityX::help::NonCopyable
	{
	public:
		typedef std::bitset<MAX_COMPONENTS> ComponentMask;

		explicit EntityManager(EventManager &event_manager);
		virtual ~EntityManager();

		/// An iterator over a view of the entities in an EntityManager.
		/// If All is true it will iterate over all valid entities and will ignore the entity mask.
		template <class Delegate, bool All = false>
		class ViewIterator : public std::iterator<std::input_iterator_tag, Entity::Id>
		{
		public:
			Delegate &operator ++()
			{
				++i_;
				next();
				return *static_cast<Delegate*>(this);
			}
			bool operator == (const Delegate& rhs) const { return i_ == rhs.i_; }
			bool operator != (const Delegate& rhs) const { return i_ != rhs.i_; }
			Entity operator * () { return Entity(pManager, pManager->create_id(i_)); }
			const Entity operator * () const { return Entity(pManager, pManager->create_id(i_)); }

		protected:
			ViewIterator(EntityManager *manager, uint32_t index)
				: pManager(manager), i_(index), capacity_(pManager->capacity()), free_cursor_(~0UL)
			{
				if (All)
				{
					std::sort(pManager->free_list_.begin(), pManager->free_list_.end());
					free_cursor_ = 0;
				}
			}
			ViewIterator(EntityManager *manager, const ComponentMask mask, uint32_t index)
				: pManager(manager), mask_(mask), i_(index), capacity_(pManager->capacity()), free_cursor_(~0UL)
			{
				if (All)
				{
					std::sort(pManager->free_list_.begin(), pManager->free_list_.end());
					free_cursor_ = 0;
				}
			}

			void next()
			{
				while (i_ < capacity_ && !predicate())
				{
					++i_;
				}

				if (i_ < capacity_) {
					Entity entity = pManager->get(pManager->create_id(i_));
					static_cast<Delegate*>(this)->next_entity(entity);
				}
			}

			inline bool predicate()
			{
				return (All && valid_entity()) || (pManager->entity_component_mask_[i_] & mask_) == mask_;
			}

			inline bool valid_entity()
			{
				const std::vector<uint32_t> &free_list = pManager->free_list_;
				if (free_cursor_ < free_list.size() && free_list[free_cursor_] == i_) {
					++free_cursor_;
					return false;
				}
				return true;
			}

			EntityManager *pManager;
			ComponentMask mask_;
			uint32_t i_;
			size_t capacity_;
			size_t free_cursor_;
		};

		template <bool All>
		class BaseView
		{
		public:
			class Iterator : public ViewIterator<Iterator, All>
			{
			public:
				Iterator(EntityManager *manager, const ComponentMask mask,
					uint32_t index) : ViewIterator<Iterator, All>(manager, mask, index)
				{
					ViewIterator<Iterator, All>::next();
				}

				void next_entity(Entity &entity) {}
			};

			Iterator begin() { return Iterator(pManager, mask_, 0); }
			Iterator end() { return Iterator(pManager, mask_, uint32_t(pManager->capacity())); }
			const Iterator begin() const { return Iterator(pManager, mask_, 0); }
			const Iterator end() const { return Iterator(pManager, mask_, pManager->capacity()); }

		private:
			friend class EntityManager;

			explicit BaseView(EntityManager *manager) : pManager(manager) { mask_.set(); }
			BaseView(EntityManager *manager, ComponentMask mask) :
				pManager(manager), mask_(mask) {}

			EntityManager *pManager;
			ComponentMask mask_;
		};

		template <bool All, typename ... Components>
		class TypedView : public BaseView<All>
		{
		public:
			template <typename T> struct identity { typedef T type; };

			void each(typename identity<std::function<void(Entity entity, Components&...)>>::type f) {
				for (auto it : *this)
					f(it, *(it.template GetComponent<Components>().get())...);
			}

		private:
			friend class EntityManager;

			explicit TypedView(EntityManager *manager) : BaseView<All>(manager) {}
			TypedView(EntityManager *manager, ComponentMask mask) : BaseView<All>(manager, mask) {}
		};

		template <typename ... Components> using View = TypedView<false, Components...>;
		typedef BaseView<true> DebugView;

		template <typename ... Components>
		class UnpackingView
		{
		public:
			struct Unpacker
			{
				explicit Unpacker(ComponentHandle<Components> & ... handles) :
					handles(std::tuple<ComponentHandle<Components> & ...>(handles...)) {}

				void unpack(Entity &entity) const
				{
					unpack_<0, Components...>(entity);
				}


			private:
				template <int N, typename C>
				void unpack_(Entity &entity) const
				{
					std::get<N>(handles) = entity.GetComponent<C>();
				}

				template <int N, typename C0, typename C1, typename ... Cn>
				void unpack_(Entity &entity) const
				{
					std::get<N>(handles) = entity.GetComponent<C0>();
					unpack_<N + 1, C1, Cn...>(entity);
				}

				std::tuple<ComponentHandle<Components> & ...> handles;
			};


			class Iterator : public ViewIterator<Iterator>
			{
			public:
				Iterator(EntityManager *manager,
					const ComponentMask mask,
					uint32_t index,
					const Unpacker &unpacker) : ViewIterator<Iterator>(manager, mask, index), unpacker_(unpacker) {
					ViewIterator<Iterator>::next();
				}

				void next_entity(Entity &entity) {
					unpacker_.unpack(entity);
				}

			private:
				const Unpacker &unpacker_;
			};


			Iterator begin() { return Iterator(pManager, mask_, 0, unpacker_); }
			Iterator end() { return Iterator(pManager, mask_, static_cast<uint32_t>(pManager->capacity()), unpacker_); }
			const Iterator begin() const { return Iterator(pManager, mask_, 0, unpacker_); }
			const Iterator end() const { return Iterator(pManager, mask_, static_cast<uint32_t>(pManager->capacity()), unpacker_); }


		private:
			friend class EntityManager;

			UnpackingView(EntityManager *manager, ComponentMask mask, ComponentHandle<Components> & ... handles) :
				pManager(manager), mask_(mask), unpacker_(handles...) {}

			EntityManager *pManager;
			ComponentMask mask_;
			Unpacker unpacker_;
		};

		/**
		 * Number of managed entities.
		 */
		size_t size() const { return entity_component_mask_.size() - free_list_.size(); }

		/**
		 * Current entity capacity.
		 */
		size_t capacity() const { return entity_component_mask_.size(); }

		/**
		 * Return true if the given entity ID is still valid.
		 */
		bool valid(Entity::Id id) const
		{
			return id.index() < entity_version_.size() && entity_version_[id.index()] == id.version();
		}

		/**
		 * Create a new Entity::Id.
		 *
		 * Emits EntityCreatedEvent.
		 */
		Entity create()
		{
			uint32_t index, version;
			if (free_list_.empty())
			{
				index = index_counter_++;
				accomodate_entity(index);
				version = entity_version_[index] = 1;
			}
			else
			{
				index = free_list_.back();
				free_list_.pop_back();
				version = entity_version_[index];
			}
			Entity entity(this, Entity::Id(index, version));
			event_manager_.emit<EntityCreatedEvent>(entity);
			return entity;
		}

		/**
		 * Create a new Entity by copying another. Copy-constructs each component.
		 *
		 * Emits EntityCreatedEvent.
		 */
		Entity create_from_copy(Entity original)
		{
			assert(original.valid());
			auto clone = create();
			auto mask = original.GetComponentMask();
			for (size_t i = 0; i < component_helpers_.size(); i++)
			{
				BaseComponentHelper *helper = component_helpers_[i];
				if (helper && mask.test(i))
					helper->copy_component_to(original, clone);
			}
			return clone;
		}


		/**
		 * Destroy an existing Entity::Id and its associated Components.
		 *
		 * Emits EntityDestroyedEvent.
		 */
		void destroy(Entity::Id entity)
		{
			assert_valid(entity);
			uint32_t index = entity.index();
			auto mask = entity_component_mask_[index];
			for (size_t i = 0; i < component_helpers_.size(); i++)
			{
				BaseComponentHelper *helper = component_helpers_[i];
				if (helper && mask.test(i))
					helper->remove_component(Entity(this, entity));
			}
			event_manager_.emit<EntityDestroyedEvent>(Entity(this, entity));
			entity_component_mask_[index].reset();
			entity_version_[index]++;
			free_list_.push_back(index);
		}

		Entity get(Entity::Id id)
		{
			assert_valid(id);
			return Entity(this, id);
		}

		/**
		 * Create an Entity::Id for a slot.
		 *
		 * NOTE: Does *not* check for validity, but the Entity::Id constructor will
		 * fail if the ID is invalid.
		 */
		Entity::Id create_id(uint32_t index) const
		{
			return Entity::Id(index, entity_version_[index]);
		}

		/**
		 * Assign a Component to an Entity::Id, passing through Component constructor arguments.
		 *
		 *     Position &position = em.assign<Position>(e, x, y);
		 *
		 * @returns Smart pointer to newly created component.
		 */
		template <typename C, typename ... Args>
		ComponentHandle<C> assign(Entity::Id id, Args && ... args)
		{
			assert_valid(id);
			const BaseComponent::Family family = component_family<C>();
			assert(!entity_component_mask_[id.index()].test(family));

			// Placement new into the component pool.
			//将新增加到组件池中。
			Pool<C> *pool = accomodate_component<C>();
			::new(pool->get(id.index())) C(std::forward<Args>(args) ...);

			// Set the bit for this component.
			entity_component_mask_[id.index()].set(family);

			// Create and return handle.
			ComponentHandle<C> _component(this, id);
			event_manager_.emit<ComponentAddedEvent<C>>(Entity(this, id), _component);
			return _component;
		}

		/**
		 * Remove a Component from an Entity::Id
		 *
		 * Emits a ComponentRemovedEvent<C> event.
		 */
		template <typename C>
		void remove(Entity::Id id)
		{
			assert_valid(id);
			const BaseComponent::Family family = component_family<C>();
			const uint32_t index = id.index();

			// Find the pool for this component family.
			BasePool *pool = component_pools_[family];
			ComponentHandle<C> _component(this, id);
			event_manager_.emit<ComponentRemovedEvent<C>>(Entity(this, id), _component);

			// Remove component bit.
			entity_component_mask_[id.index()].reset(family);

			// Call destructor.
			pool->destroy(index);
		}

		/**
		 * Check if an Entity has a component.
		 */
		template <typename C>
		bool has_component(Entity::Id id) const
		{
			assert_valid(id);
			size_t family = component_family<C>();
			// We don't bother checking the component mask, as we return a nullptr anyway.
			if (family >= component_pools_.size())
				return false;
			BasePool *pool = component_pools_[family];
			if (!pool || !entity_component_mask_[id.index()][family])
				return false;
			return true;
		}

		/**
		 * Retrieve a Component assigned to an Entity::Id.
		 *
		 * @returns Pointer to an instance of C, or nullptr if the Entity::Id does not have that Component.
		 */
		template <typename C, typename = typename std::enable_if<!std::is_const<C>::value>::type>
		ComponentHandle<C> GetComponent(Entity::Id id)
		{
			assert_valid(id);
			size_t family = component_family<C>();
			// We don't bother checking the component mask, as we return a nullptr anyway.
			if (family >= component_pools_.size())
				return ComponentHandle<C>();
			BasePool *pool = component_pools_[family];
			if (!pool || !entity_component_mask_[id.index()][family])
				return ComponentHandle<C>();
			return ComponentHandle<C>(this, id);
		}

		/**
		 * Retrieve a Component assigned to an Entity::Id.
		 *
		 * @returns Component instance, or nullptr if the Entity::Id does not have that Component.
		 */
		template <typename C, typename = typename std::enable_if<std::is_const<C>::value>::type>
		const ComponentHandle<C, const EntityManager> GetComponent(Entity::Id id) const
		{
			assert_valid(id);
			size_t family = component_family<C>();
			// We don't bother checking the component mask, as we return a nullptr anyway.
			if (family >= component_pools_.size())
				return ComponentHandle<C, const EntityManager>();
			BasePool *pool = component_pools_[family];
			if (!pool || !entity_component_mask_[id.index()][family])
				return ComponentHandle<C, const EntityManager>();
			return ComponentHandle<C, const EntityManager>(this, id);
		}

		template <typename ... Components>
		std::tuple<ComponentHandle<Components>...> GetComponents(Entity::Id id) {
			return std::make_tuple(GetComponent<Components>(id)...);
		}

		template <typename ... Components>
		std::tuple<ComponentHandle<const Components, const EntityManager>...> GetComponents(Entity::Id id) const
		{
			return std::make_tuple(GetComponent<const Components>(id)...);
		}

		/**
		 * Find Entities that have all of the specified Components.
		 *
		 * @code
		 * for (Entity entity : entity_manager.entities_with_components<Position, Direction>()) {
		 *   ComponentHandle<Position> position = entity.GetComponent<Position>();
		 *   ComponentHandle<Direction> direction = entity.GetComponent<Direction>();
		 *
		 *   ...
		 * }
		 * @endcode
		 */
		template <typename ... Components>
		View<Components...> entities_with_components()
		{
			//合并掩码
			auto mask = GetComponentMask<Components ...>();
			return View<Components...>(this, mask);
		}

		template <typename T> struct identity { typedef T type; };

		template <typename ... Components>
		void each(typename identity<std::function<void(Entity entity, Components&...)>>::type f)
		{
			return entities_with_components<Components...>().each(f);
		}

		/**
		 * Find Entities that have all of the specified Components and assign them
		 * to the given parameters.
		 *
		 * @code
		 * ComponentHandle<Position> position;
		 * ComponentHandle<Direction> direction;
		 * for (Entity entity : entity_manager.entities_with_components(position, direction)) {
		 *   // Use position and component here.
		 * }
		 * @endcode
		 */
		template <typename ... Components>
		UnpackingView<Components...> entities_with_components(ComponentHandle<Components> & ... components)
		{
			auto mask = GetComponentMask<Components...>();
			return UnpackingView<Components...>(this, mask, components...);
		}

		/**
		 * Iterate over all *valid* entities (ie. not in the free list). Not fast,
		 * so should only be used for debugging.
		 *
		 * @code
		 * for (Entity entity : entity_manager.entities_for_debugging()) {}
		 *
		 * @return An iterator view over all valid entities.
		 */
		DebugView entities_for_debugging()
		{
			return DebugView(this);
		}

		template <typename C>
		void unpack(Entity::Id id, ComponentHandle<C> &a)
		{
			assert_valid(id);
			a = GetComponent<C>(id);
		}

		/**
		 * Unpack components directly into pointers.
		 *
		 * Components missing from the entity will be set to nullptr.
		 *
		 * Useful for fast bulk iterations.
		 *
		 * ComponentHandle<Position> p;
		 * ComponentHandle<Direction> d;
		 * unpack<Position, Direction>(e, p, d);
		 */
		template <typename A, typename ... Args>
		void unpack(Entity::Id id, ComponentHandle<A> &a, ComponentHandle<Args> & ... args)
		{
			assert_valid(id);
			a = GetComponent<A>(id);
			unpack<Args ...>(id, args ...);
		}

		/**
		 * Destroy all entities and reset the EntityManager.
		 */
		void reset();

		// Retrieve the component family for a type.
		template <typename C>
		static BaseComponent::Family component_family()
		{
			return Component<typename std::remove_const<C>::type>::family();
		}

	private:
		friend class Entity;
		template <typename C, typename EM>
		friend class ComponentHandle;


		inline void assert_valid(Entity::Id id) const
		{
			assert(id.index() < entity_component_mask_.size() && "Entity::Id ID outside entity vector range");
			assert(entity_version_[id.index()] == id.version() && "Attempt to access Entity via a stale Entity::Id");
		}

		template <typename C>
		C *Get_Component_Ptr(Entity::Id id)
		{
			assert(valid(id));
			BasePool *pool = component_pools_[component_family<C>()];
			assert(pool);
			return static_cast<C*>(pool->get(id.index()));
		}

		template <typename C>
		const C *Get_Component_Ptr(Entity::Id id) const
		{
			assert_valid(id);
			BasePool *pool = component_pools_[component_family<C>()];
			assert(pool);
			return static_cast<const C*>(pool->get(id.index()));
		}

		ComponentMask GetComponentMask(Entity::Id id)
		{
			assert_valid(id);
			return entity_component_mask_.at(id.index());
		}

		template <typename C>
		ComponentMask GetComponentMask()
		{
			ComponentMask mask;
			mask.set(component_family<C>());
			return mask;
		}

		template <typename C1, typename C2, typename ... Components>
		ComponentMask GetComponentMask()
		{
			return GetComponentMask<C1>() | GetComponentMask<C2, Components ...>();
		}

		template <typename C>
		ComponentMask GetComponentMask(const ComponentHandle<C> &c)
		{
			return GetComponentMask<C>();
		}

		template <typename C1, typename ... Components>
		ComponentMask GetComponentMask(const ComponentHandle<C1> &c1, const ComponentHandle<Components> &... args)
		{
			return GetComponentMask<C1, Components ...>();
		}

		inline void accomodate_entity(uint32_t index)
		{
			if (entity_component_mask_.size() <= index)
			{
				entity_component_mask_.resize(index + 1);
				entity_version_.resize(index + 1);
				for (BasePool *pool : component_pools_)
					if (pool) pool->expand(index + 1);
			}
		}

		template <typename C>
		Pool<C> *accomodate_component()
		{
			BaseComponent::Family family = component_family<C>();
			if (component_pools_.size() <= family)
			{
				component_pools_.resize(family + 1, nullptr);
			}
			if (!component_pools_[family])
			{
				Pool<C> *pool = new Pool<C>();
				pool->expand(index_counter_);
				component_pools_[family] = pool;
			}
			if (component_helpers_.size() <= family)
			{
				component_helpers_.resize(family + 1, nullptr);
			}
			if (!component_helpers_[family])
			{
				ComponentHelper<C> *helper = new ComponentHelper<C>();
				component_helpers_[family] = helper;
			}
			return static_cast<Pool<C>*>(component_pools_[family]);
		}


		uint32_t index_counter_ = 0;

		EventManager &event_manager_;
		// Each element in component_pools_ corresponds to a Pool for a Component.
		// The index into the vector is the Component::family().
		// component_pools_中的每个元素对应一个组件池。
		// 矢量的索引是Component :: family（）。
		std::vector<BasePool*> component_pools_;
		// Each element in component_helpers_ corresponds to a ComponentHelper for a Component type.
		// The index into the vector is the Component::family().
		// component_helpers_中的每个元素都对应一个ComponentHelper作为组件类型。
		// 矢量的索引是Component :: family（）。
		std::vector<BaseComponentHelper*> component_helpers_;
		// Bitmask of components associated with each entity. Index into the vector is the Entity::Id.
		// 与每个实体关联的组件的位掩码。 向量中的索引是Entity :: Id。
		std::vector<ComponentMask> entity_component_mask_;
		// Vector of entity version numbers. Incremented each time an entity is destroyed
		// 实体版本号的向量。 每次实体销毁时都会增加
		std::vector<uint32_t> entity_version_;
		// List of available entity slots.
		// 可用实体插槽列表。
		std::vector<uint32_t> free_list_;
	};
}  // namespace EntityX


namespace std
{
	template <> struct hash<EntityX::Entity>
	{
		std::size_t operator () (const EntityX::Entity &entity) const
		{
			return static_cast<std::size_t>(entity.id().index() ^ entity.id().version());
		}
	};

	template <> struct hash<const EntityX::Entity>
	{
		std::size_t operator () (const EntityX::Entity &entity) const {
			return static_cast<std::size_t>(entity.id().index() ^ entity.id().version());
		}
	};
}


#include "Entity.inl"

#pragma warning(pop)

#endif