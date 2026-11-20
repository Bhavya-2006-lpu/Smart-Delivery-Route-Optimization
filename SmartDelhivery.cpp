#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <string>
#include <algorithm>
#include <iomanip>
#include <climits>

using namespace std;

// Maximum number of locations
const int MAX_LOCATIONS = 100;
const int MAX_ROADS = 500;
const int MAX_ORDERS = 1000;
const int INF = INT_MAX;

// Structure to store location details
struct Location {
    string id;
    string name;
    bool active = true;
};

// Structure to store order details
struct Order {
    string orderId;
    int source;
    int destination;
    int priority;
    double weight;

    string status = "Pending";
    string vehicleId = "Not Assigned";
    bool vehicleCapacityReleased = false;
};

// Structure to store road details
struct Road {
    int source;
    int destination;
    int distance;
};
// Structure to store vehicle details
struct Vehicle {
    string vehicleId;
    string vehicleName;
    double capacity;
    double loadedWeight = 0;
};

// Global data structures
vector<Location> locations;
vector<Order> orders;
vector<Vehicle> vehicles;

// Adjacency matrix for graph representation
int graph[MAX_LOCATIONS][MAX_LOCATIONS] = { 0 };

// Queue for normal orders
queue<int> normalOrders;

// Priority queue for urgent orders
priority_queue<pair<int, int>> urgentOrders;

// Stack for storing route history


// Release assigned vehicle capacity when an order is completed or cancelled.
void releaseVehicleCapacity(Order& order) {
    if (order.vehicleId == "Not Assigned" ||
        order.vehicleCapacityReleased) return;

    for (int i = 0; i < vehicles.size(); i++) {
        if (vehicles[i].vehicleId == order.vehicleId) {
            vehicles[i].loadedWeight -= order.weight;
            if (vehicles[i].loadedWeight < 0)
                vehicles[i].loadedWeight = 0;
            order.vehicleCapacityReleased = true;
            return;
        }
    }
}
stack<string> routeHistory;


// Find location using its ID
int findLocation(string id) {

    for (int i = 0; i < locations.size(); i++) {

        if (locations[i].id == id && locations[i].active) {
            return i;
        }
    }

    return -1;
}


// Check all IDs, including deleted locations, to prevent ID reuse.
bool locationIdExists(string id) {
    for (int i = 0; i < locations.size(); i++) {
        if (locations[i].id == id) return true;
    }
    return false;
}


// Find order using its ID
int findOrder(string id) {

    for (int i = 0; i < orders.size(); i++) {

        if (orders[i].orderId == id) {
            return i;
        }
    }

    return -1;
}


// Add sample locations, roads and orders
void addSampleData() {

    locations.push_back({ "L1", "Warehouse", true });
    locations.push_back({ "L2", "North_Hub", true });
    locations.push_back({ "L3", "South_Hub", true });
    locations.push_back({ "L4", "East_Hub", true });
    locations.push_back({ "L5", "West_Hub", true });
    locations.push_back({ "L6", "Hospital", true });
    locations.push_back({ "L7", "Mall", true });
    locations.push_back({ "L8", "Airport", true });

    // Roads are undirected
    graph[0][1] = graph[1][0] = 4;
    graph[0][2] = graph[2][0] = 6;
    graph[0][3] = graph[3][0] = 5;
    graph[1][4] = graph[4][1] = 3;
    graph[1][5] = graph[5][1] = 7;
    graph[2][5] = graph[5][2] = 2;
    graph[2][6] = graph[6][2] = 5;
    graph[3][6] = graph[6][3] = 3;
    graph[3][7] = graph[7][3] = 8;
    graph[4][6] = graph[6][4] = 6;
    graph[5][7] = graph[7][5] = 4;
    graph[6][7] = graph[7][6] = 5;

    // Sample orders
    orders.push_back({ "O101", 0, 5, 5, 2.5 });
    orders.push_back({ "O102", 1, 6, 2, 1.2 });
    orders.push_back({ "O103", 2, 7, 4, 3.0 });
    orders.push_back({ "O104", 3, 4, 1, 0.8 });
    orders.push_back({ "O105", 4, 5, 3, 1.5 });

    // Place orders into their respective queues
    for (int i = 0; i < orders.size(); i++) {

        if (orders[i].priority >= 4) {
            urgentOrders.push({ orders[i].priority, i });
        }
        else {
            normalOrders.push(i);
        }
    }
}


// Display all locations
void displayLocations() {

    cout << "\n----- LOCATION DETAILS -----\n";

    for (int i = 0; i < locations.size(); i++) {

        if (locations[i].active) {
            cout << locations[i].id
                << " : " << locations[i].name << endl;
        }
    }
}


// Display all roads
void displayRoads() {

    cout << "\n----- ROAD DETAILS -----\n";

    for (int i = 0; i < locations.size(); i++) {

        for (int j = i + 1; j < locations.size(); j++) {

            if (locations[i].active &&
                locations[j].active &&
                graph[i][j] > 0) {

                cout << locations[i].name
                    << " <--> "
                    << locations[j].name
                    << " : "
                    << graph[i][j] << " km\n";
            }
        }
    }
}


// Add a new location
void addLocation() {

    if (locations.size() >= MAX_LOCATIONS) {
        cout << "Location limit reached!\n";
        return;
    }

    string id, name;

    cout << "Enter Location ID: ";
    cin >> id;

    if (locationIdExists(id)) {
        cout << "Location ID already exists!\n";
        return;
    }

    cout << "Enter Location Name: ";
    cin >> name;

    locations.push_back({ id, name, true });

    cout << "Location added successfully!\n";
}


// Edit an existing location
void editLocation() {

    string id;

    cout << "Enter Location ID: ";
    cin >> id;

    int index = findLocation(id);

    if (index == -1) {
        cout << "Location not found!\n";
        return;
    }

    cout << "Enter new location name: ";
    cin >> locations[index].name;

    cout << "Location updated successfully!\n";
}


// Delete a location
void deleteLocation() {

    string id;

    cout << "Enter Location ID: ";
    cin >> id;

    int index = findLocation(id);

    if (index == -1) {
        cout << "Location not found!\n";
        return;
    }

    // Prevent deleting a location used by a pending delivery.
    for (int i = 0; i < orders.size(); i++) {
        if (orders[i].status == "Pending" &&
            (orders[i].source == index || orders[i].destination == index)) {
            cout << "Cannot delete this location: a pending order uses it!\\n";
            return;
        }
    }

    locations[index].active = false;

    // Remove all roads connected to this location
    for (int i = 0; i < locations.size(); i++) {

        graph[index][i] = 0;
        graph[i][index] = 0;
    }

    cout << "Location deleted successfully!\n";
}


// Count active undirected roads in the adjacency matrix.
int countActiveRoads() {
    int count = 0;
    for (int i = 0; i < locations.size(); i++) {
        for (int j = i + 1; j < locations.size(); j++) {
            if (locations[i].active && locations[j].active &&
                graph[i][j] > 0) count++;
        }
    }
    return count;
}

// Add, edit or delete a road
void manageRoad(int choice) {

    string sourceId, destinationId;

    cout << "Enter source location ID: ";
    cin >> sourceId;

    cout << "Enter destination location ID: ";
    cin >> destinationId;

    int source = findLocation(sourceId);
    int destination = findLocation(destinationId);

    if (source == -1 || destination == -1 ||
        source == destination) {

        cout << "Invalid locations!\n";
        return;
    }

    if (choice == 1 && graph[source][destination] > 0) {
        cout << "Road already exists!\n";
        return;
    }

    if (choice == 1 && countActiveRoads() >= MAX_ROADS) {
        cout << "Maximum 500 roads allowed!\\n";
        return;
    }

    if (choice != 1 && graph[source][destination] == 0) {
        cout << "Road does not exist!\n";
        return;
    }

    if (choice == 3) {

        graph[source][destination] = 0;
        graph[destination][source] = 0;

        cout << "Road deleted successfully!\n";
        return;
    }

    int distance;

    cout << "Enter road distance in km: ";
    cin >> distance;

    if (distance <= 0) {
        cout << "Invalid distance!\n";
        return;
    }

    graph[source][destination] = distance;
    graph[destination][source] = distance;

    if (choice == 1) {
        cout << "Road added successfully!\n";
    }
    else {
        cout << "Road updated successfully!\n";
    }
}


// Add a new delivery order
void addOrder() {

    if (orders.size() >= MAX_ORDERS) {
        cout << "Maximum 1000 orders allowed!\\n";
        return;
    }

    Order newOrder;

    string sourceId, destinationId;

    cout << "Enter Order ID: ";
    cin >> newOrder.orderId;

    if (findOrder(newOrder.orderId) != -1) {
        cout << "Order ID already exists!\n";
        return;
    }

    cout << "Enter source location ID: ";
    cin >> sourceId;

    cout << "Enter destination location ID: ";
    cin >> destinationId;

    newOrder.source = findLocation(sourceId);
    newOrder.destination = findLocation(destinationId);

    if (newOrder.source == -1 ||
        newOrder.destination == -1 ||
        newOrder.source == newOrder.destination) {

        cout << "Invalid locations!\n";
        return;
    }

    cout << "Enter priority (1-5): ";
    cin >> newOrder.priority;

    cout << "Enter parcel weight in kg: ";
    cin >> newOrder.weight;

    if (newOrder.priority < 1 ||
        newOrder.priority > 5 ||
        newOrder.weight <= 0) {

        cout << "Invalid order details!\n";
        return;
    }

    int index = orders.size();

    orders.push_back(newOrder);

    // Priority 4 and 5 are treated as urgent orders
    if (newOrder.priority >= 4) {

        urgentOrders.push({ newOrder.priority, index });

        cout << "Urgent order added successfully!\n";
    }
    else {

        normalOrders.push(index);

        cout << "Normal order added successfully!\n";
    }
}


// Display all orders
void displayOrders() {

    cout << "\n----- ORDER DETAILS -----\n";

    cout << left
        << setw(12) << "Order ID"
        << setw(15) << "Source"
        << setw(15) << "Destination"
        << setw(10) << "Priority"
        << setw(10) << "Weight"
        << "Status\n";

    for (int i = 0; i < orders.size(); i++) {

        cout << left
            << setw(12) << orders[i].orderId
            << setw(15) << locations[orders[i].source].name
            << setw(15) << locations[orders[i].destination].name
            << setw(10) << orders[i].priority
            << setw(10) << orders[i].weight
            << orders[i].status << endl;
    }
}


// Complete or cancel an order
void updateOrderStatus() {

    string id, action;

    cout << "Enter Order ID: ";
    cin >> id;

    int index = findOrder(id);

    if (index == -1) {
        cout << "Order not found!\n";
        return;
    }

    if (orders[index].status != "Pending") {
        cout << "Order is not pending!\n";
        return;
    }

    cout << "Enter action (complete/cancel): ";
    cin >> action;

    if (action == "complete" || action == "cancel") {
        releaseVehicleCapacity(orders[index]);
    }

    if (action == "complete") {
        orders[index].status = "Completed";
    }
    else if (action == "cancel") {
        orders[index].status = "Cancelled";
    }
    else {
        cout << "Invalid action!\n";
        return;
    }

    cout << "Order status updated successfully!\n";
}


// Process the next order
void processNextOrder() {

    // Remove completed or cancelled urgent orders
    while (!urgentOrders.empty() &&
        orders[urgentOrders.top().second].status != "Pending") {

        urgentOrders.pop();
    }

    // Remove completed or cancelled normal orders
    while (!normalOrders.empty() &&
        orders[normalOrders.front()].status != "Pending") {

        normalOrders.pop();
    }

    // Urgent orders are processed first
    if (!urgentOrders.empty()) {

        int index = urgentOrders.top().second;

        urgentOrders.pop();

        releaseVehicleCapacity(orders[index]);
        orders[index].status = "Completed";

        cout << "Urgent order processed: "
            << orders[index].orderId << endl;
    }

    else if (!normalOrders.empty()) {

        int index = normalOrders.front();

        normalOrders.pop();

        releaseVehicleCapacity(orders[index]);
        orders[index].status = "Completed";

        cout << "Normal order processed: "
            << orders[index].orderId << endl;
    }

    else {
        cout << "No pending orders available!\n";
    }
}


// Breadth First Search
void BFS(int start) {

    bool visited[MAX_LOCATIONS] = { false };

    queue<int> q;

    q.push(start);
    visited[start] = true;

    cout << "\nBFS Traversal: ";

    while (!q.empty()) {

        int current = q.front();
        q.pop();

        cout << locations[current].name << " -> ";

        for (int i = 0; i < locations.size(); i++) {

            if (locations[i].active &&
                graph[current][i] > 0 &&
                !visited[i]) {

                visited[i] = true;
                q.push(i);
            }
        }
    }

    cout << "END\n";
}


// Depth First Search
void DFSHelper(int current, bool visited[]) {

    visited[current] = true;

    cout << locations[current].name << " -> ";

    for (int i = 0; i < locations.size(); i++) {

        if (locations[i].active &&
            graph[current][i] > 0 &&
            !visited[i]) {

            DFSHelper(i, visited);
        }
    }
}


void DFS(int start) {

    bool visited[MAX_LOCATIONS] = { false };

    cout << "\nDFS Traversal: ";

    DFSHelper(start, visited);

    cout << "END\n";
}


// Dijkstra's shortest path algorithm
void dijkstra(int source, int destination) {

    int distance[MAX_LOCATIONS];
    int parent[MAX_LOCATIONS];

    bool visited[MAX_LOCATIONS] = { false };

    for (int i = 0; i < MAX_LOCATIONS; i++) {

        distance[i] = INF;
        parent[i] = -1;
    }

    distance[source] = 0;

    for (int count = 0; count < locations.size(); count++) {

        int current = -1;

        for (int i = 0; i < locations.size(); i++) {

            if (locations[i].active &&
                !visited[i] &&
                (current == -1 ||
                    distance[i] < distance[current])) {

                current = i;
            }
        }

        if (current == -1 || distance[current] == INF) {
            break;
        }

        visited[current] = true;

        for (int i = 0; i < locations.size(); i++) {

            if (locations[i].active &&
                graph[current][i] > 0 &&
                distance[current] != INF &&
                distance[current] + graph[current][i] < distance[i]) {

                distance[i] = distance[current] + graph[current][i];

                parent[i] = current;
            }
        }
    }

    if (distance[destination] == INF) {

        cout << "No route available!\n";
        return;
    }

    vector<int> path;

    int current = destination;

    while (current != -1) {

        path.push_back(current);
        current = parent[current];
    }

    reverse(path.begin(), path.end());

    string route = "";

    cout << "\nShortest Route: ";

    for (int i = 0; i < path.size(); i++) {

        cout << locations[path[i]].name;

        route += locations[path[i]].name;

        if (i != path.size() - 1) {
            cout << " -> ";
            route += " -> ";
        }
    }

    cout << "\nTotal Distance: "
        << distance[destination] << " km\n";

    // Save route in stack
    routeHistory.push(route);
}


// Disjoint Set Union for Kruskal's algorithm
class DSU {

    vector<int> parent;
    vector<int> size;

public:

    DSU(int n) {

        parent.resize(n);
        size.resize(n, 1);

        for (int i = 0; i < n; i++) {
            parent[i] = i;
        }
    }

    int findParent(int node) {

        if (parent[node] == node) {
            return node;
        }

        return parent[node] = findParent(parent[node]);
    }

    bool unionSets(int u, int v) {

        u = findParent(u);
        v = findParent(v);

        if (u == v) {
            return false;
        }

        if (size[u] < size[v]) {
            swap(u, v);
        }

        parent[v] = u;
        size[u] += size[v];

        return true;
    }
};


// Kruskal's Minimum Spanning Tree
void kruskal() {

    vector<Road> roads;

    // Collect all available roads
    for (int i = 0; i < locations.size(); i++) {

        for (int j = i + 1; j < locations.size(); j++) {

            if (locations[i].active &&
                locations[j].active &&
                graph[i][j] > 0) {

                roads.push_back({ i, j, graph[i][j] });
            }
        }
    }

    // Sort roads by distance
    sort(roads.begin(), roads.end(),
        [](Road a, Road b) {
            return a.distance < b.distance;
        });

    DSU dsu(locations.size());

    int totalCost = 0;
    int selectedRoads = 0;
    int activeLocations = 0;

    for (int i = 0; i < locations.size(); i++) {

        if (locations[i].active) {
            activeLocations++;
        }
    }

    cout << "\n----- MINIMUM SPANNING TREE -----\n";

    for (int i = 0; i < roads.size(); i++) {

        if (dsu.unionSets(roads[i].source,
            roads[i].destination)) {

            cout << locations[roads[i].source].name
                << " -- "
                << locations[roads[i].destination].name
                << " : "
                << roads[i].distance << " km\n";

            totalCost += roads[i].distance;
            selectedRoads++;
        }
    }

    cout << "Total Network Cost: "
        << totalCost << " km\n";

    if (selectedRoads < activeLocations - 1) {
        cout << "Network is disconnected. "
            << "Minimum spanning forest displayed.\n";
    }
}


// Sort orders according to priority
void sortOrders() {

    vector<Order> sortedOrders = orders;

    sort(sortedOrders.begin(), sortedOrders.end(),
        [](Order a, Order b) {

            if (a.priority != b.priority) {
                return a.priority > b.priority;
            }

            return a.orderId < b.orderId;
        });

    cout << "\n----- SORTED ORDERS -----\n";

    for (int i = 0; i < sortedOrders.size(); i++) {

        cout << sortedOrders[i].orderId
            << " | Priority: "
            << sortedOrders[i].priority
            << " | Status: "
            << sortedOrders[i].status << endl;
    }
}


// Search an order
void searchOrder() {

    string id;

    cout << "Enter Order ID to search: ";
    cin >> id;

    int index = findOrder(id);

    if (index == -1) {
        cout << "Order not found!\n";
        return;
    }

    cout << "\nOrder Found!\n";

    cout << "Order ID: " << orders[index].orderId << endl;

    cout << "Source: "
        << locations[orders[index].source].name << endl;

    cout << "Destination: "
        << locations[orders[index].destination].name << endl;

    cout << "Priority: "
        << orders[index].priority << endl;

    cout << "Weight: "
        << orders[index].weight << " kg\n";

    cout << "Status: "
        << orders[index].status << endl;
}


// Calculate prefix and suffix sums of pending order weights
void calculatePrefixSuffix() {

    vector<double> weights;

    for (int i = 0; i < orders.size(); i++) {

        if (orders[i].status == "Pending") {
            weights.push_back(orders[i].weight);
        }
    }

    int n = weights.size();

    if (n == 0) {
        cout << "No pending orders!\n";
        return;
    }

    vector<double> prefix(n);
    vector<double> suffix(n);

    // Prefix sum calculation
    prefix[0] = weights[0];

    for (int i = 1; i < n; i++) {

        prefix[i] = prefix[i - 1] + weights[i];
    }

    // Suffix sum calculation
    suffix[n - 1] = weights[n - 1];

    for (int i = n - 2; i >= 0; i--) {

        suffix[i] = suffix[i + 1] + weights[i];
    }

    cout << fixed << setprecision(2);

    cout << "\nPrefix Sums: ";

    for (int i = 0; i < n; i++) {
        cout << prefix[i] << " ";
    }

    cout << "\nSuffix Sums: ";

    for (int i = 0; i < n; i++) {
        cout << suffix[i] << " ";
    }

    cout << "\nTotal Pending Weight: "
        << prefix[n - 1] << " kg\n";
}


// Display route history using stack
void displayRouteHistory() {

    if (routeHistory.empty()) {

        cout << "No route history available!\n";
        return;
    }

    stack<string> temp = routeHistory;

    cout << "\n----- ROUTE HISTORY -----\n";

    while (!temp.empty()) {

        cout << temp.top() << endl;

        temp.pop();
    }
}
// Display the main dashboard
void displayDashboard() {

    int totalLocations = 0;
    int pendingOrders = 0;
    int completedOrders = 0;
    int cancelledOrders = 0;
    int urgentOrdersCount = 0;
    int normalOrdersCount = 0;

    double pendingWeight = 0;

    for (int i = 0; i < locations.size(); i++) {

        if (locations[i].active) {
            totalLocations++;
        }
    }

    for (int i = 0; i < orders.size(); i++) {

        if (orders[i].status == "Pending") {

            pendingOrders++;
            pendingWeight += orders[i].weight;

            if (orders[i].priority >= 4) {
                urgentOrdersCount++;
            }
            else {
                normalOrdersCount++;
            }
        }

        else if (orders[i].status == "Completed") {
            completedOrders++;
        }

        else if (orders[i].status == "Cancelled") {
            cancelledOrders++;
        }
    }

    cout << "\n";
    cout << "================================================\n";
    cout << "              SYSTEM DASHBOARD                  \n";
    cout << "================================================\n";

    cout << "\nNETWORK OVERVIEW\n";
    cout << "Active Locations       : " << totalLocations << endl;

    cout << "\nORDER OVERVIEW\n";
    cout << "Total Orders           : " << orders.size() << endl;
    cout << "Pending Orders         : " << pendingOrders << endl;
    cout << "Completed Orders       : " << completedOrders << endl;
    cout << "Cancelled Orders       : " << cancelledOrders << endl;

    cout << "\nPRIORITY OVERVIEW\n";
    cout << "Urgent Orders          : " << urgentOrdersCount << endl;
    cout << "Normal Orders          : " << normalOrdersCount << endl;

    cout << "\nVEHICLE OVERVIEW\n";
    cout << "Registered Vehicles    : " << vehicles.size() << endl;

    cout << "\nDELIVERY OVERVIEW\n";
    cout << fixed << setprecision(2);

    cout << "Pending Parcel Weight  : "
        << pendingWeight << " kg\n";

    cout << "\n================================================\n";
}
// Track a particular order
void trackOrder() {

    string id;

    cout << "\nEnter Order ID: ";
    cin >> id;

    int index = findOrder(id);

    if (index == -1) {

        cout << "\nOrder not found!\n";
        return;
    }

    Order order = orders[index];

    cout << "\n====================================\n";
    cout << "           ORDER TRACKING           \n";
    cout << "====================================\n";

    cout << "Order ID       : " << order.orderId << endl;

    cout << "Source         : "
        << locations[order.source].name << endl;

    cout << "Destination    : "
        << locations[order.destination].name << endl;

    cout << "Priority       : " << order.priority << endl;

    cout << "Parcel Weight  : " << order.weight << " kg\n";

    cout << "Status         : " << order.status << endl;

    cout << "Vehicle        : " << order.vehicleId << endl;

    cout << "====================================\n";
}
// Manage delivery vehicles
void manageVehicles() {

    int choice;

    do {

        cout << "\n====================================\n";
        cout << "          VEHICLE MANAGEMENT        \n";
        cout << "====================================\n";

        cout << "1. Add Vehicle\n";
        cout << "2. View All Vehicles\n";
        cout << "3. Assign Order to Vehicle\n";
        cout << "0. Back to Main Menu\n";

        cout << "\nEnter choice: ";
        cin >> choice;

        if (choice == 1) {

            Vehicle newVehicle;

            cout << "Enter Vehicle ID: ";
            cin >> newVehicle.vehicleId;

            bool alreadyExists = false;

            for (int i = 0; i < vehicles.size(); i++) {

                if (vehicles[i].vehicleId == newVehicle.vehicleId) {
                    alreadyExists = true;
                    break;
                }
            }

            if (alreadyExists) {
                cout << "Vehicle ID already exists!\n";
                continue;
            }

            cout << "Enter Vehicle Name: ";
            cin >> newVehicle.vehicleName;

            cout << "Enter Maximum Capacity (kg): ";
            cin >> newVehicle.capacity;

            if (newVehicle.capacity <= 0) {
                cout << "Invalid vehicle capacity!\n";
                continue;
            }

            newVehicle.loadedWeight = 0;

            vehicles.push_back(newVehicle);

            cout << "Vehicle added successfully!\n";
        }

        else if (choice == 2) {

            cout << "\n----- VEHICLE DETAILS -----\n";

            if (vehicles.empty()) {
                cout << "No vehicles registered!\n";
            }

            for (int i = 0; i < vehicles.size(); i++) {

                cout << "\nVehicle ID: "
                    << vehicles[i].vehicleId;

                cout << "\nVehicle Name: "
                    << vehicles[i].vehicleName;

                cout << "\nMaximum Capacity: "
                    << vehicles[i].capacity << " kg";

                cout << "\nLoaded Weight: "
                    << vehicles[i].loadedWeight << " kg";

                cout << "\nAvailable Capacity: "
                    << vehicles[i].capacity -
                    vehicles[i].loadedWeight << " kg\n";
            }
        }

        else if (choice == 3) {

            string orderId, vehicleId;

            cout << "Enter Order ID: ";
            cin >> orderId;

            int orderIndex = findOrder(orderId);

            if (orderIndex == -1) {
                cout << "Order not found!\n";
                continue;
            }

            if (orders[orderIndex].status != "Pending") {
                cout << "Only pending orders can be assigned!\n";
                continue;
            }

            if (orders[orderIndex].vehicleId != "Not Assigned") {
                cout << "Order already has a vehicle!\n";
                continue;
            }

            cout << "Enter Vehicle ID: ";
            cin >> vehicleId;

            int vehicleIndex = -1;

            for (int i = 0; i < vehicles.size(); i++) {

                if (vehicles[i].vehicleId == vehicleId) {
                    vehicleIndex = i;
                    break;
                }
            }

            if (vehicleIndex == -1) {
                cout << "Vehicle not found!\n";
                continue;
            }

            double requiredWeight = orders[orderIndex].weight;

            double availableCapacity =
                vehicles[vehicleIndex].capacity -
                vehicles[vehicleIndex].loadedWeight;

            if (requiredWeight > availableCapacity) {

                cout << "Vehicle capacity exceeded!\n";
                cout << "Available capacity: "
                    << availableCapacity << " kg\n";

                continue;
            }

            vehicles[vehicleIndex].loadedWeight += requiredWeight;

            orders[orderIndex].vehicleId = vehicleId;
            orders[orderIndex].vehicleCapacityReleased = false;

            cout << "Order assigned successfully!\n";
        }

        else if (choice != 0) {

            cout << "Invalid choice!\n";
        }

    } while (choice != 0);
}
// Calculate shortest distance between two locations
int calculateShortestDistance(int source, int destination) {

    int distance[MAX_LOCATIONS];

    bool visited[MAX_LOCATIONS] = { false };

    for (int i = 0; i < MAX_LOCATIONS; i++) {
        distance[i] = INF;
    }

    distance[source] = 0;

    for (int count = 0; count < locations.size(); count++) {

        int current = -1;

        for (int i = 0; i < locations.size(); i++) {

            if (locations[i].active &&
                !visited[i] &&
                (current == -1 ||
                    distance[i] < distance[current])) {

                current = i;
            }
        }

        if (current == -1 || distance[current] == INF) {
            break;
        }

        visited[current] = true;

        for (int i = 0; i < locations.size(); i++) {

            if (locations[i].active &&
                graph[current][i] > 0 &&
                distance[current] + graph[current][i] < distance[i]) {

                distance[i] = distance[current] + graph[current][i];
            }
        }
    }

    return distance[destination];
}
// Calculate estimated delivery cost
void calculateDeliveryCost() {

    string sourceId, destinationId;

    cout << "\nEnter Source Location ID: ";
    cin >> sourceId;

    cout << "Enter Destination Location ID: ";
    cin >> destinationId;

    int source = findLocation(sourceId);
    int destination = findLocation(destinationId);

    if (source == -1 || destination == -1) {

        cout << "Invalid location!\n";
        return;
    }

    if (source == destination) {

        cout << "Source and destination are the same!\n";
        return;
    }

    int distance = calculateShortestDistance(source, destination);

    if (distance == INF) {

        cout << "No available delivery route!\n";
        return;
    }

    const double COST_PER_KM = 8.0;

    double totalCost = distance * COST_PER_KM;

    cout << "\n====================================\n";
    cout << "        DELIVERY COST ESTIMATE      \n";
    cout << "====================================\n";

    cout << "Source: " << locations[source].name << endl;

    cout << "Destination: "
        << locations[destination].name << endl;

    cout << "Shortest Distance: " << distance << " km\n";

    cout << "Cost Per KM: Rs. " << COST_PER_KM << endl;

    cout << fixed << setprecision(2);

    cout << "Estimated Delivery Cost: Rs. "
        << totalCost << endl;

    cout << "====================================\n";
}
// Check network connectivity using BFS
void checkNetworkConnectivity() {

    string id;

    cout << "\nEnter starting location ID: ";
    cin >> id;

    int start = findLocation(id);

    if (start == -1) {

        cout << "Invalid location!\n";
        return;
    }

    bool visited[MAX_LOCATIONS] = { false };

    queue<int> q;

    q.push(start);
    visited[start] = true;

    int reachableLocations = 0;

    while (!q.empty()) {

        int current = q.front();
        q.pop();

        reachableLocations++;

        for (int i = 0; i < locations.size(); i++) {

            if (locations[i].active &&
                graph[current][i] > 0 &&
                !visited[i]) {

                visited[i] = true;
                q.push(i);
            }
        }
    }

    int totalLocations = 0;

    for (int i = 0; i < locations.size(); i++) {

        if (locations[i].active) {
            totalLocations++;
        }
    }

    cout << "\n====================================\n";
    cout << "       NETWORK CONNECTIVITY         \n";
    cout << "====================================\n";

    cout << "Total Active Locations: "
        << totalLocations << endl;

    cout << "Reachable Locations: "
        << reachableLocations << endl;

    if (reachableLocations == totalLocations) {

        cout << "Network Status: Fully Connected\n";
    }

    else {

        cout << "Network Status: Partially Connected\n";
    }

    cout << "====================================\n";
}
// Generate delivery report
void generateDeliveryReport() {

    int pending = 0;
    int completed = 0;
    int cancelled = 0;

    double pendingWeight = 0;
    double completedWeight = 0;

    int totalRoads = 0;
    int totalRoadDistance = 0;

    for (int i = 0; i < orders.size(); i++) {

        if (orders[i].status == "Pending") {

            pending++;
            pendingWeight += orders[i].weight;
        }

        else if (orders[i].status == "Completed") {

            completed++;
            completedWeight += orders[i].weight;
        }

        else if (orders[i].status == "Cancelled") {

            cancelled++;
        }
    }

    for (int i = 0; i < locations.size(); i++) {

        for (int j = i + 1; j < locations.size(); j++) {

            if (locations[i].active &&
                locations[j].active &&
                graph[i][j] > 0) {

                totalRoads++;
                totalRoadDistance += graph[i][j];
            }
        }
    }

    cout << "\n====================================\n";
    cout << "           DELIVERY REPORT          \n";
    cout << "====================================\n";

    cout << "Total Orders: " << orders.size() << endl;

    cout << "Pending Orders: " << pending << endl;

    cout << "Completed Orders: " << completed << endl;

    cout << "Cancelled Orders: " << cancelled << endl;

    cout << fixed << setprecision(2);

    cout << "Pending Parcel Weight: "
        << pendingWeight << " kg\n";

    cout << "Completed Parcel Weight: "
        << completedWeight << " kg\n";

    cout << "\nNETWORK REPORT\n";

    cout << "Total Roads: " << totalRoads << endl;

    cout << "Total Road Distance: "
        << totalRoadDistance << " km\n";

    cout << "\nVEHICLE REPORT\n";

    cout << "Registered Vehicles: "
        << vehicles.size() << endl;

    cout << "====================================\n";
}


// Main function
int main() {

    addSampleData();

    int choice;

    do {

        cout << "\n";
        cout << "+==================================================+\n";
        cout << "|                                                  |\n";
        cout << "|          SMART DELIVERY MANAGEMENT               |\n";
        cout << "|               ROUTE OPTIMIZATION                 |\n";
        cout << "|                                                  |\n";
        cout << "+==================================================+\n";

        cout << "\n                    MAIN MENU\n";

        cout << "----------------------------------------------------\n";

        cout << "  [1]  Dashboard\n";
        cout << "  [2]  Location Management\n";
        cout << "  [3]  Road Management\n";

        cout << "\n  [4]  Add New Order\n";
        cout << "  [5]  Update Order Status\n";
        cout << "  [6]  Display All Orders\n";
        cout << "  [7]  Process Next Order\n";

        cout << "\n  [8]  Vehicle Management\n";
        cout << "  [9]  Order Tracking\n";

        cout << "\n  [10] BFS Traversal\n";
        cout << "  [11] DFS Traversal\n";
        cout << "  [12] Dijkstra Shortest Path\n";
        cout << "  [13] Kruskal Minimum Spanning Tree\n";

        cout << "\n  [14] Delivery Cost Calculator\n";
        cout << "  [15] Network Connectivity\n";
        cout << "  [16] Delivery Reports\n";

        cout << "\n  [17] Sort Orders\n";
        cout << "  [18] Search Order\n";
        cout << "  [19] Prefix & Suffix Analysis\n";
        cout << "  [20] Route History\n";

        cout << "\n  [21]   Exit\n";

        cout << "----------------------------------------------------\n";

        cout << "Enter your choice: ";
        cin >> choice;

        string id;
        int start, destination;

        switch (choice) {

        case 1:
            displayDashboard();
            break;


            // LOCATION MANAGEMENT
        case 2: {
            int locationChoice;

            do {
                cout << "\n========== LOCATION MANAGEMENT ==========\n";
                cout << "1. Add Location\n";
                cout << "2. Edit Location\n";
                cout << "3. Delete Location\n";
                cout << "4. Display Locations\n";
                cout << "0. Back to Main Menu\n";

                cout << "Enter choice: ";
                cin >> locationChoice;

                switch (locationChoice) {

                case 1:
                    addLocation();
                    break;

                case 2:
                    editLocation();
                    break;

                case 3:
                    deleteLocation();
                    break;

                case 4:
                    displayLocations();
                    break;

                case 0:
                    cout << "Returning to Main Menu...\n";
                    break;

                default:
                    cout << "Invalid choice!\n";
                }

            } while (locationChoice != 0);

            break;
        }


              // ROAD MANAGEMENT
        case 3: {
            int roadChoice;

            do {
                cout << "\n============ ROAD MANAGEMENT ============\n";
                cout << "1. Add Road\n";
                cout << "2. Edit Road\n";
                cout << "3. Delete Road\n";
                cout << "4. Display All Roads\n";
                cout << "0. Back to Main Menu\n";

                cout << "Enter choice: ";
                cin >> roadChoice;

                switch (roadChoice) {

                case 1:
                    manageRoad(1);
                    break;

                case 2:
                    manageRoad(2);
                    break;

                case 3:
                    manageRoad(3);
                    break;

                case 4:
                    displayRoads();
                    break;

                case 0:
                    cout << "Returning to Main Menu...\n";
                    break;

                default:
                    cout << "Invalid choice!\n";
                }

            } while (roadChoice != 0);

            break;
        }


              // ORDER MANAGEMENT
        case 4:
            addOrder();
            break;

        case 5:
            updateOrderStatus();
            break;

        case 6:
            displayOrders();
            break;

        case 7:
            processNextOrder();
            break;


            // VEHICLE MANAGEMENT
        case 8:
            manageVehicles();
            break;


            // ORDER TRACKING
        case 9:
            trackOrder();
            break;


            // BFS
        case 10: {
            string id;

            displayLocations();

            cout << "Enter starting location ID: ";
            cin >> id;

            int start = findLocation(id);

            if (start == -1) {
                cout << "Invalid location!\n";
            }
            else {
                BFS(start);
            }

            break;
        }


               // DFS
        case 11: {
            string id;

            displayLocations();

            cout << "Enter starting location ID: ";
            cin >> id;

            int start = findLocation(id);

            if (start == -1) {
                cout << "Invalid location!\n";
            }
            else {
                DFS(start);
            }

            break;
        }


               // DIJKSTRA SHORTEST PATH
        case 12: {
            string sourceId, destinationId;

            displayLocations();

            cout << "Enter source location ID: ";
            cin >> sourceId;

            cout << "Enter destination location ID: ";
            cin >> destinationId;

            int source = findLocation(sourceId);
            int destination = findLocation(destinationId);

            if (source == -1 || destination == -1) {
                cout << "Invalid location!\n";
            }
            else if (source == destination) {
                cout << "Source and destination cannot be the same!\n";
            }
            else {
                dijkstra(source, destination);
            }

            break;
        }


               // KRUSKAL MINIMUM SPANNING TREE
        case 13:
            kruskal();
            break;


            // DELIVERY COST CALCULATOR
        case 14:
            calculateDeliveryCost();
            break;


            // NETWORK CONNECTIVITY
        case 15:
            checkNetworkConnectivity();
            break;


            // DELIVERY REPORTS
        case 16:
            generateDeliveryReport();
            break;


            // SORT ORDERS
        case 17:
            sortOrders();
            break;


            // SEARCH ORDER
        case 18:
            searchOrder();
            break;


            // PREFIX AND SUFFIX SUM
        case 19:
            calculatePrefixSuffix();
            break;


            // ROUTE HISTORY
        case 20:
            displayRouteHistory();
            break;


            // EXIT
        case 21:
            cout << "\nThank you for using Smart Delivery System!\n";
            cout << "Have a great day!\n";
            return 0;  // Exit immediately; do not display the main menu again.


            // INVALID CHOICE
        default:
            cout << "\nInvalid choice! Please try again.\n";
        }

    } while (choice != 0);

    return 0;
}