# Práctica 5

**Alumno: Joaquin Restovich**

Esta práctica implementa un módulo de **interpretación de comandos por UART** basado en una máquina de estados y lo utiliza en el programa principal para controlar el LED `LD2` de la placa NUCLEO.

## Módulo `cmdParser`

### Objetivo
Permitir la recepción de comandos de texto por `USART2`, validarlos y ejecutar acciones simples sin bloquear el `while(1)` principal.

### Interfaz
- `cmdParserInit()`: inicializa la UART y deja listo el parser para comenzar a recibir comandos.
- `cmdPoll()`: atiende la recepción, el procesamiento y la ejecución de comandos.
- `cmdPrintHelp()`: envía por UART la lista de comandos disponibles.

## Funcionamiento del programa

En `main.c`, luego de inicializar la placa, se llama a `cmdParserInit()` una única vez. Después, dentro del `while(1)`, se ejecuta continuamente `cmdPoll()` para avanzar la máquina de estados del parser.

El módulo recibe caracteres individuales por UART, arma una línea en un buffer interno y, al detectar fin de línea, la convierte a mayúsculas y evalúa el comando ingresado.

Los comandos actualmente implementados son:

- `HELP`: muestra la lista de comandos disponibles.
- `LED ON`: enciende el LED `LD2`.
- `LED OFF`: apaga el LED `LD2`.
- `LED TOGGLE`: conmuta el estado del LED `LD2`.
- `STATUS`: informa por UART si el LED está encendido o apagado.

Además, las líneas que comienzan con `#` o `//` se descartan durante el procesamiento y no se ejecutan como comandos válidos.
