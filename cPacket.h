#pragma once
#include <string>
#include <windows.h>
#include "hPackets.h"

using namespace std;

class cPacket
{
	PCAP_GENERAL_HEADER* PCAP_General_Header;
	PCAP_PACKET_HEADER* PCAP_Packet_Header;

public:
	cPacket(void);
	~cPacket(void);

	BOOL setFile(string filename);
	BOOL setBuffer(char* buffer, unsigned int size);
	BOOL setPCAPFile(string filename);
	BOOL setPCAPBuffer(char* buffer,unsigned int size);

	BOOL ProcessPacket();
	BOOL ProcessPackets();
	BOOL ProcessPCAP();

	DWORD BaseAddress;
	unsigned int Size;
	DWORD PCAPBaseAddress;
	unsigned int PCAPSize;

	ETHER_HEADER* Ether_Header;
	IP_HEADER* IP_Header;
	TCP_HEADER* TCP_Header;
	ARP_HEADER* ARP_Header;
	UDP_HEADER*	UDP_Header;
	ICMP_HEADER* ICMP_Header;
	IGMP_HEADER* IGMP_Header;

	unsigned int nPackets;
	unsigned int nPCAPPackets;
};
