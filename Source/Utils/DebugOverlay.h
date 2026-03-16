#ifndef DEBUG_OVERLAY_H
#define DEBUG_OVERLAY_H

#ifdef _DEBUG


class GameList;
class AudioManager;
class GameListManager;
class Window;


namespace ClassicLauncher::DebugOverlay
{

    void Update(AudioManager* audio, GameListManager* gameListManager, Window* window);

} // namespace ClassicLauncher::DebugOverlay

#endif


#endif // DEBUG_OVERLAY_H