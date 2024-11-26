#pragma once
#include "interfaces.h"
#include <memory>
#include <stdexcept>
#include <string>

class Leave : public ILeave {
protected:
    std::string type;
    int duration;
    bool needsApproval;
    std::string startDate;
    std::string endDate;

public:
    Leave(const std::string& t, int d, bool approval) 
        : type(t), duration(d), needsApproval(approval) {}

    std::string getType() const override { return type; }
    int getDuration() const override { return duration; }
    bool requiresApproval() const override { return needsApproval; }

    void setStartDate(const std::string& date) { startDate = date; }
    void setEndDate(const std::string& date) { endDate = date; }
    const std::string& getStartDate() const { return startDate; }
    const std::string& getEndDate() const { return endDate; }
};

class CasualLeave : public Leave {
public:
    CasualLeave(int duration) : Leave("Casual", duration, false) {}
};

class EarnedLeave : public Leave {
public:
    EarnedLeave(int duration) : Leave("Earned", duration, true) {}
};

class OfficialLeave : public Leave {
public:
    OfficialLeave(int duration) : Leave("Official", duration, false) {}
};

class UnpaidLeave : public Leave {
public:
    UnpaidLeave(int duration) : Leave("Unpaid", duration, true) {}
};

class LeaveFactory : public ILeaveFactory {
public:
    std::unique_ptr<ILeave> createLeave(const std::string& type, int duration) override {
        if (type == "Casual") return std::make_unique<CasualLeave>(duration);
        if (type == "Earned") return std::make_unique<EarnedLeave>(duration);
        if (type == "Official") return std::make_unique<OfficialLeave>(duration);
        if (type == "Unpaid") return std::make_unique<UnpaidLeave>(duration);
        throw std::invalid_argument("Invalid leave type");
    }
};

