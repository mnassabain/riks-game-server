# Messages format

* [Client to server](#client-to-server)
* [Server to client](#server-to-client)


# Client to server

* [SIGN-UP]()
* [CONNECT]()
* [DISCONNECT]()
* [CREATE_LOBBY]()
* [LOBBY_LIST]()
* [JOIN_LOBBY]()
* [START_GAME]()
* [END_PHASE]()
* [PUT]()
* [USE_TOKENS]()
* [ATTACK]()
* [ATTACKED]()
* [DEFEND]()
* [MOVE](#move)

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

### LOBBY_LIST:
```json
{
    type: LOBBY_LIST
}
```

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

Player to server message format (Game side):
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

### ATTACKED:

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

## Server to client

### Server response:
```json
{
    type: MESSAGE_TYPE,
    data: {
        error: bool
        response: "..."
    }
}
```