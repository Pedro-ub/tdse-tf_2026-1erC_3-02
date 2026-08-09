#include "button.h"
#include "main.h"
#include "tick.h"
#include "queue.h"
#include "config.h"

#define BUTTON_C_QTY    (sizeof(button_c_list) / sizeof(Button_C))
#define BUTTON_V_QTY    BUTTON_C_QTY



const Button_C button_c_list[]={
		{BTN_A_GPIO_Port, BTN_A_Pin, ID_BTN_A},
		{GPIOC, BTN_B_Pin, ID_BTN_B},
		{GPIOC, BTN_C_Pin, ID_BTN_C},
		{GPIOC, BTN_D_Pin, ID_BTN_D}
};

static Button_V button_v_list[BUTTON_V_QTY];
static bool button_event_pending[BUTTON_V_QTY];

/* Activo-bajo: pin en RESET (0V) = botón presionado */
static bool buttonIsPressed(uint8_t i)
{
	return (HAL_GPIO_ReadPin(button_c_list[i].port, button_c_list[i].pin) == GPIO_PIN_RESET);
}

void buttonInit(void){
	for(uint8_t i = 0 ; i < BUTTON_V_QTY ; i++){
		button_v_list[i].tick = 0;
		button_v_list[i].state = ST_BTN_UP;
		button_v_list[i].event = EV_BTN_UP;
		button_event_pending[i] = false;
	}
}

void buttonUpdate(void){

	for(uint8_t i = 0 ; i < BUTTON_V_QTY ; i++){

		bool pressed = buttonIsPressed(i);

		switch(button_v_list[i].state){

		case ST_BTN_UP:
			if(pressed){
				button_v_list[i].state = ST_BTN_FALLING;
				button_v_list[i].tick = tickGet();
			}
			break;

		case ST_BTN_FALLING:
			if(pressed){
				if((tickGet() - button_v_list[i].tick) >= BTN_DEBOUNCE_MS){
					button_v_list[i].state = ST_BTN_DOWN;
					button_v_list[i].event = EV_BTN_DOWN;
					button_event_pending[i] = true;

					 /* Publicar evento en la cola global */
					Event_t ev = { .type = (EventType_t)(EVENT_BTN_A_PRESSED + i), .data = i };
					queuePush(ev);
				}
			} else {
				/* fue un rebote, vuelve a UP sin generar evento */
				button_v_list[i].state = ST_BTN_UP;
			}
			break;

		case ST_BTN_DOWN:
			if(!pressed){
				button_v_list[i].state = ST_BTN_RISING;
				button_v_list[i].tick = tickGet();
			}
			break;

		case ST_BTN_RISING:
			if(!pressed){
				if((tickGet() - button_v_list[i].tick) >= BTN_DEBOUNCE_MS){
					button_v_list[i].state = ST_BTN_UP;
					button_v_list[i].event = EV_BTN_UP;
				}
			} else {
				/* rebote, vuelve a DOWN */
				button_v_list[i].state = ST_BTN_DOWN;
			}
			break;

		default:
			button_v_list[i].state = ST_BTN_UP;
			break;
		}
	}
}

bool buttonGetEvent(uint8_t id){
	if(id >= BUTTON_V_QTY) return false;

	if(button_event_pending[id]){
		button_event_pending[id] = false;
		return true;
	}
	return false;
}
