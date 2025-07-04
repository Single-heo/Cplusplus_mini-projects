#!/bin/bash
# Enhanced C++ Compilation Script
# Features: Auto-detection of distro, automatic g++ installation, improved error handling
# Author: Enhanced Version
# Usage: ./compile <filename> [extra_flags]

set -euo pipefail  # Exit on error, undefined vars, pipe failures

# Color codes for better output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
CYAN='\033[0;36m'
NC='\033[0m' # No Color

# Function to print colored output
print_colored() {
    local color=$1
    local message=$2
    echo -e "${color}${message}${NC}"
}

# Function to detect Linux distribution
detect_distro() {
    if [ -f /etc/os-release ]; then
        . /etc/os-release
        echo "$ID"
    elif [ -f /etc/debian_version ]; then
        echo "debian"
    elif [ -f /etc/redhat-release ]; then
        echo "rhel"
    elif [ -f /etc/arch-release ]; then
        echo "arch"
    elif [ -f /etc/alpine-release ]; then
        echo "alpine"
    else
        echo "unknown"
    fi
}

# Function to check if g++ is installed
check_gpp() {
    if command -v g++ &> /dev/null; then
        return 0
    else
        return 1
    fi
}

# Function to install g++ based on distribution
install_gpp() {
    local distro=$(detect_distro)
    
    print_colored $YELLOW "🔍 Detected distribution: $distro"
    print_colored $YELLOW "📦 Installing g++ compiler..."
    
    case "$distro" in
        "ubuntu"|"debian"|"linuxmint"|"pop"|"elementary")
            print_colored $CYAN "🔧 Using apt package manager..."
            sudo apt update && sudo apt install -y build-essential
            ;;
        "fedora"|"rhel"|"centos"|"rocky"|"almalinux")
            if command -v dnf &> /dev/null; then
                print_colored $CYAN "🔧 Using dnf package manager..."
                sudo dnf install -y gcc-c++ make
            elif command -v yum &> /dev/null; then
                print_colored $CYAN "🔧 Using yum package manager..."
                sudo yum install -y gcc-c++ make
            else
                print_colored $RED "❌ No suitable package manager found (dnf/yum)"
                exit 1
            fi
            ;;
        "arch"|"manjaro"|"endeavouros")
            print_colored $CYAN "🔧 Using pacman package manager..."
            sudo pacman -S --needed base-devel
            ;;
        "opensuse"|"sles")
            print_colored $CYAN "🔧 Using zypper package manager..."
            sudo zypper install -y gcc-c++ make
            ;;
        "alpine")
            print_colored $CYAN "🔧 Using apk package manager..."
            sudo apk add build-base
            ;;
        *)
            print_colored $RED "❌ Unsupported distribution: $distro"
            print_colored $YELLOW "Please install g++ manually using your package manager"
            print_colored $YELLOW "Common commands:"
            print_colored $YELLOW "  Debian/Ubuntu: sudo apt install build-essential"
            print_colored $YELLOW "  Fedora/RHEL: sudo dnf install gcc-c++"
            print_colored $YELLOW "  Arch: sudo pacman -S base-devel"
            exit 1
            ;;
    esac
}

# Function to show usage information
show_usage() {
    print_colored $CYAN "📖 Usage: ./compile <filename> [extra_flags]"
    print_colored $CYAN "Examples:"
    print_colored $CYAN "  ./compile aula03"
    print_colored $CYAN "  ./compile aula03 -g -Wall -std=c++17"
    print_colored $CYAN "  ./compile aula03 -O2 -DNDEBUG"
    echo
    print_colored $YELLOW "🔧 Available C++ standards: -std=c++11, -std=c++14, -std=c++17, -std=c++20"
    print_colored $YELLOW "🐛 Debug flags: -g -Wall -Wextra -pedantic"
    print_colored $YELLOW "⚡ Optimization: -O1, -O2, -O3, -Os"
}

# Check arguments
if [ $# -eq 0 ]; then
    print_colored $RED "❌ Error: No filename provided"
    show_usage
    exit 1
fi

PROJECT_NAME=$1
shift  # Remove first argument, keep rest as extra flags
EXTRA_FLAGS="$@"

# Check and install g++ if necessary
if ! check_gpp; then
    print_colored $YELLOW "⚠️  g++ compiler not found!"
    read -p "Do you want to install g++ now? (y/n): " -n 1 -r
    echo
    if [[ $REPLY =~ ^[Yy]$ ]]; then
        install_gpp
        if ! check_gpp; then
            print_colored $RED "❌ Failed to install g++. Please install manually."
            exit 1
        fi
        print_colored $GREEN "✅ g++ installed successfully!"
    else
        print_colored $RED "❌ g++ is required for compilation. Exiting."
        exit 1
    fi
fi

# Get script directory and search for project
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_DIR=""
DIRECT_CPP=false

print_colored $BLUE "🔍 Searching for $PROJECT_NAME.cpp..."

# Search for project in subdirectories
for dir in "$SCRIPT_DIR"/*; do
    if [ -d "$dir" ] && [ -f "$dir/src/$PROJECT_NAME.cpp" ]; then
        PROJECT_DIR="$dir"
        break
    fi
done

# If not found in subdirectories, check current directory
if [ -z "$PROJECT_DIR" ] && [ -f "$SCRIPT_DIR/src/$PROJECT_NAME.cpp" ]; then
    PROJECT_DIR="$SCRIPT_DIR"
fi

# If still not found, check for direct .cpp file
if [ -z "$PROJECT_DIR" ] && [ -f "$SCRIPT_DIR/$PROJECT_NAME.cpp" ]; then
    PROJECT_DIR="$SCRIPT_DIR"
    DIRECT_CPP=true
fi

if [ -z "$PROJECT_DIR" ]; then
    print_colored $RED "❌ Error: Could not find $PROJECT_NAME.cpp"
    print_colored $YELLOW "Searched in:"
    for dir in "$SCRIPT_DIR"/*; do
        if [ -d "$dir" ]; then
            echo "  - $dir/src/$PROJECT_NAME.cpp"
        fi
    done
    echo "  - $SCRIPT_DIR/src/$PROJECT_NAME.cpp"
    echo "  - $SCRIPT_DIR/$PROJECT_NAME.cpp"
    exit 1
fi

print_colored $GREEN "✅ Found project!"
print_colored $CYAN "📁 Project: $(basename "$PROJECT_DIR")"

cd "$PROJECT_DIR"

# Create compiled_code directory
mkdir -p compiled_code

# Build compilation command
COMPILE_CMD="g++"

# Add standard flags for better compilation
COMPILE_CMD="$COMPILE_CMD -std=c++17 -Wall -Wextra"

# Add include directories if they exist
for include_dir in "headers" "include" "inc"; do
    if [ -d "$include_dir" ]; then
        COMPILE_CMD="$COMPILE_CMD -I./$include_dir"
        print_colored $BLUE "📂 Added include directory: $include_dir"
    fi
done

# Find C++ source files
if [ "$DIRECT_CPP" = "true" ]; then
    CPP_FILES="$PROJECT_NAME.cpp"
else
    CPP_FILES=$(find src -name "*.cpp" 2>/dev/null || echo "")
fi

if [ -z "$CPP_FILES" ]; then
    print_colored $RED "❌ No .cpp files found!"
    exit 1
fi

print_colored $BLUE "📄 Source files found:"
for file in $CPP_FILES; do
    echo "  - $file"
done

# Add extra flags if provided
if [ -n "$EXTRA_FLAGS" ]; then
    COMPILE_CMD="$COMPILE_CMD $EXTRA_FLAGS"
    print_colored $BLUE "🔧 Extra flags: $EXTRA_FLAGS"
fi

# Final compilation command
COMPILE_CMD="$COMPILE_CMD $CPP_FILES -o compiled_code/$PROJECT_NAME"

print_colored $YELLOW "🚀 Compiling $PROJECT_NAME..."
print_colored $CYAN "Command: $COMPILE_CMD"
echo "----------------------------------------"

# Execute compilation
if eval $COMPILE_CMD; then
    print_colored $GREEN "✅ Compilation successful!"
    print_colored $CYAN "📂 Executable: $PROJECT_DIR/compiled_code/$PROJECT_NAME"
    
    # Show file size
    if [ -f "compiled_code/$PROJECT_NAME" ]; then
        FILE_SIZE=$(ls -lh "compiled_code/$PROJECT_NAME" | awk '{print $5}')
        print_colored $BLUE "📊 File size: $FILE_SIZE"
    fi
    
    print_colored $YELLOW "▶️  To run: cd $(basename "$PROJECT_DIR")/compiled_code && ./$PROJECT_NAME"
    
    # Ask if user wants to run immediately
    echo
    read -p "Run now? (y/n): " -n 1 -r
    echo
    if [[ $REPLY =~ ^[Yy]$ ]]; then
        print_colored $GREEN "🎯 Running $PROJECT_NAME..."
        echo "========================================"
        cd compiled_code && ./$PROJECT_NAME
        echo "========================================"
        print_colored $GREEN "🏁 Program finished."
    fi
else
    print_colored $RED "❌ Compilation failed!"
    print_colored $YELLOW "💡 Common solutions:"
    print_colored $YELLOW "  - Check for syntax errors in your code"
    print_colored $YELLOW "  - Verify all #include files exist"
    print_colored $YELLOW "  - Try compiling with debug flags: ./compile $PROJECT_NAME -g -Wall"
    exit 1
fi

print_colored $GREEN "🎉 Script completed successfully!"
