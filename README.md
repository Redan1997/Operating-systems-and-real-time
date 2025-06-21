# 🧠 Operating Systems and Real-Time Projects

A collection of academic projects exploring core concepts in operating systems, embedded systems, low-level memory access, and real-time synchronization.

> 💡 All projects were implemented in **C/C++** using either **Linux** or **XINU/DOS environments**.  
> ✅ Graded projects — each achieving top marks.

---

## 📁 Repository Structure

| Folder                              | Description |
|-------------------------------------|-------------|
| `linux-black-friday-shell/`         | 🛍️ A full Linux-based shell simulation for a Black Friday order system. |
| `low-level-mmio-pit-rtc-dos/`       | 🖥️ Assembly + C project demonstrating VGA control, PIT speaker manipulation, and RTC reads via MMIO and I/O ports. |
| `shared-stack-synchronization-xinu/`| 📦 Real-time task synchronization using semaphores in XINU over a shared integer stack. |
| `project/`                          | (Reserved for future use or general files.)

---

## 🔍 Project Highlights

### 🛍️ `linux-black-friday-shell/`
A Linux-based marketing simulation with a full shell interface.  
**Key Features:**
- Custom shell with up to 3 arguments.
- Dynamic company flyer creation.
- Order placing, discounts, and real-time totals.
- Shell commands: `CreateFlyer`, `MakeOrder`, `GetFlyer`, `GetOrderNum`, `GetSummary`, `exit`.

➡️ [View Full README](linux-black-friday-shell/README.md)

---

### 🖥️ `low-level-mmio-pit-rtc-dos/`
Low-level interaction with PC hardware in a DOS environment using inline Assembly and C.  
**Demonstrates:**
- Writing directly to **VGA memory** for custom cursor and graphics.
- Controlling **PIT (Programmable Interval Timer)** to make sound.
- Reading time using **RTC (Real Time Clock)** registers.

➡️ [View Full README](low-level-mmio-pit-rtc-dos/README.md)

---

### 📦 `shared-stack-synchronization-xinu/`
A XINU-based real-time lab demonstrating process synchronization.  
**Task:** Implement a shared bounded stack using **semaphores** and ensure mutual exclusion between `LocalPush()` and `LocalPop()`.

➡️ [View Full README](shared-stack-synchronization-xinu/README.md)

---

## ⚙️ Technologies Used

- 🐧 **Linux System Programming**
- ⚙️ **XINU OS (Educational Kernel)**
- 🧠 **Semaphores & Scheduling**
- 🧬 **Low-level MMIO / Interrupts / Assembly**
- 💻 **C, Assembly (8086), Bash**

---


## ✅ Notes

- Projects were run and tested on:
  - **DOSBox** (for legacy code)
  - **Ubuntu/Linux Terminal** (for shell-based projects)
  - **XINU emulator or real mode PC emulator**

---

## 📌 How to Use

Each folder contains its own:
- `README.md` with instructions
- Source files
- Compilation commands or scripts

---


## 🧠 Inspiration

Course: **Operating Systems and Real-Time Systems**  
Instructor: DR.Eitan Ron   
Institution: **Braude College of Engineering, Israel**

