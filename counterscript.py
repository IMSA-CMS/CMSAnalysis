import re

def extract_numbers_from_lines(file_path, phrase):
    try:
        # Open the file in read mode
        with open(file_path, 'r') as file:
            # Read all lines from the file
            lines = file.readlines()

            # Loop through all lines
            for line_number, line in enumerate(lines, start=1):
                # Check if the line starts with the specified phrase
                if line.startswith(phrase):
                    # Extract all numbers from the line
                    numbers = re.findall(r'\d+', line)
                    # Print the line number and the extracted numbers
                    print(numbers[0])

    except FileNotFoundError:
        print(f"Error: The file '{file_path}' was not found.")

# Example usage
file_path = 'nohup.out'  # Replace with your file path
phrase = "Number of events in file: "  # The phrase to search for
extract_numbers_from_lines(file_path, phrase)