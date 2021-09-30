#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <sstream>

#include <Windows.h>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <FMOD/fmod.hpp>

//for opening text
#include <rssgl/GLText.h>

#include <fstream>
#include <vector>

GLuint _width = 1280;
GLuint _height = 720;

GLFWwindow* _window = NULL;
std::string __applicationName = "check";

FMOD::System* _system = NULL;
FMOD_RESULT _result = FMOD_OK;
FMOD_RESULT _resultStartBgm = FMOD_OK;

FMOD::Sound* _sound = NULL;
FMOD::Sound* _soundStartBgm = NULL;
FMOD::Sound* _soundMidBgm = NULL;
FMOD::Sound* _soundEndBgm = NULL;
FMOD::Sound* _soundGameWon = NULL;
FMOD::Sound* _soundGameLost = NULL;
FMOD::Sound* _soundQuestion1 = NULL;
FMOD::Sound* _soundQuestion1Sound = NULL;
FMOD::Sound* _soundQuestionSound = NULL;
FMOD::Sound* _soundQuestion2Sound = NULL;
FMOD::Sound* _soundQuestion3Sound = NULL; 
FMOD::Sound* _soundClick = NULL;
FMOD::Sound* _soundBalance = NULL;
FMOD::Sound* _soundLevel2Left = NULL;
FMOD::Sound* _soundLevel2Center = NULL;
FMOD::Sound* _soundLevel2Right = NULL;

FMOD::Channel* _channel = NULL;
FMOD::Channel* _channelStartBgm = NULL;
FMOD::Channel* _channelQuestion = NULL;

RSS::GLText* _text;
//Rendering same row again and again
GLuint _textRowIndex = 2;

bool _isPaused = false;
bool _isPlaying = false;
bool _isGameEntered = false;

bool isStartgame = false;
bool isEndgame = false;
bool isLevel1 = false;
bool isLevel2 = false;
bool isLevel3 = false;

static int count = 0;

std::vector<std::string> _audioList;


GLuint _currentSoundIndex = 0;

GLuint _yourSoundVectorOrArraySize = 2;


float red = 0.2f;
float blue = 0.2f;
float green = 0.2f;


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

void incrementAndCheck() {
	_channelQuestion->isPlaying(&_isPlaying);
	if (!_isPlaying) {
		_result = _system->playSound(_soundClick, 0, false, &_channelQuestion);
		if (_result != FMOD_OK) {
			fprintf(stderr, "unable to play sound");
		}
		count++;
		if (count >= 3) {
			isEndgame = true;
			_result = _system->playSound(_soundGameLost, 0, false, &_channelQuestion);
			if (_result != FMOD_OK) {
				fprintf(stderr, "unable to play sound");
			}
		}
	}
	
}

//callback
static void glfw_key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
	else if (key == GLFW_KEY_ENTER && action == GLFW_PRESS) {
		if (!_isGameEntered) {
			red = 0.5f;
			blue = 0.5f;
			green = 0.5f;
			_isGameEntered = true;
			isStartgame = true;
			_channelStartBgm->stop();

			_result = _system->playSound(_soundMidBgm, 0, false, &_channelStartBgm);
			_channelStartBgm->setVolume(0.09);
			if (_result != FMOD_OK) {
				fprintf(stderr, "unable to play sound");
			}

			_result = _system->playSound(_soundQuestion1, 0, false, &_channelQuestion);
			if (_result != FMOD_OK) {
				fprintf(stderr, "unable to play sound");
			}
			//Sleep(12000);
			//callOption();
		}
		

	}
	else if (key == GLFW_KEY_Q && action == GLFW_PRESS) {
		_channelQuestion->isPlaying(&_isPlaying);
		if (!_isPlaying) {
			
			_result = _system->playSound(_soundQuestion1Sound, 0, false, &_channelQuestion);
			if (_result != FMOD_OK) {
				fprintf(stderr, "unable to play sound");
			}
		}
		
	}
	else if (key == GLFW_KEY_R && action == GLFW_PRESS) {
		_channelQuestion->isPlaying(&_isPlaying);
		if(isStartgame)
		callOption();
		if(isLevel1)
		callOptionLvl2();

	}
	else if (key == GLFW_KEY_P && action == GLFW_PRESS) {
		
		if (_channel) {
			_result = _channel->getPaused(&_isPaused);
			_result = _channel->setPaused(!_isPaused);
			_result = _channel->getPaused(&_isPaused);
		}
	}
	else if (key == GLFW_KEY_1 && action == GLFW_PRESS) {
		if(isStartgame)
		incrementAndCheck();
	}
	else if (key == GLFW_KEY_2 && action == GLFW_PRESS) {
		_channelQuestion->isPlaying(&_isPlaying);
		if (!_isPlaying) {
			isLevel1 = true;
			isStartgame = false;
			red = 0.8f;
			blue = 0.8f;
			green = 0.8f;
		}	
	}
	else if (key == GLFW_KEY_3 && action == GLFW_PRESS) {
		if (isStartgame)
		incrementAndCheck();
	}
	else if (key == GLFW_KEY_4 && action == GLFW_PRESS) {
		if (isStartgame)
		incrementAndCheck();
	}

	else if(key == GLFW_KEY_TAB && action == GLFW_PRESS) {
		_currentSoundIndex = (_currentSoundIndex > _yourSoundVectorOrArraySize) ? 0 : (_currentSoundIndex + 1);
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

int main(int args, char* argv) {
	//fprintf(stdout,"Hello world");
	if (!init()) {
		fprintf(stderr, "Unable to initialize app");
		exit(EXIT_FAILURE);
	}

	GLuint lastKnownRowIndex = _textRowIndex;
	GLuint currentSoundLength = 0;

	FMOD::Sound* currentSound = nullptr;


	//main loop
	while (!glfwWindowShouldClose(_window)) {
		glClearColor(red, blue, green, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		//code here
		lastKnownRowIndex = _textRowIndex;
		
		if (_channel) {
			_channel->getCurrentSound(&currentSound);
			if (currentSound) {
				_result = currentSound->getLength(&currentSoundLength, FMOD_TIMEUNIT_MS);
			}
		}

		
		_text->addLine("Enter to start the game", lastKnownRowIndex++);

		if (isStartgame) {
			_text->addLine("LEVEL 1", lastKnownRowIndex++);
			_text->addLine("Identify the sound played. You have 4 option to answer. Press Q to hear the given beat. Click 1,2,3 and 4 to select the options. 4 options can be replayed by pressing R", lastKnownRowIndex++);
		}
		else if (isLevel1) {
			_text->addLine("LEVEL 2", lastKnownRowIndex);
			_text->addLine("Identify which side the sound played. You have 3 option to answer. Press Q to hear the given beat. Click 1,2 or 3 to select the options. 3 options can be replayed by pressing R ", ++lastKnownRowIndex);

		}
		
		//std::stringstream ss;
		//ss << "Is channel paused: " << ((_isPaused) ? "Yes" : "No");
		//_text->addLine(ss.str(), lastKnownRowIndex++);
		//ss.str("");


		//ss << "Current sound length in MS: " << currentSoundLength;
		/*_text->addLine(ss.str(), lastKnownRowIndex++);
		ss.str("");

		_text->addLine("___________________________", lastKnownRowIndex++);
		_text->addLine("Press esc to exit", lastKnownRowIndex++);
		_text->render();*/

		//glClearColor(0.2f, 0.4f, 0.2f, 1.0f);
		

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

	_audioList = getSoundFromFile("audioList.txt");
	std::stringstream ss[14];
	for (int i = 0; i < _audioList.size();i++) {
		ss[i] << SOLUTION_DIR << "common\\assets\\audio\\" << _audioList.at(i);
	}
	//create a sound
	_result = _system->createSound(ss[0].str().c_str(), FMOD_LOOP_NORMAL, 0, &_soundStartBgm);
	_result = _system->createSound(ss[1].str().c_str(), FMOD_LOOP_NORMAL, 0, &_soundMidBgm);
	_result = _system->createSound(ss[2].str().c_str(), FMOD_LOOP_NORMAL, 0, &_soundEndBgm);
	_result = _system->createSound(ss[7].str().c_str(), FMOD_LOOP_OFF, 0, &_soundQuestion1);
	_result = _system->createSound(ss[4].str().c_str(), FMOD_LOOP_OFF, 0, &_soundQuestion1Sound);
	_result = _system->createSound(ss[3].str().c_str(), FMOD_LOOP_OFF, 0, &_soundQuestionSound);
	_result = _system->createSound(ss[5].str().c_str(), FMOD_LOOP_OFF, 0, &_soundQuestion2Sound);
	_result = _system->createSound(ss[6].str().c_str(), FMOD_LOOP_OFF, 0, &_soundQuestion3Sound);
	_result = _system->createSound(ss[8].str().c_str(), FMOD_LOOP_OFF, 0, &_soundGameLost);
	_result = _system->createSound(ss[9].str().c_str(), FMOD_LOOP_OFF, 0, &_soundClick);
	_result = _system->createSound(ss[10].str().c_str(), FMOD_LOOP_OFF, 0, &_soundBalance);
	_result = _system->createSound(ss[11].str().c_str(), FMOD_LOOP_OFF, 0, &_soundLevel2Left);
	_result = _system->createSound(ss[12].str().c_str(), FMOD_LOOP_OFF, 0, &_soundLevel2Center);
	_result = _system->createSound(ss[13].str().c_str(), FMOD_LOOP_OFF, 0, &_soundLevel2Right);
	
	//_result = _system->createSound(ss.str().c_str(), FMOD_LOOP_OFF, 0, &_sound);
	//_result = _system->createSound(ss.str().c_str(), FMOD_LOOP_OFF, 0, &_sound);
	if (_result != FMOD_OK) {
		fprintf(stderr, "Unable to create sound");
		return false;
	}

	//Play created sound 
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

	_result = _sound->release();
	if (_result != FMOD_OK) {
		fprintf(stderr, "Unable to release sound");
	}
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






