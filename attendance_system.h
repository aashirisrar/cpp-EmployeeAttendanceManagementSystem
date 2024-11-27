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
        loadLeaves();
    }

    ~AttendanceSystem() {
        saveEmployeeDataToFile("employees.txt");
        saveLeaves();
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
            float attendancePercentage = (employee->getWorkingHours() / (40.0f * 4)) * 100;

            if(attendancePercentage < 80)
            {
                std::cout << "Employee " << employee->getName() << " (ID: " << id << "): "
                        << employee->getWorkingHours() << " hours" <<  ", Attendance %: " << attendancePercentage << std::endl;
            }
        }
    }

    void generateAttendanceReportForEmployee(int employeeId)
    {
        auto& employees = leaveManagement.getEmployees();
        auto it = employees.find(employeeId);

        if (it != employees.end())
        {
            float attendancePercentage = (it->second->getWorkingHours() / (40.0f * 4)) * 100;
            std::cout << "Attendance Report:" << std::endl;
            std::cout << "Employee " << it->second->getName() << " (ID: " << it->second->getId() << "): "
                      << it->second->getWorkingHours() << " hours" << ", Attendance %: " << attendancePercentage << std::endl;
        }
    }

    void generateLeaveReport()
    {
        std::cout << "Leave Report:" << std::endl;
        for (const auto &[id, employee] : leaveManagement.getEmployees())
        {
            std::cout << "Employee " << employee->getName() << " (ID: " << id << "):" << std::endl;
            for (const auto &leave : employee->getLeaves())
            {
                std::cout << "  " << leave->getType() << " leave for " << leave->getDuration() << " days" << std::endl
                          << "  Leave Balance: " << "Casual " << employee->getCasualLeaveBalance() << ", Earned " << employee->getEarnedLeaveBalance() << std::endl;
            }
        }
    }

    void generateLeaveReportForEmployee(int employeeId)
    {
        std::cout << "Leave Report:" << std::endl;
        auto &employees = leaveManagement.getEmployees();
        auto it = employees.find(employeeId);

        if (it != employees.end())
        {
            std::cout << "Employee " << it->second->getName() << " (ID: " << it->second->getId() << "):" << std::endl;
            for (const auto &leave : it->second->getLeaves())
            {
                std::cout << "  " << leave->getType() << " leave for " << leave->getDuration() << " days" << std::endl
                          << "  Leave Balance: " << "Casual " << it->second->getCasualLeaveBalance() << ", Earned " << it->second->getEarnedLeaveBalance() << std::endl;
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

        // Write employee data
        for (const auto& [id, employee] : leaveManagement.getEmployees()) {
            file << id <<" "
                 << employee->getName()<<" "
                 << employee->getWorkingHours()<<" "
                 << employee->getCasualLeaveBalance()<<" "
                 << employee->getEarnedLeaveBalance();
                 

            file << std::endl;
        }

        std::cout << "Employee data has been saved to " << filename << std::endl;
    }

    void loadLeaves()
    {
        std::ifstream file(leaveDetailsFile);
        std::string line;
        while (std::getline(file, line))
        {
            std::istringstream iss(line);
            std::string type, startDate, endDate;
            int employeeId;
            int duration;
            bool approved;
            iss >> employeeId >> type >> startDate >> endDate >> approved>> duration;
            Leave leave(type, duration, approved);

            // if (approved)
            // {
            //     leave.approve();
            // }

            for(const auto& [id, employeePtr] : leaveManagement.getEmployees())
            {
                auto leave = leaveFactory->createLeave(type, duration);
                leave->setStartDate(startDate);
                leave->setEndDate(endDate);
                leaveManagement.applyLeave(employeeId, std::move(leave));
            }
        }
    }

    void saveLeaves()
    {
        std::ofstream file(leaveDetailsFile);
        for (const auto& [id, employeePtr]  : leaveManagement.getEmployees())
        {   

            for(const auto& leave: employeePtr->getLeaves())
            {
                file << employeePtr->getId() << " " << " "
                        << leave->getType() << " " << leave->getStartDate() << " "
                        << leave->getEndDate() << " " <<leave->requiresApproval() <<" "<< leave->getDuration() << " " << std::endl;
            }            
        }
    }
};