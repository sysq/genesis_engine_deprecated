#ifndef __editstatus_H__
#define __editstatus_H__

#ifdef __GENESIS_EDITOR__
#	define __EDIT_STATUS__ 1
#else
#	define __EDIT_STATUS__ 0
#endif 

#if __EDIT_STATUS__

namespace App
{
	class EditStatus
	{
	public:
		static bool IsPlayingGame();

		static void SetPlayingGame(bool play);
	private:
		static bool bPlayingGame;
	};

	inline bool EditStatus::IsPlayingGame()
	{
		return bPlayingGame;
	}

	inline void EditStatus::SetPlayingGame(bool play)
	{
		bPlayingGame = play;
	}
}

#endif

#endif //__editstatus_H__