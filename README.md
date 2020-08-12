# Android-Process-Tree :iphone:

This project is an implementation of a tree formatted printout of the current processes running in an android device.
The key objectives to this project are the following:

- Familiarizing the process of setting up a virtual device and it's functionalities.
- Installing the development tools, NDK,SDK,JDK and cross compile a program that can be ran on the AVD.
- Learning the systemcall for process control and management.
- Deepen insight on the structure of task_struct.
- Concurrent Programming.

### This project was run on a linux machine! The same can be achieved on a Windows machine or a Linux Virtual Machine!

This Project will consist of two parts!

1. The system call implementations.
2. A concurrency problem.

:round_pushpin:The results of the runned-program will be placed in each individual folder. Named testscript.txt

### 1. System Calls and Process Trees

By learning to create a module \*.ko file and adding a custom system call, by using DFS I was able to create a program that lists a tree of all process in order of parent and child.

Within this test there is a Fork Test that uses a fork call to create a child for a parent process.
The process tree function will then be invoked and a tree with the child attached will be printed.
The pid of the parent process will be printed out on the terminal when the fork is invoked.

### 2. Using multiple threads to implement the Burger Buddies Problem

Using multi-threaded programming, I was able to simulate a model of a common fast food resturaunt. Where a customer places the order, the waiter takes a burger if available, if not wait for the cook, and then serve.

## Purpose

This project was purposed to deepen my understanding in an android device and wet my feets with multi-threaded programming. I was also able to learn about cross-compilation and the structure of tasks in an android device.
