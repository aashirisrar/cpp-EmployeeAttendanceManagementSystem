#include "user_interfaces.h"
#include <iostream>

int main() {
    AttendanceSystem system;  

    GuardInterface guardInterface(system);
    SupervisorInterface supervisorInterface(system);
    EmployeeInterface employeeInterface(system);

    while (true) {
        std::cout << "\nMain Menu:\n"
                  << "1. Guard Interface\n"
                  << "2. Supervisor Interface\n"
                  << "3. Employee Interface\n"
                  << "4. Exit\n"
                  << "Enter your choice: ";
        int choice;
        std::cin >> choice;

        switch (choice) {
            case 1:
                guardInterface.run();
                break;
            case 2:
                supervisorInterface.run();
                break;
            case 3:
                employeeInterface.run();
                break;
            case 4:
                return 0;
            default:
                std::cout << "Invalid choice. Please try again.\n";
        }
    }

    return 0;
}

