#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pcap/pcap.h>
#include "fatal.h"


int main() {
	struct pcap_pkthdr header;
	const u_char *packet;
	char errbuf[PCAP_ERRBUF_SIZE];
	char *dev;
	pcap_t *pcap_handler;
	int i;

	dev = pcap_lookupdev(errbuf);
	if (dev == NULL) 
		fatal("Can't look up!");
	printf("Started sniffing on %s\n", dev);

	pcap_handler = pcap_open_live(dev, 4096, 1, 0, errbuf);
	if (pcap_handler == NULL)
		fatal("Cant open sniffing");
	packet = pcap_next(pcap_handler, &header);

	#define BUFFERSIZE 4096
	char* buffer = malloc(BUFFERSIZE);
	printf("Packet contents: \n");
	dump(packet, header.len);	
	free(buffer);
	pcap_close(pcap_handler);
	printf("Closed...");
}
