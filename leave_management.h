#pragma once
#include "interfaces.h"
#include "employee.h"
#include <unordered_map>
#include <algorithm>

class LeaveManagement : public ISubject {
private:
    std::unordered_map<int, std::unique_ptr<Employee>> employees;
    std::vector<IObserver*> observers;
    std::unique_ptr<ILeaveApprovalStrategy> approvalStrategy;

public:
    void setApprovalStrategy(std::unique_ptr<ILeaveApprovalStrategy> strategy) {
        approvalStrategy = std::move(strategy);
    }

    void addEmployee(std::unique_ptr<Employee> employee) {
        int id = employee->getId();
        employees[id] = std::move(employee);
    }

    void applyLeave(int employeeId, std::unique_ptr<ILeave> leave) {
        auto it = employees.find(employeeId);
        if (it != employees.end()) {
            it->second->applyForLeave(std::move(leave));
            if (approvalStrategy && approvalStrategy->approveLeave(*it->second->getLeaves().back())) {
                notify("Leave approved for employee " + std::to_string(employeeId));
            } else {
                notify("Leave rejected for employee " + std::to_string(employeeId));
            }
        }
    }

    void attach(IObserver* observer) override {
        observers.push_back(observer);
    }

    void detach(IObserver* observer) override {
        observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
    }

    void notify(const std::string& message) override {
        for (auto observer : observers) {
            observer->update(message);
        }
    }

    // Getters
    const std::unordered_map<int, std::unique_ptr<Employee>>& getEmployees() const { return employees; }
};

