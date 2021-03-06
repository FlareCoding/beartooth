#pragma once
#include "Adapter.h"
#include "Packets.h"
#include "MacVendorDecoder.h"
#include <map>

class net_utils
{
public:
	static void close_handles();

	static void print_packet_bytes(const char* title, const uint8_t* data, size_t dataLen, bool format = true);
	static void print_mac_address(macaddr addr, bool newline = true);
	
	//
	// *** NOTE ***
	// This method will still rely on the following:
	//		1) "Routing and Remote Access" service is running
	//		2) "IPEnableRouter" value in the registry key
	//			HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\Tcpip\Parameters
	//			is set to 1.
	//
	static bool set_system_ip_forwarding(bool forward);

	static bool  set_adapter(const Adapter& adapter);
	static void* get_native_pcap_handle();
	static bool  retrieve_local_mac_address(macaddr out_buffer);

	static int send_packet(void* packet, size_t size);
	static int recv_packet(PacketHeader* header, void* packet, size_t size);

	static bool send_arp_request(macaddr source_mac, macaddr target_mac_buffer, const char* source_ip, const char* target_ip);

	static void set_packet_dump_path(const std::string& path);
	static void reopen_dump_file();
	static void dump_packet_to_file(PacketHeader* header, void* packet);
};

class network_scanner
{
public:
	// ip_address_prefix example: 192.168.4.
	static void scan_network(macaddr source_mac, const std::string& source_ip, const std::string& ip_address_prefix, MacVendorDecoder* vendor_decoder, int range_start = 1, int range_end = 255);

	struct netscan_node
	{
		bool is_online = false;
		macaddr physical_address = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
		std::string vendor;
	};

	// ipv4 -> { is_online, mac_address }
	static std::map<std::string, netscan_node> s_network_scan_map;
};

struct PortScanNode
{
	uint16_t	port;
	bool		is_opened;
	std::string protocol;
	std::pair<std::string, std::string> service_name_and_description;
};

class port_scanner
{
public:
	// Maps that contains [service_name, service_description] for top known TCP and UDP ports
	static const std::map<uint16_t, std::pair<const char*, const char*>>& get_top_tcp_ports();
	static const std::map<uint16_t, std::pair<const char*, const char*>>& get_top_udp_ports();

	// Scan a specified list of ports
	static void scan_target(
		bool& attack_in_progress,
		macaddr local_mac,
		const std::string& local_ip,
		macaddr target_mac,
		const std::string& target_ip,
		std::vector<PortScanNode>& scanned_nodes,
		const std::map<uint16_t, std::pair<const char*, const char*>>& target_port_list
	);
};
