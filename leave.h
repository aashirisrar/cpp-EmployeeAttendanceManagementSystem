#pragma once
#include "interfaces.h"
#include <memory>
#include <stdexcept>
#include <string>

// Base Leave class implementation first
class Leave : public ILeave {
protected:
    std::string type;
    int duration;
    bool needsApproval;
    std::unique_ptr<ILeaveState> state;
    std::string startDate;
    std::string endDate;

public:
    Leave(const std::string& t, int d, bool approval) 
        : type(t), duration(d), needsApproval(approval) {}

    std::string getType() const override { return type; }
    int getDuration() const override { return duration; }
    bool requiresApproval() const override { return needsApproval; }

    void setStartDate(const std::string& date) override { startDate = date; }
    void setEndDate(const std::string& date) override { endDate = date; }
    const std::string& getStartDate() const override { return startDate; }
    const std::string& getEndDate() const override { return endDate; }

    void setState(std::unique_ptr<ILeaveState> newState) override {
        state = std::move(newState);
    }

    void approve() override {
        if (state) {
            state->approve(*this);
        }
    }

    void reject() override {
        if (state) {
            state->reject(*this);
        }
    }

    std::string getStatus() const override {
        return state ? state->getStatus() : "Unknown";
    }
};

// Complete State class implementations before using them
class ApprovedState : public ILeaveState {
public:
    void approve(Leave& leave) override {
        std::cout << "Leave is already approved." << std::endl;
    }

    void reject(Leave& leave) override {
        std::cout << "Cannot reject an approved leave." << std::endl;
    }

    std::string getStatus() const override {
        return "Approved";
    }
};

class RejectedState : public ILeaveState {
public:
    void approve(Leave& leave) override {
        std::cout << "Cannot approve a rejected leave." << std::endl;
    }

    void reject(Leave& leave) override {
        std::cout << "Leave is already rejected." << std::endl;
    }

    std::string getStatus() const override {
        return "Rejected";
    }
};

// Now PendingState can use the complete types
class PendingState : public ILeaveState {
public:
    void approve(Leave& leave) override {
        auto newState = std::unique_ptr<ILeaveState>(new ApprovedState());
        leave.setState(std::move(newState));
        std::cout << "Leave approved." << std::endl;
    }

    void reject(Leave& leave) override {
        auto newState = std::unique_ptr<ILeaveState>(new RejectedState());
        leave.setState(std::move(newState));
        std::cout << "Leave rejected." << std::endl;
    }

    std::string getStatus() const override {
        return "Pending";
    }
};

// Concrete leave types
class CasualLeave : public Leave {
public:
    CasualLeave(int duration) : Leave("Casual", duration, false) {
        auto initialState = std::unique_ptr<ILeaveState>(new ApprovedState());
        setState(std::move(initialState));
    }
};

class EarnedLeave : public Leave {
public:
    EarnedLeave(int duration) : Leave("Earned", duration, true) {
        auto initialState = std::unique_ptr<ILeaveState>(new PendingState());
        setState(std::move(initialState));
    }
};

class OfficialLeave : public Leave {
public:
    OfficialLeave(int duration) : Leave("Official", duration, false) {
        auto initialState = std::unique_ptr<ILeaveState>(new ApprovedState());
        setState(std::move(initialState));
    }
};

class UnpaidLeave : public Leave {
public:
    UnpaidLeave(int duration) : Leave("Unpaid", duration, true) {
        auto initialState = std::unique_ptr<ILeaveState>(new PendingState());
        setState(std::move(initialState));
    }
};

class LeaveFactory : public ILeaveFactory {
public:
    std::unique_ptr<ILeave> createLeave(const std::string& type, int duration) override {
        if (type == "Casual") return std::make_unique<CasualLeave>(duration);
        if (type == "Earned") return std::make_unique<EarnedLeave>(duration);
        if (type == "Official") return std::make_unique<OfficialLeave>(duration);
        if (type == "Unpaid") return std::make_unique<UnpaidLeave>(duration);
        throw std::invalid_argument("Invalid leave type: " + type);
    }
};