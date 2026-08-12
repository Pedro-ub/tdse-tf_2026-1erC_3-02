#include "game.h"
#include "leds.h"
#include "button.h"
#include "buzzer.h"
#include "random.h"
#include "tick.h"
#include "queue.h"
#include "storage.h"
#include "bluetooth.h"


#define GAME_MAX_LIVES          3u

typedef struct{
	uint32_t initialTimeoutMs;
	uint32_t minTimeoutMs;
	uint32_t timeoutStepMs;
} difficultyPreset_t;

static const difficultyPreset_t difficultyTable[GAME_DIFFICULTY_COUNT] = {
	[GAME_DIFFICULTY_EASY]   = {2000, 700, 40},
	[GAME_DIFFICULTY_NORMAL] = {1500, 400, 50},
	[GAME_DIFFICULTY_HARD]   = {1000, 250, 60},
};

static const char* difficultyNames[GAME_DIFFICULTY_COUNT] = {
	"Facil", "Normal", "Dificil"
};

static gameDifficulty_t currentDifficulty = GAME_DIFFICULTY_NORMAL;

/* Asume BUTTON_V_QTY == LED_COUNT (1 boton por LED, mismo indice).
 * Ver button_c_list[] en button.c y ledTable[] en leds.c. */
#define GAME_TARGET_QTY LED_COUNT

static bool     running    = false;
static uint8_t  activeTarget = 0;
static uint32_t targetTick   = 0;
static uint32_t timeoutMs    = 1500u;
static uint16_t score        = 0;
static uint8_t  lives        = GAME_MAX_LIVES;

static void activateNewTarget(void)
{
	ledAllOff();
	activeTarget = randomRange(0, GAME_TARGET_QTY - 1);
	ledOn(activeTarget);
	targetTick = tickGet();
}

static void gameEnd(void)
{
	running = false;
	ledAllOff();
	storageSubmitScore(score);
	bluetoothSendScore(score);
	Event_t ev = { .type = EVENT_GAME_OVER, .data = score };
	queuePush(ev);
}

static void handleHit(void)
{
	uint32_t reactionTime = tickGet() - targetTick;
	randomReseed(reactionTime);   /* semilla variable, req. 4.2 */

	score++;
	buzzerPlay(BUZZER_SOUND_HIT);

	if(timeoutMs > difficultyTable[currentDifficulty].minTimeoutMs + difficultyTable[currentDifficulty].timeoutStepMs){
			timeoutMs -= difficultyTable[currentDifficulty].timeoutStepMs;
		} else {
			timeoutMs = difficultyTable[currentDifficulty].minTimeoutMs;
		}
	activateNewTarget();
}

static void handleMiss(void)
{
	buzzerPlay(BUZZER_SOUND_MISS);

	if(lives > 0){
		lives--;
	}

	if(lives == 0){
		gameEnd();
	} else {
		activateNewTarget();
	}
}

void gameInit(void)
{
	running = false;
	score   = 0;
	lives   = GAME_MAX_LIVES;
}

void gameStart(void)
{
	score     = 0;
	lives     = GAME_MAX_LIVES;
	timeoutMs = difficultyTable[currentDifficulty].initialTimeoutMs;
	running   = true;
	activateNewTarget();
}

void gameStop(void)
{
	running = false;
	ledAllOff();
}

void gameUpdate(void)
{
	if(!running) return;

	/* Timeout: el jugador no llegó a tocar el objetivo a tiempo */
	if((tickGet() - targetTick) >= timeoutMs){
		handleMiss();
		return;
	}

	/* Chequeo de botones: solo importa el del objetivo activo y los
	 * demás (touch incorrecto), no hace falta mirar toda la cola. */
	for(uint8_t i = 0; i < GAME_TARGET_QTY; i++){
		if(buttonGetEvent(i)){
			if(i == activeTarget){
				handleHit();
			} else {
				handleMiss();
			}
			break; /* un solo evento por vuelta de gameUpdate() */
		}
	}
}

uint16_t gameGetScore(void)
{
	return score;
}

uint8_t gameGetLives(void)
{
	return lives;
}

void gameSetDifficulty(uint8_t level)
{
	if(level >= GAME_DIFFICULTY_COUNT) return;
	currentDifficulty = (gameDifficulty_t)level;
}

const char* gameGetDifficultyName(void)
{
	return difficultyNames[currentDifficulty];
}
