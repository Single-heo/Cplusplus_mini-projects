#pragma once
#include <string>
#include <iostream>
#include <chrono>
#include <thread>
#include <cstdlib> // Added for system() function

typedef unsigned short int ushort; // Shorter alias for ushort

class Rectangle
{
public:
    std::string name{"none"}; // Default initialized to "none"
    ushort base{};            // Default initialized to 0
    ushort height{};          // Default initialized to 0
    bool haveArg{};           // Default initialized to false

    mutable ushort area_value{}; // Default initialized to 0
    void changeBase(ushort &base);
    void changeHeight(ushort &height);
    void changeName(const std::string &newName) { this->name = newName; } // Added method to change name
    std::string getName() const;
    Rectangle();                                                      // Default constructor
    Rectangle(const std::string &name, ushort &base, ushort &height); // Constructor with parameters (string passed by reference for efficiency)
    ~Rectangle();                                                     // Destructor
    short int areaf() const;                                          // Calculate area function
    void println();                                                   // Print rectangle information
    void clearScreen();                                               // Added clearScreen method declaration
};
inline std::string Rectangle::getName() const
{
    return this->name;
}

inline void Rectangle::changeBase(ushort &base)
{
    this->base = base;
}

inline void Rectangle::changeHeight(ushort &height)
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
inline Rectangle::Rectangle(const std::string &name, ushort &base, ushort &height)
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
inline short int Rectangle::areaf() const
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
    std::cout << "Rectangle [" << name << "]:" << '\n';
    std::cout << "  Base: " << base << '\n';
    std::cout << "  Height: " << height << '\n';
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    // Try to calculate area and display result
    int calculatedArea = areaf();
    if (calculatedArea >= 0)
    {
        std::cout << "  Area: " << calculatedArea << '\n';
    }
    else
    {
        std::cout << "  Area: Cannot be calculated" << '\n';
        clearScreen();
    }

    std::cout << "  Has Arguments: " << (haveArg ? "Yes" : "No") << '\n';
    std::cout << "------------------------" << '\n';
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
