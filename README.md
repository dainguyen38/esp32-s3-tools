# ESP32-S3 Tools

Bộ khung firmware ESP-IDF cho bo **Waveshare ESP32-S3-Touch-LCD-3.5**, dựa trên đề xuất kiến trúc **ESP32-ARCH-001 v0.2**. Firmware hiện chạy bài kiểm tra khởi động qua USB Serial/JTAG, ghi thông tin chip và đối chiếu dung lượng Flash/PSRAM với cấu hình của bo.

| Hạng mục | Cấu hình |
| --- | --- |
| SDK và target | ESP-IDF **v5.5.4**, `esp32s3` |
| Bộ nhớ | Flash 16 MiB, Octal PSRAM 8 MiB |
| Màn hình và cảm ứng | ST7796 320 × 480 qua SPI, FT6336 qua I²C. Đang chờ tích hợp. |
| Phân vùng | NVS, PHY init và một slot factory 2 MiB. OTA và filesystem sẽ cần layout riêng nếu được triển khai. |
| Biến thể PCB | V1/V2 cần xác nhận theo nhãn và schematic. Mẫu 3.5B dùng phần cứng màn hình khác. |

Thông số phần cứng và tài liệu theo revision có tại [trang sản phẩm Waveshare](https://docs.waveshare.com/ESP32-S3-Touch-LCD-3.5) và [ma trận bo mạch và bài kiểm tra](docs/hardware/bringup.md).

## Build trên macOS

Cần Git, Python 3, Xcode Command Line Tools, CMake và Ninja. Nếu thiếu công cụ dòng lệnh của Xcode, chạy `xcode-select --install`. Với Homebrew, cài công cụ build bằng `brew install cmake ninja`. Script của ESP-IDF sẽ cài toolchain ESP32-S3. Trên Linux hoặc Windows, xem [hướng dẫn cài đặt ESP-IDF v5.5.4](https://docs.espressif.com/projects/esp-idf/en/v5.5.4/esp32s3/get-started/index.html).

```bash
mkdir -p "$HOME/esp"
git clone --recursive --branch v5.5.4 \
  https://github.com/espressif/esp-idf.git "$HOME/esp/esp-idf-v5.5.4"
"$HOME/esp/esp-idf-v5.5.4/install.sh" esp32s3
. "$HOME/esp/esp-idf-v5.5.4/export.sh"
idf.py --version
```

Chạy `export.sh` trong mỗi terminal mới. Sau đó chạy các lệnh dưới đây từ thư mục gốc `esp32-s3-tools`. CMake đã đặt target `esp32s3` cho dự án.

```bash
idf.py build
idf.py size
idf.py partition-table
```

File ứng dụng được tạo tại `build/esp32_s3_tools.bin`. Lệnh `idf.py flash` dùng các offset do build sinh ra để nạp bootloader, bảng phân vùng và ứng dụng.

## Nạp firmware và kiểm tra khởi động

1. Xác nhận mẫu bo và revision theo [hướng dẫn bring-up](docs/hardware/bringup.md). Nếu cần giữ firmware nhà máy hoặc dữ liệu trong Flash, sao lưu trước khi nạp. Bản build này ghi bảng phân vùng phát triển của dự án.
2. Cắm bo qua cáp USB truyền dữ liệu. Chạy `ls /dev/cu.*` trên macOS trước và sau khi cắm để nhận diện cổng mới, thường có dạng `/dev/cu.usbmodem*`.
3. Thay cổng ví dụ bên dưới bằng cổng thực tế rồi nạp. Thoát monitor bằng `Ctrl+]`.

Nếu cần sao lưu Flash, đọc toàn bộ nội dung trước khi chạy lệnh nạp. Chọn tên tệp sao lưu riêng cho từng bo và giữ tệp ngoài repo.

```bash
python -m esptool --chip esp32s3 --port /dev/cu.usbmodemXXXX \
  read_flash 0 ALL "$HOME/esp/waveshare-35-backup.bin"
```

```bash
idf.py -p /dev/cu.usbmodemXXXX flash monitor
```

Log ứng dụng cần có phiên bản firmware/ESP-IDF, số lõi, revision chip và mã reset. Kết quả mong đợi là `PSRAM detected=yes, bytes=8388608`, `Physical flash bytes=16777216` và `BOOT_SMOKE_PASS`. Nếu thấy `BOOT_SMOKE_FAIL`, xem dòng lỗi trước đó để phân biệt lỗi đọc Flash với dung lượng sai hoặc PSRAM chưa khởi tạo. Nếu monitor mở sau khi log đã in, nhấn RESET để đọc lại từ đầu.

| Hiện tượng | Cách kiểm tra |
| --- | --- |
| `idf.py` không có trong PATH | Nạp lại `export.sh` của đúng SDK và kiểm tra `idf.py --version`. |
| Compiler báo thiếu `tools/cmake/cflags` | Trên macOS, đặt SDK và thư mục build dưới đường dẫn cục bộ như thư mục người dùng. ESP-IDF có thể gặp lỗi khi build dưới `/tmp`. Xem [Espressif #18751](https://github.com/espressif/esp-idf/issues/18751). |
| Cổng USB không xuất hiện | Kiểm tra cáp dữ liệu, cổng USB, nguồn và thử kết nối trực tiếp. |
| Có cổng nhưng không nạp được | Đóng monitor đang giữ cổng. Nếu cần vào Download Mode, giữ BOOT, nhấn rồi thả RESET, sau đó thả BOOT và dò lại cổng. |
| Nạp xong nhưng không có log | Dò lại cổng sau reset và kiểm tra `CONFIG_ESP_CONSOLE_USB_SERIAL_JTAG` trong `sdkconfig`. |
| Cần về firmware nhà máy | Dùng ảnh factory và offset đi kèm bản phát hành Waveshare tương ứng với đúng mẫu bo. Xem [hướng dẫn của hãng](https://github.com/waveshareteam/ESP32-S3-Touch-LCD-3.5#readme). |

## Cấu trúc và phát triển

```text
.
├── .github/workflows/firmware.yml
├── CMakeLists.txt
├── main/                         # app_main gọi app_core_start
├── components/
│   ├── app_core/                 # Điều phối startup và báo kết quả
│   └── bsp_waveshare_35/          # Chẩn đoán và cấu hình riêng theo bo
├── docs/hardware/bringup.md
├── partitions/factory.csv
└── sdkconfig.defaults*
```

- `main → app_core → bsp_waveshare_35 → ESP-IDF` là luồng phụ thuộc hiện tại. Khi thêm ngoại vi, BSP sở hữu pin, bus và nguồn. Driver thiết bị khai báo phụ thuộc ở component của mình. UI gọi API ứng dụng, còn thao tác đối tượng LVGL chạy trong ngữ cảnh GUI hoặc dưới khóa của port.
- `sdkconfig.defaults` chọn bảng phân vùng. `sdkconfig.defaults.esp32s3` chọn Flash, PSRAM và cổng console. `sdkconfig` là cấu hình local được sinh ra và có thể giữ giá trị cũ sau khi sửa defaults. Để kiểm tra lại từ defaults, sao lưu tùy chỉnh local rồi chạy `idf.py set-target esp32s3` và `idf.py build`.
- Firmware hiện dùng component đi kèm SDK. Khi thêm thư viện managed, khai báo phiên bản trong `idf_component.yml` của component sử dụng và commit `dependencies.lock` do Component Manager sinh ra. `managed_components/` được bỏ qua trong Git.
- Định dạng mã C theo `.clang-format`. Nếu đã cài clang-format, chạy `clang-format --dry-run --Werror main/*.c components/*/*.c components/*/include/*.h` từ thư mục gốc.
- GitHub Actions build với SDK v5.5.4 và xuất báo cáo kích thước trên push hoặc pull request. ESP-IDF kiểm tra kích thước ảnh ứng dụng so với slot factory trong quá trình build. Kiểm thử trên PCB chạy riêng theo [quy trình bring-up](docs/hardware/bringup.md).
