# MultiProgMultiProg** 

---

### 📘 README کامل برای پروژه MultiProg

**MultiProg: برنامه‌نویس جهانی منبع‌باز برای PIC، AVR و حافظه‌های سریال**

[English](#english-version) | [فارسی](#نسخه-فارسی)

---

#### نسخه فارسی

**MultiProg** یک برنامه‌نویس (Programmer) همه‌کاره، منبع‌باز و مبتنی بر USB است که برای برنامه‌ریزی طیف گسترده‌ای از میکروکنترلرها و حافظه‌های سریال طراحی شده است. این پروژه که توسط **تیم شاپ الکترونیک و گروه سیگنال** توسعه یافته، شامل نرم‌افزاری با قابلیت اجرا بر روی سیستمعامل‌های ویندوز، لینوکس و اندروید است.

---

**ویژگی‌های کلیدی:**

*   **پشتیبانی از قطعات گسترده**: برنامه‌ریزی میکروکنترلرهای **PIC** (خانواده‌های ۱۰F، ۱۲F، ۱۶F، ۱۸F، ۲۴F، dsPIC)، **AVR** (ATmega، ATtiny)، و حافظه‌های سریال **I2C، SPI، MicroWire، OneWire و UNIO**.
*   **سخت‌افزار مرجع**: این پروژه با سخت‌افزار منبع‌باز **Open Programmer** (بر اساس میکروکنترلر PIC18F2550 یا PIC18F25K50) سازگار است. مدار و اجزای آن به راحتی در دسترس و کم‌هزینه هستند.
*   **نرم‌افزار چندسکویی**: نرم‌افزار کنترل برنامه (که با فریم‌ورک **Qt** توسعه یافته) بر روی ویندوز، لینوکس و اندروید قابل اجرا است.
*   **رابط کاربری گرافیکی (GUI)**: دارای محیطی کاربرپسند برای انتخاب دستگاه، خواندن، نوشتن، ذخیره‌سازی فایل‌های هگزادسیمال و تنظیمات پیشرفته.
*   **ابزار خط فرمان (CLI)**: برای کاربران حرفه‌ای و استفاده در اسکریپت‌ها، یک ابزار خط فرمان قدرتمند نیز ارائه شده است.
*   **رابط USB HID**: نیازی به نصب درایور اضافی ندارد و به سادگی توسط سیستمعامل شناسایی می‌شود.
*   **متن‌باز و قابل گسترش**: تمامی کدهای سخت‌افزار (فیرم‌وار) و نرم‌افزار تحت مجوز GPL2 منتشر شده‌اند و قابل شخصی‌سازی هستند.
*   **قابلیت دیباگ (ICD)**: پشتیبانی از دیباگ درون‌مدار (In-Circuit Debug) برای برخی از میکروکنترلرهای سری PIC16F.
*   **تست سخت‌افزاری**: دارای قابلیت تست سخت‌افزاری برای بررسی صحت عملکرد مدار.

**سخت‌افزار مورد نیاز:**

*   برد اصلی بر اساس طراحی **Open Programmer** (نسخه ۲.۱ یا بالاتر).
*   بردهای انبساط اختیاری برای پشتیبانی از ولتاژ ۳.۳ ولت، میکروهای ۴۰ پایه، و خانواده‌های PIC24/dsPIC.
*   اتصال به رایانه از طریق پورت USB (نوع B یا Micro-B).

**شروع سریع:**

1.  سخت‌افزار را مطابق راهنمای پروژه Open Programmer مونتاژ کنید.
2.  فیرم‌وار مناسب (برای PIC18F2550 یا 25K50) را روی میکروکنترلر اصلی برنامه‌ریزی کنید.
3.  نرم‌افزار MultiProg را از مخزن گیت‌هاب دانلود یا کامپایل کنید.
4.  دستگاه را به USB وصل کرده و نرم‌افزار را اجرا کنید.

---

**لینک‌های مفید و کانال‌های ارتباطی:**

| بخش | لینک/توضیح |
| :--- | :--- |
| **مخزن کد نرم‌افزار** | [github.com/hadipic/MultiProg](https://github.com/hadipic/MultiProg) |
| **مدار و مستندات سخت‌افزار** | [openprog.altervista.org](http://openprog.altervista.org/OP_eng.html) |
| **کانال تخصصی ESP32** | [@esp32_evm](https://t.me/esp32_evm) - کانال رسمی پروژه‌های ESP32 |
| **گروه تخصصی ESP32** | [@hadipic](https://t.me/hadipic) - گروه پرسش و پاسخ و تبادل نظر |
| **گروه Embedded & Qt** | [@Embedded_Qt](https://t.me/Embedded_Qt) - بحث و گفتگو درباره سیستم‌های نهفته و کیوت |
| **ربات فروشگاهی** | [@shop_electronic_bot](https://t.me/shop_electronic_bot) - ارتباط با فروشگاه و دریافت اطلاعات |
| **وبسایت فروشگاه** | [shop-electronic.ir](https://shop-electronic.ir) - فروشگاه تخصصی قطعات الکترونیک |
| **وبلاگ تخصصی** | [shop-electronic.ir/blog](https://shop-electronic.ir/blog) - مقالات آموزشی و راهنماها |

---

**پشتیبانی از قطعات:**

لیست کامل قطعات پشتیبانی‌شده شامل خانواده‌های زیر است (برای مشاهده جزئیات به مستندات سخت‌افزار مراجعه کنید):
*   **PIC10F/12F/16F/18F**: شامل طیف گسترده‌ای از مدل‌های ۸، ۱۴، ۱۸، ۲۰، ۲۸ و ۴۰ پایه.
*   **PIC24F/H, dsPIC30/33**: پشتیبانی از میکروکنترلرهای ۱۶ بیتی.
*   **AVR**: ATmega (مانند ATMega8, 16, 32, 48, 88, 168, 328, 644, 1284) و ATtiny (مانند Tiny11, 12, 13, 24, 25, 44, 45, 84, 85).
*   **حافظه‌های سریال**: سری‌های ۲۴ (I2C)، ۲۵ (SPI)، ۹۳ (MicroWire)، DS24xx (OneWire) و ۱۱xxx (UNIO).

---

**مجوز و مشارکت:**

*   **مجوز**: این پروژه تحت مجوز **GPL2** منتشر شده است.
*   **مشارکت**: از تمامی علاقه‌مندان برای مشارکت در توسعه، گزارش باگ‌ها و افزودن پشتیبانی برای قطعات جدید استقبال می‌شود. برای همکاری با تیم، از طریق کانال‌های ارتباطی با ما در تماس باشید.

---

#### English Version

**MultiProg** is a universal, open-source USB programmer designed for a wide range of microcontrollers and serial memories. This project, developed by **Shop Electronic & Signal Group**, includes software compatible with Windows, Linux, and Android operating systems.

---

**Key Features:**

*   **Extensive Device Support**: Programs **PIC** microcontrollers (10F, 12F, 16F, 18F, 24F, dsPIC families), **AVR** microcontrollers (ATmega, ATtiny), and serial memories including **I2C, SPI, MicroWire, OneWire, and UNIO**.
*   **Reference Hardware**: This project is compatible with the open-source **Open Programmer** hardware (based on PIC18F2550 or PIC18F25K50). The circuit and components are easily accessible and low-cost.
*   **Cross-Platform Software**: The control software, developed with the **Qt** framework, runs on Windows, Linux, and Android.
*   **Graphical User Interface (GUI)**: Features a user-friendly interface for device selection, reading, writing, saving hex files, and advanced settings.
*   **Command-Line Tool (CLI)**: A powerful command-line utility is also provided for professional users and scripting purposes.
*   **USB HID Interface**: No additional drivers are required; the device is natively recognized by the operating system.
*   **Open Source and Extensible**: All hardware (firmware) and software source codes are released under the GPL2 license and are customizable.
*   **In-Circuit Debug (ICD)**: Supports In-Circuit Debugging for select PIC16F microcontrollers.
*   **Hardware Test**: Includes a hardware test feature to verify the correct operation of the circuit.

**Hardware Requirement:**

*   Main board based on the **Open Programmer** design (version 2.1 or later).
*   Optional expansion boards for 3.3V support, 40-pin microcontrollers, and PIC24/dsPIC families.
*   Connection to the computer via USB port (Type B or Micro-B).

**Quick Start:**

1.  Assemble the hardware according to the Open Programmer project guide.
2.  Program the appropriate firmware (for PIC18F2550 or 25K50) onto the main microcontroller.
3.  Download or compile the MultiProg software from the GitHub repository.
4.  Connect the device to USB and run the software.

---

**Useful Links and Communication Channels:**

| Section | Link/Description |
| :--- | :--- |
| **Software Repository** | [github.com/hadipic/MultiProg](https://github.com/hadipic/MultiProg) |
| **Hardware Schematics & Docs** | [openprog.altervista.org](http://openprog.altervista.org/OP_eng.html) |
| **ESP32 Channel** | [@esp32_evm](https://t.me/esp32_evm) - Official channel for ESP32 projects |
| **ESP32 Group** | [@hadipic](https://t.me/hadipic) - Q&A and discussion group |
| **Embedded & Qt Group** | [@Embedded_Qt](https://t.me/Embedded_Qt) - Discussions on embedded systems and Qt |
| **Shop Bot** | [@shop_electronic_bot](https://t.me/shop_electronic_bot) - Contact shop and get information |
| **Online Store** | [shop-electronic.ir](https://shop-electronic.ir) - Specialized electronics store |
| **Tech Blog** | [shop-electronic.ir/blog](https://shop-electronic.ir/blog) - Educational articles and guides |

---

**Device Support:**

The complete list of supported devices includes the following families (see hardware documentation for details):
*   **PIC10F/12F/16F/18F**: Includes a wide range of 8, 14, 18, 20, 28, and 40-pin models.
*   **PIC24F/H, dsPIC30/33**: Support for 16-bit microcontrollers.
*   **AVR**: ATmega (e.g., ATMega8, 16, 32, 48, 88, 168, 328, 644, 1284) and ATtiny (e.g., Tiny11, 12, 13, 24, 25, 44, 45, 84, 85).
*   **Serial Memories**: 24-series (I2C), 25-series (SPI), 93-series (MicroWire), DS24xx (OneWire), and 11xxx (UNIO).

---

**License and Contribution:**

*   **License**: This project is released under the **GPL2** license.
*   **Contribution**: We welcome all enthusiasts to contribute to development, report bugs, and add support for new devices. To collaborate, please contact us through the communication channels.

---

## `MultiProg` قرار دهید. همچنین می‌توانید از محتوای  صفحه‌ی محصول در وبسایت https://shop-electronic.ir نیز استفاده کنید.
