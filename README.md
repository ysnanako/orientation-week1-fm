# orientation-week1-fm

![GitHub repo size](https://img.shields.io/github/repo-size/ysnanako/orientation-week1-fm)
![GitHub last commit](https://img.shields.io/github/last-commit/ysnanako/orientation-week1-fm)

This project is part of the **National Cheng Kung University (NCKU) - VLSI/CAD Group** course **"Physical Design for Nanometer IC"**, focusing on **2-way Partitioning using the Fiduccia-Mattheyses (FM) Algorithm**.

## 📖 Table of Contents

- [Project Overview](#project-overview)
- [Input Format](#input-format)
- [Output Format](#output-format)
- [Project Structure](#project-structure)
- [FM Algorithm Flow](#fm-algorithm-flow)
- [Example Execution](#example-execution)
- [Validation](#validation)
- [Contribution Guide](#contribution-guide)
- [Contact Information](#contact-information)

## 📝 Project Overview

This project implements **a 2-way partitioning algorithm based on the Fiduccia-Mattheyses (FM) method**, aiming to **minimize the cut size while maintaining partition balance**.

### **Key Features:**
1. **Parsing .nodes and .nets files** to extract circuit connectivity.
2. **Partitioning cells into two groups (G1 & G2) using the FM heuristic**.
3. **Computing cut size (number of nets crossing between G1 and G2)**.
4. **Using a bucket-based data structure to efficiently update gain values**.
5. **Outputting partition results including group sizes and cut size**.

## 📄 Input Format

This project follows the **UCLA benchmark format**:
- **.nodes** - Contains cell names, widths, and heights.
- **.nets** - Defines netlist connectivity.

📄 **Example .nodes File**
```
UCLA nodes 1.0
NumNodes : 211447
NumTerminals : 543
o0 8 12
o1 13 12
o2 13 12
...
```

📄 **Example .nets File**
```
UCLA nets 1.0
NumNets : 221142
NumPins : 944053
NetDegree : 4 n0
o197239 I : -0.500000 -6.000000
o197110 O : -1.500000 -3.000000
o85644 I : -6.000000 -2.000000
o0 I : -3.000000 -5.000000
```

## 📄 Output Format

The program generates a partitioned circuit file:
- **.out** - Contains cut size, group sizes, and partitioned cell assignments.

📄 **Example Output File**
```
Cut_size: 4
G1_size: 3
G2_size: 3
Time: 0.000 s
G1:
o0 o1 o2;
G2:
o4 o5 o6;
Cut_set:
n1 n2 n3 n5;
```

## 🧰 Project Structure

📂 orientation-week1-fm/  
│── 📂 src/ # (fm.cpp, fm.h, parser.cpp, parser.h, and main.cpp)  
│── 📂 obj/ # (ignored)  
│── 📄 benchmark.nodes # (ignored)  
│── 📄 benchmark.nets # (ignored)  
│── 📄 benchmark.out # (ignored)  
│── 🖥️ FM # (ignored)  
│── 🖥️ FM_checker # (ignored)  
│── 🔧 Makefile  
│── 📜 README.md # This file  
│── 📜 .gitignore  

## 🔹 **FM Algorithm Flow**

### **1. Parsing Input Files**
- Reads `.nodes` and `.nets` files.
- Extracts circuit information including **cell dimensions and net connectivity**.

### **2. Initial Partitioning**
- Cells are **initially assigned to two groups (G1 and G2) based on area balance**.

### **3. Gain Calculation & Bucket Initialization**
- Computes **gain values for each cell** based on its connected nets.
- Uses **bucket lists** to store nodes sorted by gain.

### **4. Iterative Cell Movement (FM Refinement Process)**
- **Selects the highest-gain unlocked node** and moves it to the opposite group.
- Updates **gain values dynamically** using a bucket-based approach.
- Locks moved nodes to prevent repeated selection.

### **5. Output Generation**
- The final partitioned results are saved in a `.out` file.

## ⚡ **Example Execution**

```bash
./FM adaptec1.nodes adaptec1.nets adaptec1.out
```

## ✅ Validation

### **Checker Execution**
The correctness of the FM partitioning can be verified using the `FM_checker`.

```bash
./FM_checker ./adaptec1.nodes ./adaptec1.nets ./adaptec1.out
./FM_checker ./adaptec2.nodes ./adaptec2.nets ./adaptec2.out
./FM_checker ./adaptec3.nodes ./adaptec3.nets ./adaptec3.out
./FM_checker superblue1.nodes superblue1.nets ./superblue1.out
./FM_checker superblue2.nodes superblue2.nets ./superblue2.out
```

### **Partition Evaluation Metrics**

- **Cut Size**: The number of nets spanning both partitions.
- **Group Size Ratio**: Ensures that partition sizes satisfy the balance constraint.

## 🤝 Contribution Guide

1. Fork this repository.
2. Create a new branch (`git checkout -b feature-xyz`).
3. Commit your changes (`git commit -m 'Add new feature'`).
4. Push to the remote branch (`git push origin feature-xyz`).
5. Submit a Pull Request.

## 📬 Contact Information

- 📧 Email: [m16131056@gs.ncku.edu.tw](mailto:m16131056@gs.ncku.edu.tw)
- 🌎 University: [National Cheng Kung University (NCKU)](https://www.ncku.edu.tw)
- 📖 Course: Physical Design for Nanometer IC, Fall 2024
