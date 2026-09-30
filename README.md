# Linux System Monitor & Process Manager

A Linux system monitoring and process management utility written in C.

## Overview

Linux provides detailed information about running processes and system resources through the `/proc` virtual filesystem. This project uses that information to build a terminal-based system monitor that allows users to inspect processes, view system information, search and sort processes, manage processes using Linux signals, monitor processes interactively, and review application logs.

The project is designed to demonstrate core C programming and Linux system programming concepts through a practical, modular application.

## Goals

- Build a practical Linux system utility using C.
- Learn how process information can be read from the Linux `/proc` filesystem.
- Demonstrate process discovery, inspection, searching, sorting, and management.
- Work with Linux signals for basic process control.
- Practice modular C programming using source files, header files, structures, pointers, and standard library functions.
- Use a Makefile for repeatable compilation.
- Implement input validation, error handling, and application logging.
- Maintain the project using Git and GitHub with a clean, organized structure.

## Main Features

- Display running processes.
- Search processes by name.
- View detailed process information.
- Display CPU and memory-related process information.
- Send SIGTERM, SIGKILL, SIGSTOP, and SIGCONT signals.
- Display system information such as CPU, memory, uptime, and process count.
- Sort processes by PID or memory usage.
- Interactive live process monitoring.
- Application event logging.

## Technologies

- C
- Linux
- GCC
- Make
- Linux `/proc` filesystem
- Git and GitHub