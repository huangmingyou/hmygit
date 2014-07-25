// huangmingyou@xunyou.com
// 模拟丢包和延迟
// 2014-07-25
#include <linux/module.h>  
#include <linux/kernel.h>  
#include <linux/netfilter.h>  
#include <linux/netfilter_ipv4.h>  
#include <linux/netdevice.h>  
#include <linux/skbuff.h>  
#include <linux/ip.h>  
#include <linux/udp.h>  
#include <linux/delay.h>
static struct nf_hook_ops nfho;  
static int i;
unsigned int hook_func(unsigned int hooknum,  
    struct sk_buff *skb,  
   const struct net_device *in,  
    struct net_device *out,  
    int (*okfn)(struct sk_buff *))  
{
	struct iphdr *iph = NULL;
    iph = ip_hdr(skb);
	// 只针对icmp协议
    if (iph->protocol != IPPROTO_ICMP) 
    {
	return NF_ACCEPT;
    }
    
	i++;
	if(i>10) i=0;
	// 延迟
	//if(i==5||i==7||i==6) mdelay(40);
	// 丢包
	if(i==5||i==7||i==6) return NF_DROP;

	//
	return NF_ACCEPT;
}  
  
static int __init init_nethook(void)  
{  
    nfho.hook = hook_func;   
    nfho.hooknum = NF_INET_PRE_ROUTING;	
    nfho.pf = PF_INET;  
    nfho.priority = NF_IP_PRI_FIRST;   
    nf_register_hook(&nfho);  
    return 0;  
}  
  
  
static void __exit exit_nethook(void)  
{  
    nf_unregister_hook(&nfho);  
}  
  
module_init(init_nethook);  
module_exit(exit_nethook);  

