/**
 * Rectangle Management System - Main Application
 *
 * This program demonstrates advanced C++ concepts:
 * - Object-Oriented Programming with custom classes
 * - Container management using std::vector
 * - Robust user input validation
 * - Dynamic object manipulation at runtime
 *
 * Author: Heitor Zanin
 * Version: 2.0
 * Date: 2025
 */
#include "rectangles.hpp"
#include "input.hpp"
#include <vector>
#include <iostream>
#include <chrono>
#include <thread>
#include <locale>
#include <fstream>
#include <filesystem>

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
            std::cout << "\n=== 🔨 Creating Rectangle " << (i + 1) << " of " << rectangleCount << " ===\n";

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
                for (const auto &rect : rectangles)
                {
                    total_area += rect.base * rect.height;
                }

                double average = static_cast<double>(total_area) / rectangleCount;

                std::cout << "\n=== 📊 STATISTICS 📊 ===\n";
                std::cout << "  Total rectangles: " << rectangleCount << "\n";
                std::cout << "  Combined area: " << total_area << " square units\n";
                std::cout << "  Average area: " << average << " square units\n";
                std::cout << "========================\n\n";
            }

            // ========= OPTIONAL EDITING SYSTEM =========
            if (getYesNo("📝 Do you want to make some changes?"))
            {
                while (true)
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
                    }
                    if (getYesNo("Do you want change other(s) rectangle(s) ?"))
                    {
                        continue;
                    }
                    break;
                }
            }
        }

        // ========= END MENU =========
        std::cout << "\n"
                  << std::string(40, '=') << "\n";
        std::cout << "What would you like to do next?\n";
        std::cout << "1. 🔄 Restart program (create new rectangles)\n";
        std::cout << "2. 🔚 Exit program\n";
        std::cout << "3. ⬇️ Save rectangle data\n";

        int choice = getIntegerInRange("Choose option (1-3): ", 1, 3);

        if (choice == 2)
        {
            isRunning = false;
            std::cout << "\nThank you for using Rectangle Management System!👍\n";
            std::cout << "💻This program is an evolved version of a Microsoft tutorial.\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(2500));
            clearScreen();
        }
        else if (choice == 1)
        {
            std::cout << "\n🔄 Restarting program...\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(750));
        }
        else if (choice == 3)
        {
            std::string name_file = "rectangles_db.txt";
            const std::string destination_path = "../rectangles_db/" + name_file;
            std::cout << "📁 Saving rectangles to " << destination_path << '\n';
            std::this_thread::sleep_for(std::chrono::milliseconds(500));

            if (std::filesystem::exists(destination_path))
            {
                std::cout << "⚠️ O arquivo '" << name_file << "' já existe.\n";
                if (getYesNo("Do you want to overwrite? "))
                {
                    std::ofstream file(destination_path, std::ios::trunc);
                    if (!file.is_open())
                    {
                        std::cerr << "❌ Error: Could not create file!\n";
                        return -1;
                    }

                    file << "=== Rectangle Database Export ===\n\n";
                    for (size_t i = 0; i < rectangles.size(); ++i)
                    {
                        file << "-------------------\n";
                        file << "[" << i << "] Rectangle name: " << rectangles[i].getName() << "\n";
                        file << " Base: " << rectangles[i].base << '\n';
                        file << " Height: " << rectangles[i].height << '\n';
                        file << " Area: " << (rectangles[i].base * rectangles[i].height) << "\n";
                        file << "-------------------\n";

                        std::cout << "✅ Rectangle '" << rectangles[i].getName() << "' saved!\n";
                    }
                    file.close();
                    std::cout << "\n🎉 All new rectangles saved successfully!\n";
                }
                else
                {
                    std::ofstream file(destination_path, std::ios::app); // Correção: garantir modo append
                    if (!file.is_open())
                    {
                        std::cerr << "❌ Error: Could not open file for appending!\n";
                        return -1;
                    }

                    file << "=== Rectangle Database Export (appended) ===\n\n";
                    for (size_t i = 0; i < rectangles.size(); ++i)
                    {
                        file << "-------------------\n";
                        file << "[" << i << "] Rectangle name: " << rectangles[i].getName() << "\n";
                        file << " Base: " << rectangles[i].base << '\n';
                        file << " Height: " << rectangles[i].height << '\n';
                        file << " Area: " << (rectangles[i].base * rectangles[i].height) << "\n";
                        file << "-------------------\n";

                        std::cout << "✅ Rectangle '" << rectangles[i].getName() << "' saved!\n";
                    }
                    file.close();
                    std::cout << "\n🎉 All rectangles appended successfully!\n";
                }
            }
            else
            {
                // Arquivo não existe, criar novo
                std::ofstream file(destination_path); // padrão é std::ios::out
                if (!file.is_open())
                {
                    std::cerr << "❌ Error: Could not create file!\n";
                    return -1;
                }

                file << "=== Rectangle Database Export ===\n\n";
                for (size_t i = 0; i < rectangles.size(); ++i)
                {
                    file << "-------------------\n";
                    file << "[" << i << "] Rectangle name: " << rectangles[i].getName() << "\n";
                    file << " Base: " << rectangles[i].base << '\n';
                    file << " Height: " << rectangles[i].height << '\n';
                    file << " Area: " << (rectangles[i].base * rectangles[i].height) << "\n";
                    file << "-------------------\n";

                    std::cout << "✅ Rectangle '" << rectangles[i].getName() << "' saved!\n";
                }
                file.close();
                std::cout << "\n🎉 All rectangles saved successfully!\n";
                std::this_thread::sleep_for(std::chrono::milliseconds(1500));
                clearScreen();
                // Removido o return 0; que interrompia o loop
            }
        }
    }
}