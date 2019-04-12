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
* [GET_GAME_STATUS](#get_game_status)
* [JOIN_LOBBY](#join_lobby)
* [KICK_FROM_LOBBY](#kick_from_lobby)
* [LEAVE_GAME](#leave_game)
* [LOBBY_LIST](#lobby_list)
* [MOVE](#move)
* [PLAYER_PROFILE](#player_profile)
* [PUT](#put)
* [SIGN-UP](#sign-up)
* [START_GAME](#start_game)
* [USE_TOKENS](#use_tokens)

### ATTACK:
```json
{
    type: ATTACK,
    data:
    {
        player: "...",
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
    data: {
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
        userID: "...",
        lobbyName: "...",
        lobbyPassword: "...",
        maxPlayers: "...",
        mapName: "..."
    }
}
```

### DEFEND:
```json
{
    type: DEFEND,
    data:
    {
        player: "...",
	units: "..."
    }
}
```

### DISCONNECT:
```json
{
    type: DISCONNECT,
    data:
    {
        userID: "..."
    }
}
```

### EDIT_LOBBY:

### END_PHASE:
```json
{
    type: END_PHASE,
    data:
    {
        player: "..."
    }
}
```

### GET_GAME_STATUS:

### JOIN_LOBBY:
```json
{
    type: JOIN_LOBBY,
    data:
    {
        playerID: "...",
        lobbyID: "...",
        lobbyPassword: "..."
    }
}
```

### KICK_FROM_LOBBY:

### LEAVE_GAME:

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
        player: "...",
	source: "...",
	destination: "...",
	units: "..."
    }
}
```

### PLAYER_PROFILE:

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

### SIGN UP:
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
    data:
    {
        lobbyID: "..."
    }
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