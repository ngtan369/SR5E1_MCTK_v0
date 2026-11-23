# Báo cáo dự án điều khiển motor SR5E1E7

## 1. Phạm vi và target đã xác nhận

- MCU: **ST Stellar SR5E1E7**
- Board profile: `evbe7000s`
- Core: `core1` / Cortex-M7
- Cấu hình build: `Debug`, target Flash
- SDK: `StellarESDK-1.7.0`
- MCTK: `SR5E1_MCTK-1.3.0`
- Application: `SR5E1_MCTK_TTM01V1_Demo`
- Motor: hub motor 17 inch, 28 cặp cực
- Flash code 960 KB trong cấu hình OpenOCD hiện tại

Nguồn cấu hình có trách nhiệm cao nhất là các file nguồn trong project. Thư mục
`src-gen/` là kết quả sinh code và không được sửa tay.

### Trạng thái vòng điều khiển

- Open-loop đã từng được thử trong giai đoạn bring-up; cấu hình `main` hiện tại
  chưa chạy motor.
- Hall voltage/Hall feedback đã được chọn làm hướng phát triển tiếp theo.
- `LED_ONLY_TEST` hiện đang bật trong `include/main.h`; firmware dừng trong
  nhánh test UART/Hall/LED trước phần khởi tạo driver và MotorControl.
- `OPEN_LOOP_FOC FALSE` nên các giá trị `OL_VOLT`, `OL_ANGLE`, `OL_DPP` chưa
  được áp dụng.
- Hall được chọn từ `HALL_SENSORS` trong `src-gen/drive_parameters.h`.
- Các giai đoạn Open-loop, Hall voltage, Hall current PI và speed PI là các
  bước bring-up/debug, không phải 4 mode đã được xác nhận an toàn trên motor.
- Xem [Codex.md](Codex.md) để biết vị trí code, thứ tự triển khai FOC và các
  bước kiểm tra trước khi cấp nguồn cho motor.

## 2. Luồng cấu hình và sinh code

```text
Config.gpio + myBoard.gpio_board
clock_cfg.Sr5e1_clk
config.Motorcontrol
config.Sr5e1_can + config1.Sr5e1_can
application.appcfg
        |
        v
StellarStudio generator / SR5E1_MCTK-1.3.0
        |
        v
src-gen/*.h, src-gen/*.c
        |
        v
Makefile -> compiler/linker -> build/.../Debug/
```

Các input cần sửa:

- Chân GPIO: `Config.gpio`
- Board profile: `myBoard.gpio_board`
- Clock: `clock_cfg.Sr5e1_clk`
- Motor/PWM/dòng: `config.Motorcontrol`
- CAN: `config.Sr5e1_can`, `config1.Sr5e1_can`
- Danh sách component: `application.appcfg`

Sau thay đổi, mở component editor trong StellarStudio, lưu cấu hình, chạy
Generate Code/Refresh, rồi kiểm tra `src-gen/`. Không sửa `src-gen/` làm nguồn
sự thật vì lần generate sau sẽ ghi đè.

## 3. Sơ đồ chân quan trọng

Không dùng mapping sai `TIM1_CH3 = PC15`. Mapping đã đối chiếu trong project là:

| Chân MCU | Tín hiệu board | AF | Chức năng |
|---|---|---|---|
| PC15 / E7_67 | `HIN1` | TIM1_CH1, AF4 | Pha W high side |
| PC14 / E7_66 | `LIN1` | TIM1_CH1N, AF4 | Pha W low side |
| PD1 / E7_70 | `HIN2` | TIM1_CH2, AF4 | Pha V high side |
| PD0 / E7_69 | `LIN2` | TIM1_CH2N, AF4 | Pha V low side |
| PD4 / E7_60 | `HIN3` | TIM1_CH3, AF4 | Pha U high side |
| PD3 / E7_59 | `LIN3` | TIM1_CH3N, AF4 | Pha U low side |

### PWM và feedback dòng

| Chức năng | Ngoại vi | Chân MCU | Nhãn board |
|---|---|---|---|
| Pha U high/low | TIM1_CH3/CH3N | PD4/PD3 | HIN3/LIN3 |
| Pha V high/low | TIM1_CH2/CH2N | PD1/PD0 | HIN2/LIN2 |
| Pha W high/low | TIM1_CH1/CH1N | PC15/PC14 | HIN1/LIN1 |
| Dòng pha U | ADC3 channel 1 | PB12 | CIU |
| Dòng pha V | ADC2 channel 1 | PB7 | CIV |
| Dòng pha W | ADC1 channel 1 | PB2 | CIW |

### Hall, cảm biến và driver

| Chức năng | Ngoại vi | Chân MCU | Nhãn board |
|---|---|---|---|
| Hall 1 | TIM2_CH1 | PH2 | MC_HALL_CH1 |
| Hall 2 | TIM2_CH2 | PH3 | MC_HALL_CH2 |
| Hall 3 | TIM2_CH3 | PH4 | MC_HALL_CH3 |
| VBUS | ADC5 channel 1 | PC9 | VBUS |
| Tốc độ/ga | ADC5 channel 2 | PC10 | VSPEED |
| NTC | ADC5 channel 3 | PC11 | NTC |
| Nhiệt độ | ADC4 channel 1 | PC3 | TEMP |
| Gate driver enable | GPIO output | PB1 | STDRIVE_EN |
| Gate driver fault | GPIO/TIM1_BKIN | PD2 / E7_71 | STDRIVE_FAULT |
| Input S1/S2/S3 | GPIO input | PA13/PA14/PA15 | S1/S2/S3 |
| LED 1/2/3 | GPIO output | PE3/PE2/PE1 | USER_LED_1/2/3 |

### UART, CAN và brake

| Giao tiếp | TX | RX |
|---|---|---|
| CAN1 | PA5 / E7_2 | PA4 / E7_1 |
| CAN2 | PA11 / E7_13 | PA12 / E7_14 |
| CAN3 | PH10 / E7_158 | PH11 / E7_159 |
| CAN4 | PH12 / E7_160 | PH13 / E7_161 |
| UART1 | PA1 / E7_4 | PA0 / E7_3 |

- `RBK_HIN`: PF2 / E7_101 / TIM4_CH1
- `RBK_LIN`: PF3 / E7_102 / TIM4_CH2

## 4. Cấu hình PWM và motor

Nguồn: `config.Motorcontrol` và các header sinh trong `src-gen/`.

- PWM: TIM1
- Tần số PWM: `10 kHz`
- Trigger ADC: TIM1 TRGO, channel 4 dùng timing lấy dòng
- Polarization high side: Active Low
- Polarization low side: Active Low
- Hardware dead time: `2000 ns`
- Software low-side dead time: `700 ns`
- Over-current protection: bật
- TIM1 break input polarity: high
- TIM1 break filter: `FDIV1`
- TIM1 break2: tắt
- Hall timer: TIM2
- Hall displacement: `120°`
- Hall FIFO averaging: `18`
- UART motor-control: UART1, `38400 baud`
- Cảm biến tốc độ chính: Hall
- Cặp cực motor: `28`
- Bus DC danh định: `72 V`
- Three-shunt: `0.0005 ohm`, gain `20`
- VBUS: ADC5 channel 1, PC9, divider `1220/10`
- Nhiệt độ và over-temperature firmware: bật

## 5. CAN

Nguồn cấu hình:

- `config.Sr5e1_can`
- `config1.Sr5e1_can`
- `application.appcfg`

Thông số đã sinh:

- CAN clock: `40 MHz`
- RX buffer: `4`
- TX buffer: `4`
- RX FIFO0/FIFO1: `4`
- TX FIFO queue: `4`
- TX event FIFO: `4`
- Classic filter ID: `2032`
- CAN interrupts: bật
- Receive callback: `canrx_receive_cb`
- Error callback: `canrx_error_cb`

Chân CAN hiện chỉ là cấu hình; CAN chưa chắc hoạt động nếu application chưa
khởi tạo/peripheral và interrupt tương ứng.

## 6. File và thư mục dự án

### Giữ lại

| Đường dẫn | Mục đích |
|---|---|
| `Makefile` | Entry build chính |
| `.project`, `.cproject`, `.settings/` | Metadata Eclipse/StellarStudio |
| `application.appcfg` | Danh sách component sinh code |
| `Config.gpio` | Sơ đồ chân/peripheral |
| `myBoard.gpio_board` | Board profile |
| `clock_cfg.Sr5e1_clk` | Clock tree |
| `config.Motorcontrol` | Motor, PWM, Hall, dòng, bảo vệ |
| `config.Sr5e1_can`, `config1.Sr5e1_can` | Cấu hình CAN |
| `src/` | Source application |
| `include/` | Header application |
| `src-gen/` | Đầu ra sinh code |
| `.sdk/` | Stellar SDK và MCTK local |
| `debug/sr5e1_project.cfg` | Cấu hình OpenOCD |
| `SR5E1_MC_FLASH.cmm` | Script flash TRACE32 |
| `SR5E1_window.cmm` | Cấu hình debug watch TRACE32 |

### Có thể xóa khi làm sạch

| Đường dẫn | Khuyến nghị |
|---|---|
| `build/` | Xóa tự do; build tạo lại |
| `c_objs.lst` | Xóa tự do |
| `debug/*.log` | Xóa nếu không cần lịch sử |
| `Release/` | Không dùng làm project build chính; có thể loại bỏ |

Trước khi xóa `src-gen/`, kiểm tra rằng project có thể regenerate lại. Không
chỉnh tay `src-gen/evbe7000s.h`, `mc_config.h` hoặc `parameters_conversion.h`.

## 7. Quy trình build an toàn

1. Sửa file input của generator.
2. Mở `application.appcfg`, lưu component cần đổi.
3. Chạy Generate Code/Refresh.
4. Kiểm tra:
   - `src-gen/evbe7000s.h`
   - `src-gen/mc_config.h`
   - `src-gen/parameters_conversion.h`
5. Build từ StellarStudio hoặc:

```text
make purge
make all
```

ELF hiện tại:

```text
build/sr5e1/evbe7000s/core1/Debug/
  SR5E1_MCTK_TTM01V1_Demo.elf
```

### Ký hiệu trạng thái

- **ĐẠT**: có bằng chứng đo hoặc chạy được.
- **KHÔNG ĐẠT**: không đủ bằng chứng hoặc log còn mâu thuẫn.
- **CHƯA XÁC MINH**: chưa có datasheet, đo thực tế hoặc log đủ tin cậy.

## 7.1 Test LED và nạp Flash

Ngày test: `2026-11-18`

Chế độ `LED_ONLY_TEST` được bật trong `include/main.h`:

- LED1 = PE3
- LED2 = PE2
- LED3 = PE1
- Không chạy `STDRIVE601_Init()`
- Không chạy motor-control path
- Không mở ngắt MotorControl trong nhánh LED-only

Build thử nghiệm:

- Cấu hình: `Debug (nvm)`, core `core1`
- ELF:
  `build/sr5e1/evbe7000s/core1/Debug/SR5E1_MCTK_TTM01V1_Demo.elf`
- SHA256 ELF cuối cùng:
  `AE1BF70DC741A4870BBFC99256D85A226B4A114FFADADAD739AED3B1BE3AB4B7`

### Kết quả đã xác nhận

- StellarLINK/JTAG phát hiện SR5E1 và Cortex-M7.
- ELF đã ghi và verify trong Flash.
- Vector Flash tại `0x08000000`:
  `0x24004798`
- Reset vector tại `0x08000004`:
  `0x08000401`
- `GPIOE->MODER` xác nhận PE3 ở chế độ output.
- `GPIOE->ODR` đổi được:
  - `0x00000008`: PE3 HIGH
  - `0x00000000`: PE3 LOW
- LED1 đã được quan sát nhấp nháy sau bootstrap thủ công.

## 7.2 Test UART1, Hall và ba LED

`LED_ONLY_TEST` trong `include/main.h` bật luồng bring-up polling tại đầu
`main()`:

- UART1 dùng `USART_SPEED` (`38400 baud`), 8-N-1, không flow control.
- Firmware gửi banner khi khởi động và echo từng byte nhận được.
- Hall được đọc trực tiếp theo thứ tự `H3:H2:H1`; firmware chỉ gửi dòng mới
  qua UART khi trạng thái Hall thay đổi.
- USER_LED_1, USER_LED_2, USER_LED_3 sáng lần lượt rồi tắt; trong thời gian
  chờ, firmware tiếp tục poll UART và Hall.
- Nhánh này không mở ngắt OSAL/MotorControl, không gọi
  `STDRIVE601_Init()` hoặc `MX_MotorControl_Init()`, và không chạy motor.

Đây là chức năng trong source đã được nhập từ `testled`; cần build và kiểm tra
trên board trước khi đánh dấu kết quả UART/Hall/ba LED là đã xác nhận.

## 7.3 Vấn đề boot-handoff BAF sang Flash

Trạng thái đang giữ:

```text
PC   = 0x1ffxxxxx
SP   = 0x200xxxxx
VTOR = 0x1ff00100
```

Đây là trạng thái trong BAF (Boot Assist Flash, OTP), không phải code ứng
dụng. BAF giữ chương trình boot của ST.

Một số PC đã quan sát:

- `0x1FF00150`
- `0x1FF0091E`
- `0x1FF0122A`
- `0x1FF0149E`
- `0x1FF01518`

Ngày `2026-11-18` đã đọc trực tiếp hai thanh ghi cấu hình Core 1 bằng
OpenOCD/Telnet khi CPU đứng trong BAF:

| Địa chỉ | Giá trị đọc | Ý nghĩa theo `SR5E1_RCC.h` |
|---|---|---|
| `0x4400030C` | `0x00000000` | `C1_APPLI_ADDR_REG`: địa chỉ application chưa được đăng ký |
| `0x44000304` | `0x1FF00100` | `C1_VTOR_INIT_REG`: VTOR khởi tạo hiện trỏ tới BAF |

Kết quả này xác nhận BAF đang cấu hình Core 1 khởi tạo từ vùng BAF, không
đặt VTOR ứng dụng. Do đó, sau reset độc lập, CPU có thể dừng/tiếp tục trong
BAF thay vì đọc vector tại `0x08000000`.

Không ghi giá trị vào `C1_APPLI_ADDR_REG` hoặc vùng DCF trong giai đoạn này.
Theo hướng dẫn người dùng và RM0483:

- thao tác ghi DCF không đảo ngược;
- RM giới hạn tối đa 4 lần cập nhật;
- chỉ dùng quy trình chính thức của ST hoặc hướng dẫn ST được xác nhận.

Tình trạng cần hỏi ST kèm:

- PC hiện đứng tại `0x1ffxxxxx`;
- `C1_APPLI_ADDR_REG = 0`;
- `C1_VTOR_INIT_REG = 0x1FF00100`;
- vector application tại `0x08000000` hợp lệ;
- Reset_Handler tại `0x08000401`;
- LED-only chạy sau khi GDB đặt thủ công SP/PC/VTOR.

Chuỗi GDB thủ công đã tái hiện LED:

```gdb
set $sp = 0x24004798
set $pc = 0x08000401
set *(unsigned int *)0xE000ED08 = 0x08000000
continue
```

Sau đó bấm F8/Resume khi cần. `0x08000401` là địa chỉ Thumb của
`Reset_Handler`; địa chỉ instruction thực là `0x08000400`.

Giải thích:

- `$sp = 0x24004798`: Main Stack Pointer ban đầu của ứng dụng.
- `$pc = 0x08000401`: bắt đầu tại Reset_Handler của Flash.
- VTOR `0x08000000`: Cortex-M dùng bảng vector của application.
- Cảnh báo GDB `Invalid state, unable to determine sp alias, assuming msp`
  xuất hiện do đang đứng trong trạng thái ngắt BAF/ROM, không chứng minh ELF
  hỏng.

Đây chỉ là workaround debug. Chưa chứng minh boot tự động sau power-cycle hay
external reset.

### Điều kiện pass standalone

Board không cần Python, GDB, breakpoint hay đặt PC/SP/VTOR thủ công:

1. Nạp ELF và verify.
2. Ngắt StellarLINK.
3. Cắt rồi cấp lại nguồn logic.
4. LED1 tự nhấp sau 2-3 giây.

Chỉ đạt khi cả ba điều kiện đúng:

- Không còn Debug/Breakpoint can thiệp.
- CPU tự nhảy từ BAF sang Flash.
- LED1 chạy từ vòng `main()`.

## 8. Cấu hình Debug hiện tại

File:

```text
debug/sr5e1_project.cfg
```

Nội dung hiện tại:

```tcl
source [find board/sr5e1_evb.cfg]
adapter speed 200
adapter srst delay 100

sr5e1.armv7.cpu1 configure -event gdb-attach {
    reset halt
    reg sp 0x24004798
    reg pc 0x08000401
    mww 0xE000ED08 0x08000000
    resume
}
```

Config options hợp lệ cho OpenOCD:

```text
-s "C:\StellarStudio-9.2\openocd\scripts"
-f "C:\StellarStudio-9.2\workspace\SR5E1_MCTK_v0\debug\sr5e1_project.cfg"
```

Trước debug:

- Tắt ARM semihosting.
- Tắt reset thứ hai.
- Không set breakpoint tại `main`.
- Bật `Continue`.
- Load ELF project binary.
- Load symbols từ cùng ELF.
- GDB executable là `arm-none-eabi-gdb.exe`.
- GDB remote: `localhost:3333`.

## 9. Thay đổi SDK local đã thực hiện

Bản `.sdk/SR5E1_MCTK-1.3.0` local đã chỉnh để loại warning, không thay đổi
ý nghĩa điều khiển:

- tham số Hall unused được đánh dấu unused;
- khai báo mảng `MCboot` được thống nhất;
- callback MCP được wrapper đúng kiểu;
- nhánh FDCAN Rx Buffer không hỗ trợ trả parameter error thay vì warning.

Đây là sửa local vendor-tree. Khi thay SDK, giữ patch hoặc ghi lại thay đổi.

## 10. Bảo vệ phần cứng

Không cấp bus HV cho đến khi đạt:

- TC-00: đo tĩnh board, nguồn logic.
- TC-01: bảo vệ và STDRIVE601.
- TC-02: ADC offset, Hall, VBUS, nhiệt và UART không công suất.

Không tắt bảo vệ OV/UV/OC để chạy. Không giảm tốc mạnh trước khi có đường
xả tái sinh. Không chạy motor trong chế độ LED-only.

## 11. Cấu trúc thư mục

```text
SR5E1_MCTK_v0/
|-- Makefile
|-- application.appcfg
|-- myBoard.gpio_board
|-- Config.gpio
|-- clock_cfg.Sr5e1_clk
|-- config.Motorcontrol
|-- config.Sr5e1_can
|-- config1.Sr5e1_can
|-- include/
|-- src/
|-- src-gen/
|-- debug/
|   `-- sr5e1_project.cfg
|-- build/
|-- .sdk/
|   |-- StellarESDK-1.7.0/
|   `-- SR5E1_MCTK-1.3.0/
|-- SR5E1_MC_FLASH.cmm
`-- SR5E1_window.cmm
```

## 12. Luồng dữ liệu cấu hình

```text
Config.gpio + myBoard.gpio_board
clock_cfg.Sr5e1_clk
config.Motorcontrol
config.Sr5e1_can + config1.Sr5e1_can
                |
                v
        StellarStudio Generator
                |
                v
             src-gen/
                |
                v
Makefile -> SDK/MCTK -> linker -> build/.../Debug/
```

## 13. Hướng dẫn API MCTK

Ưu tiên API high-level trong `include/MotorControl/mc_api.h` cho application:

| API | Chức năng |
|---|---|
| `MX_MotorControl_Init()` | Khởi tạo motor-control |
| `MC_RunMotorControlTasks()` | Chạy task MotorControl |
| `MC_Scheduler()` | Chạy scheduler nếu application dùng entry point trực tiếp |
| `MC_StartMotor1()` | Khởi động motor |
| `MC_StopMotor1()` | Dừng motor |
| `MC_AcknowledgeFaultMotor1()` | Xác nhận và xóa fault |
| `MC_GetIqdMotor1()` | Đọc Id/Iq |
| `MC_GetIalphabetaMotor1()` | Đọc Ialpha/Ibeta |
| `MC_SetCurrentReferenceMotor1()` | Đặt Iq/Id reference |
| `MC_ProgramCurrentRampsMotor1()` | Ramp tham chiếu dòng |
| `MC_GetElAngledppMotor1()` | Góc điện dạng DPP |
| `MC_GetAvrgMecSpeedUnitMotor1()` | Tốc độ cơ từ sensor |

Dùng `MCI_*` khi cần handle motor-interface trực tiếp. Dùng `ICS_*` chỉ cho
khởi tạo/PWM/current-sensing ở tầng thấp. Không bịa API hoặc tên type; tìm
prototype trong header MCTK rồi mới dùng.

## 14. Lộ trình phát triển FOC

Điểm bắt đầu là tạo một mode thử motor riêng, giữ `LED_ONLY_TEST` làm mode an
toàn mặc định, rồi dùng API MCTK trong `src/main.c` để start/stop và chạy
MotorControl tasks. Không gọi các helper đóng cắt pha Hall 6-step để chạy FOC.

Trước lần chạy đầu, phải chạy Generate Code trong StellarStudio vì cấu hình
nguồn của chân STDRIVE_FAULT và số cặp cực mới thay đổi. Không sửa tay
`src-gen/`. Cấu hình `OPEN_LOOP_FOC TRUE` trong MCTK hiện ghi đè đầu ra PI dòng
bằng điện áp q-axis đặt trực tiếp, nên cần kiểm tra current sensing và bảo vệ
quá dòng phần cứng trước khi quay motor.

Chi tiết code, cách build/nạp, từng giai đoạn thử và tiêu chí pass/fail nằm
trong [Codex.md](Codex.md).
