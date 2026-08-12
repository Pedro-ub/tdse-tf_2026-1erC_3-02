#include "fsm.h"
#include "leds.h"
#include "buzzer.h"
#include "dip_switch.h"
#include "queue.h"
#include "config.h"
#include "button.h"
#include "random.h"
#include "game.h"
#include "tick.h"
#include "menu.h"
#include "storage.h"
#include "ldr.h"
#include "lcd.h"
#include "bluetooth.h"

/
#define FSM_ANY_STATE   ((fsmState_t)0xFFu)

static fsmState_t currentState = FSM_STATE_INIT;
static uint32_t   lastErrorCode = 0;

/* ───────────────────────── Acciones de transicion ───────────────────────── */
/* Cada accion recibe el evento que disparo la transicion, por si necesita
 * el campo 'data'. Pueden ser NULL en la tabla si no hace falta accion. */

static void action_startGame(Event_t *ev)
{
	(void)ev;

	buzzerPlay(BUZZER_SOUND_MENU);
}

static void action_gameOver(Event_t *ev)
{
	uint16_t finalScore = ev ? (uint16_t)ev->data : 0;
	(void)finalScore;

}
static void action_enterFault(Event_t *ev)
{
	lastErrorCode = ev ? ev->data : 0xFFFFFFFFu;
	gameStop();
	buzzerPlay(BUZZER_SOUND_FAULT);
}

/* ───────────────────────── Acciones de entrada a estado ─────────────────── */
/* Se ejecutan una sola vez, al entrar a ese estado (no en cada evento). */

static void onEnter_setup(void)
{
	menuEnter();

}

static void onEnter_normal(void)
{
	gameStart();
}

static void onEnter_fault(void)
{
	/* Señal visual de falla: todos los LEDs encendidos fijos.
	 * */
	for(uint8_t i = 0; i < LED_COUNT; i++){
		ledOn(i);
	}
}

typedef void (*fsmEnterAction_t)(void);

static const fsmEnterAction_t enterActions[FSM_STATE_COUNT] = {
	[FSM_STATE_INIT]   = NULL,
	[FSM_STATE_SETUP]  = onEnter_setup,
	[FSM_STATE_NORMAL] = onEnter_normal,
	[FSM_STATE_FAULT]  = onEnter_fault,
};

/* ───────────────────────── Tabla de transiciones ─────────────────────────

 * Formato: { estado_actual, evento, accion, proximo_estado }
 * FSM_ANY_STATE matchea sin importar el estado actual (transiciones globales,
 * ej: cualquier fallo de hardware lleva a FSM_STATE_FAULT). */
typedef struct{
	fsmState_t   state;
	EventType_t  event;
	void       (*action)(Event_t *ev);
	fsmState_t   nextState;
} fsmTransition_t;

static const fsmTransition_t transitionTable[] = {

	/* --- SET_UP --- */

	{ FSM_STATE_SETUP,  EVENT_MENU_START_GAME, action_startGame, FSM_STATE_NORMAL },
	{ FSM_STATE_SETUP,  EVENT_DIP_CHANGED,   NULL,             FSM_STATE_SETUP },  /* TODO: notificar a menu.c */

	/* --- NORMAL --- */
	{ FSM_STATE_NORMAL, EVENT_GAME_OVER,     action_gameOver,  FSM_STATE_SETUP },
	{ FSM_STATE_NORMAL, EVENT_GAME_TIMEOUT,  NULL,             FSM_STATE_NORMAL }, /* game.c decide internamente, no cambia de modo */

	/* --- Transiciones globales --- */
	{ FSM_ANY_STATE,    EVENT_HW_FAULT,  action_enterFault, FSM_STATE_FAULT },
	{ FSM_ANY_STATE,    EVENT_BT_DATA_RX,    NULL,               FSM_STATE_SETUP }, /* TODO: ajustar cuando exista bluetooth.c */
};

#define TRANSITION_COUNT (sizeof(transitionTable) / sizeof(transitionTable[0]))

/* ───────────────────────── Motor de la FSM ───────────────────────────────── */

static void fsmChangeState(fsmState_t next)
{
	if(next == currentState) return;

	currentState = next;

	if(enterActions[currentState] != NULL){
		enterActions[currentState]();
	}
}

void fsmInit(void)
{
	ledInit();
	buttonInit();
	buzzerInit();
	dipSwitchInit();
	queueInit();
	randomInit(tickGet());
	storageInit();
	gameInit();
	gameSetDifficulty(storageGetDifficulty());
	menuInit();
	ldrInit();
	lcdInit();
	bluetoothInit();

	/* Lectura de configuracion inicial via DIP switch (requisito 5.1).
	 * Se lee UNA sola vez al arrancar; durante la sesion, el menu y
	 * Bluetooth tienen control total sobre la dificultad. */
	uint8_t dipMask = dipSwitchReadAll();
	uint8_t dipDifficulty = dipMask & 0x03u;   /* DIP0+DIP1 */
	if(dipDifficulty < GAME_DIFFICULTY_COUNT){
		gameSetDifficulty(dipDifficulty);
		lcdShowTemporary("Dificultad:", gameGetDifficultyName());
	}

	currentState = FSM_STATE_INIT;
	fsmChangeState(FSM_STATE_SETUP);

	if(dipMask & 0x04u){   /* DIP2: modo demo, arranca partida sola */
		Event_t ev = { .type = EVENT_MENU_START_GAME, .data = 0 };
		queuePush(ev);
	}
	if(dipMask & 0x08u){   /* DIP3: fuerza modo FALLA para pruebas */
		Event_t ev = { .type = EVENT_HW_FAULT, .data = 0xDEADu };
		queuePush(ev);
	}
}

void fsmUpdate(void)
{
	Event_t ev;

	if(!queuePop(&ev)) return;   /* nada para procesar este tick */

	for(uint8_t i = 0; i < TRANSITION_COUNT; i++){

		bool stateMatch = (transitionTable[i].state == FSM_ANY_STATE) ||
		                   (transitionTable[i].state == currentState);

		if(stateMatch && transitionTable[i].event == ev.type){

			if(transitionTable[i].action != NULL){
				transitionTable[i].action(&ev);
			}
			fsmChangeState(transitionTable[i].nextState);
			return;   /* solo una transicion por evento consumido */
		}
	}

	/* Evento sin transicion definida para el estado actual: se descarta.
	 * (ej: EVENT_BTN_B_PRESSED en FSM_STATE_NORMAL, que va a manejar
	 * game.c directamente sacando el evento por otro lado, o se ignora) */
}

fsmState_t fsmGetState(void)
{
	return currentState;
}

void fsmForceFault(uint32_t errorCode)
{
	Event_t ev = { .type = EVENT_HW_FAULT, .data = errorCode };
	queuePush(ev);
}
