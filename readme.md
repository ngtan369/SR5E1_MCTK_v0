# SR5E1E7 Motor-Control Project Report

## 1. Scope and verified target

This project is configured for:

- MCU family: **ST Stellar SR5E1**
- Target derivative: **SR5E1E7** (the TRACE32 flash script lists SR5E1E7 with 1920 KB code flash, 64 KB data flash and 448 KB RAM)
- Board profile: `evbe7000s`
- Core: `core1` / Cortex-M7
- Build configuration: `Debug`, flash target
- SDK: `StellarESDK-1.7.0`
- Motor-control toolkit: `SR5E1_MCTK-1.3.0`
- Application: `SR5E1_MCTK_TTM01V1_Demo`

The project is generated/configured through StellarStudio. The source configuration files are more authoritative than files under `src-gen`.

## 2. Important pinout result

Do not document `TIM1_CH3 = PC15`. That mapping is incorrect for this project.

The verified mapping is:

| MCU pin | Board signal | Alternate function | Motor-control use |
|---|---|---|---|
| PC15 / E7_67 | `HIN1` | `TIM1_CH1`, AF4 | Phase W high side |
| PC14 / E7_66 | `LIN1` | `TIM1_CH1N`, AF4 | Phase W low side |
| PD1 / E7_70 | `HIN2` | `TIM1_CH2`, AF4 | Phase V high side |
| PD0 / E7_69 | `LIN2` | `TIM1_CH2N`, AF4 | Phase V low side |
| PD4 / E7_60 | `HIN3` | `TIM1_CH3`, AF4 | Phase U high side |
| PD3 / E7_59 | `LIN3` | `TIM1_CH3N`, AF4 | Phase U low side |

Evidence in the workspace:

- `Config.gpio` maps `TIM1_PWM_CH1` to `E7_67` (`HIN1`) and `TIM1_PWM_CH3` to `E7_60` (`HIN3`).
- `src-gen/evbe7000s.h` expands `HIN1` to `PC15` and `HIN3` to `PD4`.
- The SDK board definition also identifies `PC15` as `TIM1_CH1`.
- `src-gen/mc_config.h` intentionally maps motor Phase U to timer channel 3, Phase V to channel 2 and Phase W to channel 1. This is a logical phase assignment and does not mean that PC15 is channel 3.

For a final hardware release, verify the exact SR5E1E7 package pin number in the official SR5E1E7 datasheet. `E7_66`, `E7_67`, etc. are the board/package labels used by the Stellar GPIO configuration, not MCU port names.

## 3. Complete relevant pinout

### PWM and current feedback

| Function | Peripheral | MCU pin | Board label |
|---|---|---|---|
| Phase U high/low | TIM1_CH3 / CH3N | PD4 / PD3 | HIN3 / LIN3 |
| Phase V high/low | TIM1_CH2 / CH2N | PD1 / PD0 | HIN2 / LIN2 |
| Phase W high/low | TIM1_CH1 / CH1N | PC15 / PC14 | HIN1 / LIN1 |
| Phase U current | ADC3 channel 1 | PB12 | CIU |
| Phase V current | ADC2 channel 1 | PB7 | CIV |
| Phase W current | ADC1 channel 1 | PB2 | CIW |

### Hall, sensing and driver control

| Function | Peripheral | MCU pin | Board label |
|---|---|---|---|
| Hall 1 | TIM2_CH1 | PH2 | MC_HALL_CH1 |
| Hall 2 | TIM2_CH2 | PH3 | MC_HALL_CH2 |
| Hall 3 | TIM2_CH3 | PH4 | MC_HALL_CH3 |
| VBUS | ADC5 channel 1 | PC9 | VBUS |
| Speed/throttle input | ADC5 channel 2 | PC10 | VSPEED |
| NTC | ADC5 channel 3 | PC11 | NTC |
| Temperature | ADC4 channel 1 | PC3 | TEMP |
| Gate driver enable | GPIO output | PB1 | STDRIVE_EN |
| Gate driver fault | GPIO input | PC5 | STDRIVE_FAULT |
| Board input S1/S2/S3 | GPIO input | PA13/PA14/PA15 | S1/S2/S3 |
| User LEDs 1/2/3 | GPIO output | PE3/PE2/PE1 | USER_LED_1/2/3 |

### CAN, UART and auxiliary outputs

| Interface | TX | RX |
|---|---|---|
| CAN1 | PA5 / E7_2 | PA4 / E7_1 |
| CAN2 | PA11 / E7_13 | PA12 / E7_14 |
| CAN3 | PH10 / E7_158 | PH11 / E7_159 |
| CAN4 | PH12 / E7_160 | PH13 / E7_161 |
| UART1 | PA1 / E7_4 | PA0 / E7_3 |

Auxiliary brake outputs:

- `RBK_HIN`: PF2 / E7_101 / TIM4_CH1
- `RBK_LIN`: PF3 / E7_102 / TIM4_CH2

## 4. Timer, PWM and motor-control configuration

Source: `config.Motorcontrol` and generated headers in `src-gen`.

- PWM timer: `TIM1`
- PWM frequency: `10 kHz`
- ADC trigger: TIM1 TRGO, with TIM1 channel 4 reserved for current-sampling timing
- High-side polarity: Active Low
- Low-side polarity: Active Low
- Hardware driver dead time: `2000 ns`
- Software low-side dead time: `700 ns`
- Over-current protection: enabled
- TIM1 break input polarity: high
- TIM1 break filter: `FDIV1`
- TIM1 break2: disabled
- Hall timer: `TIM2`
- Hall displacement: `120 degrees`
- Hall averaging FIFO: `18`
- UART motor-control interface: `UART1`, `38400 baud`
- Main speed sensor: Hall sensors
- Motor pole pairs: `14`
- Nominal DC bus: `72 V`
- Three-shunt current sensing: `0.0005 ohm`, gain `20`
- VBUS sensing: enabled, divider `1220 / 10`
- Temperature sensing and firmware over-temperature protection: enabled

## 5. CAN configuration

Source files:

- `config.Sr5e1_can`
- `config1.Sr5e1_can`
- `application.appcfg`

Current generated configuration uses:

- CAN clock: `40 MHz`
- RX buffer: `4`
- TX buffer: `4`
- RX FIFO0/FIFO1: `4` each
- TX FIFO queue: `4`
- TX event FIFO: `4`
- Classic filter ID: `2032`
- All CAN interrupts enabled
- Receive callback: `canrx_receive_cb`
- Error callback: `canrx_error_cb`

The project has pin definitions for CAN1 through CAN4. A peripheral is not necessarily active just because its pins are defined; it must also be initialized and used by application code.

## 6. Root files and folders

### Keep - required project inputs or source

| Path | Purpose |
|---|---|
| `Makefile` | Build target, SDK/MCTK paths, enabled modules and application sources |
| `.project`, `.cproject`, `.settings/` | Eclipse/StellarStudio project metadata |
| `application.appcfg` | Lists GPIO, clock, motor-control and CAN components |
| `Config.gpio` | GPIO and peripheral pin routing source |
| `myBoard.gpio_board` | Board model/profile selected by GPIO configuration |
| `clock_cfg.Sr5e1_clk` | Clock-tree generator input |
| `config.Motorcontrol` | Motor, PWM, current sensing, Hall, UART and protection input |
| `config.Sr5e1_can`, `config1.Sr5e1_can` | CAN generator inputs |
| `src/` | Application and project C sources |
| `include/` | Application headers |
| `src-gen/` | Generated headers and configuration consumed by the build |
| `.sdk/` | Local Stellar SDK and MCTK used by the Makefile |
| `debug/sr5e1_project.cfg` | Debug probe/OpenOCD configuration |
| `SR5E1_MC_FLASH.cmm` | TRACE32 flash/programming script |
| `SR5E1_window.cmm` | TRACE32 register/watch window setup |

### Generated or disposable - remove when cleaning

| Path | Purpose | Recommendation |
|---|---|---|
| `build/` | Object files, dependency files, listings and ELF/HEX/BIN/SREC output | Delete freely; recreated by build |
| `c_objs.lst` | Temporary object list used by the linker step | Delete freely |
| `debug/*.log` | Previous build/clean/debug logs | Delete if historical logs are not needed |

Use `make purge` to remove the active target build directory. Use `make purge_all` to remove all target build output. The SDK Makefile uses POSIX utilities, so run these from StellarStudio's configured build environment.

### Review before deleting

| Path | Purpose | Recommendation |
|---|---|---|
| `cfg/` | Manually copied CAN driver/configuration sources | Not compiled by the current `Makefile`; archive or delete only if manual CAN code is permanently unused |
| `src-gen/*.html`, `*.svg`, `*.xls` | Human-readable generator reports | Keep if used for review; otherwise removable and regenerable |
| `.cursor/` | Local editor/assistant metadata | Keep only if the team uses it; otherwise remove from the project |
| `debug/*.backup`, old dated folders | Old recovery/debug artifacts | Remove after confirming no rollback is needed |

Do not delete `src-gen` headers without regenerating the project. The build includes `src-gen`, and files such as `evbe7000s.h`, `mc_config.h` and `parameters_conversion.h` are required.

## 7. How to customize safely

1. Change the source generator inputs, not generated headers:
   - Pins: `Config.gpio`
   - Board profile: `myBoard.gpio_board`
   - Clock: `clock_cfg.Sr5e1_clk`
   - Motor/PWM/sensing: `config.Motorcontrol`
   - CAN: `config.Sr5e1_can` and `config1.Sr5e1_can`
2. Regenerate the project with StellarStudio.
3. Check the generated result:
   - `src-gen/evbe7000s.h` for pin and AF mapping
   - `src-gen/mc_config.h` for timer, ADC, Hall and UART mapping
   - `src-gen/parameters_conversion.h` for PWM/dead-time calculations
4. Update application logic in `src/main.c` only when signal names or behavior change.
5. Clean and rebuild:

```text
make purge
make all
```

6. Confirm the output in:

```text
build\sr5e1\evbe7000s\core1\Debug\
```

Expected firmware outputs are `.elf`, `.hex`, `.bin` and `.srec`.

## 8. Local SDK changes made during warning cleanup

The local copy under `.sdk/SR5E1_MCTK-1.3.0` was edited to remove compiler warnings without changing the intended behavior:

- unused Hall parameters are explicitly marked unused;
- `MCboot` array declarations were made consistent;
- the MCP sent-frame callback now uses a type-safe wrapper;
- the unsupported FDCAN Rx Buffer branch returns a parameter error instead of emitting a compile-time warning.

These are local vendor-tree modifications. Preserve them when copying this workspace, or record them as a patch if the SDK is replaced.