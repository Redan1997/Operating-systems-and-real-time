# Low-Level MMIO, PIT, and RTC in DOS

This project includes two C files (`tar1.c` and `tar2.c`) written for MS-DOS, showcasing **low-level interaction** with hardware using **Memory-Mapped I/O**, **ports**, **interrupts**, and **timer chips (PIT)**. These programs demonstrate real-time and embedded concepts such as keyboard ISR handling, cursor control, sound generation, and system clock access.

---

## 📦 Files

- `tar1.c`: Interactive VGA cursor program with direct memory and port access.
- `tar2.c`: Keyboard-controlled speaker program using PIT and RTC.

---

## 🧠 Concepts Demonstrated

### ✅ tar1.c Highlights:
- Direct manipulation of VGA text memory at `0xB8000`
- Custom ISR for keyboard (INT 9h)
- Cursor positioning using CRT Controller registers (ports `0x3D4/3D5`)
- Keyboard scancode parsing
- Displaying text directly to screen
- Use of BIOS interrupt `INT 10h` (video) and `INT 16h` (keyboard)
- Save/restore state of screen using global matrices

#### Used Ports/Interrupts:
- **INT 9h**: Custom keyboard ISR
- **INT 10h**: Set video mode
- **INT 16h**: Keyboard I/O
- **Ports `0x3D4` / `0x3D5`**: CRT cursor positioning registers
- **MMIO**: Access to `0xB8000` VGA memory

---

### ✅ tar2.c Highlights:
- Tone generation using **Programmable Interval Timer (PIT)** and **PC Speaker**
- Real-time delay using custom timer ISR (INT 8h)
- Real-time user input via INT 9h ISR (keyboard)
- Read RTC time using I/O ports `0x70/71` (CMOS registers)
- Convert BCD to binary time
- Use of DOS interrupts and inline assembly for full hardware control

#### Used Ports/Interrupts:
- **INT 8h**: Timer tick interrupt
- **INT 9h**: Keyboard ISR
- **Port `0x43`**: PIT command
- **Port `0x42`**: PIT channel for speaker
- **Port `0x61`**: PC speaker control
- **Port `0x70/71`**: CMOS/RTC read
- **INT 16h**: BIOS keyboard services

---

## 🖥️ Required Environment

- DOSBox or MS-DOS 6.x
- Turbo C / Borland C 3.0 compiler
- Real-mode compatible machine or VM
- Keyboard input and PC speaker supported

---

## 🎛️ Controls Summary

### In `tar1.c`:
- Move cursor: arrow keys
- Change character: F2
- Change attribute/color: F1
- Hide/show cursor: F3/F4
- Save/restore cursor position: Shift + X

### In `tar2.c`:
- Keys 1–0: Set pitch (speaker frequency)
- ↑ / ↓ arrows: Change sound duration
- Esc: Exit and print current time from RTC

---

## 📚 Learning Goals

This lab demonstrates real-world embedded programming practices like:

- Writing ISRs in C with inline assembly
- Accessing hardware without OS abstraction
- Handling concurrency and timing manually
- Direct use of system buses and I/O ports

---

## 📝 Notes

This lab was developed as part of the **Operating Systems and Real-Time Systems** course.
