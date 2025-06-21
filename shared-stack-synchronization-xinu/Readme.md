# Shared Stack Synchronization in XINU

This project demonstrates **process synchronization** in the XINU operating system using **semaphores** to safely manage access to a **shared stack**.

## 📋 Problem Statement

Multiple processes need to access a shared stack of integers with capacity `N = 10`. The stack is accessed via two operations:

- `LocalPush(int x)`: Pushes a value into the stack.
  - If the stack is full, the process is **blocked** until space is available.
- `LocalPop()`: Pops a value from the stack.
  - If the stack is empty, the process is **blocked** until a value is available.

Synchronization is handled using three semaphores:

- `mutex`: ensures mutual exclusion.
- `stackFull`: blocks push operations when the stack is full.
- `stackEmpty`: blocks pop operations when the stack is empty.

## 🧪 Example Scenario (as in `xmain`)

- One producer (`func`) pushes values 0–9 into the stack.
- The main process pops 10 values from the stack.
- Delays simulate real-time behavior and test blocking correctly.

### Output Example:
push 0
push 1
pop#0 = 1
pop#1 = 0
...

## ⚙️ Technologies Used

- Written in C for the XINU operating system.
- Uses native XINU semaphores (`screate`, `wait`, `signal`).
- Compiled and tested on the Braude-modified XINU environment.

## 🧠 Concepts Demonstrated

- Inter-process synchronization
- Classic producer-consumer problem
- Stack data structure
- Real-time delays and blocking behavior

---

### 📝 Notes

To compile and run this project, you need a configured **XINU** development environment.

This project is part of the **Operating Systems and Real-Time Systems** course lab work.

---