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
#define KARY2               KARY/2                                   //2
#define PODNUM              KARY                                     //4
#define AGGRSWITCHINPODNUM	KARY2                                    //2
#define EDGESWITCHINPODNUM	KARY2                                    //2
#define SERVERINEDGENUM	    KARY2                                    //2
#define SERVERINPODNUM	    (SERVERINEDGENUM * EDGESWITCHINPODNUM)   //4

#define CORESWITCHNUM	    (KARY2 * KARY2)                          //4
#define AGGRSWITCHNUM	    (AGGRSWITCHINPODNUM * PODNUM)            //8
#define EDGESWITCHNUM	    (EDGESWITCHINPODNUM * PODNUM)            //8
#define SERVERNUM		    (SERVERINPODNUM * PODNUM)                //16

#define COREPERAGGRSWITCH   KARY2

int
main(int argc, char* argv[])
{
    CommandLine cmd(__FILE__);
    cmd.Parse(argc, argv);

	Time::SetResolution (Time::NS);
	LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
	LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);
	
    // TODO: pick routing
	// Config::SetDefault("ns3::Ipv4GlobalRouting::RandomEcmpRouting", BooleanValue(true)); 

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

    // Ipv4GlobalRoutingHelper globalRoutingHelper;
    // stack.SetRoutingHelper(globalRoutingHelper);
    InternetStackHelper stack;
    stack.Install(cores);
    stack.Install(aggregations);
    stack.Install(edges);
    stack.Install(servers);

    PointToPointHelper pointToPoint;
    pointToPoint.SetDeviceAttribute("DataRate", StringValue("5Mbps"));
    pointToPoint.SetChannelAttribute("Delay", StringValue("2ms"));
    // pointToPoint.SetQueue("ns3::DropTailQueue", "MaxPackets", UintegerValue (BUFFER_SIZE));

    Ipv4AddressHelper ipv4;
    ipv4.SetBase ("10.0.0.0", "255.0.0.0");

    NS_LOG_UNCOND ("Creating fat-tree topology");
    // Server to Edge Switches
    for (uint32_t i = 0; i < EDGESWITCHNUM; i++)
    {
        ipv4.NewNetwork();
        for (uint32_t j = 0; j < SERVERINEDGENUM; j++)
        {
            uint32_t serverIndex = i * SERVERINEDGENUM + j;
            /*
            k = 4
            0 * 2 + 0 = 0
            0 * 2 + 1 = 1
            ...
            7 * 2 + 0 = 14
            7 * 2 + 1 = 15
            */
            NodeContainer nodeContainer = NodeContainer (edges.Get(i), servers.Get(serverIndex));
            NetDeviceContainer devices = pointToPoint.Install(nodeContainer);

            Ipv4InterfaceContainer interfaceContainer = ipv4.Assign(devices);

            NS_LOG_UNCOND ("Server-" << serverIndex << " is connected to Edge-" << i
                    << " (" << devices.Get(1)->GetIfIndex() << "<->"
                    << devices.Get(0)->GetIfIndex() << ")");
        }
    }

    // Edge to Aggregation Switches
    for (uint32_t i = 0; i < EDGESWITCHNUM; i++)
    {
        for (uint32_t j = 0; j < AGGRSWITCHINPODNUM; j++)
        {
            uint32_t aggregateIndex = (i / (AGGRSWITCHINPODNUM)) * AGGRSWITCHINPODNUM + j;
            /*
            k = 4
            (0 / 4/2) * (4/2) + 0 = 0
            (0 / 4/2) * (4/2) + 1 = 1
            (1 / 4/2) * (4/2) + 0 = 0
            (1 / 4/2) * (4/2) + 1 = 1
            (2 / 4/2) * (4/2) + 0 = 2
            (2 / 4/2) * (4/2) + 1 = 3
            ...
            (7 / 4/2) * (4/2) + 0 = 6
            (7 / 4/2) * (4/2) + 1 = 7
            */
            NodeContainer nodeContainer = NodeContainer (edges.Get(i), aggregations.Get(aggregateIndex));
            NetDeviceContainer devices = pointToPoint.Install(nodeContainer);

            Ipv4InterfaceContainer interfaceContainer = ipv4.Assign(devices);

            NS_LOG_UNCOND ("Edge-" << i << " is connected to Aggregation-" << aggregateIndex
                    << " (" << devices.Get(0)->GetIfIndex() << "<->"
                    << devices.Get(1)->GetIfIndex() << ")");
        }
    }

    // Aggregation to Core Switches
    for (uint32_t i = 0; i < AGGRSWITCHNUM; i++)
    {
        for (uint32_t j = 0; j < COREPERAGGRSWITCH; j++)
        {
            uint32_t coreIndex = (i % (COREPERAGGRSWITCH)) * COREPERAGGRSWITCH + j;
            /*
            (0 % (4/2)) * (4/2) + 0) = 0
            (0 % (4/2)) * (4/2) + 1) = 1
            (1 % (4/2)) * (4/2) + 0) = 2
            (1 % (4/2)) * (4/2) + 1) = 3
            ...
            (7 % (4/2)) * (4/2) + 0) = 2
            (7 % (4/2)) * (4/2) + 1) = 3
            */
            NodeContainer nodeContainer = NodeContainer (aggregations.Get (i), cores.Get (coreIndex));
            NetDeviceContainer devices = pointToPoint.Install (nodeContainer);

            Ipv4InterfaceContainer interfaceContainer = ipv4.Assign (devices);

            NS_LOG_UNCOND ("Aggregation-" << i << " is connected to Core-" << coreIndex
                    << " (" << devices.Get(0)->GetIfIndex() << "<->"
                    << devices.Get(1)->GetIfIndex() << ")");
        }
    }

    double END_TIME = 0.5;
    NS_LOG_UNCOND ("Start simulation");
    Simulator::Stop (Seconds (END_TIME));
    Simulator::Run ();

    Simulator::Destroy ();
    NS_LOG_UNCOND ("Stop simulation");
    return 0;
}