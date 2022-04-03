#pragma once
#include "Adapter.h"
#include "Packets.h"
#include <map>

class net_utils
{
public:
	static void print_packet_bytes(const char* title, const uint8_t* data, size_t dataLen, bool format = true);
	static void print_mac_address(macaddr addr, bool newline = true);
	
	static bool  set_adapter(const Adapter& adapter);
	static void* get_native_pcap_handle();
	static bool  retrieve_local_mac_address(macaddr out_buffer);

	static int send_packet(void* packet, size_t size);
	static int recv_packet(PacketHeader* header, void* packet, size_t size);

	static bool send_arp_request(macaddr source_mac, macaddr target_mac_buffer, const char* source_ip, const char* target_ip);
};

class network_scanner
{
public:
	// ip_address_prefix example: 192.168.4.
	static void scan_network(macaddr source_mac, const std::string& source_ip, const std::string& ip_address_prefix, int range_start = 1, int range_end = 255);

	struct netscan_node
	{
		bool is_online = false;
		macaddr physical_address = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	};

	// ipv4 -> { is_online, mac_address }
	static std::map<std::string, netscan_node> s_network_scan_map;
};