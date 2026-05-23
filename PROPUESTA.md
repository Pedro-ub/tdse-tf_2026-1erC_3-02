# "Whack-A-Mole"
## Juego Electrónico de Reflejos y Objetivos Aleatorios

**Autores:** Pedro Labollita, Marcos Masini, Ezequiel Robles 
**Padrones:** 112436, XXXXXXX, XXXXXXX  
**Fecha:** 1er cuatrimestre 2026

---

## 1. Selección del proyecto a implementar

### 1.1 Objetivo del proyecto y resultados esperados

El objetivo de este proyecto es diseñar e implementar un sistema interactivo inspirado en el juego arcade "Whack-A-Mole". El sistema generará objetivos luminosos de forma aleatoria y el jugador deberá responder rápidamente presionando el botón correspondiente antes de que expire un timeout. La velocidad y la dificultad se incrementarán progresivamente, poniendo a prueba los reflejos y la precisión del usuario.

Como funcionalidades adicionales, el sistema incluirá:

- **Ranking persistente** almacenado en EEPROM externa via I2C.
- **Comunicación inalámbrica** mediante módulo Bluetooth HM-10 (App de celular).
- **Menú interactivo** navegable con los botones del juego.
- **Modos de operación**: NORMAL (juego), SET_UP (configuración/menú) y FALLA (error de hardware).
- **Modos de dificultad** seleccionables (Fácil, Normal, Difícil).
- **Efectos sonoros** mediante buzzer gestionado por PWM.
- **Ajuste automático de brillo** de los LEDs mediante sensor analógico LDR + ADC.
- **Modos de bajo consumo** energético en estado de espera.
- **Placa base** con componentes y conectores soldados (sin protoboard ni cables Dupont).

Al finalizar el proyecto se espera obtener un prototipo funcional, configurable y expandible, con almacenamiento persistente de puntajes y comunicación Bluetooth.

---

### 1.2 Proyectos similares

Se consideran tres tipos de proyectos posibles que cumplen con los objetivos y resultados esperados, todos basados en juegos electrónicos de reflejos:

1. **Whack-A-Mole avanzado con STM32**: múltiples LEDs y botones, menú interactivo, Bluetooth HM-10, EEPROM I2C y ajuste de brillo por LDR.
2. **Juego básico con Arduino UNO**: implementación simple con LEDs y pulsadores, sin menú, sin conectividad ni persistencia.
3. **Aplicación móvil arcade**: versión en smartphone con interfaz táctil, sin hardware físico dedicado.

Para comparar estas alternativas, se tienen en cuenta seis aspectos:

- **Disponibilidad del hardware (peso: 10):** si el proyecto es implementable con componentes accesibles en el mercado regional, sin depender de importaciones costosas.
- **Escalabilidad del sistema (peso: 8):** facilidad para extender el proyecto incorporando nuevos modos de juego, periféricos adicionales o comunicación con otros dispositivos.
- **Experiencia de usuario — UX (peso: 8):** qué tan entretenida, inmersiva y configurable resulta la experiencia para el usuario final.
- **Tiempo de implementación (peso: 8):** tiempo total de diseño, programación, integración y depuración dentro del cuatrimestre.
- **Costo (peso: 5):** costo total incluyendo placa, sensores, LEDs, buzzer, EEPROM y módulo Bluetooth.
- **Interés personal (peso: 8):** motivación para trabajar con STM32, sus periféricos y el diseño de un sistema embebido complejo.

La disponibilidad del hardware es crítica porque se requiere trabajar con componentes fácilmente conseguibles localmente. La escalabilidad y la UX se valoran con peso 8 porque se busca un proyecto con posibilidad de expansión y que resulte entretenido. El tiempo de implementación también es prioritario dado el período acotado del cuatrimestre. El costo tiene menor peso porque se prioriza el aprendizaje por sobre la minimización del presupuesto. El interés personal se valora con 8 porque se busca un proyecto motivador que permita aplicar y profundizar los contenidos de la materia.

| Aspecto | Peso | STM32 Avanzado<br>Puntaje | STM32 Avanzado<br>Ponderado | Arduino Básico<br>Puntaje | Arduino Básico<br>Ponderado | App Móvil<br>Puntaje | App Móvil<br>Ponderado |
|---|:---:|:---:|:---:|:---:|:---:|:---:|:---:|
| Disponibilidad de Hardware | 10 | 9 | 90 | 10 | 100 | 10 | 100 |
| Escalabilidad | 8 | 10 | 80 | 5 | 40 | 5 | 40 |
| Experiencia de Usuario (UX) | 8 | 10 | 80 | 5 | 40 | 7 | 56 |
| Tiempo de implementación | 8 | 8 | 64 | 10 | 80 | 7 | 56 |
| Costo | 5 | 8 | 40 | 10 | 50 | 7 | 35 |
| Interés personal | 8 | 10 | 80 | 7 | 56 | 7 | 56 |
| **Puntaje Total** | | | **434** | | **366** | | **343** |

*Tabla 1.2.1: Comparación de proyectos*

---

### 1.3 Selección del proyecto

Considerando la Tabla 1.2.1, se elige implementar el juego **Whack-A-Mole avanzado con STM32**. Esta opción obtiene el mayor puntaje ponderado.

El **juego básico con Arduino UNO** presenta excelente disponibilidad de hardware y bajo costo, pero no permite aprovechar los periféricos del STM32 ni implementar arquitecturas complejas de firmware. Limita además la posibilidad de escalar el proyecto hacia conectividad, almacenamiento persistente o múltiples modos de operación.

La **aplicación móvil arcade** ofrece una experiencia de usuario razonable, pero elimina por completo el componente de hardware físico y la práctica de programación embebida, que es el objetivo central de la materia. Tampoco permite implementar los conceptos de interrupciones, PWM, ADC, I2C o manejo temporal sobre un microcontrolador real.

El **Whack-A-Mole avanzado con STM32** se destaca porque combina:

- Una experiencia de juego dinámica y configurable, con objetivos aleatorios, dificultad creciente y efectos sonoros.
- Un menú interactivo con tres modos de operación bien definidos: **NORMAL** (juego activo), **SET_UP** (configuración de dificultad, consulta de ranking, ajuste de parámetros vía Bluetooth) y **FALLA** (detección y reporte de errores de hardware).
- Ajuste automático del brillo de los LEDs mediante sensor LDR conectado al ADC del STM32.
- Persistencia de datos mediante EEPROM externa comunicada por I2C (ranking de puntajes).
- Comunicación Bluetooth HM-10 via UART para transmisión de score y recepción de configuración desde una App de celular.
- Placa base con componentes soldados, sin protoboard ni cables Dupont.

Este proyecto permite aplicar en profundidad los conceptos de: programación bare metal, interrupciones, PWM, ADC, UART, I2C, DMA, manejo temporal, tareas concurrentes no bloqueantes y FSM.

En el mercado local existen juegos de reflejos comerciales disponibles como aplicaciones de celular o juguetes electrónicos cerrados, pero ninguno permite modificar la lógica ni acceder al hardware. Este proyecto apunta a desarrollar una plataforma abierta y programable, orientada tanto al entretenimiento como al aprendizaje en ingeniería electrónica.

Los desafíos principales se centran en: la sincronización precisa de timeouts y velocidad creciente, el diseño de un esquema robusto de antirrebote, la integración del módulo Bluetooth para configuración remota y la organización del firmware en módulos y máquinas de estado que faciliten la depuración y expansión del sistema.

#### 1.3.1 Diagrama en bloques

```
           +----------------+
           |      LDR       |  (sensor analógico → ADC)
           +-------+--------+
                   |
                   v

+----------+   +------------------+   +-----------+
| Botones  |-->|                  |-->| LEDs (PWM)|
+----------+   |      STM32       |   +-----------+
               |                  |
+----------+-->|                  |-->| Buzzer    |
|DIP Switch|   |                  |   +-----------+
+----------+   |                  |
               |  UART  |  I2C   |
               |    |        |   |
               +----+--------+---+
                    |        |
               HM-10 (BT)  EEPROM externa
               (App celular)
```

*Figura 1.3.1: Diagrama en bloques del sistema*

---

## 2. Elicitación de requisitos y casos de uso

En el mercado argentino y mundial existen diversos productos relacionados con juegos de reflejos, desde juguetes electrónicos físicos hasta aplicaciones móviles que replican la experiencia de Whack-A-Mole. Estos productos suelen ofrecer una experiencia visual y sonora básica, pero son sistemas cerrados con lógica fija y sin posibilidad de configuración por parte del usuario.

Como primer competidor se pueden mencionar los juguetes electrónicos de tipo "golpea el topo" disponibles en el mercado local, los cuales ofrecen una experiencia física con botones reales pero carecen de conectividad, configuración de dificultad o almacenamiento de puntajes. El presente proyecto se diferencia por la integración de Bluetooth HM-10, EEPROM I2C, ajuste automático de brillo por LDR y menú interactivo con múltiples modos de operación.

Un segundo competidor lo constituyen las aplicaciones móviles de reflejos tipo arcade, ampliamente disponibles en tiendas digitales. Estas aplicaciones ofrecen gráficos avanzados y tablas de puntajes globales, pero eliminan el componente físico de interactuar con botones y LEDs reales, y no permiten practicar el diseño de sistemas embebidos. El presente proyecto recupera ese aspecto tangible y educativo, brindando un entorno ideal para ejercitar programación bare metal en C sobre STM32.

En resumen, el mercado de juegos de reflejos es amplio pero los sistemas embebidos programables que combinan hardware físico, conectividad y persistencia de datos ofrecen un nicho diferenciado, orientado tanto al entretenimiento como a la formación técnica en ingeniería electrónica.

### 2.1 Requisitos del proyecto

| Grupo | ID | Descripción |
|---|---|---|
| Juego | 1.1 | El sistema deberá activar objetivos luminosos de forma aleatoria. |
| Juego | 1.2 | El usuario deberá responder presionando el botón correcto antes del timeout. |
| Juego | 1.3 | El sistema deberá incrementar progresivamente la velocidad y reducir el timeout según el nivel alcanzado. |
| Juego | 1.4 | El sistema deberá contabilizar el score del jugador en tiempo real. |
| Juego | 1.5 | El sistema deberá finalizar la partida ante múltiples errores consecutivos. |
| LEDs | 2.1 | Cada objetivo deberá poseer un LED dedicado e identificable. |
| LEDs | 2.2 | El brillo de los LEDs deberá ajustarse automáticamente vía PWM según la luminosidad ambiente medida por el LDR. |
| Sensor LDR | 3.1 | El sistema contará con un sensor de luz LDR conectado a una entrada analógica (ADC) del STM32. |
| Sensor LDR | 3.2 | El sistema leerá periódicamente el valor del LDR mediante ADC gestionado por DMA o interrupción (sin polling bloqueante). |
| Sensor LDR | 3.3 | El valor del ADC se mapeará a un ciclo de trabajo PWM para ajustar el brillo de los LEDs. |
| Botones | 4.1 | Cada objetivo deberá poseer un botón dedicado. |
| Botones | 4.2 | El sistema deberá implementar antirrebote por software para todos los pulsadores. |
| Botones | 4.3 | El sistema no deberá perder pulsaciones rápidas (detección por interrupción). |
| DIP Switch | 5.1 | El sistema contará con DIP switches para selección de modo de operación o parámetros de configuración inicial. |
| Audio | 6.1 | El sistema deberá emitir sonidos de acierto mediante buzzer gestionado por PWM/timer. |
| Audio | 6.2 | El sistema deberá emitir un sonido característico ante errores o timeouts. |
| Audio | 6.3 | El sistema podrá emitir sonidos al navegar por el menú. |
| Bluetooth | 7.1 | El sistema deberá comunicarse con una App de celular mediante módulo HM-10 via UART. |
| Bluetooth | 7.2 | El sistema deberá transmitir el score en tiempo real a la App. |
| Bluetooth | 7.3 | El sistema deberá recibir parámetros de configuración (dificultad, timeout) desde la App. |
| Memoria EEPROM | 8.1 | El sistema deberá almacenar el ranking de puntajes en EEPROM externa comunicada por I2C. |
| Memoria EEPROM | 8.2 | El sistema deberá leer y mostrar el ranking al ingresar al modo SET_UP. |
| Memoria EEPROM | 8.3 | El sistema deberá permitir reiniciar el ranking desde el menú con confirmación del usuario. |
| Memoria EEPROM | 8.4 | La EEPROM almacenará además los parámetros de configuración (SET_UP) para persistencia ante apagados. |
| Modos | 9.1 | El sistema deberá implementar al menos tres modos de operación: NORMAL (juego), SET_UP (menú/config) y FALLA. |
| Modos | 9.2 | En modo FALLA el sistema deberá indicar el error mediante LEDs y buzzer, y reportarlo via Bluetooth. |
| Modos | 9.3 | El sistema deberá implementar un modo de bajo consumo durante los períodos de inactividad. |
| Arquitectura | 10.1 | El sistema deberá implementar un Super-Loop con tiempo de vuelta menor a 1 ms. |
| Arquitectura | 10.2 | El sistema deberá implementar un tick de 1 ms mediante SysTick con callback. |
| Arquitectura | 10.3 | El sistema deberá implementar tareas de código no bloqueantes (temporizadas y no temporizadas). |
| Arquitectura | 10.4 | El sistema deberá implementar FSM con interfaces mediante colas, estructuras o arrays de estructuras. |
| Arquitectura | 10.5 | El sistema deberá implementar un menú interactivo navegable con los botones del juego. |
| Arquitectura | 10.6 | Los periféricos deberán gestionarse mediante interrupciones, polling no bloqueante o DMA con callbacks. |
| Hardware | 11.1 | El sistema no deberá utilizar protoboard ni cables Dupont para la interconexión. |
| Hardware | 11.2 | El sistema deberá montarse sobre una placa base con componentes y conectores soldados. |

*Tabla 2.1: Requisitos del proyecto*

---

### 2.2 Caso de uso 1: El usuario juega una partida en modo NORMAL

| Elemento | Definición |
|---|---|
| **Disparador** | El jugador selecciona "Nuevo juego" desde el menú principal (modo SET_UP) y el sistema transiciona al modo NORMAL. |
| **Precondiciones** | El sistema está encendido. No hay partida en curso. Todos los LEDs están apagados y el buzzer en silencio. La dificultad fue previamente configurada. |
| **Flujo principal** | El jugador navega el menú en modo SET_UP y selecciona "Nuevo juego". El sistema transiciona al modo NORMAL. Se activa aleatoriamente uno o más LEDs objetivo. El jugador presiona el botón correspondiente antes de que expire el timeout. Por cada acierto: el score aumenta, se emite un sonido de éxito y la velocidad se incrementa progresivamente. El ciclo se repite hasta que el jugador acumula múltiples errores o finaliza la partida. Al finalizar, si el score supera el máximo almacenado, se actualiza la EEPROM y se transmite el score via Bluetooth. El sistema retorna al modo SET_UP. |
| **Flujos alternativos** | a) **Timeout**: el sistema registra el error, emite un sonido de falla y descuenta una vida. Si se agotan las vidas, finaliza la partida. b) **Botón incorrecto**: ídem anterior. c) **Múltiples errores**: el sistema finaliza la partida, muestra el score final y lo compara contra el ranking en EEPROM. d) **Error de hardware durante la partida**: el sistema transiciona al modo FALLA, emite señal de error por LEDs y buzzer, y reporta via Bluetooth. |

*Tabla 2.2: Caso de uso 1*

---

### 2.3 Caso de uso 2: El usuario consulta o reinicia el ranking (modo SET_UP)

| Elemento | Definición |
|---|---|
| **Disparador** | El jugador accede a la opción "Ranking" dentro del menú interactivo en modo SET_UP. |
| **Precondiciones** | El sistema está encendido en modo SET_UP. No hay partida en curso. La EEPROM ha sido inicializada correctamente y la comunicación I2C es funcional. |
| **Flujo principal** | El jugador navega hasta la sección "Ranking" del menú usando los botones. El sistema lee de la EEPROM via I2C el ranking de puntajes máximos y los muestra. El jugador puede seleccionar "Reiniciar ranking". El sistema solicita confirmación. Si el jugador confirma, el sistema borra el ranking en EEPROM y muestra un mensaje de éxito. El sistema retorna al menú principal del modo SET_UP. |
| **Flujos alternativos** | a) **Cancelación**: el jugador sale del submenú sin confirmar; el sistema conserva el ranking y retorna al menú anterior. b) **Error de comunicación I2C con EEPROM**: el sistema transiciona al modo FALLA, indica el error mediante LEDs y buzzer, y deshabilita la opción de ranking hasta reiniciar el dispositivo. |

*Tabla 2.3: Caso de uso 2*

---

### 2.4 Caso de uso 3: El usuario configura la dificultad vía Bluetooth (modo SET_UP)

| Elemento | Definición |
|---|---|
| **Disparador** | El usuario envía un comando de configuración desde la App de celular al módulo HM-10 del sistema. |
| **Precondiciones** | El sistema está encendido en modo SET_UP. No hay partida en curso. El módulo HM-10 está vinculado con el celular y la comunicación UART es funcional. |
| **Flujo principal** | El usuario abre la App en el celular y se conecta al sistema via Bluetooth HM-10. La App muestra las opciones de configuración (dificultad, timeout, número de vidas). El usuario selecciona los parámetros deseados y los envía. El sistema recibe los datos por UART en modo no bloqueante (interrupción o DMA). El sistema valida los parámetros recibidos, actualiza la configuración en memoria RAM y la persiste en EEPROM via I2C. El sistema envía confirmación a la App via Bluetooth. Los nuevos parámetros quedan activos para la siguiente partida. |
| **Flujos alternativos** | a) **Parámetros inválidos**: el sistema descarta la configuración, mantiene los valores previos y envía un mensaje de error a la App. b) **Pérdida de conexión Bluetooth durante la recepción**: el sistema descarta el paquete incompleto y mantiene la configuración anterior. c) **Error de escritura en EEPROM**: el sistema actualiza la RAM con la nueva configuración pero reporta el fallo de persistencia a la App; el modo FALLA no se activa por este motivo pero se notifica al usuario. |

*Tabla 2.4: Caso de uso 3*

---

## 3. Hardware obligatorio y adicional

### 3.1 Hardware obligatorio

| Componente | Uso en el sistema |
|---|---|
| DIP Switches | Selección de modo de operación inicial y parámetros de configuración |
| Botones (pulsadores) | Uno por objetivo luminoso + navegación de menú |
| LEDs | Uno por objetivo; brillo controlado por PWM según LDR |
| Buzzer | Efectos sonoros de acierto, error, navegación de menú y modo FALLA |
| Módulo HM-10 (Bluetooth) | Comunicación con App de celular via UART para score y configuración |
| EEPROM externa I2C | Almacenamiento persistente de ranking y parámetros de SET_UP |
| Sensor LDR (analógico) | Ajuste automático de brillo de LEDs mediante ADC |
| Placa base soldada | Montaje de todos los componentes sin protoboard ni cables Dupont |

### 3.2 Hardware adicional



- Posible incorporación de display 7 segmentos o LCD para visualización de score y menú.


---

## 4. Programación obligatoria y adicional

### 4.1 Programación obligatoria

| Ítem | Implementación en el proyecto |
|---|---|
| Bare Metal (sin SO) | Firmware en C puro sobre STM32, sin RTOS |
| Event-Triggered System | Interrupciones para botones, UART (HM-10) y ADC/DMA (LDR) |
| Estructurada/Modular (Escrutar/Procesar/Actuar) | Módulos separados: botones, LEDs, buzzer, BT, EEPROM, juego, menú |
| Super-Loop (polling & interrupts), 1 vuelta < 1 ms | Loop principal de alta frecuencia; lógica pesada delegada a tareas |
| Tick = 1 ms (SysTick → Callback) | Base temporal para timeouts de juego, debounce y tareas temporizadas |
| Tareas no bloqueantes | Todas las tareas (lectura LDR, actualización LEDs, BT, EEPROM) son no bloqueantes |
| FSM con interfaces (colas, estructuras) | FSM principal: INIT → SET_UP ↔ NORMAL ↔ FALLA; sub-FSM de menú y juego |
| Menú interactivo | Navegable con los botones del juego; muestra dificultad, ranking y score |
| Bajo consumo | Modo sleep durante inactividad prolongada en SET_UP |
| Al menos dos modos de operación | NORMAL (juego activo), SET_UP (menú/configuración), FALLA (error) |
| Periféricos SPI y/o I2C | I2C para EEPROM externa |
| Periféricos por polling, interrupts o DMA | Botones: interrupts; LDR/ADC: DMA con callback; UART HM-10: interrupción |

### 4.2 Programación adicional


- Generación de números pseudoaleatorios mediante semilla variable (tiempo de reacción del usuario).
