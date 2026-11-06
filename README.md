# Student Management System

A console-based Student Management System written in C++ with an SQLite database. It supports full CRUD operations and stores data permanently in a database file.

![Menu](screenshots/menu.png)

## Features

- Add a new student (ID, name, department, CGPA)
- Display all students
- Search a student by ID
- Update student information
- Delete a student
- Duplicate ID check
- Data stored in an SQLite database using prepared statements

## Technologies Used

- C++
- SQLite 3 (C API)

## How to Run

1. Clone the repository
2. Download the SQLite amalgamation from https://sqlite.org/download.html and place sqlite3.c and sqlite3.h in the project folder
3. Compile SQLite once: gcc -c sqlite3.c -o sqlite3.o
4. Compile the program: g++ main.cpp sqlite3.o -o student_system
5. Run: ./student_system

## Version History

- v1-textfile: first version using a text file for storage
- Current: SQLite database version

## Author

Rony - https://github.com/Rony60031
