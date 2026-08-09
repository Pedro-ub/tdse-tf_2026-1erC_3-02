#include "menu.h"
#include "fsm.h"
#include "leds.h"
#include "button.h"
#include "buzzer.h"
#include "game.h"
#include "queue.h"
#include "storage.h"
#include "lcd.h"
#include <stdio.h>

typedef enum{
	MENU_OPT_NEW_GAME = 0,
	MENU_OPT_DIFFICULTY,
	MENU_OPT_RANKING,
	MENU_OPT_COUNT
} menuOption_t;

typedef enum{
	MENU_STATE_MAIN,
	MENU_STATE_RANKING,
	MENU_STATE_RANKING_CONFIRM
} menuState_t;

static menuState_t   menuState    = MENU_STATE_MAIN;
static menuOption_t  selectedOpt  = MENU_OPT_NEW_GAME;
static uint8_t       difficultyLevel = GAME_DIFFICULTY_NORMAL;

static void showSelection(void)
{
	ledAllOff();
	if(menuState == MENU_STATE_MAIN){
		ledOn(selectedOpt);
	} else {
		ledOn(MENU_OPT_RANKING);
	}
}

static void goToNextOption(void)
{
	selectedOpt = (menuOption_t)((selectedOpt + 1) % MENU_OPT_COUNT);
	buzzerPlay(BUZZER_SOUND_MENU);
	showSelection();
}

static void confirmMainOption(void)
{
	switch(selectedOpt){
	case MENU_OPT_NEW_GAME:
	{
		buzzerPlay(BUZZER_SOUND_MENU);
		Event_t ev = { .type = EVENT_MENU_START_GAME, .data = 0 };
		queuePush(ev);
		break;
	}
	case MENU_OPT_DIFFICULTY:
		difficultyLevel = (difficultyLevel + 1) % GAME_DIFFICULTY_COUNT;
		gameSetDifficulty(difficultyLevel);
		storageSetDifficulty(difficultyLevel);
		lcdShowTemporary("Dificultad:", gameGetDifficultyName());
		buzzerPlay(BUZZER_SOUND_MENU);
		buzzerPlay(BUZZER_SOUND_MENU);
		break;

	case MENU_OPT_RANKING:
	{
			uint8_t count;
			const uint16_t *ranking = storageGetRanking(&count);
			char line[20];
			lcdClear();
			lcdSetCursor(0, 0);
			lcdPrint("Ranking:");
			lcdSetCursor(0, 1);
			snprintf(line, sizeof(line), "%u %u %u", ranking[0], ranking[1], ranking[2]);
			lcdPrint(line);
			menuState = MENU_STATE_RANKING;
			buzzerPlay(BUZZER_SOUND_MENU);
			showSelection();
			break;
		}

	default:
		break;
	}
}

static void handleRankingSubmenu(bool confirmPressed, bool backPressed)
{
	if(menuState == MENU_STATE_RANKING){
		if(confirmPressed){
			menuState = MENU_STATE_RANKING_CONFIRM;
			buzzerPlay(BUZZER_SOUND_MENU);
		} else if(backPressed){
			menuState = MENU_STATE_MAIN;
			buzzerPlay(BUZZER_SOUND_MENU);
			showSelection();
		}
	} else if(menuState == MENU_STATE_RANKING_CONFIRM){
		if(confirmPressed){
			storageResetRanking();
			buzzerPlay(BUZZER_SOUND_HIT);
			menuState = MENU_STATE_MAIN;
			showSelection();
		} else if(backPressed){
			menuState = MENU_STATE_MAIN;
			buzzerPlay(BUZZER_SOUND_MENU);
			showSelection();
		}
	}
}

void menuInit(void)
{
	menuState       = MENU_STATE_MAIN;
	selectedOpt     = MENU_OPT_NEW_GAME;
	difficultyLevel = storageGetDifficulty();
}

void menuEnter(void)
{
	menuState   = MENU_STATE_MAIN;
	selectedOpt = MENU_OPT_NEW_GAME;
	showSelection();
}

void menuUpdate(void)
{
	if(fsmGetState() != FSM_STATE_SETUP) return;

	bool next    = buttonGetEvent(1); /* BTN_B */
	bool confirm = buttonGetEvent(0); /* BTN_A */
	bool back    = buttonGetEvent(3); /* BTN_D */

	if(menuState == MENU_STATE_MAIN){
		if(next){
			goToNextOption();
		} else if(confirm){
			confirmMainOption();
		}
	} else {
		handleRankingSubmenu(confirm, back);
	}
}
