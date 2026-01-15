# RType project protocol documentation

# PACKET FORMAT
```
ENDIANESS : BIG
PREAMBLE : 4 BYTES : 0000
PACKETSIZE : 4 BYTES : INT  -> Size of incoming bytes to read (including code with its data)
DATETIME : NO
EOP : NO
```

---

# SENDING CODES
Every code (1 ... 255) is on ONE BYTE only

## Client codes to server

### 01 ... 19 → connexions codes
```
2   DISCONNEXION            [NO DATA]   ->  Sent from client unlink/erase connexion (Note: server uses same code 2)
4   PACKET LOSS             [1B code]   ->  Ask to send back a specific code
5   I MISSED SOMETHING      [NO DATA]   ->  Asks Server to send all datas needed
6   PING                    [NO DATA]   ->  Will be responded by 7
7   PONG                    [NO DATA]   ->  Calculate delay, it means server sent PING before
```

### 20 ... 29 → accounts codes
```
20  LOGIN   [32B username]      ->  Contains a username to set for user, will be responded by 22 or 25 (only alphanumerical values)
21  LOGOUT  [NO DATA]           ->  Reset username to empty
```

### 30 ... 49 → lobby codes
```
30  JOIN LOBBY                  [6B lobby_code]         ->  Lobby code is a 6 numbers random value created by server
32  CREATE LOBBY                [NO DATA]               ->  Will be responded with 33, and server needs to register admin
34  GET ALL PUBLIC LOBBIES      [NO DATA]               ->  Asks for public lobbies
36  ADMIN START GAME            [NO DATA]               ->  Sent by client, server needs to check if client is admin
40  LEAVE LOBBY                 [NO DATA]               ->  Disconnects client from lobby
41  WANT THIS TEAM              [1B team]               ->  Team that the client want to join
46  TOGGLE LOBBY PRIVATE/PUBLIC [NO DATA]               ->  Set lobby visibility (moved to avoid conflict with code 38)
```

### 50 ... 69 → in game codes
```
50  CLIENT INPUTS       [4B action + 4B x + 4B y]    ->  Action and mouse position
87  ADMIN PAUSE GAME    [NO DATA]                     ->  Player asks to pause the game / Player asks to play the game, responded by 37 if you are not admin, 61 if ok
```


## Server codes to client

### 01 ... 19 → connexions codes
```
1   CONNECTED                       [X times (1B code + 4B timestamp)]      ->  All codes needed in an interval
2   DISCONNEXION                    [NO DATA]                               ->  Server force disconnected client (same code as client)
3   ERROR TOO MANY CLIENTS          [NO DATA]                               ->  Wait and try later
6   PING                            [NO DATA]                               ->  Will be responded by 7
7   PONG                            [NO DATA]                               ->  Calculate delay, it means server sent PING before
```

### 20 ... 29 → accounts codes
```
22  LOGGED IN               [4B id]             ->  Respond to 20 if ok
23  LOGGED OUT              [4B id]             ->  Send it in respond to 21 or when the client leaves the lobby
25  USERNAME ALREADY TAKEN  [NO DATA]           ->  Respond to 20 if username already taken
```

### 30 ... 49 → lobby codes
```
31  LOBBY JOINED                [4B id]                                                   ->  Giving id of the client that joined
33  LOBBY CREATED               [6B lobby_code]                                           ->  Response to 32, send the created lobby code
35  LOBBIES LIST                [X times (6B lobby codes)]                                ->  Response to 34, send all public lobbies
37  GAME STARTING               [NO DATA]                                                 ->  Broadcast to all lobby clients
38  PLAYERS LIST                [X times (4B id + 1B is_admin + 1B team + 32B username)]  ->  Send all players names
39  LOBBY VISIBILITY CHANGED    [1B bool public]                                          ->  Send to clients in the lobby
42  LOBBY DESTROYED             [NO DATA]                                                 ->  Send to all clients in the destroyed lobby, sending them all back to main menu
43  LOBBY FULL                  [NO DATA]                                                 ->  Respond to 30 if lobby is full
44  BAD LOBBY CODE              [NO DATA]                                                 ->  Respond to 30 if invalid code given
45  NOT ADMIN                   [NO DATA]                                                 ->  Send if client that sent 35 or 68 is not admin of the lobby
47  TEAM FULL                   [NO DATA]                                                 ->  Respond to 41 if the team is full
```

### 50 ... 89 → in game codes
**these fields have fixed size, thus do not need parsing of any kind, and values are all packet together without separators**

```
51  PLAYERS INIT            [X times (4B id + 4B entity + 1B team + 4B x + 4B y + 1B champ)]
52  BUILDINGS INIT          [X times (4B id + 1B team + 4B x + 4B y + 8B hp + 4B type + 4B range)]

61  PLAYERS UPDATES         [X times (4B id + 4B x + 4B y + 4B direction_x + 4B direction_y + 4B hp + 4B mana + 1B level)]
62  BUILDINGS UPDATES       [X times (4B id + 8B hp)]
63  CREATURES UPDATES       [X times (4B id + 1B team + 4B x + 4B y + 1B direction + 8B hp + 4B type + 16 times (1B effect) )]
64  PROJECTILES UPDATES     [X times (4B id + 4B x + 4B y + 4B type)]
65  COLLECTIBLES UPDATES    [X times (4B id + 4B x + 4B y + 4B type)]
66  INVENTORIES UPDATES     [X times (4B playerid + 6 times (4B itemid) + 4B gold)]
67  STATS UPDATES           [X times (4B playerid + 4B ad + 4B ap + 4B armor + 4B resist + 4B cdr mov speed + 4B atk speed + 4B range)]

70  SCORE                   [2 times (4B team kills + 4B tower taken + 4B major mobs killed)]           ->  One for each team
71  GAME DURATION           [4B duration]                                                               ->  Send game duration since started in seconds
72  SCOREBOARD              [X times (4B id + 4B player kills + 4B player deaths + 4B player assists)]

80  PLAYER DEAD             [NO DATA]                                                                   ->  Client's player is dead

88  ADMIN GAME PAUSED       [NO DATA]                                                               ->  A player has set the game on pause / play
89  GAME END                [1B winning team]                                                       ->  Send on game end (0 or 1 based on winner), send back all clients to main menu
```
