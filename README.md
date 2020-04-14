# Tiva-FreeRTOS

Simple project to give a try with Tiva TM4C microcontroller and FreeRTOS kernel.

This project assumes, that you have in your system installed following software (all of them are available in Ubuntu 19.04 APT repository):
- gcc-arm-none-eabi, version at least 7.3.1
- gdb-multiarch, version 8.3
- GNU Make, version 4.2.1
- LM4Flash, version 0.1.3
- OpenOCD, version 0.10.0

Build instructions are written in `Makefile`. 
In order to build application, just type:

```sh
make # same as `make all`
```

Then, you can write it directly to the microcontroller (flash it):
```sh
make flash
```

... or start debug session:

```sh
make debug
```

Inside `.gdbinit` file you have defined some useful commands, which can be used during debug session:
- `reload` causes microcontroller reset, halt and program load.
- `rebuild` does the same as `reload`, but before that it invokes `make all`

## Authors

ram.techen
