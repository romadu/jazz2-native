﻿#include "OneUpCollectible.h"
#include "../../LevelInitialization.h"
#include "../Player.h"

namespace Jazz2::Actors::Collectibles
{
	OneUpCollectible::OneUpCollectible()
	{
	}

	Task<bool> OneUpCollectible::OnActivatedAsync(const ActorActivationDetails& details)
	{
		async_await CollectibleBase::OnActivatedAsync(details);

		_scoreValue = 1000;

		async_await RequestMetadataAsync("Collectible/OneUp"_s);

		SetAnimation("OneUp"_s);

		async_return true;
	}

	void OneUpCollectible::OnCollect(Player* player)
	{
		player->AddLives(1);

		CollectibleBase::OnCollect(player);
	}
}