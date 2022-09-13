#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include <unistd.h>
 
// Define encryption function
 
#define PORT        5050
#define BUFFER_SIZE 512
#define KEY_SIZE    22
#define OK          0
#define NOT_OK      1

struct sockaddr_in server_addr;

int main(void)
{
 
    int client_fd;
    int client_socket;

    /* Create socket for client */	
	if((client_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0)
	{
		perror("Socket failed");
		return (NOT_OK);
	}

	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	server_addr.sin_port = htons(PORT);

	/* Connect to server */
	if((client_fd = connect(client_socket, 
						    (struct sockaddr * ) & server_addr, 
						    sizeof(server_addr))) < 0)
	{
		printf("Connection failed \n");
		return (NOT_OK);
	}

    /* Create plain text message and key */
	char plain_text[KEY_SIZE] = "My password is 473590";
    const char key[KEY_SIZE]        = { 140, 12, 252, 17, 24, 69, 179, 212, 76, 32, 223, 122, 175, 201, 17, 16, 87, 251, 143, 20, 21 };

    char cipher_text[KEY_SIZE];

    /* Null terminate plain text and cipher text */
    plain_text[KEY_SIZE - 1] = '\0';
    cipher_text[KEY_SIZE - 1] = '\0';

    printf("The plain text is: %s\n", plain_text);

    /* Encrypt plain text with key */
    for(int i = 0; i < KEY_SIZE; i++)
    {
        cipher_text[i] = plain_text[i] ^ key[i];
    }

    printf("The cipher text is: %s\n", cipher_text);

    // Send the message
    char buffer[BUFFER_SIZE];

    memset(buffer, '\0', BUFFER_SIZE);
    strncpy(buffer, cipher_text, KEY_SIZE);

    send(client_socket, buffer, BUFFER_SIZE, 0);
 
    // Close the communication channel
    close(client_socket);

   return (OK);
}

