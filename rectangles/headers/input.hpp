#include <iostream>
#include <string>
#include <sstream>
#include <limits>
#include <algorithm>  // Para std::transform
#include <cctype>     // Para ::tolower e std::isspace
#include <thread>

/**
 * Função simples para limpar a tela em qualquer sistema operacional
 *
 * Windows: usa "cls"
 * Outros (Linux, macOS, Unix): usa "clear"
 */
void clearScreen() {
#ifdef _WIN32
    system("cls");    // Windows
#else
    system("clear");  // Linux, macOS, Unix, etc.
#endif
}

inline std::string trim(const std::string& s) {

    // Encontra o primeiro caractere que não é espaço em branco
    auto start = s.begin();
    while (start != s.end() && std::isspace(*start)) {
        ++start;
    }

    // Encontra o último caractere que não é espaço em branco
    auto end = s.end();
    do {
        --end;
    } while (std::distance(start, end) > 0 && std::isspace(*end));
    
    // Retorna substring do primeiro ao último caractere não-espaço
    return std::string(start, end + 1);
}

/**
 * Função para obter inteiros de forma segura com validação
 *
 * @param prompt Mensagem exibida ao usuário
 * @return int valor inteiro válido inserido pelo usuário
 *
 * Esta função garante que:
 * - O input seja um número inteiro válido
 * - Não contenha caracteres extras (como "123abc")
 * - Continue pedindo até receber input válido
 */
int getValidInteger(const std::string& prompt) {
    int value;           // Variável para armazenar o valor convertido
    std::string input;   // String para capturar toda a linha de input

    while (true) {  // Loop infinito até receber input válido
        std::cout << prompt;

        // getline() captura a linha inteira, incluindo espaços
        // É mais seguro que cin >> para input com espaços ou caracteres extras
        std::getline(std::cin, input);

        // stringstream permite tratar uma string como se fosse um stream de entrada/saída
        // Útil para conversões e parsing de strings
        std::stringstream ss(input);

        // Verificação em duas partes:
        // 1. ss >> value: tenta converter string para int
        // 2. ss.eof(): verifica se chegou ao fim (sem caracteres extras)
        if (ss >> value && ss.eof()) {
            // Sucesso: leu todos os caracteres e conseguiu converter para int
            return value;
        }

        // Se chegou aqui, o input é inválido
        std::cout << "Invalid input! Please enter a valid integer.\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        clearScreen();
    }
}

/**
 * Função para obter números de ponto flutuante de forma segura
 *
 * @param prompt Mensagem exibida ao usuário
 * @return double valor decimal válido inserido pelo usuário
 *
 * Funciona igual ao getValidInteger, mas para números decimais
 */
double getValidDouble(const std::string& prompt) {
    double value;        // Variável para armazenar o valor convertido
    std::string input;   // String para capturar toda a linha

    while (true) {  // Loop até input válido
        std::cout << prompt;
        std::getline(std::cin, input);  // Captura linha completa

        std::stringstream ss(input);    // Cria stream da string

        // Tenta converter para double E verifica se não sobrou nada
        if (ss >> value && ss.eof()) {
            return value;  // Conversão bem-sucedida
        }

        std::cout << "Invalid input! Please enter a valid number.\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        clearScreen();
    }
}

/**
 * Função para obter input de string (lida com espaços)
 *
 * @param prompt Mensagem exibida ao usuário
 * @return string string não-vazia inserida pelo usuário (sem espaços extras)
 *
 * Remove espaços do início/fim e garante que não seja uma string vazia
 */
std::string getValidString(const std::string& prompt) {
    std::string input;  // String para armazenar o input

    while (true) {  // Loop até string válida
        std::cout << prompt;
        std::getline(std::cin, input);  // Captura linha inteira (com espaços)

        // Remove espaços em branco do início e fim
        input = trim(input);

        // Verifica se a string não está vazia após o trim
        if (!input.empty()) {
            return input;  // String válida (não vazia após trim)
        }

        std::cout << "Input cannot be empty! Please try again.\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        clearScreen();
    }
}

/**
 * Função para obter resposta sim/não do usuário
 *
 * @param prompt Mensagem exibida ao usuário
 * @return bool true para sim, false para não
 *
 * Remove espaços extras, converte para minúsculo, aceita: y/yes para true, n/no para false
 */
bool getYesNo(const std::string& prompt) {
    std::string input;  // String para capturar resposta

    while (true) {  // Loop até resposta válida
        std::cout << prompt << " (y/n): ";
        std::getline(std::cin, input);  // Captura resposta

        // Remove espaços do início e fim
        input = trim(input);

        // Converte toda a string para minúsculo
        std::transform(input.begin(), input.end(), input.begin(), ::tolower);

        // Agora só precisa verificar versões em minúsculo
        if (input == "y" || input == "yes") {
            return true;
        }

        if (input == "n" || input == "no") {
            return false;
        }

        // Input inválido - pede novamente
        std::cout << "Please enter 'y' or 'n'.\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        clearScreen();
    }
}

/**
 * Função para obter inteiro dentro de um intervalo específico
 *
 * @param prompt Mensagem exibida ao usuário
 * @param min Valor mínimo aceito (inclusivo)
 * @param max Valor máximo aceito (inclusivo)
 * @return int valor inteiro válido dentro do intervalo especificado
 *
 * Combina validação de inteiro com validação de intervalo
 */
int getIntegerInRange(const std::string& prompt, int min, int max) {
    int value;  // Variável para armazenar o valor

    while (true) {  // Loop até valor no intervalo correto
        // Primeiro, obtém um inteiro válido usando a função anterior
        value = getValidInteger(prompt);

        // Verifica se está dentro do intervalo especificado
        if (value >= min && value <= max) {
            return value;  // Valor válido e dentro do intervalo
        }

        // Valor fora do intervalo - informa os limites
        std::cout << "Value must be between " << min << " and " << max << ".\n";
        clearScreen();
    }
}

