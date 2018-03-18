/*
 * Copyright (C) 2014 Alec Thomas <alec@swapoff.org>
 * All rights reserved.
 *
 * This software is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.
 *
 * Author: Alec Thomas <alec@swapoff.org>
 */

#ifndef _ENTITYX_QUICK_H_
#define _ENTITYX_QYICK_H_

#include "Entity.h"
#include "Event.h"
#include "System.h"
#include "config.h"

namespace EntityX
{

	/**
	 * A convenience class for instantiating an EventManager, EntityManager and
	 * SystemManager.
	 */
	class EntityX
	{
	public:
		EntityX() : m_entities(m_events), m_systems(m_entities, m_events) {}

		EventManager m_events;
		EntityManager m_entities;
		SystemManager m_systems;
	};

}  // namespace EntityX


#endif