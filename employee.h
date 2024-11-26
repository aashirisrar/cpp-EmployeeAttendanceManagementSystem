#pragma once
#include "interfaces.h"
#include "leave.h"
#include <vector>
#include <memory>
#include <iostream>

class Employee : public IObserver {
private:
    int id;
    std::string name;
    std::vector<std::unique_ptr<ILeave>> leaves;
    int workingHours;
    int casualLeaveBalance;
    int earnedLeaveBalance;

public:
    Employee(int i, const std::string& n) 
        : id(i), name(n), workingHours(0), casualLeaveBalance(15), earnedLeaveBalance(21) {}

    void applyForLeave(std::unique_ptr<ILeave> leave) {
        leaves.push_back(std::move(leave));
    }

    void addWorkingHours(int hours) {
        workingHours += hours;
    }

    void setWorkingHours(int hours) {
        workingHours = hours;
    }

    int getWorkingHours() const {
        return workingHours;
    }

    void setCasualLeaveBalance(int balance) {
        casualLeaveBalance = balance;
    }

    int getCasualLeaveBalance() const {
        return casualLeaveBalance;
    }

    void setEarnedLeaveBalance(int balance) {
        earnedLeaveBalance = balance;
    }

    int getEarnedLeaveBalance() const {
        return earnedLeaveBalance;
    }

    void update(const std::string& message) override {
        std::cout << "Employee " << name << " received notification: " << message << std::endl;
    }

    // Getters
    int getId() const { return id; }
    const std::string& getName() const { return name; }
    const std::vector<std::unique_ptr<ILeave>>& getLeaves() const { return leaves; }
};

