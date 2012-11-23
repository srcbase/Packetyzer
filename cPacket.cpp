#include "StdAfx.h"
#include "cPacket.h"
#include "cFile.h"
#include "hPackets.h"
#include <iostream>

#pragma comment(lib, "ws2_32.lib")
using namespace std;

cPacket::cPacket(void)
{
};

BOOL cPacket::setFile(string filename)
{
	cFile* File = new cFile((char*)filename.c_str());
	if (File->FileLength == 0) return false;
	
	BaseAddress = File->BaseAddress;
	Size = File->FileLength;
	return true;
};

BOOL cPacket::setPCAPFile(string filename)
{
	cFile* File = new cFile((char*)filename.c_str());
	if (File->FileLength == 0) return false;
	
	PCAPBaseAddress = File->BaseAddress;
	PCAPSize = File->FileLength;
	return true;
};

BOOL cPacket::setBuffer(char* buffer, unsigned int size)
{
	BaseAddress = (DWORD)buffer;
	Size = size;
	return true;
};

BOOL cPacket::setPCAPBuffer(char* buffer, unsigned int size)
{
	PCAPBaseAddress = (DWORD)buffer;
	PCAPSize = size;
	return true;
};

BOOL cPacket::ProcessPacket()
{
	Ether_Header = (ETHER_HEADER*)BaseAddress;

	/* packet ether type */
	if (ntohs(Ether_Header->ether_type) == ETHERTYPE_IP)
	{
		cout << "IP packet" << endl;
		IP_Header = (IP_HEADER*)(BaseAddress + sizeof(ETHER_HEADER));
		if ((unsigned short int)(IP_Header->ip_protocol) == TCP_PACKET)
		{
			cout << "TCP packet" << endl;
			TCP_Header = (TCP_HEADER*)(BaseAddress + sizeof(ETHER_HEADER) + (IP_Header->ip_header_len*4));
			
			cout << "Data size: " << (Size - sizeof(ETHER_HEADER) - (IP_Header->ip_header_len*4) - (TCP_Header->data_offset*4)) << endl;

			if (Size - sizeof(ETHER_HEADER) - (IP_Header->ip_header_len*4) - (TCP_Header->data_offset*4) != 0)
			{
				char * data = (char*)(BaseAddress + sizeof(ETHER_HEADER) + (IP_Header->ip_header_len*4) + (TCP_Header->data_offset*4));
				cout << data << endl;
			}
		}
		else if ((unsigned short int)(IP_Header->ip_protocol) == UDP_PACKET)
		{
			cout << "UDP packet" << endl;
			UDP_Header = (UDP_HEADER*)(BaseAddress + sizeof(ETHER_HEADER) + (IP_Header->ip_header_len*4));
			char* data = (char*)(BaseAddress + sizeof(ETHER_HEADER) + (IP_Header->ip_header_len*4) + sizeof(UDP_HEADER));
			cout << data << endl;
		}
		else if ((unsigned short int)(IP_Header->ip_protocol) == ICMP_PACKET)
		{
			cout << "ICMP packet" << endl;
			ICMP_Header = (ICMP_HEADER*)(BaseAddress + sizeof(ETHER_HEADER) + (IP_Header->ip_header_len*4));
			char* data = (char*)(BaseAddress + sizeof(ETHER_HEADER) + (IP_Header->ip_header_len*4) + sizeof(ICMP_HEADER));
			cout << data << endl;
		}
		else if ((unsigned short int)(IP_Header->ip_protocol) == IGMP_PACKET)
		{
			cout << "IGMP packet" << endl;
			IGMP_Header = (IGMP_HEADER*)(BaseAddress + sizeof(ETHER_HEADER) + (IP_Header->ip_header_len*4));
		}
	}
	else if (ntohs(Ether_Header->ether_type) == ETHERTYPE_ARP)
	{
		cout << "ARP packet" << endl;
		ARP_Header = (ARP_HEADER*)(BaseAddress + sizeof(ETHER_HEADER));
	}
	return true;
};

BOOL cPacket::ProcessPackets()
{
	return true;
};

BOOL cPacket::ProcessPCAP()
{
	PCAP_General_Header = (PCAP_GENERAL_HEADER*)PCAPBaseAddress;
	unsigned int psize = 0;

	PCAP_Packet_Header = (PCAP_PACKET_HEADER*)(PCAPBaseAddress + sizeof(PCAP_GENERAL_HEADER));
	psize = psize + PCAP_Packet_Header->incl_len;

	/* getting number of packets inside file */
	for(unsigned int i=1; PCAP_Packet_Header->incl_len !=0 ;i++)
	{
		PCAP_Packet_Header = (PCAP_PACKET_HEADER*)(PCAPBaseAddress + sizeof(PCAP_GENERAL_HEADER) + (sizeof(PCAP_PACKET_HEADER) * i) + psize);
		psize = psize + PCAP_Packet_Header->incl_len;
		nPCAPPackets++;
	}
	

	return true;
};

cPacket::~cPacket(void)
{
};
