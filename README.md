This is the readme. It is basically replacing Info.txt as the main source of information about
the game.

## What are widgets?

What is a widget? "Widget" refers to any one of the "characters" on the game map. For example:
```
# # # # ]
# - - - ]
# - @ - ]
# - - - ]
# # # # #
```
In the above picture, each '#', '@', and '-' is considered a widget.
(Note: although '-'s are technically widgets, they represent empty space. As such will
often not be treated as a widget, but rather the absence of a widget)
(Note 2: ']' is also a widget. It plays a special role in the creation of game maps)

## List of widget types

Here is a list of widget types, as well as a short explanation for each one:
* `@` Player: the player-controlled widget
* `#` Wall: an immovable barrier
* `K` Key: used to open doors
* `D` Door: an immovable barrier that can be opened with a key
* Chaser: an enemy that chases the player
    - `X` Chaser (idle)
    - `>`Chaser (facing right)
    - `<`Chaser (facing left)
    - `^`Chaser (facing up)
    - `V`Chaser (facing down)
* `!` Rook: instantly destroys a player in its line of sight
* `&` Bomb
* `%` Bait: attracts chasers and rooks
* `$` Coin: can be collected by a player
* Magnet: moves towards/away from other magnets
    - `A`
    - `B`
    - `C`

## Keys and doors

A door (`D`) is a non-pushable widget that acts as a barrier, similar to a wall. However,
it can be destroyed ("opened") using a key.
A key (`K`) is a pushable widget that can be pushed onto a space occupied by a door, resulting
in both the key and the door disappearing.

## Chasers

Chasers are enemies to the player. They have an orthogonal line of sight that goes in the
4 cardinal directions. The range of their sight is defined by the global variable CHASER_SIGHT
(currently 10).

If a target (player or bait) ends up in a chaser's line of sight, the chaser will turn to face the
target and begin moving in that direction. If the target leaves the chaser's line of sight,
the chaser will continue moving in that direction until: 
* it hits an obstruction, in which case it stops. Or,
* the target (or a new one) reenters its line of sight.

A chaser's line of sight can be blocked by obstructions.

If a target is in a space adjacent to a chaser, the chaser will move onto the space occupied by said
target, destroying it.

If a target steps onto a space occupied by a chaser (or if it gets pushed onto a space occupied
by a chaser), the *target* is destroyed.

Chasers are vulnerable to bombs (as is everything else, tbf).

Chasers can push pushable widgets. Chasers can be pushed.

## Rooks

Rooks are enemies to the player. Similar to chasers, they have an orthogonal line of sight
that goes in the 4 cardinal directions. The range of their sight is defined by the
global variable ROOK_SIGHT (currently 10).

If a target (player or bait) ends up in a rook's line of sight, the rook will instantly set its
own position to that of the target's, destroying the target and taking their spot (an action that's
referred to as a "bash"). Aside from this, rooks are incapable of movement.

A rook's line of sight can be blocked by obstructions.

If a target steps onto a space occupied by a rook (or if it gets pushed onto a space occupied
by a rook), the *rook* is destroyed.

Rooks can not push widgets, as they can not move (the "bash" action is closer to teleportation
than physical movement, and as such can not be used to push widgets).

Rooks can be pushed.

Yes, the rook is named after the rook from chess.
In fact, a good way to describe the movement of rooks in the Copy and Paste Game would be to
take a rook from chess and restrict it such that it can only move when it is capturing a piece.

## Bombs

Bombs are really quite simple. When a bomb is detonated, it destroys everything in its "blast zone".
The blast zone is a square that is centered around the bomb's position. The size of the square
is determined by the global variable BLAST_RADIUS (currently 3). Specifically, the side length of
this square is equal to
`(BLAST_RADIUS * 2) + 1`.

In other words, if BLAST_RADIUS = 2, the blast zone looks like:
```
- - - - - - -
- # # # # # -
- # # # # # -
- # # & # # -
- # # # # # -
- # # # # # -
- - - - - - -
```
Legend:
* `#` : in the blast zone
* `-` : *not* in the blast zone
* `&` : the detonated bomb. It's in the blast zone, of course.

Naturally, any other bombs caught in the blast zone of a detonated bomb will also be detonated.
Hooray!

Bombs can be detonated in 3 ways:
* By pushing a bomb onto another bomb
* By an enemy moving onto or getting pushed onto a bomb
* By a bomb getting pushed onto an enemy 

Since bombs are meant to be more of a tool than a hazard for players, bombs will not explode
on contact with players. However, a detonated bomb will *not* spare a player.

## Bait

Bait is really simple. If a chaser or a rook sees a bait widget, they will target and destroy it
as they would a player.

## Coins

Coins can be stepped on by a player, resulting in the destruction of the coin. Nothing happens,
but they player can feel good knowing they "collected" a coin.

Coins are pushable.

Chasers and rooks are indifferent to coins.

## Magnets

Magnets will move towards or away from other magnets. There are 3 magnet types: A, B, and C.
A and B magnets mimic real world North and South Poles; they are attracted towards each other,
but repelled by themselves.

The C magnet is attracted to A and B magnets, as well as itself. Both A and B magnets are repelled
by the C magnet. Sad.

Magnets are only influenced by magnets that are within their "line of sight".
Similar to chasers and rooks, magnets have an orthogonal line of sight that goes in the 4 cardinal
directions. The range of their sight is defined by the global variable MAGNET_RANGE (currently 5).
Yes, this means that 2 magnets that are right next to each other diagonally will have no
effect on each other. I feel that this makes it easier to control them.

I might rename the A, B, and C magnets to N, S, and M, respectively.

Magnets are pushable.

## Map creation and the `]` widget

Firstly, "map" refers to the 2D "playing field" that the game takes place on.

Maps consist of keyboard characters. As such, maps are designed by simply typing them out.
Here is an example of a map (I'll refer to it as "the example map"):
```
# # # # ]
# $ - - ]
# - @ - ]
# - - - ]
# # # # #
```

### Map creation

Once you have designed a map, you can feed it into the game and it will turn that grid of
keyboard characters (the "raw map") into a 2D vector (the "playable map") in which the game
takes place. Here is how that works:
* First, you design a map by typing it out. You can type it out directly within the program
or you can type it out in a seperate document/text editor and copy and paste the map into the game.
* Second, the game turns the inputted map into a single usable string (The "map string"):
    - The `getMapString` function is called and accepts user input. This is where you type out/
    paste the raw map.
    - There are multiple ways you can input the raw map. You can input it as a grid or a single
    line. You can input it with or without spaces. The issue is that the game is only designed to
    accept it as a single line without spaces. So, the `getMapString` function turns the input
    into something usable, via the following steps.
    - If the raw map is a grid with multiple rows, each row is concatenated with each other
    to form a single string.
    - Any spaces in the string are removed.
    - The result is returned.
* Third, the game takes the map string and passes it into the `makeMapFromString` function.
    - First, the function checks for empty input. If the input is empty, a default map is returned.
    - Second, due to the possibility of the inputted map having inconsistent row lengths, the
    function finds out the length of the longest row.
    - Third, the function creates a 2D vector and populates each spot with the appropriate
    character. Rows that are shorter than the longest row are extended. The spaces that are
    created by this extension are filled in with the `EMPTY_SPACE` character (a global char var).
    - The result is returned.
* Fourth, global variable `theMap` is set equal to the output of `makeMapFromString`.

### The `]` widget

The `]` widget is used to mark the end of a row.

## Order of operations

In order to ensure the game behaves consistently and deterministically, the game will follow
an order of operations:
* The player takes their turn.
* Everything else "takes its turn":
    - Scan the whole map (linearly, reading order) and look for rooks. If a rook is spotted,
    check if there are any nearby available rook targets. If so, have the rook bash it.
    Each rook should only bash once per turn (on that note, I need to come up with a way to
    prevent one rook from bashing multiple times).
    - Scan the map for chasers and change their orientation.
    - Move right-facing chasers.
    - Move left-facing chasers.
    - Move up-facing chasers.
    - Move down-facing chasers.
    - Scan the map for magnets and figure out which direction each magnet wants to move in.
    - Move rightward-moving magnets.
    - Move leftward-moving magnets.
    - Move upward-moving magnets.
    - Move downward-moving magnets.

## Agents

Agents are widgets capable of performing their own actions. So far there are 3 agents:
* Rooks
* Chasers
* Magnets

Players (`@`) are not considered agents because the player moves them, not the computer.
In other words, they don't need to be considered during the "everything else" section of
the order of operations.