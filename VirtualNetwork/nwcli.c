#include "graph.h"
#include <stdio.h>
#include "CommandParser/libcli.h"
#include "CommandParser/cmdtlv.h"
#include "cmdcodes.h"

extern graph_t *topo;


static int show_nw_topology_handler(param_t *param, ser_buff_t *tlv_buf, op_mode enable_or_disable) {

    int CMDCODE = -1;
    CMDCODE = EXTRACT_CMD_CODE(tlv_buf);

    switch(CMDCODE){

        case CMDCODE_SHOW_NW_TOPOLOGY:
            dump_nw_graph(topo);
            break;
        default:
            ;
    }
    return 0;
}


void nw_init_cli() {

    init_libcli();

    param_t *show   = libcli_get_show_hook();
    param_t *debug  = libcli_get_debug_hook();
    param_t *config = libcli_get_config_hook();
    param_t *run    = libcli_get_run_hook();
    param_t *debug_show = libcli_get_debug_show_hook();
    param_t *root = libcli_get_root();

    {
        /*show topology*/
         static param_t topology;
         init_param(&topology, CMD, "topology", show_nw_topology_handler, 0, INVALID, 0, "Dump Complete Network Topology");
         libcli_register_param(show, &topology);
         set_param_cmd_code(&topology, CMDCODE_SHOW_NW_TOPOLOGY);
         
        //  {
        //     /*show node*/    
        //      static param_t node;
        //      init_param(&node, CMD, "node", 0, 0, INVALID, 0, "\"node\" keyword");
        //      libcli_register_param(show, &node);
        //      libcli_register_display_callback(&node, display_graph_nodes);
            //  {
            //     /*show node <node-name>*/ 
            //      static param_t node_name;
            //      init_param(&node_name, LEAF, 0, 0, validate_node_extistence, STRING, "node-name", "Node Name");
            //      libcli_register_param(&node, &node_name);
            //      {
            //         /*show node <node-name> arp*/
            //         static param_t arp;
            //         init_param(&arp, CMD, "arp", show_arp_handler, 0, INVALID, 0, "Dump Arp Table");
            //         libcli_register_param(&node_name, &arp);
            //         set_param_cmd_code(&arp, CMDCODE_SHOW_NODE_ARP_TABLE);
            //      }
            //      {
            //         /*show node <node-name> mac*/
            //         static param_t mac;
            //         init_param(&mac, CMD, "mac", show_mac_handler, 0, INVALID, 0, "Dump Mac Table");
            //         libcli_register_param(&node_name, &mac);
            //         set_param_cmd_code(&mac, CMDCODE_SHOW_NODE_MAC_TABLE);
            //      }
            //      {
            //         /*show node <node-name> rt*/
            //         static param_t rt;
            //         init_param(&rt, CMD, "rt", show_rt_handler, 0, INVALID, 0, "Dump L3 Routing table");
            //         libcli_register_param(&node_name, &rt);
            //         set_param_cmd_code(&rt, CMDCODE_SHOW_NODE_RT_TABLE);
            //      }
        //      }
        //  } 
    }
    
    // {
    //     /*run node*/
    //     static param_t node;
    //     init_param(&node, CMD, "node", 0, 0, INVALID, 0, "\"node\" keyword");
    //     libcli_register_param(run, &node);
    //     libcli_register_display_callback(&node, display_graph_nodes);
    //     {
    //         /*run node <node-name>*/
    //         static param_t node_name;
    //         init_param(&node_name, LEAF, 0, 0, validate_node_extistence, STRING, "node-name", "Node Name");
    //         libcli_register_param(&node, &node_name);
    //         {
    //             /*run node <node-name> ping */
    //             static param_t ping;
    //             init_param(&ping, CMD, "ping" , 0, 0, INVALID, 0, "Ping utility");
    //             libcli_register_param(&node_name, &ping);
    //             {
    //                 /*run node <node-name> ping <ip-address>*/    
    //                 static param_t ip_addr;
    //                 init_param(&ip_addr, LEAF, 0, ping_handler, 0, IPV4, "ip-address", "Ipv4 Address");
    //                 libcli_register_param(&ping, &ip_addr);
    //                 set_param_cmd_code(&ip_addr, CMDCODE_PING);
    //                 {
    //                     static param_t ero;
    //                     init_param(&ero, CMD, "ero", 0, 0, INVALID, 0, "ERO(Explicit Route Object)");
    //                     libcli_register_param(&ip_addr, &ero);
    //                     {
    //                         static param_t ero_ip_addr;
    //                         init_param(&ero_ip_addr, LEAF, 0, ping_handler, 0, IPV4, "ero-ip-address", "ERO Ipv4 Address");
    //                         libcli_register_param(&ero, &ero_ip_addr);
    //                         set_param_cmd_code(&ero_ip_addr, CMDCODE_ERO_PING);
    //                     }
    //                 }
    //             }
    //         }
    //         {
    //             /*run node <node-name> resolve-arp*/    
    //             static param_t resolve_arp;
    //             init_param(&resolve_arp, CMD, "resolve-arp", 0, 0, INVALID, 0, "Resolve ARP");
    //             libcli_register_param(&node_name, &resolve_arp);
    //             {
    //                 /*run node <node-name> resolve-arp <ip-address>*/    
    //                 static param_t ip_addr;
    //                 init_param(&ip_addr, LEAF, 0, arp_handler, 0, IPV4, "ip-address", "Nbr IPv4 Address");
    //                 libcli_register_param(&resolve_arp, &ip_addr);
    //                 set_param_cmd_code(&ip_addr, CMDCODE_RUN_ARP);
    //             }
    //         }
    //     }
    // }

    // /*config node*/
    // {
    //   static param_t node;
    //   init_param(&node, CMD, "node", 0, 0, INVALID, 0, "\"node\" keyword");
    //   libcli_register_param(config, &node);  
    //   libcli_register_display_callback(&node, display_graph_nodes);
    //   {
    //     /*config node <node-name>*/
    //     static param_t node_name;
    //     init_param(&node_name, LEAF, 0, 0, validate_node_extistence, STRING, "node-name", "Node Name");
    //     libcli_register_param(&node, &node_name);
    //     {
    //         /*config node <node-name> interface*/
    //         static param_t interface;
    //         init_param(&interface, CMD, "interface", 0, 0, INVALID, 0, "\"interface\" keyword");    
    //         libcli_register_param(&node_name, &interface);
    //         {
    //             /*config node <node-name> interface <if-name>*/
    //             static param_t if_name;
    //             init_param(&if_name, LEAF, 0, 0, 0, STRING, "if-name", "Interface Name");
    //             libcli_register_param(&interface, &if_name);
    //             {
    //                 /*config node <node-name> interface <if-name> l2mode*/
    //                 static param_t l2_mode;
    //                 init_param(&l2_mode, CMD, "l2mode", 0, 0, INVALID, 0, "\"l2mode\" keyword");
    //                 libcli_register_param(&if_name, &l2_mode);
    //                 {
    //                     /*config node <node-name> interface <if-name> l2mode <access|trunk>*/
    //                     static param_t l2_mode_val;
    //                     init_param(&l2_mode_val, LEAF, 0, intf_config_handler, validate_l2_mode_value,  STRING, "l2-mode-val", "access|trunk");
    //                     libcli_register_param(&l2_mode, &l2_mode_val);
    //                     set_param_cmd_code(&l2_mode_val, CMDCODE_INTF_CONFIG_L2_MODE);
    //                 } 
    //             }
    //             {
    //                 /*config node <node-name> interface <if-name> vlan*/
    //                 static param_t vlan;
    //                 init_param(&vlan, CMD, "vlan", 0, 0, INVALID, 0, "\"vlan\" keyword");
    //                 libcli_register_param(&if_name, &vlan);
    //                 {
    //                     /*config node <node-name> interface <if-name> vlan <vlan-id>*/
    //                      static param_t vlan_id;
    //                      init_param(&vlan_id, LEAF, 0, intf_config_handler, validate_vlan_id, INT, "vlan-id", "vlan id(1-4096)");
    //                      libcli_register_param(&vlan, &vlan_id);
    //                      set_param_cmd_code(&vlan_id, CMDCODE_INTF_CONFIG_VLAN);
    //                 }   
    //             }    
    //         }
            
    //     }
        
    //     {
    //         /*config node <node-name> route*/
    //         static param_t route;
    //         init_param(&route, CMD, "route", 0, 0, INVALID, 0, "L3 route");
    //         libcli_register_param(&node_name, &route);
    //         {
    //             /*config node <node-name> route <ip-address>*/    
    //             static param_t ip_addr;
    //             init_param(&ip_addr, LEAF, 0, 0, 0, IPV4, "ip-address", "IPv4 Address");
    //             libcli_register_param(&route, &ip_addr);
    //             {
    //                  /*config node <node-name> route <ip-address> <mask>*/
    //                 static param_t mask;
    //                 init_param(&mask, LEAF, 0, l3_config_handler, validate_mask_value, INT, "mask", "mask(0-32");
    //                 libcli_register_param(&ip_addr, &mask);
    //                 set_param_cmd_code(&mask, CMDCODE_CONF_NODE_L3ROUTE);
    //                 {
    //                     /*config node <node-name> route <ip-address> <mask> <gw-ip>*/
    //                     static param_t gwip;
    //                     init_param(&gwip, LEAF, 0, 0, 0, IPV4, "gw-ip", "IPv4 Address");
    //                     libcli_register_param(&mask, &gwip);
    //                     {
    //                         /*config node <node-name> route <ip-address> <mask> <gw-ip> <oif>*/
    //                         static param_t oif;
    //                         init_param(&oif, LEAF, 0, l3_config_handler, 0, STRING, "oif", "Out-going intf Name");
    //                         libcli_register_param(&gwip, &oif);
    //                         set_param_cmd_code(&oif, CMDCODE_CONF_NODE_L3ROUTE);
    //                     }
    //                 }
    //             }
    //         }    
    //     }    
    //     support_cmd_negation(&node_name);
    //   }
    // }


    support_cmd_negation(config);

}