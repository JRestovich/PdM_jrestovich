# Práctica 3

**Alumno: Joaquin Restovich**

Esta práctica implementa y utiliza un módulo de **delay no bloqueante** para temporizar eventos sin detener el `while(1)` principal. El proyecto corre sobre la placa NUCLEO con HAL de STM32 y usa el LED `LD2` como salida visible para verificar el comportamiento del retardo.

## Objetivo

Desacoplar las temporizaciones de la ejecución principal mediante una API simple basada en `HAL_GetTick()`, de forma que el programa pueda seguir ejecutándose mientras espera el vencimiento de un intervalo.

## Módulo `API_delay`

El módulo define el tipo `delay_t`, que almacena:

- `startTime`: instante de inicio del retardo.
- `duration`: duración configurada en ticks.
- `running`: estado interno del retardo.

### Interfaz pública

- `delayInit(delay_t *delay, tick_t duration)`: inicializa una estructura de retardo con una duración dada.
- `delayRead(delay_t *delay)`: actualiza el estado del retardo y devuelve `true` cuando el tiempo configurado se cumplió.
- `delayWrite(delay_t *delay, tick_t duration)`: cambia la duración del retardo.
- `delayIsRunning(delay_t *delay)`: informa si el retardo se encuentra corriendo.

## Funcionamiento del programa principal

En `main.c` se crea una instancia global de `delay_t` y se inicializa con el primer valor del arreglo `TIEMPOS`.

Cada vez que `delayRead()` indica que el intervalo terminó:

- se conmuta el estado del LED `LD2`,
- se incrementa un contador de semiperíodos,
- y, al alcanzar `SEMIPERIODOS`, se avanza al siguiente tiempo del arreglo.

### Secuencia de tiempos implementada

El arreglo actualmente definido en el código es:

- `500 ms`
- `100 ms`
- `100 ms`
- `1000 ms`

Con `SEMIPERIODOS` configurado en `2`, el programa cambia de duración cada dos conmutaciones del LED.

## Comportamiento observable

El LED `LD2` parpadea de forma no bloqueante y va modificando su ritmo según la secuencia de tiempos configurada en `TIEMPOS`. Esto permite comprobar que la temporización se realiza sin usar retardos bloqueantes como `HAL_Delay()`.

## Archivos principales

- `Core/Src/main.c`: lógica principal de la aplicación y cambio de períodos de parpadeo.
- `Drivers/API/Inc/API_delay.h`: interfaz pública del módulo de delay.
- `Drivers/API/Src/API_delay.c`: implementación del delay no bloqueante.
