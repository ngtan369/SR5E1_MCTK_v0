# Codex.md — Hướng dẫn tiếp tục phát triển FOC

Tài liệu này ghi lại trạng thái project, nơi sửa code và quy trình phát triển
FOC an toàn theo từng bước. Đây là hướng dẫn làm việc; nó không thay thế datasheet
motor, tài liệu power stage hoặc phép đo trên board.

## 1. Trạng thái hiện tại

- MCU/board profile: SR5E1E7 / `evbe7000s`, core1.
- SDK/MCTK: StellarESDK 1.7.0 / SR5E1_MCTK 1.3.0.
- Tài liệu này mô tả branch `openloop`. `OPEN_LOOP_FOC_TEST` và
  `OPEN_LOOP_FOC` đang bật; `LED_ONLY_TEST` đã tắt.
- Gửi `s` qua UART1 để yêu cầu start, `x` để stop, `?` để xem lệnh. Firmware
  không tự start motor khi reset.
- Khi state machine ở `RUN`, UART1 xuất mỗi 100 ms:
  `FOC angle, Hall angle, delta (DPP), delta (deg)`.
- Ba LED vẫn chớp tuần tự, không blocking vòng lặp.
- UART1 được dùng làm text console trong mode này; MCTK Live Monitor qua UART1
  bị tắt để tránh trộn protocol với dòng text. `MC_RunMotorControlTasks()` chạy
  từ OSAL tick hook.
- `OL_VOLT=2600`, `OL_ANGLE=800`, `OL_DPP=78` là giá trị đang có trong header,
  chưa được xác nhận an toàn cho motor/power stage.
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

## 5. Cách chạy Open-loop angle monitor

Trong `include/main.h`, mode Open-loop hiện được chọn rõ ràng:

```text
OPEN_LOOP_FOC_TEST        enabled
OPEN_LOOP_FOC             TRUE
```

Sau khi build/nạp firmware, mở terminal UART1 ở 38400 baud, 8-N-1. UART1 được
MCTK khởi tạo theo pinmux generator; ứng dụng tắt ngắt serial protocol và dùng
polling text console. Gửi `s` để start sau khi motor/bridge đã được kiểm tra,
`x` để stop, `?` để in trợ giúp. Ba LED tiếp tục chớp trong khi chương trình
theo dõi góc.

Khi `MC_GetSTMStateMotor1() == RUN`, dòng telemetry xuất mỗi 100 ms:

```text
ANGLE_DPP foc,hall,delta,delta_deg: <foc>,<hall>,<delta>,<delta_deg>
```

`foc` đọc từ `FOCVars[M1].hElAngle`, tức góc điện mà FOC dùng cho biến đổi/
phát vector điện áp; `hall` là góc điện nội suy trong handle Hall MCTK.
`delta` là hiệu số wrap 16-bit theo miền DPP; `delta_deg` quy đổi gần đúng
theo 65536 DPP/vòng điện. Góc Hall này không phải góc rotor tuyệt đối liên tục
từ encoder; Hall có độ phân giải theo từng sector và MCTK nội suy giữa cạnh.

UART1 không thể đồng thời dùng làm MCTK Live Monitor text/binary trong mode này.
Để quay lại Live Monitor, tắt `OPEN_LOOP_FOC_TEST`, bật cấu hình serial phù hợp,
rồi generate/build theo input StellarStudio.

Lệnh `s` chỉ được nhận khi state là `IDLE` và không có current fault. Trước
`MC_StartMotor1()`, ứng dụng đặt torque ramp 0 làm lệnh MCI hợp lệ. Lệnh `x`
gọi `MC_StopMotor1()`; EXTI fault của STDRIVE601 gọi stop và kéo `STDRIVE_EN`
thấp ngay.

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
| T3 | VBUS ngắt: xác minh pinmux, trạng thái idle, EN và fault; kiểm tra gate outputs bị disable | Không có gate drive khi EN thấp; mapping logic/power được xác nhận theo schematic |
| T4 | Tháo motor; bus cách ly/current-limit ở mức đã duyệt; kiểm tra HIN/LIN rồi HVG/LVG và U/V/W | Đúng channel/polarity/frequency; không overlap gate; phase switching đúng và không fault bất thường |
| T5 | Tắt nguồn, xả bus, nối motor; thử Open-loop ở nguồn thấp, giới hạn năng lượng; gửi `s`, quan sát UART, gửi `x` | Motor quay ổn định; log góc hợp lý; dòng nằm trong ngưỡng đã duyệt; stop/fault tắt PWM |
| T6 | Chuyển sang Hall feedback/current PI | TIM2 Hall sequence, góc/phase alignment và chiều quay khớp; current PI ổn định |
| T7 | Thêm speed loop/tăng tốc | Speed tracking, ramp, nhiệt và OV/UV/OC ổn định qua nhiều lần start/stop |

Open-loop voltage không tự giới hạn dòng pha. Nguồn DC có giới hạn dòng đầu
vào không thay thế over-current protection phía pha. Phải xác minh phần cứng OC
thực sự có đường trip độc lập trước T5; giữ khả năng ngắt nguồn và xử lý regen.

### T4 chi tiết — đo U/V/W khi chưa nối motor

**Giữ motor tháo rời trong lần cấp VBUS đầu và khi đo switching U/V/W.** Chỉ
nối motor sau khi T3/T4 pass, đã tắt nguồn, xả tụ bus và xác nhận bus về mức an
toàn bằng đồng hồ. Tháo motor tránh quay ngoài ý muốn; bridge vẫn nguy hiểm khi
có điện vì MOSFET và các đầu phase đang switching.

1. Trước khi nối nguồn, xác định giới hạn điện áp thấp nhất của MOSFET/IGBT,
   tụ bus, connector, shunt, power stage và nguồn. `nominal_dc_voltage=72 V`,
   `max_rated_bus_voltage=100 V` trong XML là cấu hình MCTK, **không xác nhận**
   rating phần cứng. Nếu chưa có schematic/BOM hoặc rating rõ, dừng ở T3.
   Xác định terminal U/V/W trên schematic trước khi đo: `Config.gpio` đặt
   HIN1/LIN1 = E7_67/E7_66, HIN2/LIN2 = E7_70/E7_69,
   HIN3/LIN3 = E7_60/E7_59; đây là mapping gate input, không chứng minh
   HIN1 tương ứng phase U. XML MCTK còn khai báo U=TIM1_CH3, W=TIM1_CH1;
   đối chiếu pinmux sinh và schematic để xác nhận thứ tự phase.
2. Tháo U/V/W khỏi motor, bọc cách điện từng đầu. Không để pha chạm nhau,
   khung máy hoặc bàn đo. Xả bus qua mạch xả phù hợp; đo lại VBUS bằng DMM
   trước khi chạm/đổi probe.
3. Dùng nguồn DC cách ly, fuse và current limit đã duyệt cho board; bắt đầu ở
   mức bus thấp nhất mà power stage và bootstrap cho phép. Không đặt con số thử
   tùy ý từ thông số STDRIVE601; giới hạn dòng đầu vào không thay thế bảo vệ
   quá dòng pha.
4. Đo lần lượt `U–DC−`, `V–DC−`, `W–DC−`, sau đó `U–V`, `V–W`, `W–U`. Dùng
   probe vi sai có common-mode/rating phù hợp với VBUS và xung switching. Không
   kẹp ground của scope nối đất vào U/V/W, DC+, HVG hoặc switch-node; các kênh
   scope dùng chung ground có thể tự nối tắt các phase. Gắn probe khi bus đã
   xả, đặt scope từ xa rồi mới cấp nguồn.
5. Giữ gate driver disable bằng `MCTK_DEBUG_OFF_STDRIVE601`; cấp bus thấp đã
   được xác nhận và giới hạn dòng. MCTK config hiện đặt ngưỡng bus 20–100 V,
   nhưng đây không phải rating phần cứng. Nếu firmware báo undervoltage hoặc
   không nhận `s`, **không bypass bảo vệ**; kiểm tra dải bus đã sinh và rating
   board trước khi tiếp tục. Khi PWM chạy, đo HIN1–3/LIN1–3 so với ground logic.
   Xác nhận channel/polarity trước khi cho driver phát gate.
6. Tắt nguồn và xả bus; chỉ bỏ gate-disable sau khi logic inputs đạt. Cấp lại
   cùng bus đã giới hạn dòng, motor vẫn tháo rời, gửi `s`, quan sát HVG/LVG và
   phase. Có người đứng ở công tắc ngắt nguồn. Dừng ngay nếu dòng tăng bất
   thường, FAULT tác động, phase lệch mapping hoặc ringing vượt rating.
7. Lặp lại đủ ba nhánh. Ghi VBUS thực, dòng nguồn, PWM frequency, HIN/LIN
   polarity, dead-time, gate VGS, phase mapping, peak/overshoot và fault. Gửi
   `x`, ngắt nguồn, xả bus và xác minh hết điện trước khi đổi kết nối.

**Dạng sóng cần thấy:** cấu hình sinh hiện đặt `PWM_FREQUENCY=10000 Hz`, chu kỳ
carrier khoảng `100 µs`; xác nhận lại trong `src-gen/drive_parameters.h` sau
Generate Code. Ở mỗi `U/V/W–DC−`, node phase được PWM kéo gần DC− hoặc DC+
(có sụt áp/overshoot thực tế). Điện áp `U–V`, `V–W`, `W–U` có xung PWM nhiều
mức, xấp xỉ `−VBUS`, `0`, `+VBUS`; trung bình qua nhiều carrier tạo thành phần
cơ bản ba pha lệch nhau khoảng 120° điện, tổng gần 0. Dạng oscilloscope **không
phải sin sạch**. Đo line-line bằng probe vi sai. Không suy ra RMS/fundamental
volts chỉ từ `OL_VOLT=2600`; còn phụ thuộc scale MCTK, VBUS thực và modulation.
Để xem carrier, đặt timebase thấy vài chu kỳ 100 µs; để xem thành phần cơ bản,
mở timebase đủ dài cho một vòng điện. Dùng acquisition/sample rate đủ cao để
bắt dead-time và overshoot; tránh dây ground dài khi xem overshoot.

Đo `LVGx` so với source low-side và `HVGx` so với `OUTx` (switch node) bằng
probe vi sai. Khi bật, VGS đạt mức do nguồn gate-drive board quy định; khi tắt,
VGS gần 0. Trong dead-time cả hai VGS của cùng half-bridge phải thấp; không
được có khoảng hai transistor cùng dẫn. `EN` thấp phải ép HVG/LVG thấp.
Bootstrap UVLO có thể giữ HVG tắt khi BOOT–OUT chưa đủ, nên xác minh bootstrap
trước khi kết luận PWM lỗi. Các hành vi EN, active-low input, interlock/dead-time
và UVLO được mô tả trong [datasheet STDRIVE601](https://www.st.com/resource/en/datasheet/stdrive601.pdf).
Project XML hiện ghi `hw_inserted_dead_time=2000 ns` và
`sw_inserted_dead_time=700 ns`; coi đây là cấu hình đầu vào, không phải phép đo.
Datasheet ST nêu dead-time nội tại 200–400 ns (typ. 300 ns) trong điều kiện
đo riêng của họ. Đo dead-time thực trên LIN/HIN và VGS của board, rồi đối chiếu
với cấu hình TIM/generator; không cộng các con số này một cách máy móc.

**Pass T4:** ba phase đúng mapping theo schematic, carrier đúng cấu hình, không
có overlap gate, dead-time đo được, fault/EN ngắt switching, VBUS và overshoot
nằm trong giới hạn đã xác nhận, không có lỗi bất thường. Không đánh dấu pass
chỉ vì thấy xung hoặc vì motor đang tháo rời.

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
- start, stop, break/fault và bus protection đã pass ở T0–T5;
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
