## Application Name : Guess the Beat
#### Execute in Debug X64 mode
---
#### Gameplay:
-  Game consists of LEVEL 1,2 and 3
-  Player wins the game by completing all 3 Levels
-  Three attempts in each LEVEL. If incorrect, it will be a Gameover
-  Each level will have a beat When Q is pressed. Player need to find the exact beat with the given options by pressing R. Options are selected using 1,2,3 or 4.

---
#### User Manual:
- To change audio files without need for recompilation, replace the files in SOLUTION_DIR\common\assets\audio\

---


#### Note:
- 3 streaming sounds. Search for "_bgm" will show three places of continuous sounds used. 
   + When game starts initial bgm starts playing.
   + When game is entered, new bgm starts playing.
   + When game is over(lost). losing bgm starts playing. 
- 8 sampled sounds. Search for "_sampledsound" will show eight places where sampled sounds are used.
   + 4 different sound option are played in LEVEL 1
   + 2 sounds for game won and game lose
   + 3 sounds for LEVEL 2 where it says options as left, right and center
   + When options are pressed, a click sound is played
- ability to pause and play. Search for "_pauseplay" will show place where it is defined
   + Pressing Tab button will pause/play the background music
- 5 sounds where attributes are changed. Search for "_attrib" will show places where they are defined
   + Volume attribute is changed for three background music
   + Pan attribute is changed for Level 2 question
   + Reverb, Lowpass, Pitch are used in Level 3 answer options
- Loaded information from an external file. Search for "_file" will show places where they are defined
   + Retrieved Game won/lose mp3 from audioGameWonOrLose.txt
   + Retrieved automated mp3 from audioAutomatedSound.txt
   + Retrieved LEVEL 1 Option mp3 from audioQuestionLevel1.txt
   + Retrieved LEVEL 2 and 3 Option mp3 from audioQuestionLevel2and3.txt
   + Retrieved Game background music mp3 from audioBGM.txt 

---
#### Controls:
-  ENTER - To start the game
-  Q - Replay the given beat
-  R - Replay the given beat options
-  TAB - Pause/Play background music
-  ESC - To Quit the game
-  1, 2, 3, 4 - Options to guess the beat