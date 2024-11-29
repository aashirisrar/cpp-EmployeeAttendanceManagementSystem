#pragma once
#include <string>
#include <vector>
#include <memory>
#include <stdexcept>
#include <iostream>
#include "leave_management.h"

// Forward declaration to resolve circular dependency
class Leave;
class ILeaveState;

class ILeaveState {
public:
    virtual ~ILeaveState() = default;
    virtual void approve(Leave& leave) = 0;
    virtual void reject(Leave& leave) = 0;
    virtual std::string getStatus() const = 0;
};

class ILeave {
public:
    virtual ~ILeave() = default;
    virtual std::string getType() const = 0;
    virtual int getDuration() const = 0;
    virtual bool requiresApproval() const = 0;
    virtual void setStartDate(const std::string& date) = 0;
    virtual void setEndDate(const std::string& date) = 0;
    virtual void setState(std::unique_ptr<ILeaveState> newState) = 0;
    virtual const std::string& getStartDate() const = 0;
    virtual const std::string& getEndDate() const = 0;
    virtual std::string getStatus() const = 0;
    virtual void approve() = 0;
    virtual void reject() = 0;
};

class ILeaveFactory {
public:
    virtual ~ILeaveFactory() = default;
    virtual std::unique_ptr<ILeave> createLeave(const std::string& type, int duration) = 0;
};

class ILeaveApprovalStrategy {
public:
    virtual ~ILeaveApprovalStrategy() = default;
    virtual bool approveLeave(const ILeave& leave) = 0;
};

class IObserver {
public:
    virtual ~IObserver() = default;
    virtual void update(const std::string& message) = 0;
};

class ISubject {
public:
    virtual ~ISubject() = default;
    virtual void attach(IObserver* observer) = 0;
    virtual void detach(IObserver* observer) = 0;
    virtual void notify(const std::string& message) = 0;
};

class IReportGenerator {
public:
    virtual ~IReportGenerator() = default;
    virtual void generateReport() = 0;
};