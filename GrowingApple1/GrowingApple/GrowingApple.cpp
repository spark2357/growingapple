#include <bangtal.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

SceneID mainScene, title;
ObjectID player, apple, background1, background2, seed, arrow[16], target, start;
TimerID playerRunTimer, playerJumpTimer, appleRotateTimer, backgroundTimer, appleScaleTimer, createSeedTimer, moveSeedTimer, moveArrowTimer, successImgTimer;

int runidx = 1, jumpidx = 0, vel = 0, gravity = -12, playerY = 130, appleidx = 1, appleX = 900, seedX = 950, level = 4, arrowSpeed = 20, tile = -1, success;
int backgroundx1 = -1600, backgroundx2 = -160;
int arrowX[50], arrowidx, keycode, arrowcode[50];
double levelA;

bool isjump = false;
double applescale = 0.5;
char RunFileName[8][21] = {
	"Img/player/Run/0.png",
	"Img/player/Run/1.png",
	"Img/player/Run/2.png",
	"Img/player/Run/3.png",
	"Img/player/Run/4.png",
	"Img/player/Run/5.png",
	"Img/player/Run/6.png",
	"Img/player/Run/7.png",
};
char JumpFileName[12][23] = {
	"Img/player/Jump/0.png",
	"Img/player/Jump/1.png",
	"Img/player/Jump/2.png",
	"Img/player/Jump/3.png",
	"Img/player/Jump/4.png",
	"Img/player/Jump/5.png",
	"Img/player/Jump/6.png",
	"Img/player/Jump/7.png",
	"Img/player/Jump/8.png",
	"Img/player/Jump/9.png",
	"Img/player/Jump/10.png",
	"Img/player/Jump/11.png"
}; 
char appleFileName[12][29] = {
	"Img/applerotate/apple0.png",
	"Img/applerotate/apple330.png",
	"Img/applerotate/apple300.png",
	"Img/applerotate/apple270.png",
	"Img/applerotate/apple240.png",
	"Img/applerotate/apple210.png",
	"Img/applerotate/apple180.png",
	"Img/applerotate/apple150.png",
	"Img/applerotate/apple120.png",
	"Img/applerotate/apple90.png",
	"Img/applerotate/apple60.png",
	"Img/applerotate/apple30.png"
};
char arrowFileName[4][14] = {
	"Img/up.png",
	"Img/down.png",
	"Img/right.png",
	"Img/left.png"
};
char tileFileName[4][16] = {
	"Img/target1.png",
	"Img/target2.png",
	"Img/target3.png",
	"Img/target4.png"
};

void init();
void timerCallback(TimerID timer);
void keyboard(KeyCode code, KeyState state);
void mouseCallback(ObjectID object, int x, int y, MouseAction action);
void createarrow();
void checkarrow();

int main() {
	init();
}

void init() {

	setGameOption(GameOption::GAME_OPTION_ROOM_TITLE, false);
	setGameOption(GameOption::GAME_OPTION_INVENTORY_BUTTON, false);
	setGameOption(GameOption::GAME_OPTION_MESSAGE_BOX_BUTTON, false);

	srand(time(NULL));

	setTimerCallback(timerCallback);
	setKeyboardCallback(keyboard);
	setMouseCallback(mouseCallback);

	mainScene = createScene("", "Img/tempscene.png");

	title = createScene("", "Img/title.png");
	
	start = createObject("Img/start.png");
	locateObject(start, title, 593, 200);
	showObject(start);

	background1 = createObject("Img/background2.png");
	locateObject(background1, mainScene, backgroundx1, 0);
	showObject(background1);

	background2 = createObject("Img/background2.png");
	locateObject(background2, mainScene, backgroundx2, 0);
	showObject(background2);

	player = createObject("Img/player/Run/0.png");
	scaleObject(player, 0.3f);
	locateObject(player, mainScene, 50, 130);
	showObject(player);

	apple = createObject("Img/applerotate/apple0.png");
	scaleObject(apple, 0.5f);
	locateObject(apple, mainScene, 900, 130);
	showObject(apple);

	seed = createObject("Img/seed.png");

	target = createObject("Img/target.png");
	locateObject(target, mainScene, 500, 17);
	showObject(target);

	playerRunTimer = createTimer(0.06f);
	playerJumpTimer = createTimer(0.06f);
	appleRotateTimer = createTimer(0.06f);
	backgroundTimer = createTimer(0.04f);
	appleScaleTimer = createTimer(1.5f);
	createSeedTimer = createTimer(4.0f);
	moveSeedTimer = createTimer(0.06f);
	moveArrowTimer = createTimer(0.06f);
	successImgTimer = createTimer(0.3f);

	startGame(title);
}

void timerCallback(TimerID timer) {
	if (timer == playerRunTimer) {
		if (++runidx > 7) runidx = 0;
		setObjectImage(player, RunFileName[runidx]);
		setTimer(playerRunTimer, 0.06f);
		startTimer(playerRunTimer);
	}
	if (timer == playerJumpTimer) {
		locateObject(player, mainScene, 50, playerY += vel);
		setObjectImage(player, JumpFileName[jumpidx]);
		setTimer(playerJumpTimer, 0.06f);
		startTimer(playerJumpTimer);
		vel += gravity;
		if (++jumpidx > 11) {
			stopTimer(playerJumpTimer);
			setTimer(playerJumpTimer, 0.06f);
			startTimer(playerRunTimer);
			jumpidx = 0;
			isjump = false;
		}
	}
	if (timer == appleRotateTimer) {
		if (++appleidx > 11) appleidx = 0;
		setObjectImage(apple, appleFileName[appleidx]);
		setTimer(appleRotateTimer, 0.06f);
		startTimer(appleRotateTimer);
	}
	if (timer == backgroundTimer) {
		backgroundx1 += 20;
		backgroundx2 += 20;
		if (backgroundx2 >= 1280) backgroundx2 = -1600;
		if (backgroundx1 >= 1280) backgroundx1 = -1600;
		locateObject(background1, mainScene, backgroundx1, 0);
		locateObject(background2, mainScene, backgroundx2, 0);
		setTimer(backgroundTimer, 0.04f);
		startTimer(backgroundTimer);
	}
	if (timer == appleScaleTimer) {
		applescale += 0.01;
		appleX -= 5;
		scaleObject(apple, applescale);
		locateObject(apple, mainScene, appleX, 130);
		setTimer(appleScaleTimer, 1.5f);
		startTimer(appleScaleTimer);
	}
	if (timer == createSeedTimer) {
		locateObject(seed, mainScene, seedX, 150);
		showObject(seed);
		seedX = appleX + 50;
		startTimer(moveSeedTimer);
	}
	if (timer == moveSeedTimer) {
		seedX -= 40;
		locateObject(seed, mainScene, seedX, 150);
		printf("%d\n", playerY);
		if (75 <= seedX && seedX <= 215 && playerY + 15 <= 200) {
			endGame();
		}
		if (seedX < -90) {
			hideObject(seed);
			seedX = appleX + 50;
			createarrow();
		}
		else {
			startTimer(moveSeedTimer);
		}
		setTimer(moveSeedTimer, 0.06f);
	}
	if (timer == moveArrowTimer) {
		for (int i = 0; i < level; i++) {
			arrowX[i] += arrowSpeed;
			if (arrowX[i] > 780) hideObject(arrow[i]);
			locateObject(arrow[i], mainScene, arrowX[i], 20);
		}
		if (arrowX[level-1] > 780) {
			arrowSpeed += 1;
			applescale += 0.03f * (level - success);
			appleX -= 10 * (level - success);
			success = 0;
			scaleObject(apple, applescale);
			locateObject(apple, mainScene, appleX, 130);
			if (arrowSpeed > 80) arrowSpeed = 80;
			if (arrowSpeed % 3 == 0) level++;
			if (level >= 50) level = 50;

			setTimer(createSeedTimer, 1.0f);
			startTimer(createSeedTimer);
			stopTimer(moveArrowTimer);
		}
		else {
			setTimer(moveArrowTimer, 0.06f);
			startTimer(moveArrowTimer);
		}
	}
	if (timer == successImgTimer) {
		setTimer(successImgTimer, 0.3f);
		setObjectImage(target, "Img/target.png");
	}
}

void keyboard(KeyCode code, KeyState state) {
	if (state == KeyState::KEY_PRESSED) {
		if (code == KeyCode::KEY_SPACE && isjump == false) {
			stopTimer(playerRunTimer);
			setTimer(playerRunTimer, 0.06f);
			startTimer(playerJumpTimer);
			vel = 66;
			isjump = true;
			keycode = -1;
		}
		if (arrowidx < level) {
			if (code == KeyCode::KEY_UP_ARROW)
				keycode = 0;
			if (code == KeyCode::KEY_DOWN_ARROW)
				keycode = 1;
			if (code == KeyCode::KEY_RIGHT_ARROW)
				keycode = 2;
			if (code == KeyCode::KEY_LEFT_ARROW)
				keycode = 3;
			if (keycode != -1) {
				checkarrow();
			}
		}
	}
}

void createarrow() {
	arrowidx = 0;
	for (int i = 0; i < 16; i++) {
		arrowX[i] = -100 * (i + 1);
	}
	for (int i = 0; i < level; i++) {
		int code = rand() % 4;
		arrow[i] = createObject(arrowFileName[code]);
		arrowcode[i] = code;
		locateObject(arrow[i], mainScene, arrowX[i], 17);
		showObject(arrow[i]);
	}
	startTimer(moveArrowTimer);
}

void checkarrow() {
	if (arrowcode[arrowidx] == keycode) {
		tile = -1;
		int tmp = arrowX[arrowidx] + 25;
		if (500 <= tmp && tmp <= 570)
			tile = 0;
		else if (570 < tmp && tmp <= 640)
			tile = 1;
		else if (640 < tmp && tmp <= 710)
			tile = 2;
		else if (710 < tmp && tmp <= 780)
			tile = 3;
		else {
			success++;
			applescale += 0.03f;
			appleX -= 10;
		}
		if (tile != -1) {
			success++;
			setObjectImage(target, tileFileName[tile]);
			startTimer(successImgTimer);
			applescale -= 0.01f * double(tile);
			appleX += tile;
		}
	}
	else {
		success++;
		applescale += 0.03f;
		appleX -= 10;
	}
	printf("%d\n", success);
	scaleObject(apple, applescale);
	hideObject(arrow[arrowidx++]);
}

void mouseCallback(ObjectID object, int x, int y, MouseAction action) {
	if (object == start) {
		startTimer(playerRunTimer);
		startTimer(appleRotateTimer);
		startTimer(backgroundTimer);
		startTimer(appleScaleTimer);
		startTimer(createSeedTimer);
		enterScene(mainScene);
	}
}