#include "ns3/applications-module.h"
#include "ns3/core-module.h"
#include "ns3/internet-module.h"
#include "ns3/network-module.h"
#include "ns3/point-to-point-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("k-fat-tree");

/*
Inter-connect racks (of servers) using a fat-tree topology
K-ary fat tree: three-layer topology (edge, aggregation and core)
– each pod consists of (k/2)^2 servers & 2 layers of k/2 k-port switches
– each edge switch connects to k/2 servers & k/2 aggr. switches
– each aggr. switch connects to k/2 edge & k/2 core switches
– (k/2)^2 core switches: each connects to k pods
*/

#define KARY		        4
#define KARY2               KARY/2
#define PODNUM              KARY
#define AGGRSWITCHINPODNUM	KARY2
#define EDGESWITCHINPODNUM	KARY2
#define NODEINEDGENUM	    KARY2
#define SERVERINPODNUM	    (NODEINEDGENUM * EDGESWITCHINPODNUM)

#define CORESWITCHNUM	    (KARY2 * KARY2)
#define AGGRSWITCHNUM	    (AGGRSWITCHINPODNUM * PODNUM)
#define EDGESWITCHNUM	    (EDGESWITCHINPODNUM * PODNUM)
#define SERVERNUM		    (SERVERINPODNUM * PODNUM)

#define BUFFERSIZE = 10

int
main(int argc, char* argv[])
{
    CommandLine cmd(__FILE__);
    cmd.Parse(argc, argv);

	Time::SetResolution (Time::NS);
	LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
	LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);
	
    // TODO: pick routing
	Config::SetDefault("ns3::Ipv4GlobalRouting::RandomEcmpRouting", BooleanValue(true)); 

    NodeContainer cores;
    NodeContainer aggregations;
    NodeContainer edges;
    NodeContainer servers;

    // (2 * 2) = 4
    cores.Create(CORESWITCHNUM);
    // (4 * 2) = 8
    aggregations.Create(AGGRSWITCHNUM);
    // (4 * 2) = 8
    edges.Create(EDGESWITCHNUM);
     // (2 * 2) * 4 = 16
    servers.Create(SERVERNUM);

    Ipv4GlobalRoutingHelper globalRoutingHelper;
    InternetStackHelper stack;
    stack.SetRoutingHelper(globalRoutingHelper);
    stack.Install(cores);
    stack.Install(aggregations);
    stack.Install(edges);
    stack.Install(servers);

    PointToPointHelper pointToPoint;
    pointToPoint.SetDeviceAttribute("DataRate", StringValue("5Mbps"));
    pointToPoint.SetChannelAttribute("Delay", StringValue("2ms"));
    // pointToPoint.SetQueue("ns3::DropTailQueue", "MaxPackets", UintegerValue (BUFFER_SIZE));

    Ipv4AddressHelper ipv4;
    ipv4.SetBase ("10.0.0.0", "255.255.255.0");
}