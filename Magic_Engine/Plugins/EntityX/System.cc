/*
 * Copyright (C) 2012 Alec Thomas <alec@swapoff.org>
 * All rights reserved.
 *
 * This software is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.
 *
 * Author: Alec Thomas <alec@swapoff.org>
 */

#include "System.h"

namespace EntityX {

	BaseSystem::Family BaseSystem::family_counter_;

	BaseSystem::~BaseSystem()
	{
	}

	void SystemManager::update_all(::EntityX::Entity _NowEntity, TimeDelta dt)
	{
		assert(initialized_ && "SystemManager::configure() not called");
		for (auto &pair : systems_) {
			pair.second->Update(entity_manager_, event_manager_, _NowEntity, dt);
		}
	}

	void SystemManager::configure()
	{
		for (auto &pair : systems_)
		{
			pair.second->configure(entity_manager_, event_manager_);
		}
		initialized_ = true;
	}

}  // namespace EntityX
