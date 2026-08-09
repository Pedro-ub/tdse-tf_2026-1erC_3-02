<p align="center">
  <img src="./img/Logo-FIUBA.png" alt="Logo FIUBA">
</p>

# Memoria del trabajo final: "Whack-A-Mole" — Juego Electrónico de Reflejos y Objetivos Aleatorios

<table align="center">
  <tr>
    <th>Autor</th>
    <th>Padrón</th>
    <th>Mail</th>
  </tr>
  <tr>
    <td>Labollita, Pedro</td>
    <td>112436</td>
    <td>[PENDIENTE: mail institucional]</td>
  </tr>
  <tr>
    <td>Masini, Marcos</td>
    <td>[PENDIENTE]</td>
    <td>[PENDIENTE]</td>
  </tr>
  <tr>
    <td>Robles, Ezequiel</td>
    <td>[PENDIENTE]</td>
    <td>[PENDIENTE]</td>
  </tr>
</table>

<p align="center">
  2026 | 1er Cuatrimestre
</p>

<p align="center">
  Taller de Sistemas Embebidos (TA134)
</p>

<p align="center">
  Universidad de Buenos Aires | Facultad de Ingeniería
</p>

<p align="center">
  [PENDIENTE: lugar y período de realización del trabajo]
</p>

---

# Resumen

[PENDIENTE — completar al cierre del trabajo. Debe incluir en 4-6 líneas: qué se construyó (juego de reflejos tipo Whack-A-Mole sobre STM32), arquitectura de firmware (bare metal, super-loop, FSM), periféricos principales (LEDs, botones, buzzer, DIP switch, EEPROM I2C, HM-10 Bluetooth, LDR+ADC) y el resultado obtenido (prototipo funcional, configurable, con ranking persistente y control desde app de celular).]

---

# Registro de versiones

| Revisión | Cambios realizados | Fecha |
| :---: | --- | :---: |
| 0.1 | Esqueleto inicial de memoria (esta entrega) | 18/07/2026 |
| ... | [PENDIENTE] | ... |

<em>Tabla 0.1 — Registro de versiones del documento.</em>

---

# Índice general

- [Capítulo 1: Introducción general](#capítulo-1-introducción-general)
  - [1.1. Selección del proyecto a implementar](#11-selección-del-proyecto-a-implementar)
  - [1.2. Proyectos similares](#12-proyectos-similares)
  - [1.3. Justificación del enfoque técnico](#13-justificación-del-enfoque-técnico)
  - [1.4. Alcance y limitaciones](#14-alcance-y-limitaciones)
- [Capítulo 2: Introducción específica](#capítulo-2-introducción-específica)
  - [2.1. Requisitos](#21-requisitos)
  - [2.2. Casos de uso](#22-casos-de-uso)
  - [2.3. Descripción de módulos principales](#23-descripción-de-módulos-principales)
- [Capítulo 3: Diseño e implementación](#capítulo-3-diseño-e-implementación)
  - [3.1. Hardware](#31-hardware)
  - [3.2. Firmware](#32-firmware)
- [Capítulo 4: Ensayos y resultados](#capítulo-4-ensayos-y-resultados)
  - [4.1. Pruebas funcionales de hardware](#41-pruebas-funcionales-de-hardware)
  - [4.2. Pruebas funcionales de firmware](#42-pruebas-funcionales-de-firmware)
  - [4.3. Pruebas de integración](#43-pruebas-de-integración)
  - [4.4. Circuito esquemático y cableado](#44-circuito-esquemático-y-cableado)
  - [4.5. Console and Build Analyzer](#45-console-and-build-analyzer)
  - [4.6. Medición y análisis de WCET por tarea](#46-medición-y-análisis-de-wcet-por-tarea)
  - [4.7. Cálculo del factor de uso de CPU (U)](#47-cálculo-del-factor-de-uso-de-cpu-u)
  - [4.8. Medición y análisis de consumo](#48-medición-y-análisis-de-consumo)
  - [4.9. Cumplimiento de requisitos](#49-cumplimiento-de-requisitos)
  - [4.10. Comparación con sistemas similares](#410-comparación-con-sistemas-similares)
- [Capítulo 5: Conclusiones](#capítulo-5-conclusiones)
- [Capítulo 6: Uso de herramientas de IA](#capítulo-6-uso-de-herramientas-de-ia)
- [Capítulo 7: Bibliografía y referencias](#capítulo-7-bibliografía-y-referencias)

---

# Capítulo 1: Introducción general

## 1.1. Selección del proyecto a implementar

### 1.1.1. Objetivo del proyecto y resultados esperados

El objetivo de este proyecto es diseñar e implementar un sistema interactivo inspirado en el juego arcade "Whack-A-Mole". El sistema genera objetivos luminosos de forma aleatoria y el jugador debe responder rápidamente presionando el botón correspondiente antes de que expire un timeout. La velocidad y la dificultad se incrementan progresivamente, poniendo a prueba los reflejos y la precisión del usuario.

Funcionalidades adicionales incluidas:
- Ranking persistente almacenado en EEPROM externa vía I2C.
- Comunicación inalámbrica mediante módulo Bluetooth HM-10 (app de celular).
- Menú interactivo navegable con los botones del juego.
- Modos de operación: NORMAL (juego), SET_UP (configuración/menú) y FALLA (error de hardware).
- Modos de dificultad seleccionables (Fácil, Normal, Difícil).
- Efectos sonoros mediante buzzer gestionado por PWM.
- Ajuste automático de brillo de los LEDs mediante sensor analógico LDR + ADC.
- Uso de DIP switches para selección rápida de dificultad y modo de prueba/debug.
- LED de estado dedicado para distinguir visualmente entre modo juego, menú y falla.
- Modos de bajo consumo energético en estado de espera.
- Placa base con componentes y conectores soldados (sin protoboard ni cables Dupont) para la entrega final.

### 1.1.2. Proyectos similares

Se compararon tres alternativas: Whack-A-Mole avanzado con STM32, juego básico con Arduino UNO, y una aplicación móvil arcade sin hardware dedicado. Se evaluaron seis aspectos ponderados: disponibilidad de hardware, escalabilidad, experiencia de usuario, tiempo de implementación, costo e interés personal.

| Aspecto | Peso | STM32 Avanzado | Arduino Básico | App Móvil |
|---|:---:|:---:|:---:|:---:|
| Disponibilidad de Hardware | 10 | 90 | 100 | 100 |
| Escalabilidad | 8 | 80 | 40 | 40 |
| Experiencia de Usuario (UX) | 8 | 80 | 40 | 56 |
| Tiempo de implementación | 8 | 64 | 80 | 56 |
| Costo | 5 | 40 | 50 | 35 |
| Interés personal | 8 | 80 | 56 | 56 |
| **Total** | | **434** | **366** | **343** |

<em>Tabla 1.1 — Comparación de alternativas de proyecto.</em>

## 1.2. Justificación del enfoque técnico

Se eligió el Whack-A-Mole avanzado con STM32 por combinar experiencia de juego dinámica, menú interactivo con tres modos bien definidos, ajuste automático de brillo por LDR, persistencia por EEPROM I2C, comunicación Bluetooth vía HM-10, y una placa base soldada sin protoboard ni Dupont. Permite aplicar en profundidad conceptos de programación bare metal, interrupciones, PWM, ADC, UART, I2C, DMA, tareas concurrentes no bloqueantes y FSM.

[PENDIENTE: agregar diagrama en bloques definitivo del sistema como figura, con numeración e imagen real en vez del diagrama ASCII preliminar.]

## 1.3. Alcance y limitaciones

Alcance implementado hasta el momento de esta entrega (esqueleto):
- Núcleo de arquitectura: super-loop, tick 1 ms vía SysTick, scheduler de tareas cooperativo.
- FSM principal con modos SET_UP / NORMAL / FALLA.
- Lógica de juego completa: generación de objetivos aleatorios, timeout decreciente, vidas, score.
- Generador pseudoaleatorio (xorshift32) resembrado con tiempo de reacción del jugador.
- Módulos de entrada/salida: botones (con antirrebote FSM), LEDs, buzzer (PWM/TIM3), DIP switches (con antirrebote).
- Menú interactivo navegable con los botones del juego (reutilización de hardware, según requisito 10.5).

Pendiente de implementación al momento de esta entrega (a completar antes de la entrega final):
- `eeprom.c` — persistencia real de ranking y configuración por I2C.
- `bluetooth.c` — comunicación con app de celular vía HM-10/UART.
- `ldr.c` — lectura de sensor de luz por ADC+DMA y ajuste de brillo PWM de LEDs.
- `low_power.c` — modo de bajo consumo en inactividad.
- Diseño y fabricación de la placa base definitiva (actualmente en desarrollo sobre protoboard).

[PENDIENTE: actualizar esta sección a medida que se cierren los módulos restantes.]

---

# Capítulo 2: Introducción específica

## 2.1. Requisitos

La Tabla 2.1 resume los requisitos definidos para el proyecto, agrupados por área funcional.

| Grupo | ID | Descripción |
|---|---|---|
| Juego | 1.1 | Activar objetivos luminosos de forma aleatoria. |
| Juego | 1.2 | Responder presionando el botón correcto antes del timeout. |
| Juego | 1.3 | Incrementar progresivamente la velocidad y reducir el timeout según el nivel. |
| Juego | 1.4 | Contabilizar el score del jugador en tiempo real. |
| Juego | 1.5 | Finalizar la partida ante múltiples errores. |
| LEDs | 2.1 | Cada objetivo posee un LED dedicado e identificable. |
| LEDs | 2.2 | Brillo de LEDs ajustado automáticamente vía PWM según luminosidad ambiente (LDR). |
| Sensor LDR | 3.1 – 3.3 | Lectura de luz por ADC/DMA, mapeada a ciclo de trabajo PWM de brillo. |
| Botones | 4.1 – 4.3 | Un botón por objetivo, antirrebote por software, sin pérdida de pulsaciones rápidas. |
| DIP Switch | 5.1 | Selección de modo de operación o parámetros de configuración inicial. |
| Audio | 6.1 – 6.3 | Sonidos de acierto, error y navegación de menú vía buzzer/PWM. |
| Bluetooth | 7.1 – 7.3 | Comunicación con app vía HM-10/UART: transmisión de score y recepción de configuración. |
| EEPROM | 8.1 – 8.4 | Ranking persistente y parámetros de SET_UP almacenados por I2C. |
| Modos | 9.1 – 9.3 | Al menos tres modos (NORMAL/SET_UP/FALLA) y modo de bajo consumo. |
| Arquitectura | 10.1 – 10.6 | Super-loop <1 ms, tick 1 ms, tareas no bloqueantes, FSM con interfaces, menú interactivo, periféricos por interrupción/DMA/polling no bloqueante. |
| Hardware | 11.1 – 11.2 | Sin protoboard ni Dupont; placa base soldada en la entrega final. |

<em>Tabla 2.1 — Requisitos del proyecto (versión completa en README del repositorio).</em>

[PENDIENTE: si hubo requisitos ajustados/recortados respecto a la propuesta original, agregar una segunda tabla de "requisitos ajustados" como se hizo en el ejemplo del Dimmer+Switch, con trazabilidad de qué cambió y por qué.]

## 2.2. Casos de uso

Se definieron tres casos de uso principales:

1. **El usuario juega una partida en modo NORMAL** — desde SET_UP se activa una partida; se generan objetivos aleatorios con timeout decreciente; se registran aciertos/errores; al finalizar se compara el score contra el ranking en EEPROM y se transmite por Bluetooth.
2. **El usuario consulta o reinicia el ranking (modo SET_UP)** — navegación al submenú de ranking, lectura vía I2C, opción de reinicio con confirmación explícita del usuario.
3. **El usuario configura la dificultad vía Bluetooth (modo SET_UP)** — la app envía parámetros (dificultad, timeout, vidas) por UART; el sistema valida, actualiza RAM y persiste en EEPROM; confirma a la app.

[PENDIENTE: incluir las tablas completas de cada caso de uso (Disparador / Precondiciones / Flujo principal / Flujos alternativos), ya definidas en el documento de propuesta, formateadas igual que en los ejemplos de referencia (Tablas 2.2 a 2.4).]

## 2.3. Descripción de módulos principales

- **Módulo de control (NUCLEO-F103RB):** ejecuta el super-loop con tick de 1 ms (SysTick → callback) y un scheduler cooperativo de tareas periódicas.
- **Módulo de entradas:** botones (antirrebote por FSM de 4 estados: UP/FALLING/DOWN/RISING), DIP switches (antirrebote análogo), sensor LDR (ADC/DMA, pendiente).
- **Módulo de salidas:** LEDs de objetivo + LED de estado, buzzer (PWM sobre TIM3).
- **Módulo de comunicación:** HM-10 vía UART (pendiente), EEPROM externa vía I2C (pendiente).
- **Módulo de lógica:** máquina de estados principal (`fsm.c`), lógica de juego (`game.c`), menú interactivo (`menu.c`), generador pseudoaleatorio (`random.c`).
- **Módulo de interfaces:** cola de eventos genérica (`queue.c`) que desacopla las fuentes de eventos (botones, DIP, futuros HM-10/EEPROM) de la FSM consumidora.

[PENDIENTE: diagrama de bloques/dependencias entre módulos como figura numerada — ya existe una versión preliminar en texto plano, falta convertirla a diagrama gráfico.]

---

# Capítulo 3: Diseño e implementación

## 3.1. Hardware

### 3.1.1. Placa con microcontrolador
Se utiliza una placa NUCLEO-F103RB (STM32F103RB, ARM Cortex-M3).

### 3.1.2. Botones
Cuatro pulsadores, uno por objetivo/LED, reutilizados también para navegación del menú interactivo en modo SET_UP (requisito 10.5).

### 3.1.3. LEDs
Cuatro LEDs de objetivo (uno por botón) más un LED de estado adicional para distinguir visualmente entre modo NORMAL, SET_UP y FALLA.

[PENDIENTE: justificar la incorporación del LED de estado como parte del hardware adicional consensuado, y actualizar el diagrama en bloques y la tabla de pines en consecuencia.]

### 3.1.4. Buzzer
Gestionado por PWM sobre TIM3, canal 1. [PENDIENTE: documentar el ajuste de prescaler realizado para evitar overflow del ARR de 16 bits en frecuencias graves.]

### 3.1.5. DIP switches
Utilizados para selección rápida de dificultad (Fácil/Normal/Difícil) y modo de prueba/debug, complementando la navegación del menú por botones.

### 3.1.6. Sensor LDR
[PENDIENTE: módulo aún no implementado — documentar conexión, divisor resistivo y canal de ADC utilizado una vez definido.]

### 3.1.7. Módulo Bluetooth HM-10
[PENDIENTE: módulo aún no implementado — documentar configuración AT, UART utilizada y protocolo de trama.]

### 3.1.8. Memoria EEPROM externa
[PENDIENTE: módulo aún no implementado — documentar dirección I2C, mapa de memoria (ranking + configuración SET_UP).]

### 3.1.9. Criterio de interconexión y montaje
[PENDIENTE: reemplazar protoboard de desarrollo por placa base soldada (circuito impreso o placa experimental con wire wrap/soldado) antes de la entrega final, según requisito 11.1/11.2.]

### 3.1.10. Pinout del sistema
[PENDIENTE: tabla de pinout completa, análoga a la del ejemplo Dimmer+Switch, una vez cerrada la asignación definitiva de pines en CubeMX (incluyendo LEDs, DIP switches, LDR, I2C y UART del HM-10).]

## 3.2. Firmware

### 3.2.1. Arquitectura de ejecución
Bare metal (sin sistema operativo), tipo Event-Triggered System. Super-loop con `schedulerUpdate()` llamado desde `main()`, tick de 1 ms generado por `HAL_SYSTICK_Callback()` → `tick_callback()`. Tareas registradas en `task_scheduler.c`: `appUpdate`, `buttonUpdate`, `buzzerUpdate`, `dipSwitchUpdate`, `gameUpdate`, `menuUpdate`.

### 3.2.2. Máquina de estados principal
`fsm.c` implementa la FSM global mediante un array de estructuras `{estado, evento, acción, próximo_estado}` (requisito 10.4), con estados `FSM_STATE_INIT → FSM_STATE_SETUP ↔ FSM_STATE_NORMAL ↔ FSM_STATE_FAULT` y soporte de transiciones globales (comodín `FSM_ANY_STATE`) para eventos como fallas de hardware.

[PENDIENTE: diagrama de estados (Harel/Itemis o Mermaid) como figura numerada.]

### 3.2.3. Módulo de juego
`game.c` gestiona la partida: selección aleatoria de objetivo (`random.c`, xorshift32 resembrado con el tiempo de reacción real del jugador), timeout decreciente por dificultad, conteo de vidas y score, y generación del evento `EVENT_GAME_OVER` hacia la FSM.

### 3.2.4. Módulo de menú
`menu.c` implementa la navegación en modo SET_UP reutilizando los botones del juego, con submenú de ranking (consulta/reinicio con confirmación) y selección cíclica de dificultad.

### 3.2.5. Interfaz de eventos
`queue.c` implementa una cola FIFO de eventos de un solo consumidor (la FSM), utilizada para desacoplar los módulos que generan cambios de modo (menú, fin de juego, fallas, y a futuro Bluetooth/EEPROM) de la lógica que decide la transición de estado.

### 3.2.6 Persistencia (EEPROM I2C)
[PENDIENTE: módulo `eeprom.c` no implementado a la fecha de este esqueleto.]

### 3.2.7 Comunicación Bluetooth
[PENDIENTE: módulo `bluetooth.c` no implementado a la fecha de este esqueleto.]

### 3.2.8 Bajo consumo
[PENDIENTE: módulo `low_power.c` no implementado a la fecha de este esqueleto.]

---

# Capítulo 4: Ensayos y resultados

> Todas las subsecciones de este capítulo dependen de tener el hardware montado en la placa base definitiva y los módulos de EEPROM/Bluetooth/LDR terminados. Se dejan como placeholders a completar antes de la entrega final.

## 4.1. Pruebas funcionales de hardware
[PENDIENTE: tabla de ensayos de hardware (continuidad, verificación de cada periférico por separado) análoga a la Tabla 4.1 de los ejemplos de referencia.]

## 4.2. Pruebas funcionales de firmware
[PENDIENTE: tabla de ensayos de firmware — debounce de botones/DIP, FSM, lógica de juego, menú, y a futuro EEPROM/Bluetooth/LDR.]

## 4.3. Pruebas de integración
[PENDIENTE: video breve del trabajo final funcionando — **obligatorio para la entrega final**, con link a YouTube o similar.]

## 4.4. Circuito esquemático y cableado
[PENDIENTE: esquemático eléctrico completo y fotos del cableado/montaje final, siguiendo el estilo de figuras usado en "A Beginner's Guide to Designing Embedded System Applications on Arm Cortex-M Microcontroller".]

## 4.5. Console and Build Analyzer
[PENDIENTE: captura de la salida de memoria (text/data/bss en bytes, % de RAM y FLASH usados) de la compilación de la versión definitiva.]

## 4.6. Medición y análisis de WCET por tarea
[PENDIENTE: instrumentar `DWT->CYCCNT` (o mecanismo equivalente) para medir el peor tiempo de ejecución de cada tarea del scheduler (`appUpdate`, `buttonUpdate`, `buzzerUpdate`, `dipSwitchUpdate`, `gameUpdate`, `menuUpdate`, y las que se sumen de EEPROM/Bluetooth/LDR).]

## 4.7. Cálculo del factor de uso de CPU (U)
[PENDIENTE: calcular $U = \sum C_i / T_i$ con los WCET medidos en 4.6 y el período de tick de 1 ms, siguiendo la metodología del ejemplo Dimmer+Switch.]

## 4.8. Medición y análisis de consumo
[PENDIENTE: medición de corriente/tensión en 3,3 V y 5 V, con y sin modo de bajo consumo activo, usando miliamperímetro y osciloscopio, en los distintos modos de operación (NORMAL, SET_UP, FALLA, con/sin Bluetooth activo).]

## 4.9. Cumplimiento de requisitos
[PENDIENTE: tabla de cumplimiento final por requisito (🟢 implementado / 🟡 parcial / 🔴 no implementado), con justificación de cualquier recorte de alcance, siguiendo el formato de la Tabla 4.6 del ejemplo Dimmer+Switch.]

## 4.10. Comparación con sistemas similares
[PENDIENTE: retomar la comparación de la sección 1.1.2 (juguete físico comercial vs. app móvil arcade) y contrastarla contra el resultado final obtenido.]

---

# Capítulo 5: Conclusiones

## 5.1. Resultados obtenidos
[PENDIENTE — completar al cierre del trabajo.]

## 5.2. Lecciones aprendidas
[PENDIENTE — completar al cierre del trabajo.]

## 5.3. Próximos pasos
[PENDIENTE — completar al cierre del trabajo, por ejemplo: display adicional para mostrar score/ranking, más modos de dificultad, multijugador, etc.]

---

# Capítulo 6: Uso de herramientas de IA

Se utilizó asistencia de IA (Claude, Anthropic) durante el desarrollo del firmware, principalmente para:
- Revisión de código y detección de errores de compilación (includes faltantes, redefiniciones de macros, desbordes de registros de hardware como el ARR de 16 bits de TIM3).
- Discusión de decisiones de diseño de arquitectura (reparto de responsabilidades entre módulos, uso de la cola de eventos como interfaz de desacople, mapeo de botones/DIP switches/LEDs a las distintas funcionalidades del sistema).
- Generación de esqueletos de módulos (`random.c/h`, `game.c/h`, `menu.c/h`) a partir de los requisitos y la arquitectura ya definida por el equipo.



# Capítulo 7: Bibliografía y referencias

1. STMicroelectronics, *UM1724 - STM32 Nucleo-64 boards user manual*.
2. STMicroelectronics, *STM32F103RB Datasheet*.
3. STMicroelectronics, *RM0008 - STM32F101/102/103/105/107 Reference manual*.
4. [PENDIENTE: datasheet del módulo HM-10.]
5. [PENDIENTE: datasheet de la EEPROM I2C utilizada.]
6. Repositorio del proyecto: [PENDIENTE: link al repositorio].

---

**Fin del esqueleto de la Memoria Técnica — Whack-A-Mole**
Autores: Pedro Labollita, Marcos Masini, Ezequiel Robles
Fecha de esta versión: 18/07/2026
