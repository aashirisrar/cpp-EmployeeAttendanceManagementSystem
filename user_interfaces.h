#pragma once
#include "attendance_system.h"
#include <iostream>
#include <chrono>

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
                bool res = system.markAttendance(employeeId, hours);
                if(res){
                    std::cout << "Attendance marked successfully.\n";
                } else {
                    std::cout<<"Employee with this id doesn't exists!";
                }
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
                      << "1. Generate Low Attendance Report\n"
                      << "2. Generate Leave Report\n"
                      << "3. Approve Leaves\n"
                      << "4. Exit\n"
                      << "Enter your choice: ";
            int choice;
            std::cin >> choice;

            if (choice == 1) {
                system.generateAttendanceReport();
            } else if (choice == 2) {
                system.generateLeaveReport();
            } else if (choice == 3) {
                system.approveLeave();
            } else if (choice == 4) {
                break;
            } else {
                std::cout << "Invalid choice. Please try again.\n";
            }
        }
    }
};

class EmployeeInterface
{
private:
    AttendanceSystem &system;

public:
    EmployeeInterface(AttendanceSystem &sys) : system(sys) {}

    void run()
    {
        while (true)
        {
            std::cout << "\nEmployee Menu:\n"
                      << "1. Apply for Leave\n"
                      << "2. Exit\n"
                      << "Enter your choice: ";
            int choice;
            std::cin >> choice;

            if (choice == 1)
            {
                int employeeId;
                std::string leaveType, startDate, endDate;
                std::cout << "Enter Employee ID: ";
                std::cin >> employeeId;
                std::cout << "Enter Leave Type (Casual/Earned/Official/Unpaid): ";
                std::cin >> leaveType;
                std::cout << "Enter Start Date (YYYY-MM-DD): ";
                std::cin >> startDate;
                std::cout << "Enter End Date (YYYY-MM-DD): ";
                std::cin >> endDate;

                // Calculate duration
                std::tm start = {}, end = {};
                std::istringstream ss_start(startDate);
                std::istringstream ss_end(endDate);
                ss_start >> std::get_time(&start, "%Y-%m-%d");
                ss_end >> std::get_time(&end, "%Y-%m-%d");

                if (ss_start.fail() || ss_end.fail())
                {
                    std::cout << "Error: Invalid date format. Please use YYYY-MM-DD.\n";
                    continue;
                }

                auto start_time = std::chrono::system_clock::from_time_t(std::mktime(&start));
                auto end_time = std::chrono::system_clock::from_time_t(std::mktime(&end));

                auto duration = std::chrono::duration_cast<std::chrono::hours>(end_time - start_time).count() / 24 + 1;

                if (duration <= 0)
                {
                    std::cout << "Error: End date must be after start date.\n";
                    continue;
                }

                if (leaveType == "Casual" && duration > 4)
                {
                    std::cout << "Cannot avail more than 4 casual leaves at a time.\n";
                    continue;
                }
                else if (leaveType == "Earned" && duration < 4)
                {
                    std::cout << "Cannot avail less than 4 earned leaves at a time.\n";
                    continue;
                }

                // check if user has enough leave balance
                bool res = system.checkLeaveBalance(employeeId, leaveType, duration);
                if (res)
                {
                    system.applyLeave(employeeId, leaveType, duration, startDate, endDate);
                    std::cout << "Leave application submitted successfully for " << duration << " day(s).\n";
                }
                else
                {
                    std::cout << "Error: Leaves duration out of balance or employee id not correct.\n";
                }
            }
            else if (choice == 2)
            {
                break;
            }
            else
            {
                std::cout << "Invalid choice. Please try again.\n";
            }
        }
    }
};

class AdminInterface {
private:
    AttendanceSystem& system;

public:
    AdminInterface(AttendanceSystem& sys) : system(sys) {}

    void run() {
        while (true) {
            std::cout << "\nAdmin Menu:\n"
                      << "1. Add Employee\n"
                      << "2. Exit\n"
                      << "Enter your choice: ";
            int choice;
            std::cin >> choice;

            if (choice == 1) {
                int id;
                std::string name;
                std::cout << "Enter Employee Name: ";
                std::cin.ignore();
                std::getline(std::cin, name);
                std::cout << "Enter Employee ID: ";
                std::cin >> id;

                if (system.addEmployee(id, name)) {
                    std::cout << "Employee added successfully.\n";
                } else {
                    std::cout << "Failed to add employee. Employee ID might already exist.\n";
                }
            } else if (choice == 2) {
                break;
            } else {
                std::cout << "Invalid choice. Please try again.\n";
            }
        }
    }
};