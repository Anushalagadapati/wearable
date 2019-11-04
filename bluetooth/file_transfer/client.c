#include<stdio.h>
#include<unistd.h>
#include<sys/socket.h>
#include<bluetooth/bluetooth.h>
#include<bluetooth/rfcomm.h>

#define NET_BUF_SIZE 32 
#define cipherKey 'S' 
#define sendrecvflag 0 

FILE *fp1;

// function to clear buffer 
void clearBuf(char* b)
{
    int i;
    for (i = 0; i < NET_BUF_SIZE; i++)
        b[i] = '\0';
}

// function for decryption 
char Cipher(char ch)
{
    return ch ^ cipherKey;
}

// function to receive file 
int recvFile(char* buf, int s)
{
    int i,n;
    char ch;

    for (i = 0; i < s; i++) {
        ch = buf[i];
        ch = Cipher(ch);
        if (ch == EOF)
            return 1;
        else
         {
                printf("%c", ch);
                n=fputc(ch,fp1);
    }
}

    return 0;
}

int main(int argc, char **argv)
{
struct sockaddr_rc addr = { 0 };
int s, status,nBytes;
//struct addr_con;
int addrlen = sizeof(addr);
char net_buf[NET_BUF_SIZE];
char dest[18] = "01:23:45:67:89:AB";
fp1 = fopen("client.txt", "w");
// allocate a socket
s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);
 if (s < 0)
        printf("\nfile descriptor not received!!\n");
    else
        printf("\nfile descriptor %d received\n", s);

// set the connection parameters (who to connect to)
addr.rc_family = AF_BLUETOOTH;
addr.rc_channel = (uint8_t) 1;
str2ba( dest, &addr.rc_bdaddr );
// connect to server
status = connect(s, (struct sockaddr *)&addr, sizeof(addr));
// send a message
if( status == 0 ) {
//while (1) 
        {
        printf("\nPlease enter file name to receive:\n");
        scanf("%s", net_buf);
        sendto(s, net_buf, NET_BUF_SIZE,
               sendrecvflag, (struct sockaddr*)&addr,
               addrlen);

        printf("\n---------Data Received---------\n");

        while (1) {
            // receive 
            clearBuf(net_buf);
            nBytes = recvfrom(s, net_buf, NET_BUF_SIZE,
                              sendrecvflag, (struct sockaddr*)&addr,
                              &addrlen);

            // process 
            if (recvFile(net_buf, NET_BUF_SIZE)) {
                break;
            }
        }
        printf("\n-------------------------------\n");
    }
}
if( status < 0 ) perror("uh oh");
close(s);
fclose(fp1);
return 0;
}
