# Oscars Program

A C program to explore Oscars award data with filtering and search capabilities.

## Features
- Reads Oscars award data from a file.
- Filters awards by:
  - Award type (e.g., Actor, Actress, Picture).
  - Year range.
- Searches awards by:
  - Recipient name (case-insensitive).
  - Film title (case-insensitive).
- Prints individual award details and lists of awards.

## File Structure

Oscars-Program/
├── main.c       # Main program for testing functionalities.
├── oscars.c     # Implements functions declared in oscars.h.
├── oscars.h     # Header file with struct definitions and function declarations.
├── oscars.txt   # Example Oscars data file (optional, for testing).


## How to Compile
Run the following command:
```bash
gcc -o oscars main.c oscars.c

## How to Run
Run the program as follows:
```bash
./oscars