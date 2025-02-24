﻿#include "PlayerCorpse.h"
#include "../LevelInitialization.h"

namespace Jazz2::Actors
{
	PlayerCorpse::PlayerCorpse()
	{
	}

	Task<bool> PlayerCorpse::OnActivatedAsync(const ActorActivationDetails& details)
	{
		PlayerType playerType = (PlayerType)details.Params[0];
		SetFacingLeft(details.Params[1] != 0);

		SetState(ActorState::ForceDisableCollisions, true);
		SetState(ActorState::CollideWithTileset | ActorState::CollideWithOtherActors | ActorState::ApplyGravitation, false);

		switch (playerType) {
			default:
			case PlayerType::Jazz:
				async_await RequestMetadataAsync("Interactive/PlayerJazz"_s);
				break;
			case PlayerType::Spaz:
				async_await RequestMetadataAsync("Interactive/PlayerSpaz"_s);
				break;
			case PlayerType::Lori:
				async_await RequestMetadataAsync("Interactive/PlayerLori"_s);
				break;
		}

		SetAnimation("Corpse"_s);

		async_return true;
	}
}