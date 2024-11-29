#pragma once
#include "attendance_system.h"
#include <iostream>
#include <iomanip>

class AttendanceReport : public IReportGenerator {
    LeaveManagement &leaveManagement;
public:
    AttendanceReport(LeaveManagement &lm): leaveManagement(lm) {}
    
    void generateReport() {
        std::cout << "Attendance Report:" << std::endl;
        for (const auto& [id, employee] : leaveManagement.getEmployees()) {
            float attendancePercentage = (employee->getWorkingHours() / (40.0f * 4)) * 100;

            if (attendancePercentage < 80) {
                std::cout << "Employee " << employee->getName() << " (ID: " << id << "): "
                          << employee->getWorkingHours() << " hours" << ", Attendance %: " << attendancePercentage << std::endl;
            }
        }
    }
};

class LeaveReport : public IReportGenerator {
        LeaveManagement &leaveManagement;
public:
    LeaveReport(LeaveManagement &lm): leaveManagement(lm) {}

    void generateReport() {
        std::cout << "Leave Report:" << std::endl;
        for (const auto &[id, employee] : leaveManagement.getEmployees()) {
            std::cout << "Employee " << employee->getName() << " (ID: " << id << "):" << std::endl;
            for (const auto &leave : employee->getLeaves()) {
                if (leave->getStatus() == "Approved") {
                    std::cout << "  " << leave->getType() << " leave for " << leave->getDuration() << " days" << std::endl
                              << "  Leave Balance: " << "Casual " << employee->getCasualLeaveBalance() << ", Earned " << employee->getEarnedLeaveBalance() << std::endl;
                }
            }
        }
    }
};

class OutstandingLeaveReport : public IReportGenerator {
        LeaveManagement &leaveManagement;
public:
    OutstandingLeaveReport(LeaveManagement &lm): leaveManagement(lm) {}

    void generateReport() {
        std::cout << "Leave Report:" << std::endl;
        for (const auto &[id, employee] : leaveManagement.getEmployees()) {
            std::cout << "Employee " << employee->getName() << " (ID: " << id << "):" << std::endl;
            std::cout << "  Leave Balance: " << "Casual " << employee->getCasualLeaveBalance() << ", Earned " << employee->getEarnedLeaveBalance() << std::endl;
        }
    }
};
