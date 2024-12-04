#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>

#define PORT 8080
#define BUFFER_SIZE 1024

using namespace std;

int main() {
    int sock;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];
    
    // Create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }
    
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    
    // Connect to server (replace with actual IP address if necessary)
    if (inet_pton(AF_INET, "127.0.0.1/8", &server_addr.sin_addr) <= 0) {
        perror("Invalid address");
        exit(EXIT_FAILURE);
    }
    
    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    // Read server greeting and options
    memset(buffer, 0, BUFFER_SIZE);
    read(sock, buffer, BUFFER_SIZE);
    cout << buffer;

    memset(buffer, 0, BUFFER_SIZE);
    read(sock, buffer, BUFFER_SIZE);
    cout << buffer;
    
    // Send choice (Register or Login)
    string choice;
    cout << "Enter your choice: ";
    cin >> choice;
    cin.ignore(); // Ignore any remaining newline characters in the input buffer.
    send(sock, choice.c_str(), choice.length(), 0);
    
    // Handle registration or login based on choice
    if (choice == "1") {
        string username, password;
        cout << "Enter username: ";
        getline(cin, username);
        cout << "Enter password: ";
        getline(cin, password);

        // Send username and password to server
        send(sock, username.c_str(), username.length(), 0);
        send(sock, "\n", 1, 0); // Send newline to separate username and password
        send(sock, password.c_str(), password.length(), 0);
        cout << "Registering..." << endl;
    } else if (choice == "2") {
        cout << "Logging in..." << endl;
    }
    
    // Close connection
    close(sock);
    return 0;
}
