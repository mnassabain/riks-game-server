# Messages format

* [Client to server]()
* [Server to client]()


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
* [MOVE]()

## SIGN UP:
{
    type: SIGN_UP,
    data:
    {
        userID: "...",
        userPassword: "..."
    }
}

## CONNECT:
{
    type: CONNECT,
    data: {
        userID: "...",
        userPassword: "..."
    }
}

## DISCONNECT:
{
    type: DISCONNECT,
    data:
    {
        userID: "..."
    }
}

## CREATE_LOBBY:
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

## LOBBY_LIST:
{
    type: LOBBY_LIST
}

## JOIN_LOBBY:
{
    type: JOIN_LOBBY,
    data:
    {
        playerID: "...",
        lobbyID: "...",
        lobbyPassword: "..."
    }
}

Player to server message format (Game side):
## START_GAME:
{
    type: START_GAME,
    data:
    {
        lobbyID: "..."
    }
}

## END_PHASE:
{
    type: END_PHASE,
    data:
    {
        player: "..."
    }
}

## PUT:
{
    type: PUT,
    data:
    {
        player: "...",
	territory: "...",
	units: "..."
    }
}

## USE_TOKENS:
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

## ATTACK:
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

## ATTACKED:

## DEFEND:
{
    type: DEFEND,
    data:
    {
        player: "...",
	units: "..."
    }
}

## MOVE:
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

Server response:
{
    type: MESSAGE_TYPE,
    data: {
        error: bool
        response: "..."
    }
}