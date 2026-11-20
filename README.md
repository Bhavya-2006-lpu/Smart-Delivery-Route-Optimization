# 🚚 Smart Delivery Management & Route Optimization System

### A C++ Based Data Structures and Algorithms Project

---

## 📌 Project Overview

The **Smart Delivery Management & Route Optimization System** is a console-based application developed using C++ and Data Structures and Algorithms (DSA).

The project aims to simplify delivery management by organizing delivery locations, roads, customer orders, and vehicles while optimizing routes through graph-based algorithms.

It provides functionalities such as order processing, vehicle capacity management, shortest-path calculation, network connectivity analysis, and delivery reporting.

The system demonstrates the practical implementation of fundamental and advanced Data Structures and Algorithms in a real-world-inspired delivery management scenario.

---

## 🎯 Project Objectives

- To develop an efficient delivery management system using C++.
- To manage delivery locations, roads, orders, and vehicles.
- To implement graph algorithms for route exploration and optimization.
- To calculate shortest delivery routes using Dijkstra's algorithm.
- To optimize network connectivity using Kruskal's Minimum Spanning Tree algorithm.
- To manage normal and urgent orders efficiently.
- To demonstrate practical applications of Data Structures and Algorithms.

---

## ⚙️ Key Features

### 1. Dashboard
Displays an overview of the delivery management system.

### 2. Location Management
- Add new delivery locations.
- Edit existing locations.
- Delete locations.

### 3. Road Management
- Add roads between locations.
- Edit road information.
- Delete road connections.
- Manage road distances.

### 4. Order Management
- Add new delivery orders.
- Update order status.
- Display all orders.
- Process the next available order.

### 5. Priority-Based Order Processing
- Normal orders are managed using a Queue (FIFO).
- Urgent orders are managed using a Priority Queue.

### 6. Vehicle Management
- Add and manage delivery vehicles.
- Assign orders to vehicles.
- Track vehicle loading capacity.
- Release vehicle capacity when an order is completed or cancelled.

### 7. Order Tracking
Provides order-related information and allows users to track delivery status.

### 8. BFS Traversal
Implements Breadth-First Search to explore connected delivery locations.

### 9. DFS Traversal
Implements Depth-First Search to explore the delivery network.

### 10. Dijkstra's Shortest Path
Calculates the shortest distance between selected delivery locations.

### 11. Kruskal's Minimum Spanning Tree
Finds a minimum-cost spanning network using Kruskal's algorithm and Disjoint Set Union.

### 12. Delivery Cost Calculator
Estimates delivery costs based on the calculated route distance and configured delivery rate.

### 13. Network Connectivity
Checks connectivity between delivery locations.

### 14. Delivery Reports
Generates delivery-related summaries and reports.

### 15. Sorting Orders
Sorts delivery orders based on selected criteria.

### 16. Searching Orders
Searches for specific delivery orders.

### 17. Prefix and Suffix Analysis
Uses prefix and suffix sums for cumulative analysis.

### 18. Route History
Maintains route history using a Stack.

---

## 🧠 Data Structures Implemented

| Data Structure | Application |
|---|---|
| Array | Adjacency Matrix representation |
| Structure | Storing locations, orders, roads and vehicles |
| Vector | Dynamic storage of project records |
| Queue | Normal order processing |
| Priority Queue | Urgent order processing |
| Stack | Route history management |
| Adjacency Matrix | Representing the delivery network |
| Disjoint Set Union (DSU) | Cycle detection and Kruskal's algorithm |

---

## 🔍 Algorithms Implemented

| Algorithm | Purpose |
|---|---|
| Breadth-First Search (BFS) | Graph traversal |
| Depth-First Search (DFS) | Graph traversal |
| Dijkstra's Algorithm | Shortest path calculation |
| Kruskal's Algorithm | Minimum Spanning Tree |
| Disjoint Set Union | Efficient component management |
| Sorting Algorithms | Organizing delivery orders |
| Searching Algorithms | Finding specific orders |
| Prefix Sum | Cumulative analysis |
| Suffix Sum | Reverse cumulative analysis |

---

## 💻 Technology Stack

- **Programming Language:** C++
- **C++ Standard:** C++17
- **IDE:** Microsoft Visual Studio
- **Core Concepts:** Data Structures and Algorithms
- **Graph Representation:** Adjacency Matrix

---

## ▶️ How to Run the Project

### Using Microsoft Visual Studio

1. Clone or download this repository.
2. Open Microsoft Visual Studio.
3. Create or open a C++ project.
4. Add the downloaded `.cpp` source file to the project.
5. Set the language standard to C++17 or later.
6. Build the solution.
7. Run the application using `Ctrl + F5`.

### Using GCC Compiler

If GCC is installed, compile the source file using:

```bash
g++ -std=c++17 Smart_Delivery_Route_Optimization_Improved.cpp -o SmartDelivery
```

Run on Windows:

```bash
SmartDelivery.exe
```

---

## 📂 Project Structure

```text
Smart-Delivery-Route-Optimization/
│
├── Smart_Delivery_Route_Optimization_Improved.cpp
│
├── README.md
│
└── .gitignore
```

---

## 📊 System Constraints

The system is designed with the following capacity limits:

- Maximum Locations: 100
- Maximum Roads: 500
- Maximum Orders: 1000
- Delivery Priority Range: 1–5
- Road Distances: Non-negative values

---

## 🚀 Future Scope

The project can be extended with additional functionalities:

- Graphical User Interface (GUI).
- Database integration for permanent data storage.
- Real-time GPS-based delivery tracking.
- Traffic-aware route optimization.
- Web-based delivery management dashboard.
- Advanced delivery analytics.

---

## ⚠️ Project Limitations

- The current application uses a console-based interface.
- Data is stored in memory during program execution.
- Real-time GPS tracking is not implemented.
- Real-time traffic information is not integrated.
- Delivery costs are estimated using a configured rate.

---

## 👩‍💻 Project Submission Details

| Field | Details |
|---|---|
| **Project Title** | Smart Delivery Management & Route Optimization System |
| **Submitted By** | Bhhavya Mishra |
| **Registration Number** | 12523737 |
| **Roll Number** | 15 |
| **Section** | K3P25BX |
| **Programme** | B.Tech CSE (Generative AI) |
| **Course** | DSA-2 (CSB213) |
| **Submitted To** | Mr. Abhay Nath Singh |
| **Institution** | Lovely Professional University |

---

## 📚 Academic Relevance

This project demonstrates the practical application of Data Structures and Algorithms in a delivery management scenario.

It integrates arrays, vectors, queues, priority queues, stacks, graph representations, traversal algorithms, shortest-path algorithms, and minimum spanning tree concepts into one application.

The project was developed as part of the **DSA-2 (CSB213)** course.

---

### ⭐ Thank You

Thank you for visiting this repository!

**Developed by Bhhavya Mishra**  
B.Tech CSE (Generative AI)  
Lovely Professional University
