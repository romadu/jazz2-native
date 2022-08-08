﻿#pragma once

#include "../../ActorBase.h"
#include "../../LevelInitialization.h"

namespace Jazz2::Actors
{
	class Player;
}

namespace Jazz2::Actors::Weapons
{
	class ShotBase : public ActorBase
	{
	public:
		ShotBase();

		bool OnHandleCollision(std::shared_ptr<ActorBase> other) override;

		int GetStrength() {
			return _strength;
		}

		Player* GetOwner();
		virtual WeaponType GetWeaponType();
		void TriggerRicochet(ActorBase* other);

	protected:
		std::shared_ptr<ActorBase> _owner;
		float _timeLeft;
		bool _firedUp;
		uint8_t _upgrades;
		int _strength;
		ActorBase* _lastRicochet;

		Task<bool> OnActivatedAsync(const ActorActivationDetails& details) override;
		void OnUpdate(float timeMult) override;
		virtual void OnRicochet();

		void TryMovement(float timeMult, TileCollisionParams& params);

	private:
		int _lastRicochetFrame;

	};
}