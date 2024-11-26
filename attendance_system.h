#pragma once
#include "leave_management.h"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>

class AttendanceSystem {
private:
    LeaveManagement leaveManagement;
    std::unique_ptr<ILeaveFactory> leaveFactory;
    std::string employeeDataFile;
    std::string leaveDetailsFile;

public:
    AttendanceSystem(const std::string& empFile = "employees.txt", const std::string& leaveFile = "leave_details.txt")
        : leaveFactory(std::make_unique<LeaveFactory>()), employeeDataFile(empFile), leaveDetailsFile(leaveFile) {
        loadEmployeesFromFile();
    }

    ~AttendanceSystem() {
        saveEmployeeDataToFile("employees.txt");
    }

    void loadEmployeesFromFile() {
        std::ifstream file(employeeDataFile);
        if (!file) {
            std::cerr << "Unable to open employee data file. Starting with empty employee list." << std::endl;
            return;
        }

        std::string line;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            int id, hours;
            std::string name;
            int casualLeaveBalance, earnedLeaveBalance;

            if (iss >> id >> name >> hours >> casualLeaveBalance >> earnedLeaveBalance) {
                auto employee = std::make_unique<Employee>(id, name);
                employee->setWorkingHours(hours);
                employee->setCasualLeaveBalance(casualLeaveBalance);
                employee->setEarnedLeaveBalance(earnedLeaveBalance);
                leaveManagement.addEmployee(std::move(employee));
            }
        }
    }

    void markAttendance(int employeeId, int hours) {
        auto& employees = leaveManagement.getEmployees();
        auto it = employees.find(employeeId);
        if (it != employees.end()) {
            it->second->addWorkingHours(hours);
        }
    }

    void applyLeave(int employeeId, const std::string& leaveType, int duration, const std::string& startDate, const std::string& endDate) {
        auto leave = leaveFactory->createLeave(leaveType, duration);
        leave->setStartDate(startDate);
        leave->setEndDate(endDate);
        leaveManagement.applyLeave(employeeId, std::move(leave));
    }

    void generateAttendanceReport() {
        std::cout << "Attendance Report:" << std::endl;
        for (const auto& [id, employee] : leaveManagement.getEmployees()) {
            std::cout << "Employee " << employee->getName() << " (ID: " << id << "): "
                      << employee->getWorkingHours() << " hours" << std::endl;
        }
    }

    void generateLeaveReport() {
        std::cout << "Leave Report:" << std::endl;
        for (const auto& [id, employee] : leaveManagement.getEmployees()) {
            std::cout << "Employee " << employee->getName() << " (ID: " << id << "):" << std::endl;
            for (const auto& leave : employee->getLeaves()) {
                std::cout << "  " << leave->getType() << " leave for " << leave->getDuration() << " hours" << std::endl;
            }
        }
    }

    // New function to save employee data to file
    void saveEmployeeDataToFile(const std::string& filename) {
        std::ofstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Error: Unable to open file " << filename << " for writing." << std::endl;
            return;
        }

        // Write header
        file << std::left << std::setw(10) << "ID" 
             << std::setw(20) << "Name" 
             << std::setw(15) << "Working Hours" 
             << "Leave Details" << std::endl;
        file << std::string(80, '-') << std::endl;

        // Write employee data
        for (const auto& [id, employee] : leaveManagement.getEmployees()) {
            file << std::left << std::setw(10) << id
                 << std::setw(20) << employee->getName()
                 << std::setw(15) << employee->getWorkingHours();

            // Write leave details
            if (employee->getLeaves().empty()) {
                file << "No leaves taken";
            } else {
                bool first = true;
                for (const auto& leave : employee->getLeaves()) {
                    if (!first) {
                        file << "; ";
                    }
                    file << leave->getType() << " (" << leave->getDuration() << " hours)";
                    first = false;
                }
            }
            file << std::endl;
        }

        std::cout << "Employee data has been saved to " << filename << std::endl;
    }
};

