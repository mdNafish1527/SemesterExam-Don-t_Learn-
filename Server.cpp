#include <iostream>
#include <fstream>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <sstream>

#define PORT 8080
#define BUFFER_SIZE 1024

using namespace std;

bool registerUser(int client_sock) {
    char buffer[BUFFER_SIZE];
    string username, password;
    
    // Receive username and password
    read(client_sock, buffer, BUFFER_SIZE);
    username = string(buffer);
    
    memset(buffer, 0, BUFFER_SIZE);
    read(client_sock, buffer, BUFFER_SIZE);
    password = string(buffer);
    
    // Check if user file already exists
    ifstream infile(username + ".txt");
    if (infile.good()) {
        write(client_sock, "Username already exists.\n", 24);
        return false;
    }
    
    // Store username and password in file
    ofstream outfile(username + ".txt");
    outfile << username << endl;
    outfile << password << endl;
    outfile.close();
    
    write(client_sock, "Registration successful.\n", 25);
    return true;
}

int main() {
    int server_fd, new_sock;
    struct sockaddr_in address;
    int addrlen = sizeof(address);  // Correct the address length
    char buffer[BUFFER_SIZE];
    
    // Create socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }
    
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;  // Listen on all interfaces
    address.sin_port = htons(PORT);
    
    // Bind the socket
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }
    
    // Listen for connections
    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }
    
    cout << "Server is listening..." << endl;
    
    while (true) {
        // Accept incoming connection
        if ((new_sock = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            perror("Accept failed");
            exit(EXIT_FAILURE);
        }
        
        // Handle registration and login
        write(new_sock, "Welcome to the server!\n", 23);
        write(new_sock, "1. Register\n2. Login\n", 21);
        
        read(new_sock, buffer, BUFFER_SIZE);
        string choice = string(buffer);
        
        if (choice == "1") {
            if (registerUser(new_sock)) {
                write(new_sock, "You can now login.\n", 19);
            }
        } else if (choice == "2") {
            // Handle login (similar to register function)
            write(new_sock, "Login functionality coming soon.\n", 32);
        }
        
        // Close the connection
        close(new_sock);
    }
    
    return 0;
}
