#pragma once
#include <string>
#include <iostream>
#include <chrono>
#include <thread>
#include <cstdlib> // Added for system() function

class Rectangle
{
public:
    std::string name{"none"};              // Default initialized to "none"
    std::size_t base{};                    // Default initialized to 0
    std::size_t height{};                  // Default initialized to 0
    bool haveArg{};                        // Default initialized to false
    mutable std::size_t perimeter_value{}; // Default initialized to
    mutable std::size_t area_value{};      // Default initialized to 0
    void changeBase(std::size_t &base);
    void changeHeight(std::size_t &height);
    void changeName(const std::string &newName) { this->name = newName; }       // Added method to change name
    std::string getName() const;                                                // method to getname
    Rectangle();                                                                // Default constructor
    Rectangle(const std::string &name, std::size_t &base, std::size_t &height); // Constructor with parameters (string passed by reference for efficiency)
    ~Rectangle();                                                               // Destructor
    void println();                                                             // Print rectangle information
    void clearScreen();                                                         // Added clearScreen method declaration
    std::size_t areaf() const;                                                  // Declaration for areaf() method
};

inline std::string Rectangle::getName() const
{
    return this->name;
}

inline void Rectangle::changeBase(std::size_t &base)
{
    this->base = base;
}

inline void Rectangle::changeHeight(std::size_t &height)
{
    this->height = height;
}

// Default constructor implementation if without args :-)
Rectangle::Rectangle()
    : name("unnamed-object"), base(0), height(0), haveArg(false), area_value(0) // Member initialization list (more efficient)
{
    std::cout << "Rectangle has been initialized without arguments" << '\n';
    std::cout << "If you want to modify the variables, use constructor with args!" << '\n';
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
}

// Parameterized constructor implementation
inline Rectangle::Rectangle(const std::string &name, std::size_t &base, std::size_t &height)
    : base(base), height(height), haveArg(true), area_value(0) // Initialize members in initialization list
{
    std::cout << name << " has been initialized with arguments" << '\n';
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    // Use ternary operator for cleaner code
    this->name = name.empty() ? "none" : name;

    // Fixed condition: check if ANY dimension is zero (OR logic, not ANDd)
    if (this->height == 0 || this->base == 0)
    {
        std::cout << "Warning: In which dimension does this rectangle exist?" << '\n';
    }
    else if (this->height == this->base)
    {
        std::cout << "Warning: Rectangle is a square, base and height are equal!" << '\n';
    }
}

// Destructor implementation
inline Rectangle::~Rectangle()
{
}

// Area calculation function
inline std::size_t Rectangle::areaf() const
{
    // Check if object was initialized without proper arguments
    if (!haveArg)
    { // Cleaner than "== false"
        std::cout << "[Error] Object was initialized without proper arguments!" << '\n';
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        return -1; // Return error code immediately
    }

    // Additional validation: check for zero dimensions
    if (height == 0 || base == 0)
    {
        std::cout << "[Error] Cannot calculate area with zero dimensions!" << '\n';
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        return -1; // Return error code
    }

    // Calculate and store area
    area_value = height * base;
    return area_value;
}

// Print function implementation
inline void Rectangle::println()
{
    std::cout << "------------------------------------------\n";
    std::cout << "Rectangle [" << name << "]:" << '\n';
    std::cout << "  Base: " << base << '\n';
    std::cout << "  Height: " << height << '\n';
    std::cout << "  Perimeter: " << 2 * (height + base) << '\n';
    std::cout << "  Area: " << height * base << '\n';
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
}

// Clear screen function implementation
inline void Rectangle::clearScreen()
{
#ifdef _WIN32
    system("cls"); // Windows-specific clear screen command
#else
    system("clear"); // Unix/Linux/MacOS clear screen command
#endif
}
