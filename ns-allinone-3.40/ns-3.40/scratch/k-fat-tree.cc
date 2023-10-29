#include "ns3/applications-module.h"
#include "ns3/core-module.h"
#include "ns3/internet-module.h"
#include "ns3/network-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/netanim-module.h"

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

#define KARY		        2
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

    // std::map<std::pair<int, int>, uint32_t> edgeToAggregationPath;
    // std::map<std::pair<int, int>, uint32_t> aggregationToCorePath;

    Ipv4AddressHelper ipv4;
    ipv4.SetBase ("10.0.0.0", "255.255.255.0");

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
            // interfaceContainer.Add(ipv4.Assign(devices));

            uint32_t nNodes = interfaceContainer.GetN();
            // NS_LOG_UNCOND(nNodes);

            NS_LOG_UNCOND ("Server-" << serverIndex << " " << interfaceContainer.GetAddress(nNodes - 1) << " is connected to Edge-" << i
                    << " " << interfaceContainer.GetAddress(nNodes - 2) << " (" << devices.Get(1)->GetIfIndex() << "<->"
                    << devices.Get(0)->GetIfIndex() << ")");
        }
    }

    // uint32_t nNodes = interfaceContainer.GetN();
    // for (uint32_t i = 0; i < nNodes; ++i)
    // {
    //     std::pair<Ptr<Ipv4>, uint32_t> pair = interfaceContainer.Get (i);
    //     NS_LOG_UNCOND (pair.first << " " << pair.second);
    //     NS_LOG_UNCOND (interfaceContainer.GetAddress(i) << " " << i);
    // }

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
            // interfaceContainer.Add(ipv4.Assign(devices));

            uint32_t nNodes = interfaceContainer.GetN();
            // NS_LOG_UNCOND(nNodes);

            NS_LOG_UNCOND ("Edge-" << i << " " << interfaceContainer.GetAddress(nNodes - 1) << " is connected to Aggregation-" << aggregateIndex
                    << " " << interfaceContainer.GetAddress(nNodes - 2) << " (" << devices.Get(0)->GetIfIndex() << "<->"
                    << devices.Get(1)->GetIfIndex() << ")");
        }
    }

    // nNodes = interfaceContainer.GetN();
    // for (uint32_t i = 0; i < nNodes; ++i)
    // {
    //     std::pair<Ptr<Ipv4>, uint32_t> pair = interfaceContainer.Get (i);
    //     NS_LOG_UNCOND (pair.first << " " << pair.second);
    //     NS_LOG_UNCOND (interfaceContainer.GetAddress(i) << " " << i);
    // }

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

            Ipv4InterfaceContainer interfaceContainer = ipv4.Assign(devices);
            // interfaceContainer.Add(ipv4.Assign(devices));

            uint32_t nNodes = interfaceContainer.GetN();
            // NS_LOG_UNCOND(nNodes);

            NS_LOG_UNCOND ("Aggregation-" << i << " " << interfaceContainer.GetAddress(nNodes - 2) << " is connected to Core-" << coreIndex
                    << " " << interfaceContainer.GetAddress(nNodes - 1) << " (" << devices.Get(0)->GetIfIndex() << "<->"
                    << devices.Get(1)->GetIfIndex() << ")");
        }
    }

    // UdpEchoServerHelper echoServer(9);
    
    // ApplicationContainer serverApps = echoServer.Install(edges.Get(0));
    // serverApps.Start(Seconds(1.0));
    // serverApps.Stop(Seconds(10.0));

    // nNodes = interfaceContainer.GetN();
    // for (uint32_t i = 0; i < nNodes; ++i)
    // {
    //     std::pair<Ptr<Ipv4>, uint32_t> pair = interfaceContainer.Get (i);
    //     NS_LOG_UNCOND (pair.first << " " << pair.second);
    //     NS_LOG_UNCOND (interfaceContainer.GetAddress(i) << " " << i);
    // }

    // UdpEchoClientHelper echoClient(interfaceContainer.GetAddress(1), 9);
    // echoClient.SetAttribute("MaxPackets", UintegerValue(1));
    // echoClient.SetAttribute("Interval", TimeValue(Seconds(1.0)));
    // echoClient.SetAttribute("PacketSize", UintegerValue(1024));

    // ApplicationContainer clientApps = echoClient.Install(servers.Get(0));
    // clientApps.Start(Seconds(2.0));
    // clientApps.Stop(Seconds(10.0));

    // AnimationInterface anim("k_fat_tree.xml");  // where "animation.xml" is any arbitrary filename

    NS_LOG_UNCOND ("Start simulation");
    Simulator::Run ();
    Simulator::Destroy ();
    NS_LOG_UNCOND ("Stop simulation");
    return 0;
}