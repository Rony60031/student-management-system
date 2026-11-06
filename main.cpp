#include <iostream>
#include <string>
#include <limits>
#include "sqlite3.h"
using namespace std;

sqlite3* db = nullptr;

void checkError(int rc, char* errMsg) {
    if (rc != SQLITE_OK) {
        cout << "SQL error: " << (errMsg ? errMsg : "unknown") << "\n";
        sqlite3_free(errMsg);
    }
}

void createTable() {
    const char* sql =
        "CREATE TABLE IF NOT EXISTS students ("
        "id INTEGER PRIMARY KEY,"
        "name TEXT NOT NULL,"
        "department TEXT NOT NULL,"
        "cgpa REAL NOT NULL);";
    char* errMsg = nullptr;
    int rc = sqlite3_exec(db, sql, nullptr, nullptr, &errMsg);
    checkError(rc, errMsg);
}

bool studentExists(int id) {
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, "SELECT 1 FROM students WHERE id = ?;", -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, id);
    bool exists = (sqlite3_step(stmt) == SQLITE_ROW);
    sqlite3_finalize(stmt);
    return exists;
}

void addStudent() {
    int id;
    string name, dept;
    double cgpa;
    cout << "Enter ID: ";
    cin >> id;
    if (studentExists(id)) {
        cout << "A student with this ID already exists.\n";
        return;
    }
    cin.ignore();
    cout << "Enter Name: ";
    getline(cin, name);
    cout << "Enter Department: ";
    getline(cin, dept);
    cout << "Enter CGPA: ";
    cin >> cgpa;

    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db,
        "INSERT INTO students (id, name, department, cgpa) VALUES (?, ?, ?, ?);",
        -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, id);
    sqlite3_bind_text(stmt, 2, name.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, dept.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_double(stmt, 4, cgpa);
    if (sqlite3_step(stmt) == SQLITE_DONE)
        cout << "Student added successfully.\n";
    else
        cout << "Failed to add student.\n";
    sqlite3_finalize(stmt);
}

void displayAll() {
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db,
        "SELECT id, name, department, cgpa FROM students ORDER BY id;",
        -1, &stmt, nullptr);
    bool found = false;
    cout << "\nID\tName\t\tDepartment\tCGPA\n";
    cout << "--------------------------------------------------\n";
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        found = true;
        cout << sqlite3_column_int(stmt, 0) << "\t"
             << sqlite3_column_text(stmt, 1) << "\t\t"
             << sqlite3_column_text(stmt, 2) << "\t\t"
             << sqlite3_column_double(stmt, 3) << "\n";
    }
    if (!found) cout << "No students found.\n";
    sqlite3_finalize(stmt);
}

void searchStudent() {
    int id;
    cout << "Enter ID to search: ";
    cin >> id;
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db,
        "SELECT name, department, cgpa FROM students WHERE id = ?;",
        -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, id);
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        cout << "Name: " << sqlite3_column_text(stmt, 0) << "\n"
             << "Department: " << sqlite3_column_text(stmt, 1) << "\n"
             << "CGPA: " << sqlite3_column_double(stmt, 2) << "\n";
    } else {
        cout << "Student not found.\n";
    }
    sqlite3_finalize(stmt);
}

void updateStudent() {
    int id;
    string name, dept;
    double cgpa;
    cout << "Enter ID to update: ";
    cin >> id;
    if (!studentExists(id)) {
        cout << "Student not found.\n";
        return;
    }
    cin.ignore();
    cout << "New Name: ";
    getline(cin, name);
    cout << "New Department: ";
    getline(cin, dept);
    cout << "New CGPA: ";
    cin >> cgpa;

    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db,
        "UPDATE students SET name = ?, department = ?, cgpa = ? WHERE id = ?;",
        -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, dept.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_double(stmt, 3, cgpa);
    sqlite3_bind_int(stmt, 4, id);
    if (sqlite3_step(stmt) == SQLITE_DONE)
        cout << "Student updated successfully.\n";
    else
        cout << "Failed to update student.\n";
    sqlite3_finalize(stmt);
}

void deleteStudent() {
    int id;
    cout << "Enter ID to delete: ";
    cin >> id;
    if (!studentExists(id)) {
        cout << "Student not found.\n";
        return;
    }
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, "DELETE FROM students WHERE id = ?;", -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, id);
    if (sqlite3_step(stmt) == SQLITE_DONE)
        cout << "Student deleted successfully.\n";
    else
        cout << "Failed to delete student.\n";
    sqlite3_finalize(stmt);
}

int main() {
    if (sqlite3_open("students.db", &db) != SQLITE_OK) {
        cout << "Cannot open database.\n";
        return 1;
    }
    createTable();

    int choice;
    do {
        cout << "\n=== Student Management System (SQLite) ===\n";
        cout << "1. Add Student\n";
        cout << "2. Display All Students\n";
        cout << "3. Search Student\n";
        cout << "4. Update Student\n";
        cout << "5. Delete Student\n";
        cout << "0. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            choice = -1;
        }

        switch (choice) {
            case 1: addStudent(); break;
            case 2: displayAll(); break;
            case 3: searchStudent(); break;
            case 4: updateStudent(); break;
            case 5: deleteStudent(); break;
            case 0: cout << "Goodbye!\n"; break;
            default: cout << "Invalid choice. Try again.\n";
        }
    } while (choice != 0);

    sqlite3_close(db);
    return 0;
}