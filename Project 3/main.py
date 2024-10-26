from typing import List, Tuple

class BankersAlgorithm:
    def __init__(self, available: List[int], maximum: List[List[int]], allocation: List[List[int]]):
        """
        Initializes the Banker's Algorithm with the available, maximum, and allocation matrices.
        Parameters:
            available (List[int]): The available vector representing the number of each type of resource.
            maximum (List[List[int]]): The maximum matrix specifying the maximum demand of each process for each resource.
            allocation (List[List[int]]): The allocation matrix representing the current resource allocation for each process.
        """
        self.available = available  # Available vector
        self.maximum = maximum  # Maximum matrix
        self.allocation = allocation  # Allocation matrix
        self.num_processes = len(allocation)
        self.num_resources = len(available)
        self.need = self.calculate_need()  # Calculate the Need matrix

    def calculate_need(self) -> List[List[int]]:
        """
        Calculates the Need matrix where Need[i][j] = Maximum[i][j] - Allocation[i][j].
        This matrix represents the remaining resources each process may still request.
        Returns:
            List[List[int]]: The Need matrix.
        """
        return [
            [self.maximum[i][j] - self.allocation[i][j] for j in range(self.num_resources)]
            for i in range(self.num_processes)
        ]

    def is_safe(self) -> Tuple[bool, List[int]]:
        """
        Safety algorithm to determine if the system is in a safe state.
        Returns:
            Tuple[bool, List[int]]: (True, safe_sequence) if safe, (False, []) if not.
        """
        work = self.available[:]  # Clone of available resources
        finish = [False] * self.num_processes  # Finish flag for each process
        safe_sequence = []  # Holds the safe sequence if found

        while len(safe_sequence) < self.num_processes:
            allocated = False
            for i in range(self.num_processes):
                # Check if process can be allocated resources safely
                if not finish[i] and all(self.need[i][j] <= work[j] for j in range(self.num_resources)):
                    # Allocate resources to this process temporarily
                    for j in range(self.num_resources):
                        work[j] += self.allocation[i][j]
                    finish[i] = True
                    safe_sequence.append(i)
                    allocated = True
            if not allocated:
                return False, []  # No safe sequence found
        return True, safe_sequence

    def request_resources(self, process_num: int, request: List[int]) -> bool:
        """
        Checks and grants a resource request if it keeps the system in a safe state.
        Parameters:
            process_num (int): The process number requesting resources.
            request (List[int]): The resource request vector for the process.
        Returns:
            bool: True if the request is granted, False otherwise.
        """
        # Check if the request exceeds the process's maximum or the available resources
        if any(request[j] > self.need[process_num][j] for j in range(self.num_resources)) or \
           any(request[j] > self.available[j] for j in range(self.num_resources)):
            print(f"Error: Not enough resources available for process {process_num}")
            return False

        # Temporarily allocate resources
        for j in range(self.num_resources):
            self.available[j] -= request[j]
            self.allocation[process_num][j] += request[j]
            self.need[process_num][j] -= request[j]

        # Check if system is in a safe state
        is_safe, safe_sequence = self.is_safe()
        if is_safe:
            print(f"System is in a safe state. Safe Sequence: {safe_sequence}")
            return True
        else:
            # Rollback allocation if unsafe
            for j in range(self.num_resources):
                self.available[j] += request[j]
                self.allocation[process_num][j] -= request[j]
                self.need[process_num][j] += request[j]
            print("Request cannot be granted as it leads to an unsafe state.")
            return False

# Sample usage with test cases
if __name__ == "__main__":
    # Initialize available, maximum, and allocation matrices
    available = [3, 3, 2]  # Example available resources
    maximum = [
        [7, 5, 3],
        [3, 2, 2],
        [9, 0, 2],
        [2, 2, 2],
        [4, 3, 3]
    ]
    allocation = [
        [0, 1, 0],
        [2, 0, 0],
        [3, 0, 2],
        [2, 1, 1],
        [0, 0, 2]
    ]

    bankers = BankersAlgorithm(available, maximum, allocation)

    # Test Case 1: Check initial system safety
    print("Test Case 1: Check initial system safety")
    safe, sequence = bankers.is_safe()
    if safe:
        print(f"System is initially in a safe state. Safe Sequence: {sequence}")
    else:
        print("System is initially not in a safe state.")

    # Test Case 2: Process 1 requests [1, 0, 2]
    print("\nTest Case 2: Process 1 requests [1, 0, 2]")
    request_1 = [1, 0, 2]
    bankers.request_resources(1, request_1)

    # Test Case 3: Process 4 requests [3, 3, 1]
    print("\nTest Case 3: Process 4 requests [3, 3, 1]")
    request_2 = [3, 3, 1]
    bankers.request_resources(4, request_2)

    # Additional Test Case 4: Process 0 requests [0, 2, 0]
    print("\nTest Case 4: Process 0 requests [0, 2, 0]")
    request_3 = [0, 2, 0]
    bankers.request_resources(0, request_3)

    # Additional Test Case 5: Process 2 requests [2, 0, 1]
    print("\nTest Case 5: Process 2 requests [2, 0, 1]")
    request_4 = [2, 0, 1]
    bankers.request_resources(2, request_4)

    # Additional Test Case 6: Edge case - Process 3 requests more than maximum need
    print("\nTest Case 6: Process 3 requests [2, 2, 3] (more than maximum need)")
    request_5 = [2, 2, 3]
    bankers.request_resources(3, request_5)
