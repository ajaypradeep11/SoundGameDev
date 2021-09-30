#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <sstream>

//To use Sleep function included <Windows.h>
#include <Windows.h>

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <FMOD/fmod.hpp>

#include <rssgl/GLText.h>
#include <fstream>
#include <vector>

GLuint _width = 1920;
GLuint _height = 1080;

GLFWwindow* _window = NULL;
std::string __applicationName = "Guess the beat";

FMOD::System* _system = NULL;
FMOD_RESULT _result = FMOD_OK;

//Sound variable for Background music
FMOD::Sound* _soundStartBgm = NULL;
FMOD::Sound* _soundMidBgm = NULL;
FMOD::Sound* _soundEndBgm = NULL;

//Sound variable for Game won or lose
FMOD::Sound* _soundGameWon = NULL;
FMOD::Sound* _soundGameLost = NULL;

//Sound variable for automated question sound
FMOD::Sound* _soundQuestion1 = NULL;
FMOD::Sound* _soundQuestion2 = NULL;
FMOD::Sound* _soundQuestion3 = NULL;

//Sound variable for click sound when clicking options
FMOD::Sound* _soundClick = NULL;

//LEVEL 1
//Sound variable for LEVEL 1 questions options
FMOD::Sound* _soundQuestionSound = NULL;
FMOD::Sound* _soundQuestion1Sound = NULL;
FMOD::Sound* _soundQuestion2Sound = NULL;
FMOD::Sound* _soundQuestion3Sound = NULL; 

//LEVEL 2
//Sound variable for LEVEL 2 question sound
FMOD::Sound* _soundLevel2Sound = NULL;

//Sound variable for automated sound LEVEL 2 answer
FMOD::Sound* _soundLevel2Left = NULL;
FMOD::Sound* _soundLevel2Center = NULL;
FMOD::Sound* _soundLevel2Right = NULL;

//LEVEL 3
//Sound variable for automated sound LEVEL 3 question and answer
FMOD::Sound* _soundLevel3Sound = NULL;

//Channel for sounds
FMOD::Channel* _channel = NULL;
FMOD::Channel* _channelStartBgm = NULL;
FMOD::Channel* _channelQuestion = NULL;
FMOD::Channel* _channelQuestion2 = NULL;
FMOD::Channel* _channelQuestion3 = NULL;


RSS::GLText* _text;
//Rendering same row again and again
GLuint _textRowIndex = 2;

//Boolean values to check condition
bool _isPaused = false;
bool _isPlaying = false;
bool _isGameEntered = false;
bool isStartgame = false;
bool isEndgame = false;
bool isLevel1 = false;
bool isLevel2 = false;
bool isLevel3 = false;

//To check whether the max attempts is 3. if greater than 3, then gameover.
static int count = 0;

std::vector<std::string> _audioList;
std::vector<std::string> _audioBGM;
std::vector<std::string> _audioQuestionLevel1;
std::vector<std::string> _audioQuestionLevel2and3;
std::vector<std::string> _audioAutomatedSound;
std::vector<std::string> _audioGameWonOrLose;


//Used to change background color
float red = 0.2f;
float blue = 0.2f;
float green = 0.2f;

//Function used to replay the options for LEVEL 1
void callOption() {
	if (!_isPlaying) {
		_result = _system->playSound(_soundQuestionSound, 0, false, &_channelQuestion);
		Sleep(1000);
		_result = _system->playSound(_soundQuestion1Sound, 0, false, &_channelQuestion);
		Sleep(1000);
		_result = _system->playSound(_soundQuestion2Sound, 0, false, &_channelQuestion);
		Sleep(1000);
		_result = _system->playSound(_soundQuestion3Sound, 0, false, &_channelQuestion);
		if (_result != FMOD_OK) {
			fprintf(stderr, "unable to play sound");
		}
	}
}

void callOptionLvl2() {
	if (!_isPlaying) {
		_result = _system->playSound(_soundLevel2Left, 0, false, &_channelQuestion);
		Sleep(1000);
		_result = _system->playSound(_soundLevel2Center, 0, false, &_channelQuestion);
		Sleep(1000);
		_result = _system->playSound(_soundLevel2Right, 0, false, &_channelQuestion);
		if (_result != FMOD_OK) {
			fprintf(stderr, "unable to play sound");
		}
	}
}

void callOptionLvl3() {
	if (!_isPlaying) {

		_result = _system->playSound(_soundLevel3Sound, 0, false, &_channelQuestion3);
		_channelQuestion3->setReverbProperties(4, 1.0f);
		
		Sleep(1000);

		_result = _system->playSound(_soundLevel3Sound, 0, false, &_channelQuestion3);
		_channelQuestion3->setLowPassGain(1.0f);
		Sleep(1000);

		_result = _system->playSound(_soundLevel3Sound, 0, false, &_channelQuestion3);
		_channelQuestion3->setPitch(2.0);

		if (_result != FMOD_OK) {
			fprintf(stderr, "unable to play sound");
		}
	}
}

void incrementAndCheck() {

	if (isStartgame)
	{
		_channelQuestion->isPlaying(&_isPlaying);
	} else if (isLevel1)
	{
		_channelQuestion2->isPlaying(&_isPlaying);
	} else if (isLevel2)
	{
		_channelQuestion3->isPlaying(&_isPlaying);
	}

	if (!_isPlaying) {
		_result = _system->playSound(_soundClick, 0, false, &_channelQuestion);
		if (_result != FMOD_OK) {
			fprintf(stderr, "unable to play sound");
		}
		count++;
		if (count >= 3) {
			isEndgame = true;
			isStartgame = false;
			isLevel1 = false;
			isLevel2 = false;
			red = 0.74f;
			blue = 0.0f;
			green = 0.070f;
			count = 0;
			_channelStartBgm->stop();

			_result = _system->playSound(_soundEndBgm, 0, false, &_channelStartBgm);
			_channelStartBgm->setVolume(0.09);
			_result = _system->playSound(_soundGameLost, 0, false, &_channelQuestion);
			if (_result != FMOD_OK) {
				fprintf(stderr, "unable to play sound");
			}
		}
	}
	
}

void changeBackground(float r, float g, float b) {
	red = r;
	green = g;
	blue = b;
}

//Keypress callback
static void glfw_key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
	else if (key == GLFW_KEY_ENTER && action == GLFW_PRESS) {

		//When game is entered, this snippet is called
		if (!_isGameEntered) {
			changeBackground(0.5f, 0.5f, 0.5f);
			_isGameEntered = true;
			isStartgame = true;
			_channelStartBgm->stop();

			//_soundMidBgm is called as that is the bgm for level1
			_result = _system->playSound(_soundMidBgm, 0, false, &_channelStartBgm);
			_channelStartBgm->setVolume(0.09);
			if (_result != FMOD_OK) {
				fprintf(stderr, "unable to play sound");
			}

			//_soundQuestion1 is called as that is the automated voice for level1 question
			_result = _system->playSound(_soundQuestion1, 0, false, &_channelQuestion);
			if (_result != FMOD_OK) {
				fprintf(stderr, "unable to play sound");
			}
		}
	}

	// LEVEL 1,2 and 3
	//Q press to hear question
	else if (key == GLFW_KEY_Q && action == GLFW_PRESS) {
		//we check isStartgame,isLevel1 and isLevel2 to check whether user is in LEVEL 1, 2 or 3 respectively
		if (isStartgame) {
			_channelQuestion->isPlaying(&_isPlaying);
			if (!_isPlaying) {

				_result = _system->playSound(_soundQuestion1Sound, 0, false, &_channelQuestion);
				if (_result != FMOD_OK) {
					fprintf(stderr, "unable to play sound");
				}
			}
		}
		else if (isLevel1) {
			_channelQuestion2->isPlaying(&_isPlaying);
			if (!_isPlaying) {
				_result = _system->playSound(_soundLevel2Sound, 0, false, &_channelQuestion2);
				_channelQuestion2->setPan(1);
				if (_result != FMOD_OK) {
					fprintf(stderr, "unable to play sound");
				}
			}
		}
		else if(isLevel2) {
			_channelQuestion3->isPlaying(&_isPlaying);
			if (!_isPlaying) {

				_result = _system->playSound(_soundLevel3Sound, 0, false, &_channelQuestion3);
				_channelQuestion3->setPitch(2.0);
				if (_result != FMOD_OK) {
					fprintf(stderr, "unable to play sound");
				}
			}
		}
	}

	//R press to hear options DEPENDS ON LEVEL
	else if (key == GLFW_KEY_R && action == GLFW_PRESS) {
		if (isStartgame) {
			_channelQuestion->isPlaying(&_isPlaying);
			callOption();
		}
		else if (isLevel1) {
			_channelQuestion2->isPlaying(&_isPlaying);
			callOptionLvl2();
		}
		else if (isLevel2) {
			_channelQuestion3->isPlaying(&_isPlaying);
			callOptionLvl3();
		}


	}

	//pause background music we have three bgm. Initial bgm, game start bgm and game lost bgm.
	else if (key == GLFW_KEY_TAB && action == GLFW_PRESS) {
		
		if (_channelStartBgm) {
			_result = _channelStartBgm->getPaused(&_isPaused);
			_result = _channelStartBgm->setPaused(!_isPaused);
			_result = _channelStartBgm->getPaused(&_isPaused);
		}
	}

	//Level 1,2 and 3 answer options
	else if (key == GLFW_KEY_1 && action == GLFW_PRESS) {
		if (isStartgame) {
			incrementAndCheck();
		}		
		else if (isLevel1) {
			incrementAndCheck();
		}
		else if (isLevel2) {
			incrementAndCheck();
		}
	}
	else if (key == GLFW_KEY_2 && action == GLFW_PRESS) {
		if (isStartgame) {
			_channelQuestion->isPlaying(&_isPlaying);
			if (!_isPlaying) {
				isLevel1 = true;
				isStartgame = false;
				changeBackground(0.8f, 0.8f, 0.8f);
				_result = _system->playSound(_soundQuestion2, 0, false, &_channelQuestion2);
				if (_result != FMOD_OK) {
					fprintf(stderr, "unable to play sound");
				}
			}
			count = 0;
		}
		else if (isLevel1) {
			incrementAndCheck();
		}
		else if (isLevel2) {
			incrementAndCheck();
		}
	}
	else if (key == GLFW_KEY_3 && action == GLFW_PRESS) {
		if (isStartgame) {
			incrementAndCheck();
		}		
		else if (isLevel1) {
			_channelQuestion2->isPlaying(&_isPlaying);
			if (!_isPlaying) {
				isLevel2 = true;
				isLevel1 = false;
				changeBackground(0.3f, 0.3f, 0.3f);
				count = 0;
				_result = _system->playSound(_soundQuestion3, 0, false, &_channelQuestion3);
				if (_result != FMOD_OK) {
					fprintf(stderr, "unable to play sound");
				}
			}
			count = 0;
		}
		else if (isLevel2) {
			_channelQuestion3->isPlaying(&_isPlaying);
			if (!_isPlaying) {
				isLevel3 = true;
				isLevel2 = false;
				changeBackground(0.0f, 0.0f,0.6f);
				count = 0;
				_result = _system->playSound(_soundGameWon, 0, false, &_channelQuestion3);
				if (_result != FMOD_OK) {
					fprintf(stderr, "unable to play sound");
				}
			}
			count = 0;
		}
	}
	else if (key == GLFW_KEY_4 && action == GLFW_PRESS) {
		if (isStartgame) {
			incrementAndCheck();
		}
	}
}


void glfw_error_callback(int error, const char* description) {
	fprintf(stderr, "FMOD Error(%d): %s", error, description);
}

bool init();
bool initGLFW();
bool initGL();
bool initFMOD();
void shutdown();


std::vector<std::string> getSoundFromFile(std::string fileName);


//Main Function
int main(int args, char* argv) {
	//fprintf(stdout,"Hello world");
	if (!init()) {
		fprintf(stderr, "Unable to initialize app");
		exit(EXIT_FAILURE);
	}

	GLuint lastKnownRowIndex = _textRowIndex;
	GLuint currentSoundLength = 0;

	FMOD::Sound* currentSound = nullptr;

	//Main loop
	while (!glfwWindowShouldClose(_window)) {
		//passing color values to change background
		glClearColor(red, green, blue, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		lastKnownRowIndex = _textRowIndex;

		//Initial starting game
		_text->addLine("Enter to start the game", lastKnownRowIndex++);

		//LEVEL 1
		if (isStartgame) {
			_text->addLine("LEVEL 1", lastKnownRowIndex++);
			_text->addLine("Identify the sound played. You have 4 option to answer. Press Q to hear the given beat. Press R to hear 4 options of sound. Click 1,2,3 and 4 to select the options.", lastKnownRowIndex++);
		}
		//LEVEL 2
		else if (isLevel1) {
			_text->addLine("LEVEL 2", lastKnownRowIndex);
			_text->addLine("Identify which side the sound played. You have 3 option to answer. Press Q to hear the given beat. Press R to hear 3 options of sound, Click 1,2 or 3 to select the options.", ++lastKnownRowIndex);

		}
		//LEVEL 3
		else if (isLevel2) {
			_text->addLine("LEVEL 3", lastKnownRowIndex);
			_text->addLine("Press Q to play the sound. Press R to hear 3 variations of the sound. Which did it match? Click 1,2 or 3  ", ++lastKnownRowIndex);

		}
		//Game won
		else if (isLevel3) {
			_text->addLine("CONGRATULATIONS!", lastKnownRowIndex);
			_text->addLine("Looks like we have a winner with a keen ear! Press Esc to Exit. ", ++lastKnownRowIndex);

		}
		if (isEndgame) {
			_text->addLine("Game Over", --lastKnownRowIndex);
			_text->addLine("You Lose", lastKnownRowIndex);
			_text->addLine("Press Esc to quit. Relaunch to try again.", ++lastKnownRowIndex);
			_text->addLine(" ", ++lastKnownRowIndex);
		}
		_text->render();
		glfwSwapBuffers(_window);
		glfwPollEvents();
	}
	shutdown();

	return 0;
}

bool init() {

	if (!initGLFW()) {
		return false;
	}
	if (!initGL()) {
		return false;
	}
	//dependency GLtext and Opengl
	_text = new RSS::GLText(__applicationName, _width, _height);

	if (!initFMOD()) {
		return false;
	}
	return true;
}

bool initGLFW() {
	if (!glfwInit()) {
		fprintf(stderr, "Unable to initalize GLFW");
		return false;
	}

	//error callback
	glfwSetErrorCallback(glfw_error_callback);

	_window = glfwCreateWindow(_width, _height, __applicationName.c_str(), NULL, NULL);
	if (!_window) {
		fprintf(stderr, "Unable to create GLFW window");
		return false;
	}

	//set keyboard callback
	glfwSetKeyCallback(_window, glfw_key_callback);
	glfwMakeContextCurrent(_window);
	glfwSwapInterval(1);

	return true;
}

bool initGL() {
	if (!gladLoadGL(glfwGetProcAddress)) {
		fprintf(stderr, "unable to initialize glad");
		return false;
	}

	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	return true;
}

bool initFMOD() {
	_result = FMOD::System_Create(&_system);
	if (_result != FMOD_OK) {
		fprintf(stderr, "Unable to create FMOD system");
		return false;
	}
	_result = _system->init(32, FMOD_INIT_NORMAL, NULL);
	if (_result != FMOD_OK) {
		fprintf(stderr, "Unable to initialize FMOD system");
		return false;
	}

	//AudioBGM text file
	_audioBGM = getSoundFromFile("audioBGM.txt");
	std::stringstream bgm[4];
	for (int i = 0; i < _audioBGM.size(); i++) {
		bgm[i] << SOLUTION_DIR << "common\\assets\\audio\\" << _audioBGM.at(i);
	}
	_result = _system->createSound(bgm[0].str().c_str(), FMOD_LOOP_NORMAL, 0, &_soundStartBgm);
	_result = _system->createSound(bgm[1].str().c_str(), FMOD_LOOP_NORMAL, 0, &_soundMidBgm);
	_result = _system->createSound(bgm[2].str().c_str(), FMOD_LOOP_NORMAL, 0, &_soundEndBgm);


	/*LEVEL 1 starts*/
	//AudioQuestion LEVEL 1 text file
	_audioQuestionLevel1 = getSoundFromFile("audioQuestionLevel1.txt");
	std::stringstream questionsLevel1[4];
	for (int i = 0; i < _audioQuestionLevel1.size(); i++) {
		questionsLevel1[i] << SOLUTION_DIR << "common\\assets\\audio\\" << _audioQuestionLevel1.at(i);
	}
	_result = _system->createSound(questionsLevel1[0].str().c_str(), FMOD_LOOP_OFF, 0, &_soundQuestionSound);
	_result = _system->createSound(questionsLevel1[1].str().c_str(), FMOD_LOOP_OFF, 0, &_soundQuestion1Sound);
	_result = _system->createSound(questionsLevel1[2].str().c_str(), FMOD_LOOP_OFF, 0, &_soundQuestion2Sound);
	_result = _system->createSound(questionsLevel1[3].str().c_str(), FMOD_LOOP_OFF, 0, &_soundQuestion3Sound);
	/*LEVEL 1 ends*/


	/*LEVEL 2 and 3 starts*/
	//AudioQuestion LEVEL 2 and LEVEL 3 text file
	_audioQuestionLevel2and3 = getSoundFromFile("audioQuestionLevel2and3.txt");
	std::stringstream questionLevel2and3[2];
	for (int i = 0; i < _audioQuestionLevel2and3.size(); i++) {
		questionLevel2and3[i] << SOLUTION_DIR << "common\\assets\\audio\\" << _audioQuestionLevel2and3.at(i);
	}
	_result = _system->createSound(questionLevel2and3[0].str().c_str(), FMOD_LOOP_OFF, 0, &_soundLevel2Sound);
	_result = _system->createSound(questionLevel2and3[1].str().c_str(), FMOD_LOOP_OFF, 0, &_soundLevel3Sound);
	/*LEVEL 2 and 3 ends*/


	/*Automated sound starts*/
	//Audio for automated sound
	_audioAutomatedSound = getSoundFromFile("audioAutomatedSound.txt");
	std::stringstream audioAutomatedSound[7];
	for (int i = 0; i < _audioAutomatedSound.size(); i++) {
		audioAutomatedSound[i] << SOLUTION_DIR << "common\\assets\\audio\\" << _audioAutomatedSound.at(i);
	}
	_result = _system->createSound(audioAutomatedSound[0].str().c_str(), FMOD_LOOP_OFF, 0, &_soundLevel2Left);
	_result = _system->createSound(audioAutomatedSound[1].str().c_str(), FMOD_LOOP_OFF, 0, &_soundLevel2Center);
	_result = _system->createSound(audioAutomatedSound[2].str().c_str(), FMOD_LOOP_OFF, 0, &_soundLevel2Right);
	_result = _system->createSound(audioAutomatedSound[3].str().c_str(), FMOD_LOOP_OFF, 0, &_soundClick);
	_result = _system->createSound(audioAutomatedSound[4].str().c_str(), FMOD_LOOP_OFF, 0, &_soundQuestion1);
	_result = _system->createSound(audioAutomatedSound[5].str().c_str(), FMOD_LOOP_OFF, 0, &_soundQuestion2);
	_result = _system->createSound(audioAutomatedSound[6].str().c_str(), FMOD_LOOP_OFF, 0, &_soundQuestion3);
	/*Automated sound ends*/



	/*Game win or lose starts*/
	//Audio for game won/lose
	_audioGameWonOrLose = getSoundFromFile("audioGameWonOrLose.txt");
	std::stringstream audioGameWonOrLose[2];
	for (int i = 0; i < _audioGameWonOrLose.size(); i++) {
		audioGameWonOrLose[i] << SOLUTION_DIR << "common\\assets\\audio\\" << _audioGameWonOrLose.at(i);
	}
	_result = _system->createSound(audioGameWonOrLose[1].str().c_str(), FMOD_LOOP_OFF, 0, &_soundGameLost);
	_result = _system->createSound(audioGameWonOrLose[0].str().c_str(), FMOD_LOOP_OFF, 0, &_soundGameWon);
	/*Game win or lose ends*/
	

	//Initial Play start BGm
	_result = _system->playSound(_soundStartBgm, 0, false, &_channelStartBgm);
	_channelStartBgm->setVolume(0.09);
	if (_result != FMOD_OK) {
		fprintf(stderr, "Unable to play sound");
		return false;
	}

	return true;
}


void shutdown() {
	glfwTerminate();

	/*_result = _sound->release();
	if (_result != FMOD_OK) {
		fprintf(stderr, "Unable to release sound");
	}*/
	_result = _system->close();
	if (_result != FMOD_OK) {
		fprintf(stderr, "Unable to close system");
	}

	_result = _system->release();
	if (_result != FMOD_OK) {
		fprintf(stderr, "Unable to release system");
	}

	if (_text) {
		delete _text;
		_text = nullptr;
	}

	exit(EXIT_SUCCESS);
}

//Readfile and get the path
std::vector<std::string> getSoundFromFile(std::string fileName) {

	std::vector<std::string> result;
	std::stringstream ss;
	ss << SOLUTION_DIR << "common\\assets\\audio\\" << fileName;

	//read file 
	std::ifstream inputFile(ss.str());
	std::string line;
	while (std::getline(inputFile, line)) {
		result.push_back(line);
	}

	return result;
}






