#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>

void RefreshList();

static void html_content(void)
{
	printf("Content-type: text/html\r\n\r\n");
}

static void html_start(const char *title)
{
	printf("<HTML>\r\n");
	printf("<HEAD>\r\n");
	printf("<TITLE>%s</TITLE>\r\n", title);
	printf("</HEAD>\r\n");
	printf("<BODY>\r\n");
}

static void html_text(const char *text)
{
	printf("%s\r\n", text);
}

static void html_end(void)
{
	printf("</BODY>\r\n");
	printf("</HTML>\r\n");
}

int main()
{
	html_content();
	html_start("ClanOrg Matches");
	html_text("<P>Welcome to the ClanOrg web match listing. While you will not be able to add or remove matches from the web, you can view a list of the currently added matches.</P>");
	html_text("<TABLE WIDTH=\"100%\">");
	html_text("<TR><TH>Clan Name</TH><TH>IRC Server</TH><TH>IRC Channel</TH><TH>Quake Mod</TH><TH>Version</TH><TH>Num. Players</TH></TR>");
	RefreshList();
	html_text("</TABLE>");
	html_end();
	exit (EXIT_SUCCESS);
}

/*
 * Read "n" bytes from a descriptor
 * Use in place of read() when fd is a stream socket
 */

int readn(int fd, char *ptr, int nbytes)
{
	int nleft, nread;

	nleft = nbytes;
	while (nleft > 0)
	{
		nread = read(fd, ptr, nleft);
		if (nread > 0)
			return(nread);
		else if (nread == 0)
			break;

		nleft -= nread;
		ptr += nread;
	}
	return (nbytes - nleft);
}

/*
 * Write "n" bytes to a descriptor
 * Use in place of write() when fd is a stream socket
 */

int writen(int fd, char *ptr, int nbytes)
{
	int nleft, nwritten;

	nleft= nbytes;
	while (nleft > 0)
	{
		nwritten = write(fd, ptr, nleft);
		if (nwritten <= 0)
			return(nwritten);

		nleft -= nwritten;
		ptr += nwritten;
	}
	return (nbytes - nleft);
}

int readuntil(int fd, char *ptr, int maxlen, char sep)
{
	int n, rc;
	char c;

	for (n=1;n<maxlen;n++)
	{
		if ( (rc = read(fd, &c, 1)) == 1)
		{
			*ptr++ = c;
			if (c == sep)
				break;
		} else if (rc == 0) {
			if (n == 1)
				return(0);
			else
				break;
		} else
			return(-1);
	}

	*ptr = 0;
	return(n);
}

void RefreshList()
{
	int sockfd;
	struct sockaddr_in address;
	struct hostent *hostinfo;
	int result;
	char buffer[256];
	char *token;
	int iTok = 0;
	char *clanname, *server, *channel, *quakemod, *quakever, *numplayers;

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = inet_addr("209.45.184.186");
	//bcopy(hostinfo->h_addr, (char *) &address.sin_addr, hostinfo->h_length);
	address.sin_port = htons(36910);


	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	
	result = connect(sockfd, (struct sockaddr *)&address, sizeof(address));
	if (result == -1)
	{
		printf("Could not connect to server.\n");
		exit(1);
	}

	strcpy(buffer, "LST");
	
	writen(sockfd, buffer, 3);

	while ( readuntil(sockfd, buffer, 255, 2) > 0)
	{

		buffer[strlen(buffer)-1] = 0;
		if (strcmp(buffer,"OK") == 0)
			break;

		iTok = 0;
		token = strtok(buffer, "\001");
		while ( token != NULL )
		{
			switch (iTok)
			{
				case 1:
					clanname = (char *)malloc(strlen(token) + 1);
					strcpy(clanname,token);
					break;
				case 2:
					channel = (char *)malloc(strlen(token) + 1);
					strcpy(channel,token);
					break;
				case 3:
					server = (char *)malloc(strlen(token) + 1);
					strcpy(server,token);
					break;
				case 4:
					quakemod = (char *)malloc(strlen(token) + 1);
					strcpy(quakemod,token);
					break;
				case 5:
					quakever = (char *)malloc(strlen(token) + 1);
					strcpy(quakever,token);
					break;
				case 6:
					numplayers = (char *)malloc(strlen(token) + 1);
					strcpy(numplayers,token);
			}
			token = strtok(NULL, "\001");
			iTok++;
		}

		sprintf(buffer,"<TR><TD>%s</TD><TD>%s</TD><TD>%s</TD><TD>%s</TD><TD>%s</TD><TD>%s</TD></TR>", clanname, server, channel, quakemod, quakever, numplayers);
		html_text(buffer);
		free(clanname);
		free(channel);
		free(server);
		free(quakever);
		free(quakemod);
		free(numplayers);
	}

	close(sockfd);
}
