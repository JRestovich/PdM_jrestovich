# Práctica 2

**Alumno: Joaquin Restovich**

Esta práctica implementa un módulo de **delay no bloqueante** basado en `HAL_GetTick()` y lo utiliza en el programa principal para generar un patrón de parpadeo sobre el LED `LD2`.

## Módulo `delay`

### Objetivo
Permitir temporizaciones sin el bucle principal, para mantener la lógica reactiva y escalable.

### Interfaz
- `delayInit(delay_t *delay, tick_t duration)`: inicializa la estructura y duración base.
- `delayRead(delay_t *delay)`: consulta si venció el tiempo configurado.
- `delayWrite(delay_t *delay, tick_t duration)`: actualiza la duración para el próximo ciclo.

## Funcionamiento del programa

En el `while(1)` principal se utiliza el módulo de delay para hacer parpadear el LED `LD2` de forma no bloqueante. Cada vez que el retardo vence, el LED cambia de estado.

El programa cuenta 10 semiperíodos de parpadeo y luego cambia la duración del delay, recorriendo cíclicamente los valores `1000 ms`, `200 ms` y `100 ms`. De esta manera, el LED alterna entre distintas velocidades de parpadeo sin frenar la ejecución principal.
