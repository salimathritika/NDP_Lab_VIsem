#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#define MAXSIZE 90

main()
{
	int sockfd,newsockfd,retval,i;
	socklen_t actuallen;
	int recedbytes,sentbytes;
	struct sockaddr_in serveraddr,clientaddr;

	char buff[MAXSIZE];
	int a=0;
	sockfd=socket(AF_INET,SOCK_DGRAM,0);

	if(sockfd==-1)
	{
		printf("\nSocket creation error");
	}
	clientaddr.sin_family=AF_INET;
	clientaddr.sin_port=htons(3389);
	clientaddr.sin_addr.s_addr=htons(INADDR_ANY);


	serveraddr.sin_family=AF_INET;
	serveraddr.sin_port=htons(3388);
	serveraddr.sin_addr.s_addr=htons(INADDR_ANY);

	retval=bind(sockfd,(struct sockaddr*)&serveraddr,sizeof(serveraddr));
	if(retval==1)
	{
		printf("Binding error");
		close(sockfd);
	}
	
	for (i = 0; ; i+=1)
	{
		
		
		actuallen=sizeof(clientaddr);
		recedbytes=recvfrom(sockfd,buff,sizeof(buff),0,(struct sockaddr*)&clientaddr,&actuallen);

		if(recedbytes==-1)
		{
			printf("Reciving error\n");
			close(sockfd);
		}
		
		//Recieved string
		int len=0;
		int a=0,e=0,vi=0,o=0,u=0;
		for(len=0;buff[len]!='\0';len++);
		int flag=1;
		if(len==0)
		  printf("No string passed\n");
		else
		{

			for(int i=0;i<len/2;i++)
			{
			  if(buff[i]!=buff[len-i-1])
			    {
			      flag=0;
			      break;
			    }
			}
			
			for(int i=0;i<len;i++)
			{
			  if(buff[i]=='a')
			    a++;
			  else if(buff[i]=='e')
			    e++;
			  else if(buff[i]=='i')
			    vi++;
			  else if(buff[i]=='o')
			    o++;
			  else if(buff[i]=='u')
			    u++;
			  
			}
		}
		  
		
		
		//puts(buff);
		//printf("\n");
		
		if (buff[0] == 'H' && buff[1] == 'a' && buff[2] == 'l' && buff[3] == 't')
		{
			break;
		}		

		if(flag)
		  sprintf(buff,"Palindrome. Length=%d. a=%d, e=%d,i=%d, o=%d, u=%d",len,a,e,vi,o,u);
		else
		  sprintf(buff,"Not Palindrome. Length=%d. a=%d, e=%d,i=%d, o=%d, u=%d",len,a,e,vi,o,u);
		sentbytes=sendto(sockfd,buff,sizeof(buff),0,(struct sockaddr*)&clientaddr,sizeof(clientaddr));
		if(sentbytes==-1)
		{
			printf("sending error");
			close(sockfd);
		}

		if (buff[0] == 'H' && buff[1] == 'a' && buff[2] == 'l' && buff[3] == 't')
		{
			break;
		}
	}

	

	close(sockfd);
	close(newsockfd);
}
