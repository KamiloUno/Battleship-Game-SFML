# Battleship Game

## Project Description
The project is a computer implementation of the classic board game Battleship. The goal of the project was to faithfully reproduce the gameplay rules known from the paper version, maintaining the rules and clarity of the user interface. 

* **Board Mechanics**: The game board has a size of 10x10 squares and is prepared for both the first and the second player.
* **Placement Rules**: Each player has a set of ships of various sizes, which must be placed on their board in such a way that they do not touch each other by sides or corners, which means it is necessary to leave a minimum of one empty square around each ship.
* **Available Units**:
  * One four-mast ship occupying four squares.
  * Two three-mast ships occupying three squares each.
  * Three two-mast ships, each two squares long.
  * And four one-mast ships occupying one square.
* **Orientation**: Ships can be placed vertically or horizontally, depending on the player's preference.
* **Validation**: The interface allows dragging ships onto the board grid and ensures validation of the placement (whether the ship does not go out of bounds and does not collide with others).
* **Gameplay**: The gameplay consists of players taking alternating shots. A shot is an attempt to guess the position of the opponent's ship; if a player hits a part of a ship, they can continue their turn and take another shot until they miss.
* **Visuals**: Hits and misses are visually marked, and the player receives appropriate information about the effectiveness of their move.
* **Sinking**: In the event that all squares of a given ship are hit, the ship is considered sunk, and this is also appropriately signaled.
* **Victory**: The game will end when one of the players sinks all the opponent's ships - then a victory will be announced.
* **Goal**: The project aims not only to faithfully reproduce the game mechanics but also to ensure gameplay fluidity, a clear interface, and positive user experience.

---

## Interface
* **Startup**: After launching the program, the user sees a start screen, and to proceed to the game, they must click any button.
  
  <img width="751" height="425" alt="image" src="https://github.com/user-attachments/assets/28ae07c5-9eac-4ca7-8d49-167fe3e860e4" />
 
* **Aesthetics**: The game window also has an appropriate background, referring to the theme of the game.
* **Setup Phase**: 
  * Player 1 has the opportunity to set their ships on a 10x10 board. 
  * To rotate a ship, the player clicks the right mouse button.
    
    <img width="732" height="426" alt="image" src="https://github.com/user-attachments/assets/412cc2c4-f4eb-4d86-a73a-01e1a380285b" />
    <img width="750" height="437" alt="image" src="https://github.com/user-attachments/assets/b19d476b-9086-4e0e-9a5a-5291c52848d3" />

  * After setting them and clicking the "Go to player 2" button, Player 2 will have the same opportunity.
  * The interface for setting ships for Player 2 looks similar. 
  * After Player 2 sets all ships, a "Start game" button will appear in the bottom right corner.
    
    <img width="753" height="437" alt="image" src="https://github.com/user-attachments/assets/5ea3ffd3-f2f4-49e5-9013-6df16894fbad" />
 
* **Transitions**: Each new screen is separated by a 2-second "flash" so that players can swap places and not peek at the opponent's ship placement.
* **Main Game Interface**:
  * **Left Side**: Players see their own board and how the opponent has shot.
  * **Right Side**: Players can shoot at the opponent's ships.
  * **Markings**: A miss is marked with a gray dot, while a hit is marked with a red X.
  * **Post-Sink Logic**: After sinking a ship, a "Ship sunk" message appears, and all squares around it are marked as misses (because ships cannot touch each other).
    
    <img width="752" height="445" alt="image" src="https://github.com/user-attachments/assets/0989a089-b10e-4a51-b553-19f3eeba037b" />

  * **Game Over**: When one of the players sinks all the opponent's ships, the game ends, and an end screen appears with information about the winner.
    
    <img width="730" height="427" alt="image" src="https://github.com/user-attachments/assets/630d5353-ce18-4675-9e6e-4f7aa66edc15" />


---

## Most important algorithms and mechanisms

1. **Setting ships on the board**
   * The player drags ships with the mouse and places them on the grid squares.
   * It is checked whether the ship fits on the board and does not overlap with others.
   * Rotating the ship is done with the right mouse button.
2. **Player's turn and shooting**
   * Players alternately choose a square on the opponent's board.
   * Shot markings: 0 - none, 1 - miss, 2 - hit.
   * After a hit, it is checked whether the given ship has been sunk and whether all opponent's ships are sunk (end of game).
3. **Marking hit and sunk ships**
   * After sinking a ship, the squares around it are automatically marked as misses (according to Battleship rules).
4. **Transitions between game states**
   * The `flashTransition` effect (screen brightening) visually separates game stages: MENU, SETTING, GAME, END.
5. **File validation (fonts, textures)**
   * Files are loaded only if their name matches the regex pattern (safe resource loading).
6. **Saving the winner to a file**
   * After the game ends, the winner of the last match is saved in a text file.
