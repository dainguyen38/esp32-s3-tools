# Bring-up phần cứng

| Mẫu in trên bo hoặc bao bì | Thiết bị | Đối chiếu |
| --- | --- | --- |
| ESP32-S3-Touch-LCD-3.5 | ST7796/SPI và FT6336/I²C | Đọc revision V1 hay V2 trên PCB, lấy schematic tương ứng. |
| ESP32-S3-Touch-LCD-3.5-C | Biến thể có vỏ, kèm OV5640 | Revision V1/V2 vẫn xác định theo PCB. Camera nằm ngoài phạm vi firmware hiện tại. |
| ESP32-S3-Touch-LCD-3.5B | AXS15231B/QSPI | Cần BSP riêng. |

Đối chiếu pinout và đường nguồn với [schematic V1/V2 của Waveshare](https://docs.waveshare.com/ESP32-S3-Touch-LCD-3.5/Resources-And-Documents). Khi bổ sung GPIO, đặt dữ liệu theo revision trong BSP và lưu liên kết tới sơ đồ đã dùng.

## Thứ tự kiểm tra

| Bước | Thực hiện | Bằng chứng |
| --- | --- | --- |
| LAB-00 | Xác nhận model, revision và cổng USB. | Ảnh nhãn PCB và schematic tương ứng. |
| LAB-01 | Build, nạp và chạy [bài kiểm tra khởi động](../../README.md#nạp-firmware-và-kiểm-tra-khởi-động). | Log phiên bản, chip, reset reason, 16 MiB Flash, 8 MiB PSRAM và `BOOT_SMOKE_PASS`. |
| LAB-02 | Sau khi có cấu hình bus theo schematic, probe các địa chỉ I²C đã đối chiếu với timeout hữu hạn. | Địa chỉ, kết quả ACK/NACK và mã lỗi. |
| LAB-03 | Xác minh quan hệ giữa PMU AXP2101, TCA9554, reset và backlight theo sơ đồ mạch. | Trạng thái đường nguồn và thứ tự khởi tạo theo revision. |
| LAB-04 | Tích hợp ST7796. Vẽ màu cơ bản và đường biên trước khi tích hợp GUI. | Màu, chiều xoay, cách cấu hình SPI và log flush. |
| LAB-05 | Tích hợp FT6336 và thử chạm bốn góc, chạm ở giữa, kéo và xoay. | Bản đồ tọa độ phù hợp hướng LCD và kết quả khi bus có tải. |
| LAB-06 | Thử port LVGL với bộ đệm vẽ từng phần. | Vùng nhớ DMA, kết quả flush, heap nội và thời gian đáp ứng đo trên bo. |

Firmware hiện hỗ trợ LAB-01. Khi thực hiện LAB-02 đến LAB-06, ghi lại cùng một bộ thông tin cho mỗi lần chạy để so sánh giữa các revision.

| LAB-ID | PCB | Firmware SHA | SDK/driver version | Nguồn cấp và thiết lập | Log/ảnh | Kết quả | Người thử và ngày |
| --- | --- | --- | --- | --- | --- | --- | --- |
| LAB-01 | Chờ xác nhận | | | | | Chưa chạy trên bo | |
