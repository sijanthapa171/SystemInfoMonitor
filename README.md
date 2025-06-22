# System Info Monitor

A command-line utility for monitoring system hardware information on Linux systems. This tool provides detailed information about CPU, GPU, memory, and storage devices.


## Installation

### Using Nix (Recommended)

If you have Nix with flakes enabled:

1. Clone the repository:
   ```bash
   git clone https://github.com/sijanthapa171/SystemInfoMonitor.git
   cd SystemInfoMonitor
   ```

2. Build and run using Nix:
   ```bash
   # Build and install
   nix build
   
   # Run the installed binary
   ./result/bin/system-info-monitor
   ```

3. Development shell options:
   ```bash
   # Basic development shell (without NVIDIA support)
   nix develop

   # Development shell with NVIDIA support
   # Note: Requires allowUnfree to be enabled
   nix develop .#nvidia
   ```

   If you need NVIDIA support and encounter unfree package errors, you can:
   ```bash
   # Enable unfree packages for a single command
   NIXPKGS_ALLOW_UNFREE=1 nix develop .#nvidia --impure
   ```

   Or add the following to your `~/.config/nixpkgs/config.nix`:
   ```nix
   {
     allowUnfree = true;
   }
   ```

### Manual Installation

#### Prerequisites

* Linux operating system
* GCC compiler
* make
* For NVIDIA GPU support: nvidia-smi
* Basic system utilities: lspci, lscpu

#### Building

1. Clone the repository:
   ```bash
   git clone https://github.com/sijanthapa171/SystemInfoMonitor.git
   cd SystemInfoMonitor
   ```

2. Build the project:
   ```bash
   make
   ```

The executable will be created in the `build` directory.

## Usage

Basic usage:
```bash
./build/system-info-monitor
```

Options:
* `-c` or `--continuous`: Enable continuous monitoring mode
* `-i <seconds>` or `--interval <seconds>`: Set update interval for continuous mode (default: 1 second)

Example with continuous monitoring:
```bash
./build/system-info-monitor -c -i 2
```

## Output Example

```
=== CPU Information ===
Model: AMD Ryzen 7 5800X
Architecture: x86_64
Cores: 8
Threads: 16
Current Frequency: 3800.00 MHz
CPU Usage: 5.2%

=== GPU Information ===
Model: NVIDIA GeForce RTX 3070
Driver Version: 470.82.01
Memory Total: 8192 MB
Memory Used: 1024 MB
Temperature: 65.0°C

=== Memory Information ===
Total RAM: 32768 MB
Used RAM: 8192 MB
Free RAM: 22528 MB
Cached RAM: 2048 MB
Swap Total: 8192 MB
Swap Used: 0 MB

=== Storage Information ===
Mount Point: /
Device: /dev/nvme0n1p2
Total Space: 512 GB
Used Space: 256 GB
Free Space: 256 GB
Usage: 50.0%
```
