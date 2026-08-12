#ifndef CONFIG_H_
#define CONFIG_H_

#include <stdint.h>

/* ─────────────── Timing general ─────────────── */
#define TICK_MS                     1u
#define BTN_DEBOUNCE_MS             20u
#define DIP_DEBOUNCE_MS             30u

/* ─────────────── Juego ─────────────── */
#define GAME_TIMEOUT_INITIAL_MS     1500u
#define GAME_TIMEOUT_MIN_MS         300u
#define GAME_TIMEOUT_STEP_MS        50u
#define GAME_MAX_LIVES              3u
#define GAME_TARGET_COUNT           4u


/* ─────────────── Bluetooth (HM-10 / UART) ─────────────── */
#define BT_UART_RX_BUFFER_SIZE      32u
#define BT_UART_TX_BUFFER_SIZE      32u
#define BT_CMD_TIMEOUT_MS           500u

/* ─────────────── LDR / ADC ─────────────── */
#define LDR_ADC_SAMPLE_PERIOD_MS    50u
#define LDR_PWM_MIN_DUTY            10u
#define LDR_PWM_MAX_DUTY            100u

/* ─────────────── Cola de eventos ─────────────── */
#define QUEUE_SIZE                  16u     /


/* ─────────────── Bajo consumo ─────────────── */
#define LOW_POWER_IDLE_TIMEOUT_MS   30000u
#endif /* CONFIG_H_ */
