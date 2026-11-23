# Codex.md — Hướng dẫn tiếp tục phát triển FOC

Tài liệu này ghi lại trạng thái project, nơi sửa code và quy trình phát triển
FOC an toàn theo từng bước. Đây là hướng dẫn làm việc; nó không thay thế datasheet
motor, tài liệu power stage hoặc phép đo trên board.

## 1. Trạng thái hiện tại

- MCU/board profile: SR5E1E7 / `evbe7000s`, core1.
- SDK/MCTK: StellarESDK 1.7.0 / SR5E1_MCTK 1.3.0.
- `LED_ONLY_TEST` đang bật trong `include/main.h`.
- `OPEN_LOOP_FOC` đang là `FALSE`; `OL_VOLT`, `OL_ANGLE`, `OL_DPP` vì vậy
  không hoạt động.
- `MCTK_MC_MODE` đang comment. `MC_RunMotorControlTasks()` ở vòng lặp main
  chỉ được gọi khi bật mode này.
- Lời gọi `MC_StartMotor1()` hiện gắn với `TEST_ADC_REG_CONV`; test đó chỉ được
  định nghĩa khi `EXTERNAL_THROTTLE` bật, và macro này hiện đang tắt.
- Vì `LED_ONLY_TEST` vào vòng lặp vô hạn trước `STDRIVE601_Init()` và
  `MX_MotorControl_Init()`, firmware hiện tại chỉ chạy UART/Hall/LED; nó không
  khởi tạo hoặc chạy motor.
- Cấu hình nguồn trên nhánh `main` đặt `pole_pairs="28"` và `STDRIVE_FAULT`
  tại PD2/E7_71. Các header trong `src-gen/` vẫn có thể phản ánh cấu hình cũ
  cho đến khi chạy StellarStudio Generate Code.

Chỉ xem `src-gen/` để đối chiếu kết quả sinh. Không sửa tay file trong thư mục
đó. Sửa `config.Motorcontrol`, `Config.gpio` hoặc input generator tương ứng,
sau đó chạy Generate Code/Refresh trong StellarStudio.

## 2. Sửa code ở đâu

| File | Vai trò |
|---|---|
| `src/main.c` | Entry point, chọn mode ứng dụng, init board/MCTK, start/stop và vòng lặp tasks |
| `include/main.h` | Compile-time mode và các macro thử nghiệm như `LED_ONLY_TEST`, `OPEN_LOOP_FOC`, `OL_*` |
| `config.Motorcontrol` | Motor, sensor, PWM, sensing, limits và startup parameters đầu vào cho MCTK generator |
| `Config.gpio` | Pinmux GPIO, UART, Hall, phase PWM và fault input |
| `application.appcfg` | Danh sách component được generator đưa vào application |
| `src-gen/` | Output generator; chỉ đọc/kiểm tra, không sửa tay |
| `.sdk/SR5E1_MCTK-1.3.0/include/MotorControl/mc_api.h` | Prototype API ứng dụng MCTK |
| `.sdk/SR5E1_MCTK-1.3.0/src/MotorControl/mc_tasks.c` | Luồng FOC và vị trí Open-loop voltage được áp vào |
| `readme.md` | Tóm tắt project, pinout và trạng thái bring-up |

Trong `src/main.c` còn có các helper tự đóng cắt pha theo Hall 6-step. Không
dùng `HW_Set_Phase_*` hoặc `SixStep_Commutate()` làm đường phát PWM cho FOC.
FOC phải đi qua ADC interrupt, current feedback, Park/Clarke, voltage vector
và PWM của MCTK.

## 3. Các điểm phải chốt trước khi cho motor quay

1. Xác nhận motor thật sự có 28 **cặp cực** (56 cực), không phải 28 cực. Input
   `config.Motorcontrol` hiện dùng 28; datasheet/nameplate và kết quả đo phải
   khớp.
2. Xác nhận điện áp/dòng liên tục và dòng đỉnh của motor, inverter, MOSFET,
   gate driver, shunt, connector và nguồn. `72 V`, `150 A` trong file config
   là cấu hình hiện tại, không tự chứng minh toàn bộ stage chịu được các mức đó.
3. Xác nhận topology current sensing thật sự tương thích. Project XML khai báo
   `Three Shunt Resistor`, trong khi tài liệu UM3465 của bộ SR5E1-MCTK-01 mô tả
   topology 2-shunt/2-ICS cho thư viện ở bộ kit đó. Board và MCTK package trong
   project này khác bộ kit tham khảo; cần xác minh đúng tài liệu/version và
   mapping ADC trước khi có điện áp motor.
4. Đo/kiểm tra current-sense offset, polarity, scale, ADC sample timing, VBUS
   scaling, temperature input, STDRIVE_FAULT và TIM1 break path.
5. Xác minh dead-time và phase order trên phần cứng với nguồn DC motor tắt;
   không suy ra dead-time thực từ một giá trị trong XML.
6. Chốt phương án ngắt nguồn, cầu chì/giới hạn dòng, giữ cơ khí motor và xử lý
   năng lượng tái sinh. Bảo vệ OV/UV/OC phải hoạt động; không bypass để làm motor
   quay.

UM3465 là tài liệu cho bộ kit EVBE3000D + X-NUCLEO-IHM08M1: tài liệu nêu motor
24 V/64 W và board 10–48 V, 15 Arms. Đây không phải giới hạn được áp cho stage
72 V của project này; chỉ dùng làm tài liệu tham khảo về workflow MCTK. [ST UM3465](https://www.st.com/resource/en/user_manual/um3465-sr5e1mctk01-motor-control-toolkit-stmicroelectronics.pdf)

## 4. Open-loop FOC trong source hiện làm gì

Trong `.sdk/SR5E1_MCTK-1.3.0/src/MotorControl/mc_tasks.c`, khi
`OPEN_LOOP_FOC == TRUE`, vòng FOC đặt `Vd = 0`, ép `Vq = OL_VOLT`, đặt góc điện
từ `OL_ANGLE` rồi cộng `OL_DPP` ở mỗi lần chạy high-frequency task.

Do đoạn này chạy sau các PI dòng, nó thay thế đầu ra PI. Vì vậy đây là **điều
khiển điện áp vòng hở**, không phải chế độ giới hạn dòng bằng Iq PI. Các giá
trị hiện có (`OL_VOLT=2600`, `OL_ANGLE=800`, `OL_DPP=78`) là giá trị thử cũ,
không mặc nhiên là giá trị an toàn cho motor/stage hiện tại. Không bật chúng
chỉ bằng cách đổi `FALSE` thành `TRUE`.

Với `MC_StartMotor1()`, MCTK yêu cầu trước đó phải đặt một lệnh speed ramp,
torque ramp hoặc current reference. Hàm start chỉ gửi yêu cầu chuyển state
machine; nó không chờ motor vào RUN. Ứng dụng phải kiểm tra kết quả lệnh/state
và tiếp tục chạy `MC_RunMotorControlTasks()`/scheduler. Prototype và điều kiện
này được ghi trong `mc_api.h`/`mc_api.c` tại SDK cục bộ.

## 5. Thiết kế mode ứng dụng trước khi code motor

Đổi macro LED-only luôn bật thành lựa chọn mode tường minh, loại trừ lẫn nhau:

```text
LED_ONLY_TEST             default ON / không có motor
OPEN_LOOP_FOC_TEST        default OFF / chỉ bật trong build bench có kiểm soát
```

Không để cả hai mode cùng bật. Khi bật Open-loop test:

1. Khởi tạo clock/GPIO/board/IRQ/OSAL.
2. Init gate driver và kiểm tra trạng thái fault/enable.
3. Init MCTK, ADC current feedback, timer và interrupt theo cấu hình đã sinh.
4. Giữ trạng thái disarmed sau reset; chỉ nhận lệnh start sau khi interlock và
   các giá trị VBUS/current/fault hợp lệ.
5. Gửi lệnh MCTK cần thiết trước `MC_StartMotor1()`; kiểm tra giá trị trả về và
   state machine. Không gọi start tự động khi boot.
6. Trong loop, chạy tasks MCTK và đọc telemetry ở tần số thấp. Lệnh stop hoặc
   bất kỳ lỗi nào phải gọi stop và đưa output về trạng thái disable.

Pseudo-code để định hướng, không phải code có thể copy chạy ngay:

```c
MX_MotorControl_Init();
/* Init/check driver, IRQ, ADC feedback, bus voltage và safety interlocks. */

while (true)
{
    MC_RunMotorControlTasks();

    if (start_command && all_interlocks_ok)
    {
        /* Đặt speed/torque/current command đã được tính và review trước. */
        /* Kiểm tra kết quả lệnh và state; sau đó mới MC_StartMotor1(). */
    }

    if (stop_command || fault_detected)
    {
        MC_StopMotor1();
    }
}
```

Không dùng polling UART test trong cùng lúc với UART1 của MCTK Live Monitor.
`config.Motorcontrol` chọn UART1 cho serial interface; chọn một chủ sở hữu cho
UART ở mỗi build. Có thể dùng Live Monitor theo quy trình UM3465 hoặc chuyển
telemetry thử nghiệm sang giao tiếp khác.

## 6. Trình tự build và nạp

Sau khi sửa input generator:

1. Mở `application.appcfg` và component editor trong StellarStudio.
2. Generate Code/Refresh.
3. Đối chiếu các output liên quan với input, tối thiểu:
   - `src-gen/pmsm_motor_parameters.h`: `POLE_PAIR_NUM`;
   - `src-gen/evbe7000s.h`: `STDRIVE_FAULT` và pin phase/UART/Hall;
   - `src-gen/drive_parameters.h`: drive/current/sensor constants;
   - `src-gen/mc_config.h`: PWM/ADC/timer assignment.
4. Build đúng `Debug`, `nvm`, `core1` và đúng ELF.
5. Nạp/verify ELF rồi kiểm tra debugger có đang dùng workaround BAF-to-Flash
   không. Thành công khi debugger bootstrap không đồng nghĩa board tự boot sau
   power-cycle.

Không chép các file `.o`, `.lst`, `.elf`, `.hex` hoặc nội dung `build/` từ một
branch/build cũ để thay cho bước generate/build.

## 7. Kế hoạch thử theo các cổng pass/fail

Không nhảy qua cổng khi chưa lưu kết quả và log. Không cấp bus 72 V theo thói
quen; điện áp/dòng thử phải nằm trong giới hạn nhỏ nhất đã xác nhận giữa motor,
inverter, gate driver, sensor và nguồn.

| Cổng | Thử nghiệm | Điều kiện pass |
|---|---|---|
| T0 | Build `LED_ONLY_TEST`, UART echo, raw Hall, LED; không chạy motor | Build/nạp đúng ELF; không vào driver/MCTK motor path |
| T1 | Generate lại sau khi đổi `Config.gpio` và `config.Motorcontrol` | Header sinh khớp pin PD2, pole pairs và channel mapping |
| T2 | Board logic-only: đọc VBUS, nhiệt, Hall và current offset | Đọc ổn định, scale/polarity phù hợp phép đo ngoài; fault input đúng |
| T3 | Bridge chưa cấp DC motor: kiểm tra gate enable, break/fault, PWM/dead-time bằng thiết bị đo | Output disable khi disarmed; fault tắt PWM; dead-time/phase order đo được |
| T4 | Open-loop voltage ở nguồn thấp, giới hạn năng lượng, motor không tải | Motor quay ổn định ở hướng dự kiến; dòng nằm trong ngưỡng đã duyệt; stop/fault tắt PWM |
| T5 | Chuyển sang Hall feedback/current PI | TIM2 Hall sequence, góc/phase alignment và chiều quay khớp; current PI ổn định |
| T6 | Thêm speed loop/tăng tốc | Speed tracking, ramp, nhiệt và OV/UV/OC ổn định qua nhiều lần start/stop |

Open-loop voltage không tự giới hạn dòng pha. Nguồn DC có giới hạn dòng đầu
vào không thay thế over-current protection phía pha. Phải xác minh phần cứng OC
thực sự có đường trip độc lập trước T4; giữ khả năng ngắt nguồn và xử lý regen.

Mỗi lượt test ghi: commit/config, ELF hash, bus voltage, peak phase current,
speed, Hall sequence, chiều quay, fault flags, điều kiện stop và ảnh waveform
PWM/dead-time. Nếu thiếu phép đo thì đánh dấu **CHƯA XÁC MINH**, không suy ra
pass từ việc compile hoặc nạp Flash thành công.

## 8. Tiêu chí chuyển khỏi Open-loop

Chỉ chuyển `OPEN_LOOP_FOC` về `FALSE` và thử Hall/current closed-loop sau khi:

- current sensing và phase mapping được đo xác nhận;
- số cặp cực và motor parameters đúng với motor thực;
- Hall state sequence theo một vòng quay không có trạng thái lỗi và map được
  theo thứ tự pha;
- start, stop, break/fault và bus protection đã pass ở T0–T4;
- có telemetry đủ để bắt lỗi current/speed ngay trong lượt đầu.

Sau đó mới tăng dần current PI, rồi speed PI. Không bật speed control đồng thời
với lần thử đầu của angle/phase mapping.

## 9. Quy tắc cho các lượt Codex tiếp theo

- Sửa generator input; không edit `src-gen/`.
- Tôn trọng thay đổi người dùng chưa commit trong workspace.
- Không bật motor tự động khi reset và không tắt/bỏ qua protection.
- Không sửa source vendor trong `.sdk/` nếu có thể giải quyết ở application hoặc
  generator config.
- Khi báo trạng thái, tách rõ **đã compile**, **đã nạp**, **đã quay motor** và
  **đã pass độc lập**; đây là các mức bằng chứng khác nhau.
