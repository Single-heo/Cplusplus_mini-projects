/**
 * Rectangle Management System - Main Application
 *
 * Este programa demonstra conceitos avançados de C++:
 * - Orientação a objetos com classes personalizadas
 * - Gerenciamento de containers (std::vector)
 * - Validação robusta de entrada do usuário
 * - Manipulação de objetos em tempo de execução
 *
 * Evolução de um tutorial básico da Microsoft para um sistema completo
 *
 * @author Seu Nome
 * @version 2.0
 * @date 2025
 */

#include "aula03.hpp"
#include "input.hpp"
#include <vector>
#include <iostream>
#include <chrono>
#include <thread>
#include <locale>
 /**
  * Função principal do programa
  * Coordena todo o fluxo de execução: criação, exibição e edição de retângulos
  */
int main() {
    std::locale::global(std::locale(""));
    std::cout.imbue(std::locale());
    bool isRunning = true;

    // Loop principal da aplicação - permite reiniciar o programa
    while (isRunning) {
        clearScreen();

        // ========================================
        // COLETA DO NÚMERO DE RETÂNGULOS
        // ========================================

        ushort rectangles;

        do {
            rectangles = getValidInteger("How many rectangles do you want to create? ");
        } while (rectangles < 1);

        // ========================================
        // CONTAINER PARA ARMAZENAR OS RETÂNGULOS
        // ========================================

        std::vector<Rectangle> rectangles_db;
        rectangles_db.reserve(rectangles); // Otimização: pré-aloca memória

        // ========================================
        // LOOP DE CRIAÇÃO DOS RETÂNGULOS
        // ========================================

        for (ushort i = 0; i < rectangles; i++) {
            clearScreen();
            std::cout << "\n=== Creating Rectangle " << (i + 1) << " of " << rectangles << " ===\n";

            std::string rectangle_name = getValidString("Rectangle name: ");

            ushort rectangle_base = 0;
            ushort rectangle_height = 0;
            bool isGettingValues = true;

            // Loop de validação aprimorado com múltiplas regras
            while (isGettingValues) {
                std::cout << "Enter rectangle dimensions:\n";

                rectangle_base = getValidInteger("Base length: ");
                rectangle_height = getValidInteger("Height length: ");

                if (rectangle_base == 0 || rectangle_height == 0) {
                    std::cout << "❌ Error: Base and height cannot be zero. Please enter valid values.\n";
                    std::this_thread::sleep_for(std::chrono::milliseconds(1500));
                }
                else if (rectangle_base == rectangle_height) {
                    std::cout << "❌ Error: This would create a square (base = height = "
                        << rectangle_base << "). Please enter different values for a rectangle.\n";
                    std::this_thread::sleep_for(std::chrono::milliseconds(1500));
                }
                else {
                    std::cout << "✅ Valid rectangle: " << rectangle_base << " x " << rectangle_height
                        << " (Area: " << (rectangle_base * rectangle_height) << ")\n";
                    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                    isGettingValues = false;
                }
            }

            rectangles_db.emplace_back(rectangle_name, rectangle_base, rectangle_height);
        }

        // ========================================
        // INTERFACE DE EXIBIÇÃO DOS DADOS
        // ========================================

        bool show = getYesNo("Do you want to show the rectangle information?");

        if (show) {
            clearScreen();
            std::cout << "\n=== Rectangle Database ===\n";

            // Exibição melhorada com estatísticas
            for (ushort i = 0; i < rectangles; i++) {
                std::cout << "[" << i << "] ";
                rectangles_db[i].println();
            }

            // Exibe estatísticas gerais
            if (rectangles > 1) {
                int total_area = 0;
                for (const auto& rect : rectangles_db) {
                    total_area += rect.base * rect.height;
                }

                std::cout << "\n=== STATISTICS ===\n";
                std::cout << "  Total rectangles: " << rectangles << "\n";
                std::cout << "  Combined area: " << total_area << " square units\n";
                std::cout << "  Average area: " << (total_area / rectangles) << " square units\n";
                std::cout << "========================\n\n";
            }

            // ========================================
            // SISTEMA DE EDIÇÃO OPCIONAL
            // ========================================

            bool edit_rectangle = getYesNo("Do you want to make some changes?");

            if (edit_rectangle) {
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
                clearScreen();
                
                std::cout << "Available rectangles for editing:\n";
                for (ushort i = 0; i < rectangles; i++) {
                    std::cout << "[" << i << "] " << rectangles_db[i].name
                        << " (" << rectangles_db[i].base << "x"
                        << rectangles_db[i].height << ")\n";
                }
                if (rectangles == 1) {
                    ushort rectangle_index = 0;
                    std::cout << "\nCurrent rectangle:\n";
                    rectangles_db[rectangle_index].println();

                    std::string new_name = getValidString("New name: ");
                    ushort new_base = getValidInteger("New base: ");
                    ushort new_height = getValidInteger("New height: ");

                    rectangles_db[rectangle_index].name = new_name;
                    rectangles_db[rectangle_index].base = new_base;
                    rectangles_db[rectangle_index].height = new_height;

                    std::cout << "\n✅ Rectangle updated successfully!\n";
                    rectangles_db[rectangle_index].println();
                }
                ushort rectangle_index = getIntegerInRange(
                    "Select rectangle index: ", 0, rectangles - 1);

                // Exibe informações atuais antes da edição
                std::cout << "\nCurrent rectangle:\n";
                rectangles_db[rectangle_index].println();

                std::string new_name = getValidString("New name: ");
                ushort new_base = getValidInteger("New base: ");
                ushort new_height = getValidInteger("New height: ");

                // Validação similar à criação
                if (new_base == 0 || new_height == 0) {
                    std::cout << "⚠️  Warning: Zero dimensions detected. Applying anyway...\n";
                }
                else if (new_base == new_height) {
                    std::cout << "⚠️  Warning: This creates a square. Applying anyway...\n";
                }

                rectangles_db[rectangle_index].name = new_name;
                rectangles_db[rectangle_index].base = new_base;
                rectangles_db[rectangle_index].height = new_height;

                std::cout << "\n✅ Rectangle updated successfully!\n";
                rectangles_db[rectangle_index].println();
            }
        }

        // ========================================
        // MENU DE CONTINUAÇÃO
        // ========================================

        std::cout << "\n" << std::string(40, '=') << "\n";
        std::cout << "What would you like to do next?\n";
        std::cout << "1. Restart program (create new rectangles)\n";
        std::cout << "2. Exit program\n";

        int choice = getIntegerInRange("Choose option (1-2): ", 1, 2);

        if (choice == 2) {
            isRunning = false;
            std::cout << "\n🎉 Thank you for using Rectangle Management System!\n";
            std::cout << "Program developed as evolution of Microsoft tutorial.\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
            clearScreen();
        }
        else {
            std::cout << "\n🔄 Restarting program...\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
    }

    return 0;
}