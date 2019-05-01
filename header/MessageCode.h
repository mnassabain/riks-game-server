/**
 * @file MessageCode.h
 * @brief 
 */

#ifndef MESSAGECODES_H
#define MESSAGECODES_H


typedef enum
{
    /* general */
    CODE_ERROR,
    CODE_UNHANDLED,

    /* connection */
    CODE_SIGN_UP,
    CODE_CONNECT,
    CODE_DISCONNECT,

    /* lobby & matchmaking */
    CODE_LOBBY_LIST,
    CODE_CREATE_LOBBY,
    CODE_JOIN_LOBBY,
    CODE_EDIT_LOBBY,
    CODE_LOBBY_STATE,
    CODE_KICK_FROM_LOBBY,
    CODE_KICKED,
    CODE_CLOSE_LOBBY,
    CODE_LEAVE_GAME,
	CODE_START_GAME,

    /* misc */
    CODE_GAME_RESULTS,
    CODE_PLAYER_PROFILE,
    CODE_MATCHMAKING,

    /* gameplay */
	// Player actions
	CODE_END_PHASE,
    CODE_PUT,
	CODE_USE_TOKENS,
    CODE_ATTACK,
    CODE_DEFEND,
	CODE_MOVE,

	// Server gameplay updates
    CODE_REINFORCEMENT,
    CODE_ATTACKED,
    CODE_COMBAT_RESULTS,
    CODE_PLAYER_ELIMINATION,
    CODE_GIVE_TOKENS,
    CODE_CURRENT_PHASE,
    CODE_GAME_OVER,

	// Meta communication
	CODE_CHAT,

	CODE_SYNC_DEMAND,
    CODE_GAME_STATUS,

    CODE_POKE,

    CODE_HOVER,
    CODE_SELECT_TOKEN,
    CODE_DIE, //To facilitate client gestion of deaths

} MessageCode;



#endif // MESSAGE_CODES_H
