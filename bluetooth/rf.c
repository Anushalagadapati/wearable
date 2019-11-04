#include<stdio.h>
#include<unistd.h>
#include<sys/socket.h>
#include<bluetooth/bluetooth.h>
#include<bluetooth/rfcomm.h>

#define NET_BUF_SIZE 32 
#define cipherKey 'S' 
#define sendrecvflag 0 
#define nofile "File Not Found!" 


// function to clear buffer 
void clearBuf(char* b)
{
    int i;
    for (i = 0; i < NET_BUF_SIZE; i++)
        b[i] = '\0';
}

// function to encrypt 
char Cipher(char ch)
{
    return ch ^ cipherKey;
}

// function sending file 
int sendFile(FILE* fp, char* buf, int s)
{
    int i, len;
    if (fp == NULL) {
        strcpy(buf, nofile);
        len = strlen(nofile);
        buf[len] = EOF;
        for (i = 0; i <= len; i++)
            buf[i] = Cipher(buf[i]);
        return 1;
    }

    char ch, ch2;
    for (i = 0; i < s; i++) {
        ch = fgetc(fp);
        ch2 = Cipher(ch);
        buf[i] = ch2;
        if (ch == EOF)
            return 1;
    }
    return 0;
}

int main(int argc, char **argv)
{
struct sockaddr_rc loc_addr = { 0 }, rem_addr = { 0 };
int addrlen = sizeof(loc_addr);
char buf[1024] = { 0 };
int s, client, bytes_read,nBytes;
int opt = sizeof(rem_addr);
char net_buf[NET_BUF_SIZE];
    FILE* fp;
// allocate socket
s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);
// bind socket to port 1 of the first available
// local bluetooth adapter
loc_addr.rc_family = AF_BLUETOOTH;
loc_addr.rc_bdaddr = *BDADDR_ANY;
loc_addr.rc_channel = (uint8_t) 1;
bind(s, (struct sockaddr *)&loc_addr, sizeof(loc_addr));
// put socket into listening mode
listen(s, 1);
// accept one connection
client = accept(s, (struct sockaddr *)&rem_addr, &opt);
ba2str( &rem_addr.rc_bdaddr, buf );
fprintf(stderr, "accepted connection from %s\n", buf);
memset(buf, 0, sizeof(buf));
// read data from the client
/*bytes_read = read(client, buf, sizeof(buf));
if( bytes_read > 0 ) {
printf("received [%s]\n", buf);
}*/
//while (1) 
        {
        printf("\nWaiting for file name...\n");

        // receive file name 
        clearBuf(net_buf);

     /*   nBytes = recvfrom(sockfd, net_buf,
                          NET_BUF_SIZE, sendrecvflag,
                          (struct sockaddr*)&addr_con, &addrlen);*/
	printf("please enter the file name\n");
	scanf("%s",net_buf);
        fp = fopen(net_buf, "r");
        printf("\nFile Name Received: %s\n", net_buf);
        if (fp == NULL)
            printf("\nFile open failed!\n");
        else
            printf("\nFile Successfully opened!\n");

        while (1) {

            // process 
            if (sendFile(fp, net_buf, NET_BUF_SIZE)) {
                sendto(s, net_buf, NET_BUF_SIZE,
                       sendrecvflag,
                    (struct sockaddr*)&rem_addr, opt);
                break;
            }

            // send 
            sendto(s, net_buf, NET_BUF_SIZE,
                   sendrecvflag,
                (struct sockaddr*)&rem_addr, opt);
            clearBuf(net_buf);
        }
        if (fp != NULL)
            fclose(fp);
    }

// close connection
close(client);

close(s);
return 0;
}
