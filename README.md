# GROMACS XTC Frame Extractor

This C++ program reads atom positions from an XTC trajectory file and writes them to an XYZ format file, while also including atom names extracted from a GRO file. The program allows you to extract and write multiple frames to an output file, which can then be used for further analysis or visualization.

## Features

- **Read .gro file:** Atom names are extracted from the `.gro` file.
- **Read .xtc file:** Atom positions from the `.xtc` trajectory file are read.
- **Write XYZ file:** Atom positions and names are written to an `.xyz` file for each frame in the trajectory.
- **Customizable frame count:** The user can specify how many frames from the XTC file they want to process.

## Requirements

This program requires the following libraries:

- **GROMACS library:** This code uses the GROMACS file I/O library for reading `.xtc` files.
  - Make sure GROMACS is installed and configured on your system.
  
- **C++11 or higher:** The program uses modern C++ features, so it requires a C++11 or higher compliant compiler.

## Compilation

To compile the program, you need to link it against the GROMACS library. Here’s an example of how to compile it using `g++`:

    g++ -o xtc_reader xtc_reader.cpp -lgromacs

## Usage

Once compiled, you can run the program as follows:

    ./xtc_reader <gro_file> <xtc_file>

### Example:

    ./xtc_reader ../md_0_1.gro ../md_0_1.xtc

## Example Output (compareWithAnalysis.xyz):

```bash
1000
=============Frame=1=========time=1.000000ps================         By Amogha|Abhishek
   H    0.001234   0.002345   0.003456
   O    0.004567   0.005678   0.006789
   ...
1000
=============Frame=2=========time=2.000000ps================         By Amogha|Abhishek
   H    0.010234   0.020345   0.030456
   O    0.040567   0.050678   0.060789
   ...
```

Where each frame contains:

    The number of atoms
    A header with the frame number and timestamp
    Atom names and their positions (in nanometers converted to angstroms) for each atom in the frame.

## Acknowledgments

**GROMACS:** This code uses GROMACS for reading and writing trajectory files. Please refer to the official GROMACS documentation for more information.
