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
* [ATTACKED](#attacked)

### Server response:
(Standard server response)
```json
{
    type: MESSAGE_TYPE,
    data: {
        error: <true|false>
        response: "..."
    }
}
```

### ATTACKED: