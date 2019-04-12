# Messages format

* [Client to server](#client-to-server)
* [Server to client](#server-to-client)


# Client to server

* [ATTACK](#attack)
* [CONNECT](#connect)
* [CREATE_LOBBY](#create_lobby)
* [DEFEND](#defend)
* [DISCONNECT](#disconnect)
* [EDIT_LOBBY](#edit_lobby)
* [END_PHASE](#end_phase)
* [GAME_STATUS](#game_status)
* [JOIN_LOBBY](#join_lobby)
* [KICK_FROM_LOBBY](#kick_from_lobby)
* [LEAVE_GAME](#leave_game)
* [LOBBY_LIST](#lobby_list)
* [MOVE](#move)
* [PLAYER_PROFILE](#player_profile)
* [PUT](#put)
* [SIGN_UP](#sign_up)
* [START_GAME](#start_game)
* [USE_TOKENS](#use_tokens)

### ATTACK:
```json
{
    type: ATTACK,
    data:
    {
	    source: "...",
	    destination: "...",
	    units: "..."
    }
}
```
### CONNECT:
```json
{
    type: CONNECT,
    data: 
    {
        userID: "...",
        userPassword: "..."
    }
}
```
### CREATE_LOBBY:
```json
{
    type: CREATE_LOBBY,
    data:
    {
        lobbyName: "...",
        lobbyPassword: "...",
        maxPlayers: "...",
        mapName: "...",
        hidden: <true|false>
    }
}
```

### DEFEND:
```json
{
    type: DEFEND,
    data:
    {
	    units: "..."
    }
}
```

### DISCONNECT:
```json
{
    type: DISCONNECT
}
```

### EDIT_LOBBY:
```json
{
    type: EDIT_LOBBY,
    data:
    {
        lobbyName: "...",
        lobbyPassword: "...",
        maxPlayers: "...",
        mapName: "...",
        hidden: <true|false>
    }
}
```

### END_PHASE:
```json
{
    type: END_PHASE
}
```

### GAME_STATUS:
```json
{
    type: GAME_STATUS
}
```

### JOIN_LOBBY:
```json
{
    type: JOIN_LOBBY,
    data:
    {
        lobbyID: "...",
        lobbyPassword: "..."
    }
}
```

### KICK_FROM_LOBBY:
```json
{
    type: KICK_FROM_LOBBY,
    data:
    {
        playerToBeKicked: "..."
    }
}
```

### LEAVE_GAME:
```json
{
    type: LEAVE_GAME
}
```

### LOBBY_LIST:
```json
{
    type: LOBBY_LIST
}
```

### MOVE:
```json
{
    type: MOVE,
    data:
    {
	    source: "...",
	    destination: "...",
	    units: "..."
    }
}
```

### PLAYER_PROFILE:
```json
{
    type: PLAYER_PROFILE,
    data:
    {
        playerName: "..."
    }
}
```

### PUT:
```json
{
    type: PUT,
    data:
    {
	    territory: "...",
	    units: "..."
    }
}
```

### SIGN_UP:
```json
{
    type: SIGN_UP,
    data:
    {
        userID: "...",
        userPassword: "..."
    }
}
```

### START_GAME:
```json
{
    type: START_GAME,
}
```

### USE_TOKENS:
```json
{
    type: USE_TOKENS,
    data:
    {
	    token1: "...",
	    token2: "...",
	    token3: "..."
    }
}
```


## Server to client
* [Server response](#server-response)
* [ATTACK](#attack)
* [ATTACKED](#attacked)
* [COMBAT_RESULTS]()
* [CURRENT_PHASE]()
* [DEFEND]()
* [ERROR]()
* [GAME_OVER]()
* [GAME_RESULTS]()
* [GAME_STATUS]()
* [GIVE_TOKENS]()
* [KICKED]()
* [LEAVE_GAME]()
* [LOBBY_STATE]()
* [MOVE]()
* [PLAYER_ELIMINATION]()
* [PLAYER_PROFILE]()
* [PUT]()
* [REINFORCEMENT]()
* [START_GAME]()
* [USE_TOKENS]()

### Server response:
(Standard server response for "personnal" messages, for example SIGN_UP, CONNECT, JOIN_LOBBY,...)
```json
{
    type: MESSAGE_TYPE,
    data: {
        error: <true|false>
        response: "..."
    }
}
```

### ATTACK:
```json
{
    type: ATTACK,
    data:
    {
        attackerName: "...",
        defenderName: "...",
	    source: "...",
	    destination: "...",
	    units: "..."
    }
}
```

### ATTACKED:
```json
{
    type: ATTACKED,
    data:
    {
	    units: "..."
    }
}
```

### COMBAT_RESULTS:
```json
{
    type: COMBAT_RESULTS,
    data:
    {
	    source: "...",
	    destination: "...",
	    attackerLoss: "...",
	    defenderLoss: "..."
    }
}
```

### CURRENT_PHASE:
```json
{
    type: CURRENT_PHASE,
    data:
    {
	    phase: <-1|0|1|2>
    }
}
```

### DEFEND:
```json
{
    type: DEFEND,
    data:
    {
        defenderName: "...",
	    units: "..."
    }
}
```

### ERROR:
```json
{
    type: ERROR,
    data:
    {
	    errType : <MESSAGE_TYPE>,
	    message : "MESSAGE_TYPE : ..."
    }
}
```

### GAME_OVER:
```json
{
    type: GAME_OVER,
    data:
    {
	    winner: "..."
    }
}
```

### GAME_RESULTS:
```json
{
    type: GAME_RESULTS,
    data:
    {
	    ranking:{first,second,...}
    }
}
```

### GAME_STATUS:
```json
{
    type: GAME_STATUS,
    data:
    {
	    name: "...",
	    nbPlayers: "...",
	    players: {name1,name2,...},
	    mapName:"...",
	    board: {{terId,ownerId,nbUnits},{terId,ownerId,nbUnits},...},
	    freeTerritories: "...",
	    phase: <-1|0|1|2>,
	    activePlayer: <0|1|2|3|4|5>,
	    totalExchangedSets: "..."
    }
}
```

### GIVE_TOKENS:
```json
{
    type: GIVE_TOKENS,
    data:
    {
	    player: "...",
	    token1: "...",
	    token2: "...",
	    token3: "..."
    }
}
```
### JOIN_LOBBY:
```json
{
    type: JOIN_LOBBY,
    data:
    {
	    player: "..."
    }
}
```

### KICKED:
```json
{
    type: KICKED,
    data:
    {
	    kickerName: "..."
    }
}
```

### LEAVE_GAME:
```json
{
    type: LEAVE_GAME,
    data:
    {
	    player: "..."
    }
}
```

### LOBBY_STATE:
```json
{
    type: LOBBY_STATE,
    data:
    {
        lobbyName: "...",
        lobbyPassword: "...",
        maxPlayers: "...",
        mapName: "...",
        hidden: <true|false>
    }
}
```

### MOVE:
```json
{
    type: MOVE,
    data:
    {
	    source: "...",
	    destination: "...",
	    units: "..."
    }
}
```

### PLAYER_ELIMINATION:
```json
{
    type: PLAYER_ELIMINATION,
    data:
    {
	    player: "..."
    }
}
```

### PLAYER_PROFILE:
```json
{
    type: PLAYER_PROFILE,
    data:
    {
        player:"...",
        victories:"...",
        defeats: "...",
        gainedTerritories: "...",
        lostTerritories: "...",
        reinforcementEarned: "...",
        setsOfTokensUsed: "..."
    }
}
```

### PUT:
```json
{
    type: PUT,
    data:
    {
        player: "...",
	    territory: "...",
	    units: "..."
    }
}
```

### REINFORCEMENT:
```json
{
    type: REINFORCEMENT,
    data:
    {
	    player: "...",
	    units: "..."
    }
}
```

### START_GAME:
```json
{
    type: START_GAME,
}
```

### USE_TOKENS:
```json
{
    type: USE_TOKENS,
    data:
    {
        player: "...",
	    token1: "...",
	    token2: "...",
	    token3: "..."
    }
}
```