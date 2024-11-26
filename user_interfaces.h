#pragma once
#include "attendance_system.h"
#include <iostream>

class GuardInterface {
private:
    AttendanceSystem& system;

public:
    GuardInterface(AttendanceSystem& sys) : system(sys) {}

    void run() {
        while (true) {
            std::cout << "\nGuard Menu:\n"
                      << "1. Mark Attendance\n"
                      << "2. Exit\n"
                      << "Enter your choice: ";
            int choice;
            std::cin >> choice;

            if (choice == 1) {
                int employeeId, hours;
                std::cout << "Enter Employee ID: ";
                std::cin >> employeeId;
                std::cout << "Enter Hours Worked: ";
                std::cin >> hours;
                system.markAttendance(employeeId, hours);
                std::cout << "Attendance marked successfully.\n";
            } else if (choice == 2) {
                break;
            } else {
                std::cout << "Invalid choice. Please try again.\n";
            }
        }
    }
};

class SupervisorInterface {
private:
    AttendanceSystem& system;

public:
    SupervisorInterface(AttendanceSystem& sys) : system(sys) {}

    void run() {
        while (true) {
            std::cout << "\nSupervisor Menu:\n"
                      << "1. Generate Attendance Report\n"
                      << "2. Generate Leave Report\n"
                      << "3. Exit\n"
                      << "Enter your choice: ";
            int choice;
            std::cin >> choice;

            if (choice == 1) {
                system.generateAttendanceReport();
            } else if (choice == 2) {
                system.generateLeaveReport();
            } else if (choice == 3) {
                break;
            } else {
                std::cout << "Invalid choice. Please try again.\n";
            }
        }
    }
};

class EmployeeInterface {
private:
    AttendanceSystem& system;

public:
    EmployeeInterface(AttendanceSystem& sys) : system(sys) {}

    void run() {
        while (true) {
            std::cout << "\nEmployee Menu:\n"
                      << "1. Apply for Leave\n"
                      << "2. Exit\n"
                      << "Enter your choice: ";
            int choice;
            std::cin >> choice;

            if (choice == 1) {
                int employeeId, duration;
                std::string leaveType, startDate, endDate;
                std::cout << "Enter Employee ID: ";
                std::cin >> employeeId;
                std::cout << "Enter Leave Type (Casual/Earned/Official/Unpaid): ";
                std::cin >> leaveType;
                std::cout << "Enter Leave Duration (in hours): ";
                std::cin >> duration;
                std::cout << "Enter Start Date (YYYY-MM-DD): ";
                std::cin >> startDate;
                std::cout << "Enter End Date (YYYY-MM-DD): ";
                std::cin >> endDate;
                system.applyLeave(employeeId, leaveType, duration, startDate, endDate);
                std::cout << "Leave application submitted successfully.\n";
            } else if (choice == 2) {
                break;
            } else {
                std::cout << "Invalid choice. Please try again.\n";
            }
        }
    }
};

