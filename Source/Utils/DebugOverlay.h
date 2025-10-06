#ifndef DEBUG_OVERLAY_H
#define DEBUG_OVERLAY_H

#ifdef _DEBUG


class GameList;
class AudioManager;
class GameListManager;


namespace ClassicLauncher::DebugOverlay
{
    
    void Update(AudioManager* audio, GameListManager* gameListManager);

} // namespace ClassicLauncher

#endif


#endif // DEBUG_OVERLAY_H