/**
 * Rectangle Management System - Main Application
 *
 * This program demonstrates advanced C++ concepts:
 * - Object-Oriented Programming with custom classes
 * - Container management using std::vector
 * - Robust user input validation
 * - Dynamic object manipulation at runtime
 *
 * Author: Your Name
 * Version: 2.0
 * Date: 2025
 */

#include "aula03.hpp"
#include "input.hpp"
#include <vector>
#include <iostream>
#include <chrono>
#include <thread>
#include <locale>

int main()
{
    // Set system locale for correct output (e.g., UTF-8, accents)
    std::locale::global(std::locale(""));
    std::cout.imbue(std::locale());

    bool isRunning = true; // Main control flag for application loop

    while (isRunning)
    {
        clearScreen(); // Clear the console screen

        // ========= USER INPUT: Number of Rectangles =========
        ushort rectangleCount;
        do
        {
            rectangleCount = getValidInteger("How many rectangles do you want to create? ");
        } while (rectangleCount < 1); // Ensure the user enters at least 1 rectangle

        // ========= CREATE CONTAINER TO STORE RECTANGLES =========
        std::vector<Rectangle> rectangles;
        rectangles.reserve(rectangleCount); // Reserve memory to avoid reallocations

        // ========= RECTANGLE CREATION LOOP =========
        for (std::size_t i = 0; i < rectangleCount; ++i)
        {
            clearScreen();
            std::cout << "\n=== Creating Rectangle " << (i + 1) << " of " << rectangleCount << " ===\n";

            std::string name = getValidString("Rectangle name: ");
            ushort base = 0, height = 0;

            // Ask for dimensions until valid (non-zero and not a square)
            while (true)
            {
                std::cout << "Enter rectangle dimensions:\n";
                base = getValidInteger("Base length: ");
                height = getValidInteger("Height length: ");

                if (base == 0 || height == 0)
                {
                    std::cout << "❌ Error: Base and height cannot be zero.\n";
                    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                }
                else if (base == height)
                {
                    std::cout << "❌ Error: Base and height cannot be equal (square).\n";
                    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                }
                else
                {
                    std::cout << "✅ Valid rectangle: " << base << " x " << height
                              << " (Area: " << (base * height) << ")\n";
                    std::this_thread::sleep_for(std::chrono::milliseconds(750));
                    break;
                }
            }

            // Store the validated rectangle in the vector
            rectangles.emplace_back(name, base, height);
        }

        // ========= DISPLAY RECTANGLE INFORMATION =========
        if (getYesNo("Do you want to show the rectangle information?"))
        {
            clearScreen();
            std::cout << "\n=== Rectangle Database ===\n";
            for (std::size_t i = 0; i < rectangles.size(); ++i)
            {
                std::cout << "[" << i << "] ";
                rectangles[i].println();
            }

            // Show general statistics (only if more than one rectangle exists)
            if (rectangleCount > 1)
            {
                int total_area = 0;
                for (const auto& rect : rectangles)
                {
                    total_area += rect.base * rect.height;
                }

                double average = static_cast<double>(total_area) / rectangleCount;

                std::cout << "\n=== STATISTICS ===\n";
                std::cout << "  Total rectangles: " << rectangleCount << "\n";
                std::cout << "  Combined area: " << total_area << " square units\n";
                std::cout << "  Average area: " << average << " square units\n";
                std::cout << "========================\n\n";
            }

            // ========= OPTIONAL EDITING SYSTEM =========
            if (getYesNo("Do you want to make some changes?"))
            {
                clearScreen();

                std::cout << "Available rectangles for editing:\n";
                for (std::size_t i = 0; i < rectangles.size(); ++i)
                {
                    std::cout << "[" << i << "] " << rectangles[i].name
                              << " (" << rectangles[i].base << "x" << rectangles[i].height << ")\n";
                }

                // Choose index (skip prompt if only one rectangle exists)
                std::size_t index = 0;
                if (rectangleCount > 1)
                {
                    index = getIntegerInRange("Select rectangle index: ", 0, rectangleCount - 1);
                }

                std::cout << "\nCurrent rectangle:\n";
                rectangles[index].println();

                // Editing loop with validation
                while (true)
                {
                    std::string new_name = getValidString("New name: ");
                    ushort new_base = getValidInteger("New base: ");
                    ushort new_height = getValidInteger("New height: ");

                    if (new_base == 0 || new_height == 0)
                    {
                        std::cout << "❌ Error: Base and height cannot be zero.\n";
                        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                        clearScreen();
                    }
                    else if (new_base == new_height)
                    {
                        std::cout << "❌ Error: Base and height cannot be equal (square).\n";
                        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                        clearScreen();
                    }
                    else
                    {
                        // Apply changes
                        rectangles[index].name = new_name;
                        rectangles[index].base = new_base;
                        rectangles[index].height = new_height;

                        std::cout << "\n✅ Rectangle updated successfully!\n";
                        rectangles[index].println();
                        break;
                    }
                }
            }
        }

        // ========= END MENU =========
        std::cout << "\n" << std::string(40, '=') << "\n";
        std::cout << "What would you like to do next?\n";
        std::cout << "1. Restart program (create new rectangles)\n";
        std::cout << "2. Exit program\n";

        int choice = getIntegerInRange("Choose option (1-2): ", 1, 2);

        if (choice == 2)
        {
            isRunning = false;
            std::cout << "\n🎉 Thank you for using Rectangle Management System!\n";
            std::cout << "This program is an evolved version of a Microsoft tutorial.\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(1500));
            clearScreen();
        }
        else
        {
            std::cout << "\n🔄 Restarting program...\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(750));
        }
    }

    return 0;
}
