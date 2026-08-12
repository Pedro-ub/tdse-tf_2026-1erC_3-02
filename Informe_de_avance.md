# "Whack-A-Mole"
## Juego Electrónico de Reflejos y Objetivos Aleatorios
### Primer Informe de Avances

**Autores:** Pedro Labollita, Marcos Masini
**Padrones:** 112436, 110838
**Fecha:** 13/06/2026
**1er cuatrimestre 2026**

---

A continuación se detalla el primer informe de avances del proyecto a partir de los requerimientos establecidos.

| Estado | Descripción |
|:---:|---|
| 🟢 | Ya implementado |
| 🟡 | En proceso de implementarse |
| 🔴 | No implementado aún |

---

## Arquitectura base

| Req ID | Descripción | Estado |
|---|---|:---:|
| 10.1 | El sistema implementa un Super-Loop con tiempo de vuelta menor a 1 ms. | 🟢 |
| 10.2 | El sistema implementa un tick de 1 ms mediante SysTick con callback (`tickCallback`). | 🟢 |
| 10.3 | El sistema implementa tareas no bloqueantes temporizadas mediante scheduler (`schedulerUpdate`). | 🟢 |
| 10.4 | El sistema implementa la estructura de FSM principal con estados `APP_INIT`, `APP_SETUP`, `APP_NORMAL` y `APP_FAULT`. | 🟡 |
| 10.5 | El sistema implementa un menú interactivo navegable con los botones del juego. | 🔴 |
| 10.6 | Los periféricos se gestionan mediante interrupciones, polling no bloqueante o DMA con callbacks. | 🟡 |

---

## LEDs

| Req ID | Descripción | Estado |
|---|---|:---:|
| 2.1 | Cada objetivo posee un LED dedicado e identificable. | 🟢 |
| 2.2 | El brillo de los LEDs se ajusta automáticamente vía PWM según la luminosidad ambiente medida por el LDR. | 🔴 |

---

## Botones

| Req ID | Descripción | Estado |
|---|---|:---:|
| 4.1 | Cada objetivo posee un botón dedicado. | 🟢 |
| 4.2 | El sistema implementa antirrebote por software para todos los pulsadores (FSM de debounce de 20 ms por botón). | 🟢 |
| 4.3 | El sistema no pierde pulsaciones rápidas (eventos autoconsumibles mediante `buttonGetEvent`). | 🟢 |

---

## Sensor LDR

| Req ID | Descripción | Estado |
|---|---|:---:|
| 3.1 | El sistema contará con un sensor LDR conectado a una entrada analógica (ADC) del STM32. | 🔴 |
| 3.2 | El sistema leerá periódicamente el valor del LDR mediante ADC gestionado por DMA o interrupción. | 🔴 |
| 3.3 | El valor del ADC se mapeará a un ciclo de trabajo PWM para ajustar el brillo de los LEDs. | 🔴 |

---

## Audio (Buzzer)

| Req ID | Descripción | Estado |
|---|---|:---:|
| 6.1 | El sistema emitirá sonidos de acierto mediante buzzer gestionado por PWM/timer. | 🔴 |
| 6.2 | El sistema emitirá un sonido característico ante errores o timeouts. | 🔴 |
| 6.3 | El sistema podrá emitir sonidos al navegar por el menú. | 🔴 |

---

## Bluetooth (HM-10)

| Req ID | Descripción | Estado |
|---|---|:---:|
| 7.1 | El sistema se comunicará con una App de celular mediante módulo HM-10 via UART. | 🔴 |
| 7.2 | El sistema transmitirá el score en tiempo real a la App. | 🔴 |
| 7.3 | El sistema recibirá parámetros de configuración (dificultad, timeout) desde la App. | 🔴 |

---

## Memoria EEPROM (I2C)

| Req ID | Descripción | Estado |
|---|---|:---:|
| 8.1 | El sistema almacenará el ranking de puntajes en EEPROM externa comunicada por I2C. | 🔴 |
| 8.2 | El sistema leerá y mostrará el ranking al ingresar al modo SET_UP. | 🔴 |
| 8.3 | El sistema permitirá reiniciar el ranking desde el menú con confirmación del usuario. | 🔴 |
| 8.4 | La EEPROM almacenará los parámetros de configuración para persistencia ante apagados. | 🔴 |

---

## Modos de operación

| Req ID | Descripción | Estado |
|---|---|:---:|
| 9.1 | El sistema implementará tres modos de operación: `NORMAL` (juego), `SET_UP` (menú/config) y `FALLA`. | 🟡 |
| 9.2 | En modo `FALLA` el sistema indicará el error mediante LEDs y buzzer, y lo reportará via Bluetooth. | 🔴 |
| 9.3 | El sistema implementará un modo de bajo consumo durante los períodos de inactividad. | 🔴 |

---

## Juego

| Req ID | Descripción | Estado |
|---|---|:---:|
| 1.1 | El sistema activará objetivos luminosos de forma aleatoria. | 🔴 |
| 1.2 | El usuario responderá presionando el botón correcto antes del timeout. | 🔴 |
| 1.3 | El sistema incrementará progresivamente la velocidad y reducirá el timeout según el nivel. | 🔴 |
| 1.4 | El sistema contabilizará el score del jugador en tiempo real. | 🔴 |
| 1.5 | El sistema finalizará la partida ante múltiples errores consecutivos. | 🔴 |

---

## DIP Switch

| Req ID | Descripción | Estado |
|---|---|:---:|
| 5.1 | El sistema contará con DIP switches para selección de modo de operación o parámetros de configuración inicial. | 🔴 |

---

## Hardware

| Req ID | Descripción | Estado |
|---|---|:---:|
| 11.1 | El sistema no utilizará protoboard ni cables Dupont para la interconexión. | 🔴 |
| 11.2 | El sistema se montará sobre una placa base con componentes y conectores soldados. | 🔴 |

---

## Resumen de avance

Al momento de este primer informe, se encuentra completamente funcional la **base de arquitectura del firmware**: el tick de 1 ms por SysTick, el scheduler no bloqueante con tabla de tareas temporizadas, la estructura de la FSM principal (`app.c`) y el módulo BSP de LEDs (`bsp_led.c`) con control individual por índice.

Adicionalmente, se implementó el módulo de botones (`bsp_button.c`) con antirrebote, verificado en hardware con los 4 pulsadores del juego.
