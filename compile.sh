#!/bin/bash

# Script avançado para compilar projetos C++
# Uso: ./compile <arquivo> [flags_extras]

if [ $# -eq 0 ]; then
    echo "Uso: ./compile <nome_do_arquivo> [flags_extras]"
    echo "Exemplo: ./compile aula03"
    echo "Exemplo: ./compile aula03 -g -Wall"
    exit 1
fi

PROJECT_NAME=$1
shift  # Remove o primeiro argumento, mantém o resto como flags extras
EXTRA_FLAGS="$@"

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_DIR=""

# Procura pelo projeto
for dir in "$SCRIPT_DIR"/*; do
    if [ -d "$dir" ] && [ -f "$dir/src/$PROJECT_NAME.cpp" ]; then
        PROJECT_DIR="$dir"
        break
    fi
done

if [ -z "$PROJECT_DIR" ]; then
    echo "❌ Erro: Não foi possível encontrar $PROJECT_NAME.cpp"
    echo "Procurei em:"
    for dir in "$SCRIPT_DIR"/*; do
        if [ -d "$dir" ]; then
            echo "  - $dir/src/$PROJECT_NAME.cpp"
        fi
    done
    exit 1
fi

echo "🔧 Compilando $PROJECT_NAME..."
echo "📁 Projeto: $(basename "$PROJECT_DIR")"

cd "$PROJECT_DIR"

# Cria pasta compiled_code se não existir
mkdir -p compiled_code

# Monta o comando de compilação
COMPILE_CMD="g++"

# Adiciona diretórios de headers se existirem
if [ -d "headers" ]; then
    COMPILE_CMD="$COMPILE_CMD -I./headers"
fi
if [ -d "include" ]; then
    COMPILE_CMD="$COMPILE_CMD -I./include"
fi

# Encontra todos os arquivos .cpp na pasta src
CPP_FILES=$(find src -name "*.cpp" 2>/dev/null)
if [ -z "$CPP_FILES" ]; then
    echo "❌ Nenhum arquivo .cpp encontrado na pasta src/"
    exit 1
fi

# Adiciona flags extras se fornecidas
if [ -n "$EXTRA_FLAGS" ]; then
    COMPILE_CMD="$COMPILE_CMD $EXTRA_FLAGS"
fi

# Compila direto para compiled_code
COMPILE_CMD="$COMPILE_CMD $CPP_FILES -o compiled_code/$PROJECT_NAME"

echo "🚀 Executando: $COMPILE_CMD"
eval $COMPILE_CMD

if [ $? -eq 0 ]; then
    echo "✅ Compilação concluída!"
    echo "📂 Executável: $PROJECT_DIR/compiled_code/$PROJECT_NAME"
    echo "▶️  Para executar: cd $(basename "$PROJECT_DIR")/compiled_code && ./$PROJECT_NAME"
    
    # Pergunta se quer executar
    read -p "Executar agora? (y/n): " -n 1 -r
    echo
    if [[ $REPLY =~ ^[Yy]$ ]]; then
        echo "🎯 Executando $PROJECT_NAME..."
        echo "----------------------------------------"
        cd compiled_code && ./$PROJECT_NAME
    fi
else
    echo "❌ Erro na compilação"
    exit 1
fi
