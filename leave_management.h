#pragma once
#include "interfaces.h"
#include "employee.h"
#include <unordered_map>
#include <algorithm>

class LeaveManagement : public ISubject {
private:
    std::unordered_map<int, std::unique_ptr<Employee>> employees;
    std::vector<IObserver*> observers;

public:
    void addEmployee(std::unique_ptr<Employee> employee) {
        int id = employee->getId();
        employees[id] = std::move(employee);
    }

    void applyLeave(int employeeId, std::unique_ptr<ILeave> leave) {
        auto it = employees.find(employeeId);
        if (it != employees.end()) {
            it->second->applyForLeave(std::move(leave));
            auto& currentLeave = it->second->getLeaves().back();
            if (currentLeave->getStatus() == "Approved") {
                notify("Leave approved for employee " + std::to_string(employeeId));
            } else if(currentLeave->getStatus() == "Pending"){
                notify("Leave pending for employee " + std::to_string(employeeId));
            } else if(currentLeave->getStatus() == "Rejected") {
                notify("Leave rejected for employee " + std::to_string(employeeId));
            }
            attach(it->second.get());
        }
    }

    void attach(IObserver* observer) override {
        if (observer && std::find(observers.begin(), observers.end(), observer) == observers.end()) {
            observers.push_back(observer);
        }
    }

    void detach(IObserver* observer) override {
        observers.erase(
            std::remove(observers.begin(), observers.end(), observer),
            observers.end()
        );
    }

    void notify(const std::string& message) override {
        for (auto* observer : observers) {
            if (observer) {
                observer->update(message);
            }
        }
    }

    // Getters
    const std::unordered_map<int, std::unique_ptr<Employee>>& getEmployees() const { return employees; }
};

