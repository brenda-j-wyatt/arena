#include "ArenagameGameMode.h"
#include "Player/ArenagameCharacter.h"

AArenagameGameMode::AArenagameGameMode()
{
    DefaultPawnClass = AArenagameCharacter::StaticClass();
}
