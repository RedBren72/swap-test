# Swap Test Program

This is a C program designed to stress test the system's swap usage by allocating memory until the physical swap reaches 30% usage.

## Prerequisites

- Linux system with swap configured
- GCC compiler
- Root privileges (since it accesses /proc/swaps and allocates large amounts of memory)

## Configuration

Before running, edit `swap-test.c` and change the `PHYSICAL_SWAP_DEVICE` define to match your system's physical swap device. You can find this by running `cat /proc/swaps`.

Example:
```c
#define PHYSICAL_SWAP_DEVICE "/dev/nvme0n1p3"
```

## Building

Run `make` to compile the program:

```bash
make
```

This will create an executable named `swap-test`.

## Running

Run the program with root privileges:

```bash
sudo ./swap-test
```

The program will continuously allocate memory and monitor physical swap usage until it reaches 30%, then exit automatically.

**Warning:** This program allocates large amounts of memory and can cause system instability if not monitored. Use with caution and ensure you have adequate free RAM and swap space.

## Cleaning

To remove the executable:

```bash
make clean
```

## Troubleshooting

- If the program doesn't find the swap device, double-check the `PHYSICAL_SWAP_DEVICE` path.
- Ensure you have read access to `/proc/swaps`.
- The program requires sufficient free memory to allocate chunks of 256MB.