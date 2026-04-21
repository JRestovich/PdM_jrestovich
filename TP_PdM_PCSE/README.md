# TP_PdM_PCSE

Proyecto embebido para STM32F446RE que implementa una interfaz de usuario simple
con teclado tactil capacitivo, display LCD 16x2, lectura de sensores internos del
microcontrolador y control del LED de usuario de la placa.

La aplicacion esta organizada alrededor de una maquina de estados finita (MEF).
Desde un menu principal se puede navegar entre lecturas de sensores analogicos
internos y modos de control de luces. La salida principal de usuario es un LCD
16x2 y la entrada principal es un teclado tactil basado en MPR121.

## Hardware utilizado

- Placa STM32F446RE, con LED de usuario en `PA5`.
- UART2 por `PA2`/`PA3` a `115200 8N1`, usada para mensajes de diagnostico y
  redireccion de `printf`.
- MPR121 como controlador tactil capacitivo de 12 teclas por I2C.
- LCD 16x2 con expansor I2C, direccion `0x27`.
- ADC1 para leer sensores internos del microcontrolador:
  - sensor interno de temperatura;
  - referencia interna `VREFINT`.

## Perifericos y conexiones logicas

| Modulo | Periferico | Pines / direccion | Uso |
| --- | --- | --- | --- |
| UART de debug | USART2 | `PA2` TX, `PA3` RX | Mensajes de inicio, errores y `printf` |
| LED usuario | GPIO | `PA5` | Encendido fijo o parpadeo |
| MPR121 | I2C1 | `PB8` SCL, `PB9` SDA, addr `0x5A` | Teclado tactil |
| LCD 16x2 | I2C2 | `PB10` SCL, `PB3` SDA, addr `0x27` | Interfaz visual |
| Sensores internos | ADC1 | canales internos | Temperatura y VREFINT |

La configuracion del clock usa HSI con PLL. El sistema queda a 84 MHz
(`16 MHz / 16 * 336 / 4`), con APB1 a 42 MHz y APB2 a 84 MHz.

## Estructura del proyecto

```text
TP_PdM_PCSE/
|-- Core/
|   |-- Inc/                  # Headers generados/base de STM32CubeIDE
|   `-- Src/
|       |-- main.c            # Punto de entrada, clock, UART y loop principal
|       |-- stm32f4xx_it.c    # Handlers de interrupciones
|       `-- stm32f4xx_hal_msp.c
|-- Drivers/
|   |-- API/
|   |   |-- Inc/              # Interfaces de la aplicacion y drivers propios
|   |   `-- Src/              # Implementaciones de la MEF y drivers
|   |-- CMSIS/
|   `-- STM32F4xx_HAL_Driver/
|-- STM32F446RETX_FLASH.ld
`-- STM32F446RETX_RAM.ld
```

## Flujo principal

El archivo `Core/Src/main.c` realiza el arranque basico:

1. Inicializa HAL con `HAL_Init()`.
2. Configura el reloj del sistema con `SystemClock_Config()`.
3. Inicializa UART2 con `uartInit()`.
4. Inicializa la aplicacion con `APP_init()`.
5. Ejecuta indefinidamente `APP_engine()`.

Tambien redefine `_write()` para que `printf` transmita por UART2. Esto permite
ver mensajes como errores de inicializacion o confirmacion de la MEF por la
terminal serie.

## Maquina de estados de la aplicacion

La MEF principal esta implementada en `Drivers/API/Src/app.c` y declarada en
`Drivers/API/Inc/app.h`. Sus estados son:

- `init`: muestra el mensaje de bienvenida durante 5 segundos.
- `home`: menu principal, con acceso a sensores o luces.
- `analogSensors`: submenu de sensores internos.
- `temperatureSensors`: lectura periodica de temperatura interna.
- `voltimeterSensor`: lectura periodica de VREFINT en milivoltios.
- `lights`: submenu de control del LED.
- `lightsFix`: LED fijo, con opciones de encendido/apagado.
- `lightsBlink`: LED intermitente, con opciones de encendido/apagado/frecuencia.
- `lightsBlinkSetFreq`: ingreso de frecuencia de parpadeo.
- `error`: pantalla temporal para errores de operacion, por ejemplo frecuencia
  invalida.

### Navegacion por teclado

- En `home`:
  - `1`: entrar al menu de sensores.
  - `2`: entrar al menu de luces.
- En sensores:
  - `1`: mostrar temperatura.
  - `2`: mostrar tension de referencia interna.
  - `*`: volver al menu principal.
- En luces:
  - `1`: modo fijo.
  - `2`: modo parpadeo.
  - `*`: volver al menu principal.
- En modo fijo:
  - `1`: encender LED.
  - `2`: apagar LED.
- En modo parpadeo:
  - `1`: activar parpadeo.
  - `2`: detener parpadeo.
  - `3`: ingresar nueva frecuencia.
- Al ingresar frecuencia:
  - digitos `0` a `9`: cargan el valor;
  - `#`: confirma;
  - `*`: cancela y vuelve al inicio.

La frecuencia valida es mayor que `0 Hz` y menor que `200 Hz`. El ingreso acepta
hasta 3 digitos.

## Modulos propios

### `app`

Integra todos los drivers y contiene la logica de usuario. Durante `APP_init()`
inicializa MPR121, LCD, sensores internos, LED y temporizadores no bloqueantes.
Si algun modulo falla, acumula banderas de error (`ERROR_MPR121`,
`ERROR_LCD16x2`, `ERROR_INT_SENSORS`) y `APP_engine()` queda inhibida.

### `API_MPR121`

Driver de alto nivel para el teclado tactil. Lee el estado de las 12 entradas
del MPR121 y reporta una tecla solo cuando detecta el ciclo presion/liberacion.
Esto evita ejecutar acciones repetidas mientras una tecla permanece presionada.

El port `API_MPR121_port` configura el MPR121 por I2C1:

- soft reset;
- modo stop para configurar registros;
- umbrales de touch/release para 12 electrodos;
- filtros y debounce;
- habilitacion final de los 12 electrodos.

### `API_LCD16x2`

Driver de alto nivel para un LCD 16x2 sobre expansor I2C. Maneja la secuencia de
arranque en modo de 4 bits, comandos de control, escritura de caracteres,
limpieza de pantalla, posicionamiento de cursor, actualizacion de filas y
retroiluminacion.

El port `API_LCD16x2_port` usa la abstraccion generica `API_I2C_DEVICE` sobre
I2C2.

### `API_I2C_DEVICE`

Abstraccion comun para dispositivos I2C. Define un descriptor con
`I2C_HandleTypeDef` y direccion de esclavo, carga una configuracion por defecto
de `100 kHz`, inicializa el hardware y ofrece operaciones de transmision,
recepcion y lectura de registros.

Este modulo es compartido por el LCD y el MPR121.

### `API_intSensors`

API de alto nivel para leer sensores internos. Delega la configuracion y
conversion al port `API_intSensors_port`.

El port usa ADC1 con dos conversiones:

- `ADC_CHANNEL_TEMPSENSOR`;
- `ADC_CHANNEL_VREFINT`.

La temperatura se convierte a grados Celsius con una aproximacion lineal. La
tension de referencia se calcula en milivolts usando la macro LL
`__LL_ADC_CALC_VREFANALOG_VOLTAGE`.

### `API_led`

Control de alto nivel para el LED. Soporta dos modos:

- `FIX`: estado fijo, encendido o apagado;
- `BLINK`: parpadeo periodico.

El parpadeo usa `API_delay` para temporizacion no bloqueante. La frecuencia se
convierte a semiperiodo con `1000 / (2 * freq)`.

### `API_delay`

Implementa retardos no bloqueantes basados en `HAL_GetTick()`. La funcion
`delayRead()` arranca el temporizador si no esta corriendo y devuelve `true`
cuando se cumple la duracion configurada.

### `API_led_port`

Capa de acceso GPIO del LED. Configura el pin como salida push-pull y provee
operaciones fisicas de encendido, apagado, toggle y lectura de estado.

## Partes mas relevantes

- `main.c`: concentra el arranque del sistema y mantiene el loop principal
  llamando a la MEF.
- `app.c`: contiene el comportamiento visible del proyecto: menus, navegacion,
  lecturas periodicas, control de LED y manejo de errores.
- `API_MPR121.c`: transforma lecturas crudas del teclado en eventos de tecla
  validos al soltar.
- `API_LCD16x2.c`: encapsula todos los detalles del protocolo LCD 4 bits sobre
  I2C.
- `API_I2C_DEVICE.c`: evita duplicar configuracion I2C entre perifericos.
- `API_intSensors_port.c`: configura ADC1 y convierte mediciones internas a
  unidades utiles.
- `API_led.c` + `API_delay.c`: implementan el parpadeo sin bloquear la MEF.

## Uso esperado

1. Abrir el proyecto en STM32CubeIDE.
2. Compilar y cargar sobre la placa STM32F446RE.
3. Abrir una terminal serie a `115200 baudios`, 8 bits, sin paridad, 1 bit de
   stop.
4. Al iniciar, el LCD muestra `Bienvenidos` durante 5 segundos.
5. Navegar con el teclado tactil segun las opciones mostradas en el LCD.

Si falla la inicializacion de algun modulo, se informa por UART y la aplicacion
entra en `Error_Handler()`.
