#pragma once
#include "leave_management.h"
#include "reports.h"
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

    bool markAttendance(int employeeId, int hours) {
        auto& employees = leaveManagement.getEmployees();
        auto it = employees.find(employeeId);
        if (it != employees.end()) {
            it->second->addWorkingHours(hours);
            return 1;
        }

        return 0;
    }

    void applyLeave(int employeeId, const std::string& leaveType, int duration, const std::string& startDate, const std::string& endDate) {
        auto leave = leaveFactory->createLeave(leaveType, duration);
        leave->setStartDate(startDate);
        leave->setEndDate(endDate);
        leaveManagement.applyLeave(employeeId, std::move(leave));
    }

    void generateAttendanceReport() {
        AttendanceReport report(this->leaveManagement);
        report.generateReport();
    }

    void generateLeaveReport() {
        LeaveReport report(this->leaveManagement);
        report.generateReport();
    }

    void generateOutStandingLeaveReport() {
        OutstandingLeaveReport report(this->leaveManagement);
        report.generateReport();
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

    void approveLeave()
    {
        bool show = 0;
        std::cout << "Pending leaves:" << std::endl;
        for (const auto &[id, employee] : leaveManagement.getEmployees())
        {
            for (const auto &leave : employee->getLeaves())
            {
                if (leave->requiresApproval() && leave->getStatus() == "Pending")
                {
                    show = 1;
                    std::cout << "Employee " << employee->getName() << " (ID: " << id << "):" << std::endl;
                    std::cout << "Type: " << leave->getType()
                              << ", From: " << leave->getStartDate() << ", To: " << leave->getEndDate() << " Duration: " << leave->getDuration() << " days" << std::endl
                              << std::endl;
                }
            }
        }

        if (show)
        {
            int employeeId;
            std::cout << "Enter employee ID to proceed to approval: ";
            std::cin >> employeeId;

            auto &employees = leaveManagement.getEmployees();
            auto it = employees.find(employeeId);

            if (it != employees.end())
            {
                for (const auto &leave : it->second->getLeaves())
                {
                    if (leave->requiresApproval() && leave->getStatus() == "Pending")
                    {
                        int c;
                        std::cout << "Enter 1 to approve and 0 to reject: ";
                        std::cin >> c;
                        if (c == 1)
                        {
                            leave->approve();
                        }
                        else if (c == 0)
                        {
                            leave->reject();
                        }
                        else
                        {
                            std::cout << "Invalid Input!<<" << std::endl;
                        }
                    }
                }
            }
            else
            {
                std::cout << "Employee not found." << std::endl;
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
            std::string type, startDate, endDate, status;
            int employeeId, duration;
            bool approved;
            iss >> employeeId >> type >> startDate >> endDate >> approved >> duration >> status;
            auto leave = leaveFactory->createLeave(type, duration);
            leave->setStartDate(startDate);
            leave->setEndDate(endDate);
            if (status == "Pending")
            {
                leave->setState(std::unique_ptr<ILeaveState>(new PendingState()));
            }
            else if (status == "Approved")
            {
                leave->setState(std::unique_ptr<ILeaveState>(new ApprovedState()));
            }
            else if (status == "Rejected")
            {
                leave->setState(std::unique_ptr<ILeaveState>(new RejectedState()));
            }
            leaveManagement.applyLeave(employeeId, std::move(leave));
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
                        << leave->getEndDate() << " " <<leave->requiresApproval() <<" "<< leave->getDuration() << " " << leave->getStatus() << std::endl;
            }            
        }
    }

    bool addEmployee(int employeeId, const std::string &name)
    {
        auto &employees = leaveManagement.getEmployees();
        auto it = employees.find(employeeId);
        if (it == employees.end())
        {
            auto employee = std::make_unique<Employee>(employeeId, name);
            leaveManagement.addEmployee(std::move(employee));
            std::cout << "Employee added successfully. Employee ID: " << employeeId << std::endl;
            return 1;
        }

        std::cout << "Error: Employee with ID " << employeeId << " already exists.\n";
        return 0;
    }

    bool checkLeaveBalance(int employeeId, const std::string &type, int duration)
    {
        int casualLeaves=0, earnedLeaves=0;
        auto &employees = leaveManagement.getEmployees();
        auto it = employees.find(employeeId);
        if (it != employees.end())
        {
            if (type == "Casual")
            {
                casualLeaves = it->second->getCasualLeaveBalance();
                
                if(casualLeaves - duration > 0) return 1;
            }
            else if (type == "Earned")
            {
                earnedLeaves = it->second->getEarnedLeaveBalance();

                if(earnedLeaves - duration > 0) return 1;
            }
        }

        return 0;
    }
};