/*
 * How to compile and run : 
 * Compile : gcc ip_maths.c -lm -o solution.o
 * Run     : ./solution.o
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h> //add -lm flag in order to use 'pow'

#define PREFIX_LEN  16  // ip address string length
#define MASK_LEN  32

/*
 *  convert ip address string to int
 */

uint32_t ip_string_to_int(char const *ip_addr);

/*
 *  get the broadcast address
 */

void get_broadcast_address(char const *ip_addr, char mask, char *output_buffer);

/*
 *  get the Network ID
 */

void get_network_id(char const *ip_addr, char mask, char *output_buffer);

/*
 *  convert ip address int to string
 */

void get_abcd_ip_format(uint32_t ip_address, char *output_buffer);

/*
 *  get the Max Nbr of ip addresses based on the mask
 */

uint32_t get_subnet_cardinality(char mask);

/*
 *  return 1 if the check_ip address is a member of the network_id 
 *  else return 0
 */

uint8_t check_ip_subnet_memebership(char const *network_id, char mask, char const *check_ip);

int main(){
    
    char ip_broadcast[PREFIX_LEN], network_id[PREFIX_LEN];
    uint32_t ip_integeral, nbr;
    uint8_t result;
    char  *ip_add = "192.168.2.10";
    char mask = 24;
    char *network_id1 = "192.168.0.0", *check_ip =  "192.168.0.10";

    memset(ip_broadcast, 0, PREFIX_LEN);
    memset(network_id, 0, PREFIX_LEN);

    get_broadcast_address(ip_add, mask, ip_broadcast);
    get_network_id(ip_add, mask, network_id);
    ip_integeral = ip_string_to_int(ip_add);
    nbr = get_subnet_cardinality(mask);

    printf("%s | %s | %s | %u | %u\n", ip_add, ip_broadcast, network_id, ip_integeral, nbr);

    result = check_ip_subnet_memebership(network_id1, mask, check_ip);
    if (result)
        printf("Subnet memebership\n");
    else
        printf("Not Subnet memebership\n");

    return 0;
}

uint32_t ip_string_to_int(char const *ip_addr){
    int f1,f2,f3,f4;
    char sep;

    sscanf(ip_addr, "%d%c%d%c%d%c%d", &f1, &sep, &f2, &sep, &f3, &sep, &f4);

    return (f1<<24) | (f2<<16) | (f3<<8) | (f4<<0); 
}

void get_abcd_ip_format(uint32_t ip_address, char *output_buffer){

    uint8_t b1,b2,b3,b4;

    b1 = (uint8_t)((ip_address>>24) & 0xFF);
    b2 = (uint8_t)((ip_address>>16) & 0xFF);
    b3 = (uint8_t)((ip_address>>8) & 0xFF);
    b4 = (uint8_t)((ip_address>>0) & 0xFF);
 
    sprintf(output_buffer, "%d.%d.%d.%d", b1,b2,b3,b4);
}

void get_broadcast_address(char const *ip_addr, char mask, char *output_buffer){

    uint32_t ip_addr_b, mask_b, mask_bc;

    ip_addr_b = ip_string_to_int(ip_addr);

    mask_b = (uint32_t)(pow(2,mask)-1)<<(MASK_LEN-mask);
    mask_bc = ~mask_b;
    ip_addr_b &= mask_b;
    ip_addr_b |= mask_bc;

    get_abcd_ip_format(ip_addr_b, output_buffer);
}


void get_network_id(char const *ip_addr, char mask, char *output_buffer){

    uint32_t ip_addr_b, mask_b;
    uint8_t b1,b2,b3,b4;

    ip_addr_b = ip_string_to_int(ip_addr);
    mask_b = (uint32_t)(pow(2,mask)-1)<<(MASK_LEN-mask);
    ip_addr_b &= mask_b;   

    b1 = (uint8_t)((ip_addr_b>>24) & 0xFF);
    b2 = (uint8_t)((ip_addr_b>>16) & 0xFF);
    b3 = (uint8_t)((ip_addr_b>>8) & 0xFF);
    b4 = (uint8_t)((ip_addr_b>>0) & 0xFF);

    sprintf(output_buffer, "%d.%d.%d.%d", b1,b2,b3,b4);
}

uint32_t get_subnet_cardinality(char mask){
    return (pow(2,MASK_LEN-mask)-2);
}

uint8_t check_ip_subnet_memebership(char const *network_id, char mask, char const *check_ip){
    uint32_t network_id_b, check_ip_b, ip_broadcast_b;
    char ip_broadcast[PREFIX_LEN];

    network_id_b = ip_string_to_int(network_id);
    check_ip_b = ip_string_to_int(check_ip);
    get_broadcast_address(network_id, mask, ip_broadcast);
    ip_broadcast_b = ip_string_to_int(ip_broadcast);

    if((network_id_b == check_ip_b) || (ip_broadcast_b == check_ip_b))
        return 0;
    
    if ((network_id_b>>(MASK_LEN-mask)) != (check_ip_b>>(MASK_LEN-mask)))
        return 0;
    return 1;

}
