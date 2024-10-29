#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define the connector structure
struct connector {
    int row;  // Row position of the connector
    int col;  // Column position of the connector
    int deg;  // Initial degree (number of connections needed)
    int rem_deg;  // Remaining degree (connections still needed)
    int direc[4];  // Direction array: [up, down, left, right]
}typedef node;

// Define the garden cell structure
struct garden{
    int deg;  // Degree requirement for this cell
    int isEmpty;  // Flag to indicate if the cell is empty (1) or occupied (0)
}typedef Garden;

Garden** garden;
node* connector;

// Function prototypes
int all_valid_connections(int r, int c, int row, int col, int** connections, int size);
int is_valid(int* connection, int r, int c, int row, int col, int size, int num);
int backtrack(int r, int c, int row, int col, int size, int numofConnec);
void undo(int num, int i, int size, int numofConnec);

int main() {
    int r, c, num = 0, capacity = 20;
    scanf("%d %d", &r, &c);  // Read garden dimensions
    garden = (Garden**)malloc(sizeof(Garden*) * r);  // Allocate memory for garden grid
    connector = (node*)malloc(sizeof(node) * capacity);  // Allocate memory for connectors array

    // Read garden data and initialize structures
    for (int i = 0; i < r; i++) {
        garden[i] = (Garden*)malloc(sizeof(Garden) * c);
        for (int j = 0; j < c; j++) {
            scanf("%d", &garden[i][j].deg);  // Read cell degree
            garden[i][j].isEmpty = 1;  // Initialize as empty
            if (garden[i][j].deg != 0) {
                garden[i][j].isEmpty = 0;  // Mark as occupied if degree is non-zero
                if (num + 1 > capacity) {  // Expand capacity if needed
                    capacity *= 2;
                    connector = (node*)realloc(connector, capacity * sizeof(node));
                }
                connector[num].row = i;  // Set row of connector
                connector[num].col = j;  // Set column of connector
                connector[num].deg = garden[i][j].deg;  // Set connector degree
                connector[num].rem_deg = garden[i][j].deg;  // Initialize remaining degree
                memset(connector[num++].direc, 0, sizeof(int) * 4);  // Initialize directions
            }
        }
    }

    // Run backtracking function to find a solution
    if (backtrack(r, c, 0, 0, 0, num)) {
        for (int i = 0; i < num; i++) {
            printf("%d %d %d %d %d %d\n", connector[i].row + 1, connector[i].col + 1, connector[i].direc[0], connector[i].direc[1], connector[i].direc[2], connector[i].direc[3]);
        }
    } else {
        printf("No Solution");  // Output if no solution exists
    }
    return 0;
}

// Backtracking function to connect all connectors
int backtrack(int r, int c, int row, int col, int size, int numofConnec) {
    int flag;
    if (row == r)  // End of rows reached
        return 1;
    int next_row = row, next_col = col + 1;
    if (next_col == c) {  // End of columns reached, move to next row
        next_col = 0;
        next_row = row + 1;
    }
    if (garden[row][col].deg == 0)
        return backtrack(r, c, next_row, next_col, size, numofConnec);  // Skip non-connector cell
    else if (connector[size].rem_deg == 0)
        return backtrack(r, c, next_row, next_col, size + 1, numofConnec);  // Move to next connector if no remaining degree

    int num;  // Number of possible combinations
    switch (connector[size].rem_deg) {
        case 1: num = 2; break;
        case 2: num = 1; break;
        case 3: return 0;
        case 4: return 0;
    }

    int** connections = (int**)malloc(sizeof(int*) * num);  // Store possible connections
    for (int i = 0; i < num; i++)
        connections[i] = (int*)malloc(sizeof(int) * 4);
    all_valid_connections(r, c, row, col, connections, size);  // Fill connections

    // Try each connection and backtrack
    for (int i = 0; i < num; i++) {
        if (is_valid(connections[i], r, c, row, col, size, numofConnec)) {
            if (backtrack(r, c, next_row, next_col, size + 1, numofConnec)){
                free(connections);
                return 1;
            }
            else {
                undo(num, i, size, numofConnec);  // Undo if path invalid
            }
        }
    }
    free(connections);
    return 0;
}

// Generate all valid connections based on the remaining degree
int all_valid_connections(int r, int c, int row, int col, int** connections, int size) {
    int t = size;  // Current connector index

    if (connector[t].rem_deg > 2) return 0;  // Too many connections, return invalid

    // Set connection patterns based on remaining degree
    switch (connector[t].rem_deg) {
        case 0: {
            connections[0][0] = connector[t].direc[0];
            connections[0][1] = 0;
            connections[0][2] = connector[t].direc[2];
            connections[0][3] = 0;
            return 1;
        }
        case 1: {
            for (int i = 0; i < 2; i++) {
                connections[i][0] = connector[t].direc[0];
                connections[i][1] = 1 - i;
                connections[i][2] = connector[t].direc[2];
                connections[i][3] = i;
            }
            return 1;
        }
        case 2: {
            connections[0][0] = connector[t].direc[0];
            connections[0][1] = 1;
            connections[0][2] = connector[t].direc[2];
            connections[0][3] = 1;
            return 1;
        }
    }
}

// Check if the current connection is valid based on the garden's layout
int is_valid(int* connection, int r, int c, int row, int col, int size, int num) {
    // Check if the connection would lead to an out-of-bounds error
    if (col == c - 1 && connection[3] == 1 || 
        row == r - 1 && connection[1] == 1 || 
        col == 0 && connection[2] == 1 || 
        row == 0 && connection[0] == 1) {
        return 0;  // Invalid connection due to boundary constraints
    }
    
    int right, flag = 0;  // Initialize variables for tracking connections
    if (connection[3] == 1) {  // Check for a connection to the right
        for (int i = size + 1; i < num; i++) {  // Loop through other connectors
            if (connector[size].row == connector[i].row) {  // Check if in the same row
                // Ensure no cells are occupied between current and next connector
                for (int j = col + 1; j < connector[i].col; j++) {
                    if (garden[row][j].isEmpty == 0) {  // Found an occupied cell
                        return 0;  // Invalid connection
                    }
                }
                // Check if the next connector needs no more connections
                if (connector[i].rem_deg == 0) 
                    return 0;  // Invalid, as it cannot accept more connections

                // If no blockage, update garden status
                if (connection[1] == 0) {
                    for (int j = col + 1; j < connector[i].col; j++) {
                        garden[row][j].isEmpty = 0;  // Mark cells as occupied
                    }
                    connector[size].rem_deg--;  // Decrease remaining degree
                    connector[size].direc[3] = 1;  // Mark direction as used
                    connector[i].rem_deg--;  // Decrease next connector's degree
                    connector[i].direc[2] = 1;  // Mark the direction for next connector
                    return 1;  // Valid connection
                } else if (connection[1] == 1) {  // If already occupied down
                    right = i;  // Store the index of the next connector
                    flag = 1;  // Set flag indicating right connection attempted
                    break;  // Exit loop
                }
            }
        }
    }

    if (connection[1] == 1) {  // Check for a connection down
        for (int i = size + 1; i < num; i++) {  // Loop through other connectors
            if (connector[size].col == connector[i].col) {  // Check if in the same column
                // Ensure no cells are occupied between current and next connector
                for (int j = row + 1; j < connector[i].row; j++) {
                    if (garden[j][col].isEmpty == 0)  // Found an occupied cell
                        return 0;  // Invalid connection
                }
                // Check if the next connector needs no more connections
                if (connector[i].rem_deg == 0)
                    return 0;  // Invalid, as it cannot accept more connections
                
                // Mark garden cells as occupied
                for (int j = row + 1; j < connector[i].row; j++) {
                    garden[j][col].isEmpty = 0;  // Mark cells as occupied
                }
                connector[size].rem_deg--;  // Decrease remaining degree
                connector[size].direc[1] = 1;  // Mark downward direction as used
                connector[i].rem_deg--;  // Decrease next connector's degree
                connector[i].direc[0] = 1;  // Mark upward direction for next connector
                
                // If a right connection was attempted previously
                if (flag) {
                    for (int j = col + 1; j < connector[i].col; j++) {
                        garden[row][j].isEmpty = 0;  // Mark cells as occupied
                    }
                    connector[size].rem_deg--;  // Decrease remaining degree
                    connector[size].direc[3] = 1;  // Mark right direction as used
                    connector[right].rem_deg--;  // Decrease right connector's degree
                    connector[right].direc[2] = 1;  // Mark left direction for right connector
                }
                return 1;  // Valid connection
            }
        }
    }
    return 0;  // Return false if no valid connection found
}

// Undo the last connection made if the path is invalid
void undo(int num, int i, int size, int numofConnec) {
    int flag = 1;  // Flag to track if connections have been undone
    if (num == 2 && i == 0) {  // Handle specific case for two connections
        for (int j = size + 1; j < numofConnec; j++) {  // Loop through connectors
            if (connector[size].col == connector[j].col) {  // Same column check
                // Mark garden cells as empty in the range
                for (int i = connector[size].row + 1; i < connector[j].row; i++)
                    garden[i][connector[size].col].isEmpty = 1;  // Mark cells as empty
                // Restore remaining degrees and direction
                connector[size].rem_deg++;  
                connector[size].direc[1] = 0;  
                connector[j].rem_deg++;  
                connector[j].direc[0] = 0;  
                return;  // Exit function after undoing
            }
        }
    } else if (num == 2 && i == 1) {  // Handle case when the second connection is invalid
        for (int j = size + 1; j < numofConnec; j++) {  // Loop through connectors
            if (connector[size].row == connector[j].row) {  // Same row check
                // Mark garden cells as empty in the range
                for (int i = connector[size].col + 1; i < connector[j].col; i++)
                    garden[connector[size].row][i].isEmpty = 1;  // Mark cells as empty
                // Restore remaining degrees and direction
                connector[size].rem_deg++;  
                connector[size].direc[3] = 0;  
                connector[j].rem_deg++;  
                connector[j].direc[2] = 0;  
                return;  // Exit function after undoing
            }
        }
    } else {  // General case for undoing connections
        for (int j = size + 1; j < numofConnec; j++) {  // Loop through connectors
            if (connector[size].row == connector[j].row && flag) {  // Check same row
                // Mark garden cells as empty in the range
                for (int i = connector[size].col + 1; i < connector[j].col; i++)
                    garden[connector[size].row][i].isEmpty = 1;  // Mark cells as empty
                // Restore remaining degrees and direction
                connector[size].rem_deg++;  
                connector[size].direc[3] = 0;  
                connector[j].rem_deg++;  
                connector[j].direc[2] = 0;  
                flag = 0;  // Update flag to indicate connection was undone
            }
            if (connector[size].col == connector[j].col) {  // Check same column
                // Mark garden cells as empty in the range
                for (int i = connector[size].row + 1; i < connector[j].row; i++)
                    garden[i][connector[size].col].isEmpty = 1;  // Mark cells as empty
                // Restore remaining degrees and direction
                connector[size].rem_deg++;  
                connector[size].direc[1] = 0;  
                connector[j].rem_deg++;  
                connector[j].direc[0] = 0;  
                return;  // Exit function after undoing
            }
        }
    }
}
