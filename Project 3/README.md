# Banker's Algorithm Implementation

This project is an implementation of the Banker's Algorithm in Python, designed to manage resource allocation in operating systems and prevent deadlocks.

## Installation and Usage

1. **Clone the Repository**

   ```bash
   git clone https://github.com/foojanbabaeeian/CECS-326.git
   cd Project 3
   ```

2. **Run the Script**
   Execute the script in your terminal:
   ```bash
   python main.py
   ```

## Project Overview

The Banker's Algorithm is used to ensure safe resource allocation to prevent deadlocks. This Python script checks if a given system state is safe and processes resource requests by simulating allocations and validating safe states.

### Key Components

- **Available Vector**: Total available units of each resource type.
- **Maximum Matrix**: Maximum resource demands for each process.
- **Allocation Matrix**: Resources currently allocated to each process.
- **Need Matrix**: Remaining resources required by each process (`Need = Maximum - Allocation`).

### Features

- **Safety Check**: Determines if the system is in a safe state.
- **Resource Request**: Simulates granting resources and validates system safety.
- **Safe Sequence**: Generates a sequence in which processes can execute safely.

### Sample Output

Test cases demonstrate the safety check and resource request functions:

1. **Initial Safety Check**:
   - Example Output: `System is initially in a safe state. Safe Sequence: [1, 3, 4, 0, 2]`
2. **Resource Request**:
   - Process requests and their results are shown, including whether the system remains in a safe state after granting requests.

### Requirements

- Python 3.11

### Contributors

- My Lu Student ID number: 029895591
- Fozhan Babaeiyan Student ID number: 029701865
