#include <iostream>
#include <fstream>
#include <gromacs/fileio/xtcio.h>
#include <iomanip>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

// Function to read atom names from a .gro file
vector<string> readAtomNames(const string& groFilename) {
    ifstream groFile(groFilename);
    if (!groFile.is_open()) {
        cerr << "Error opening .gro file: " << groFilename << endl;
        exit(1);
    }

    string line;
    vector<string> atomNames;
    // Skip the first two lines (title and number of atoms)
    getline(groFile, line);
    getline(groFile, line);

    // Read atom information
    while (getline(groFile, line)) {
        if (line.find("  0.00000  0.00000  0.00000") != string::npos) {
            break; // End of atom data
        }
        // Extract atom name (columns 13-15)
        string atomName = line.substr(12, 3);

        // Remove leading & trailing spaces
        atomName.erase(atomName.find_last_not_of(' ') + 1);
        atomName.erase(0, atomName.find_first_not_of(' '));
    
        atomNames.push_back(atomName);
    }
    
    // Remove the last element (box dimensions)
    if (!atomNames.empty()) {
        atomNames.pop_back();
    }

    groFile.close();
    return atomNames;
}

int main(int argc, char* argv[]) {
    // Check if the file names were passed as arguments
    if (argc != 3) {
        cerr << "Usage: " << argv[0] << " <gro_file> <xtc_file>" << endl;
        return 1;
    }

    const string groFilename = argv[1];
    const string xtcFilename = argv[2];
    struct t_fileio* fio;

    // Open the xtc file for reading
    fio = open_xtc(xtcFilename.c_str(), "r");
    if (!fio) {
        cerr << "Error opening file " << xtcFilename << endl;
        return 1;
    }

    int natoms;
    int64_t step;
    real time;
    matrix box;
    rvec* x = nullptr; // Positions of atoms
    real prec;
    gmx_bool bOK;

    // Open the output file
    ofstream outputFile("compareWithAnalysis.xyz");
    if (!outputFile.is_open()) {
        cerr << "Error opening output file compareWithAnalysis.xyz" << endl;
        close_xtc(fio);
        return 1;
    }

    // Read the first frame from the xtc file
    if (!read_first_xtc(fio, &natoms, &step, &time, box, &x, &prec, &bOK)) {
        cerr << "Error reading first frame from file." << endl;
        close_xtc(fio);
        return 1;
    }

    // Read atom names from the .gro file
    vector<string> atomNames = readAtomNames(groFilename);
    // for (int i; i<atomNames.size(); ++i) {
    //    cout<< atomNames[i];}
    if (atomNames.size() != natoms) {
        cout << atomNames.size() << " in gro. In xtc = " << natoms << endl;
        cerr << "Number of atoms in .gro file does not match number of atoms in .xtc file." << endl;
        close_xtc(fio);
        return 1;
    }

    // Loop over all frames in the xtc file and write to the output file
    int nf;
    cout << "Enter number of Frames needed: "; cin >> nf;
    for (int i = 0; i<nf; i++)  {
         cout << "\rReading frame no: "<< i << " / " << nf << flush;
    
        outputFile << natoms << endl; // First line: number of atoms
        outputFile << "=============Frame=" << i+1 << "=========time=" << time << "ps================" << "         By Amogha|Abhishek" << endl; // Optional header for the frame

        // Loop through the atoms in the frame and write their positions
        for (int i = 0; i < natoms; ++i) {
            // Print atom name and positions with 6 decimal places
            outputFile << setw(5) << atomNames[i] << " "
                       << fixed << setprecision(6) << setw(15) << x[i][0] * 10 << " "
                       << setw(15) << x[i][1] * 10 << " "
                       << setw(15) << x[i][2] * 10 << endl;
        }

        // Read the next frame
        if (!read_next_xtc(fio, natoms, &step, &time, box, x, &prec, &bOK)) {
            cerr << "Error reading next frame." << endl;
            break;
        }
    }

    // Close the output file and xtc file after reading all frames
    outputFile.close();
    close_xtc(fio);
    delete[] x; // Free memory

    cout << endl << "Data written to compareWithAnalysis.xyz" << endl;

    return 0;
}

