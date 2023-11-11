/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2017 NITK Surathkal
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Authors: Shravya K.S. <shravya.ks0@gmail.com>
 *
 */

#include "ns3/applications-module.h"
#include "ns3/constant-position-mobility-model.h"
#include "ns3/core-module.h"
#include "ns3/internet-module.h"
#include "ns3/ipv4-static-routing.h"
#include "ns3/mpi-interface.h"
#include "ns3/netanim-module.h"
#include "ns3/network-module.h"
#include "ns3/nix-vector-helper.h"
#include "ns3/point-to-point-layout-module.h"
#include "ns3/point-to-point-module.h"

#include <cstdint>
#include <iostream>
#include <mpi.h>
#include <sys/types.h>

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("FatTreeAnimation");

/**
 * \ingroup point-to-point-layout
 *
 * \brief A helper to make it easier to create a Fat tree topology
 * with p2p links
 */
class PointToPointFatTreeHelper
{
  public:
    /**
     * Create a PointToPointFatTreeHelper in order to easily create
     * Fat tree topologies using p2p links
     *
     * \param numPods total number of Pods in Fat tree
     *
     * \param pointToPoint the PointToPointHelper which is used
     *                     to connect all of the nodes together
     *                     in the Fat tree
     */
    PointToPointFatTreeHelper(uint32_t numPods, PointToPointHelper pointToPoint);

    ~PointToPointFatTreeHelper();

    /**
     * \param col the column address of the desired edge switch
     *
     * \returns a pointer to the edge switch specified by the
     *          column address
     */
    Ptr<Node> GetEdgeSwitchNode(uint32_t col) const;

    /**
     * \param col the column address of the desired aggregate switch
     *
     * \returns a pointer to the aggregate switch specified by the
     *          column address
     */
    Ptr<Node> GetAggregateSwitchNode(uint32_t col) const;

    /**
     * \param col the column address of the desired core switch
     *
     * \returns a pointer to the core switch specified by the
     *          column address
     */
    Ptr<Node> GetCoreSwitchNode(uint32_t col) const;

    /**
     * \param col the column address of the desired server
     *
     * \returns a pointer to the server specified by the
     *          column address
     */
    Ptr<Node> GetServerNode(uint32_t col) const;

    /**
     * This returns an IPv4 address at the edge switch specified by
     * column address. Technically, an edge switch will have multiple
     * interfaces in the Fat tree; therefore, it also has multiple
     * IPv4 addresses. This method only returns one of the addresses.
     * The address being returned belongs to an interface which connects
     * the lowest index server to this switch.
     *
     * \param col the column address of the desired edge switch
     *
     * \returns Ipv4Address of one of the interfaces of the edge switch
     *          column address
     */
    Ipv4Address GetEdgeSwitchIpv4Address(uint32_t col) const;

    /**
     * This returns an IPv4 address at the aggregate switch specified by
     * column address. Technically, an aggregate switch will have multiple
     * interfaces in the Fat tree; therefore, it also has multiple IPv4
     * addresses. This method only returns one of the addresses. The address
     * being returned belongs to an interface which connects the lowest index
     * server to this switch.
     *
     * \param col the column address of the desired aggregate switch
     *
     * \returns Ipv4Address of one of the interfaces of the aggregate switch
     *          column address
     */
    Ipv4Address GetAggregateSwitchIpv4Address(uint32_t col) const;

    /**
     * This returns an IPv4 address at the core switch specified by
     * column address. Technically, a core switch will have multiple
     * interfaces in the Fat tree; therefore, it also has multiple IPv4
     * addresses. This method only returns one of the addresses. The
     * address being returned belongs to an interface which connects the
     * lowest index server to this switch.
     *
     * \param col the column address of the desired core switch
     *
     * \returns Ipv4Address of one of the interfaces of the core switch
     *          column address
     */
    Ipv4Address GetCoreSwitchIpv4Address(uint32_t col) const;

    /**
     * This returns an IPv6 address at the edge switch specified by
     * column address. Technically, an edge switch will have multiple
     * interfaces in the Fat tree; therefore, it also has multiple
     * IPv6 addresses. This method only returns one of the addresses.
     * The address being returned belongs to an interface which connects
     * the lowest index server to this switch.
     *
     * \param col the column address of the desired edge switch
     *
     * \returns Ipv6Address of one of the interfaces of the edge switch
     *          column address
     */
    Ipv6Address GetEdgeSwitchIpv6Address(uint32_t col) const;

    /**
     * This returns an IPv6 address at the aggregate switch specified by
     * column address. Technically, an aggregate switch will have multiple
     * interfaces in the Fat tree; therefore, it also has multiple IPv6
     * addresses. This method only returns one of the addresses. The address
     * being returned belongs to an interface which connects the lowest index
     * server to this switch.
     *
     * \param col the column address of the desired aggregate switch
     *
     * \returns Ipv6Address of one of the interfaces of the aggregate switch
     *          column address
     */
    Ipv6Address GetAggregateSwitchIpv6Address(uint32_t col) const;

    /**
     * This returns an IPv6 address at the core switch specified by
     * column address. Technically, a core switch will have multiple
     * interfaces in the Fat tree; therefore, it also has multiple IPv6
     * addresses. This method only returns one of the addresses. The
     * address being returned belongs to an interface which connects the
     * lowest index server to this switch.
     *
     * \param col the column address of the desired core switch
     *
     * \returns Ipv6Address of one of the interfaces of the core switch
     *          column address
     */
    Ipv6Address GetCoreSwitchIpv6Address(uint32_t col) const;

    /**
     * This returns an IPv4 address at the server specified by
     * the column address.
     *
     * \param col the column address of the desired server
     *
     * \returns Ipv4Address of one of the interfaces of the server
     *          specified by the column address
     */
    Ipv4Address GetServerIpv4Address(uint32_t col) const;

    /**
     * This returns an IPv6 address at the server specified by
     * the column address.
     *
     * \param col the column address of the desired server
     *
     * \returns Ipv6Address of one of the interfaces of the server
     *          specified by the column address
     */
    Ipv6Address GetServerIpv6Address(uint32_t col) const;

    /**
     * \param stack an InternetStackHelper which is used to install
     *              on every node in the Fat tree
     */
    void InstallStack(InternetStackHelper stack);

    /**
     * Assigns IPv4 addresses to all the interfaces of switches and servers
     *
     * \param network an IPv4 address representing the network portion
     *                of the IPv4 address
     *
     * \param mask the mask length
     */
    void AssignIpv4Addresses(Ipv4Address network, Ipv4Mask mask);

    /**
     * Assigns IPv6 addresses to all the interfaces of the switches and servers
     *
     * \param network an IPv6 address representing the network portion
     *                of the IPv6 address
     *
     * \param prefix the prefix length
     */
    void AssignIpv6Addresses(Ipv6Address network, Ipv6Prefix prefix);

    /**
     * Sets up the node canvas locations for every node in the Fat tree.
     * This is needed for use with the animation interface
     *
     * \param ulx upper left x value
     * \param uly upper left y value
     * \param lrx lower right x value
     * \param lry lower right y value
     */
    void BoundingBox(double ulx, double uly, double lrx, double lry);

    NodeContainer GetMServers() const;
    uint32_t GetNumServers() const;

  private:
    uint32_t m_numPods; //!< Number of pods
    std::vector<NetDeviceContainer>
        m_edgeSwitchDevices; //!< Net Device container for edge switches and servers
    std::vector<NetDeviceContainer>
        m_aggregateSwitchDevices; //!< Net Device container for aggregate switches and edge switches
    std::vector<NetDeviceContainer>
        m_coreSwitchDevices; //!< Net Device container for core switches and aggregate switches
    Ipv4InterfaceContainer m_edgeSwitchInterfaces;       //!< IPv4 interfaces of edge switch
    Ipv4InterfaceContainer m_aggregateSwitchInterfaces;  //!< IPv4 interfaces of aggregate switch
    Ipv4InterfaceContainer m_coreSwitchInterfaces;       //!< IPv4 interfaces of core switch
    Ipv4InterfaceContainer m_serverInterfaces;           //!< IPv4 interfaces of server
    Ipv6InterfaceContainer m_edgeSwitchInterfaces6;      //!< IPv6 interfaces of edge switch
    Ipv6InterfaceContainer m_aggregateSwitchInterfaces6; //!< IPv6 interfaces of aggregate switch
    Ipv6InterfaceContainer m_coreSwitchInterfaces6;      //!< IPv6 interfaces of core switch
    Ipv6InterfaceContainer m_serverInterfaces6;          //!< IPv6 interfaces of server
    NodeContainer m_edgeSwitches;                        //!< all the edge switches in the Fat tree
    NodeContainer m_aggregateSwitches; //!< all the aggregate switches in the Fat tree
    NodeContainer m_coreSwitches;      //!< all the core switches in the Fat tree
    NodeContainer m_servers;           //!< all the servers in the Fat tree
    uint32_t numServers;
};

PointToPointFatTreeHelper::PointToPointFatTreeHelper(uint32_t numPods, PointToPointHelper p2pHelper)
    : m_numPods(numPods)
{
    // Bounds check
    if (numPods == 0)
    {
        NS_FATAL_ERROR("Need more pods for FatTree.");
    }
    if (numPods % 2 != 0)
    {
        NS_FATAL_ERROR("Number of pods should be even in FatTree.");
    }

    uint32_t numEdgeSwitches = numPods / 2;
    uint32_t numAggregateSwitches = numPods / 2;  // number of aggregate switches in a pod
    uint32_t numGroups = numPods / 2;             // number of group of core switches
    uint32_t numCoreSwitches = numPods / 2;       // number of core switches in a group
    numServers = numPods * numPods * numPods / 4; // number of servers in the entire network
    m_edgeSwitchDevices.resize(numPods * numEdgeSwitches);
    m_aggregateSwitchDevices.resize(numPods * numAggregateSwitches);
    m_coreSwitchDevices.resize(numGroups * numCoreSwitches);

    m_servers.Create(numServers);
    m_edgeSwitches.Create(numEdgeSwitches * numPods);
    m_aggregateSwitches.Create(numAggregateSwitches * numPods);
    m_coreSwitches.Create(numCoreSwitches * numGroups);

    InternetStackHelper stack;

    // Connect servers to edge switches
    uint32_t hostId = 0;
    for (uint32_t i = 0; i < numPods * numPods / 2; i++)
    {
        for (uint32_t j = 0; j < numEdgeSwitches; j++)
        {
            NetDeviceContainer nd = p2pHelper.Install(m_servers.Get(hostId), m_edgeSwitches.Get(i));
            m_edgeSwitchDevices[i].Add(nd.Get(0));
            m_edgeSwitchDevices[i].Add(nd.Get(1));
            hostId += 1;
        }
    }

    // Connect edge switches to aggregate switches
    for (uint32_t i = 0; i < numPods; i++)
    {
        for (uint32_t j = 0; j < numAggregateSwitches; j++)
        {
            for (uint32_t k = 0; k < numEdgeSwitches; k++)
            {
                NetDeviceContainer nd =
                    p2pHelper.Install(m_edgeSwitches.Get(i * numEdgeSwitches + k),
                                      m_aggregateSwitches.Get(i * numAggregateSwitches + j));
                m_aggregateSwitchDevices[i * numAggregateSwitches + j].Add(nd.Get(0));
                m_aggregateSwitchDevices[i * numAggregateSwitches + j].Add(nd.Get(1));
            }
        }
    }

    // Connect aggregate switches to core switches
    for (uint32_t i = 0; i < numGroups; i++)
    {
        for (uint32_t j = 0; j < numCoreSwitches; j++)
        {
            for (uint32_t k = 0; k < numPods; k++)
            {
                NetDeviceContainer nd =
                    p2pHelper.Install(m_aggregateSwitches.Get(k * numAggregateSwitches + i),
                                      m_coreSwitches.Get(i * numCoreSwitches + j));
                m_coreSwitchDevices[i * numCoreSwitches + j].Add(nd.Get(0));
                m_coreSwitchDevices[i * numCoreSwitches + j].Add(nd.Get(1));
            }
        }
    }
}

PointToPointFatTreeHelper::~PointToPointFatTreeHelper()
{
}

void
PointToPointFatTreeHelper::InstallStack(InternetStackHelper stack)
{
    stack.Install(m_servers);
    stack.Install(m_edgeSwitches);
    stack.Install(m_aggregateSwitches);
    stack.Install(m_coreSwitches);
}

void
PointToPointFatTreeHelper::BoundingBox(double ulx, double uly, double lrx, double lry)
{
    NS_LOG_FUNCTION(this << ulx << uly << lrx << lry);
    double xDist;
    double yDist;
    if (lrx > ulx)
    {
        xDist = lrx - ulx;
    }
    else
    {
        xDist = ulx - lrx;
    }
    if (lry > uly)
    {
        yDist = lry - uly;
    }
    else
    {
        yDist = uly - lry;
    }

    uint32_t numServers = m_numPods * m_numPods * m_numPods / 4;
    uint32_t numSwitches = m_numPods * m_numPods / 2;

    double xServerAdder = xDist / numServers;
    double xEdgeSwitchAdder = xDist / numSwitches;
    double xAggregateSwitchAdder = xDist / numSwitches;
    double xCoreSwitchAdder = xDist / (numSwitches / 2);
    double yAdder = yDist / 4; // 3 layers of switches and 1 layer of servers

    // Place the servers
    double xLoc = 0.0;
    double yLoc = yDist / 2;
    for (uint32_t i = 0; i < numServers; ++i)
    {
        Ptr<Node> node = m_servers.Get(i);
        Ptr<ConstantPositionMobilityModel> loc = node->GetObject<ConstantPositionMobilityModel>();
        if (loc == 0)
        {
            loc = CreateObject<ConstantPositionMobilityModel>();
            node->AggregateObject(loc);
        }
        Vector locVec(xLoc, yLoc, 0);
        loc->SetPosition(locVec);
        if (i % 2 == 0)
        {
            xLoc += 3 * xServerAdder;
        }
        else
        {
            xLoc += 1.1 * xServerAdder;
        }
    }

    yLoc -= yAdder;

    // Place the edge switches
    xLoc = xEdgeSwitchAdder;
    for (uint32_t i = 0; i < numSwitches; ++i)
    {
        Ptr<Node> node = m_edgeSwitches.Get(i);
        Ptr<ConstantPositionMobilityModel> loc = node->GetObject<ConstantPositionMobilityModel>();
        if (loc == 0)
        {
            loc = CreateObject<ConstantPositionMobilityModel>();
            node->AggregateObject(loc);
        }
        Vector locVec(xLoc, yLoc, 0);
        loc->SetPosition(locVec);
        xLoc += 2 * xEdgeSwitchAdder;
    }

    yLoc -= yAdder;

    // Place the aggregate switches
    xLoc = xAggregateSwitchAdder;
    for (uint32_t i = 0; i < numSwitches; ++i)
    {
        Ptr<Node> node = m_aggregateSwitches.Get(i);
        Ptr<ConstantPositionMobilityModel> loc = node->GetObject<ConstantPositionMobilityModel>();
        if (loc == 0)
        {
            loc = CreateObject<ConstantPositionMobilityModel>();
            node->AggregateObject(loc);
        }
        Vector locVec(xLoc, yLoc, 0);
        loc->SetPosition(locVec);
        xLoc += 2 * xAggregateSwitchAdder;
    }

    yLoc -= yAdder;

    // Place the core switches
    xLoc = xCoreSwitchAdder;
    for (uint32_t i = 0; i < numSwitches / 2; ++i)
    {
        Ptr<Node> node = m_coreSwitches.Get(i);
        Ptr<ConstantPositionMobilityModel> loc = node->GetObject<ConstantPositionMobilityModel>();
        if (loc == 0)
        {
            loc = CreateObject<ConstantPositionMobilityModel>();
            node->AggregateObject(loc);
        }
        Vector locVec(xLoc, yLoc, 0);
        loc->SetPosition(locVec);
        xLoc += 2 * xCoreSwitchAdder;
    }
}

void
PointToPointFatTreeHelper::AssignIpv4Addresses(Ipv4Address network, Ipv4Mask mask)
{
    NS_LOG_FUNCTION(this << network << mask);
    Ipv4AddressGenerator::Init(network, mask);
    Ipv4Address v4network;
    Ipv4AddressHelper addrHelper;

    for (uint32_t i = 0; i < m_edgeSwitchDevices.size(); ++i)
    {
        for (uint32_t j = 0; j < m_edgeSwitchDevices[i].GetN(); j += 2)
        {
            v4network = Ipv4AddressGenerator::NextNetwork(mask);
            addrHelper.SetBase(v4network, mask);
            Ipv4InterfaceContainer ic = addrHelper.Assign(m_edgeSwitchDevices[i].Get(j));
            m_serverInterfaces.Add(ic);
            ic = addrHelper.Assign(m_edgeSwitchDevices[i].Get(j + 1));
            m_edgeSwitchInterfaces.Add(ic);
        }
    }

    for (uint32_t i = 0; i < m_aggregateSwitchDevices.size(); ++i)
    {
        v4network = Ipv4AddressGenerator::NextNetwork(mask);
        addrHelper.SetBase(v4network, mask);
        for (uint32_t j = 0; j < m_aggregateSwitchDevices[i].GetN(); j += 2)
        {
            Ipv4InterfaceContainer ic = addrHelper.Assign(m_aggregateSwitchDevices[i].Get(j));
            m_edgeSwitchInterfaces.Add(ic);
            ic = addrHelper.Assign(m_aggregateSwitchDevices[i].Get(j + 1));
            m_aggregateSwitchInterfaces.Add(ic);
        }
    }

    for (uint32_t i = 0; i < m_coreSwitchDevices.size(); ++i)
    {
        v4network = Ipv4AddressGenerator::NextNetwork(mask);
        addrHelper.SetBase(v4network, mask);
        for (uint32_t j = 0; j < m_coreSwitchDevices[i].GetN(); j += 2)
        {
            Ipv4InterfaceContainer ic = addrHelper.Assign(m_coreSwitchDevices[i].Get(j));
            m_aggregateSwitchInterfaces.Add(ic);
            ic = addrHelper.Assign(m_coreSwitchDevices[i].Get(j + 1));
            m_coreSwitchInterfaces.Add(ic);
        }
    }
}

void
PointToPointFatTreeHelper::AssignIpv6Addresses(Ipv6Address addrBase, Ipv6Prefix prefix)
{
    NS_LOG_FUNCTION(this << addrBase << prefix);
    Ipv6AddressGenerator::Init(addrBase, prefix);
    Ipv6Address v6network;
    Ipv6AddressHelper addrHelper;

    for (uint32_t i = 0; i < m_edgeSwitchDevices.size(); ++i)
    {
        v6network = Ipv6AddressGenerator::NextNetwork(prefix);
        addrHelper.SetBase(v6network, prefix);
        for (uint32_t j = 0; j < m_edgeSwitchDevices[i].GetN(); j += 2)
        {
            Ipv6InterfaceContainer ic = addrHelper.Assign(m_edgeSwitchDevices[i].Get(j));
            m_serverInterfaces6.Add(ic);
            ic = addrHelper.Assign(m_edgeSwitchDevices[i].Get(j + 1));
            m_edgeSwitchInterfaces6.Add(ic);
        }
    }

    for (uint32_t i = 0; i < m_aggregateSwitchDevices.size(); ++i)
    {
        v6network = Ipv6AddressGenerator::NextNetwork(prefix);
        addrHelper.SetBase(v6network, prefix);
        for (uint32_t j = 0; j < m_aggregateSwitchDevices[i].GetN(); j += 2)
        {
            Ipv6InterfaceContainer ic = addrHelper.Assign(m_aggregateSwitchDevices[i].Get(j));
            m_edgeSwitchInterfaces6.Add(ic);
            ic = addrHelper.Assign(m_aggregateSwitchDevices[i].Get(j + 1));
            m_aggregateSwitchInterfaces6.Add(ic);
        }
    }

    for (uint32_t i = 0; i < m_coreSwitchDevices.size(); ++i)
    {
        v6network = Ipv6AddressGenerator::NextNetwork(prefix);
        addrHelper.SetBase(v6network, prefix);
        for (uint32_t j = 0; j < m_coreSwitchDevices[i].GetN(); j += 2)
        {
            Ipv6InterfaceContainer ic = addrHelper.Assign(m_coreSwitchDevices[i].Get(j));
            m_aggregateSwitchInterfaces6.Add(ic);
            ic = addrHelper.Assign(m_coreSwitchDevices[i].Get(j + 1));
            m_coreSwitchInterfaces6.Add(ic);
        }
    }
}

Ipv4Address
PointToPointFatTreeHelper::GetServerIpv4Address(uint32_t i) const
{
    NS_LOG_FUNCTION(this << i);
    return m_serverInterfaces.GetAddress(i);
}

Ipv4Address
PointToPointFatTreeHelper::GetEdgeSwitchIpv4Address(uint32_t i) const
{
    NS_LOG_FUNCTION(this << i);
    return m_edgeSwitchInterfaces.GetAddress(i);
}

Ipv4Address
PointToPointFatTreeHelper::GetAggregateSwitchIpv4Address(uint32_t i) const
{
    NS_LOG_FUNCTION(this << i);
    return m_aggregateSwitchInterfaces.GetAddress(i);
}

Ipv4Address
PointToPointFatTreeHelper::GetCoreSwitchIpv4Address(uint32_t i) const
{
    NS_LOG_FUNCTION(this << i);
    return m_coreSwitchInterfaces.GetAddress(i);
}

Ipv6Address
PointToPointFatTreeHelper::GetServerIpv6Address(uint32_t i) const
{
    NS_LOG_FUNCTION(this << i);
    return m_serverInterfaces6.GetAddress(i, 1);
}

Ipv6Address
PointToPointFatTreeHelper::GetEdgeSwitchIpv6Address(uint32_t i) const
{
    NS_LOG_FUNCTION(this << i);
    return m_edgeSwitchInterfaces6.GetAddress(i, 1);
}

Ipv6Address
PointToPointFatTreeHelper::GetAggregateSwitchIpv6Address(uint32_t i) const
{
    NS_LOG_FUNCTION(this << i);
    return m_aggregateSwitchInterfaces6.GetAddress(i, 1);
}

Ipv6Address
PointToPointFatTreeHelper::GetCoreSwitchIpv6Address(uint32_t i) const
{
    NS_LOG_FUNCTION(this << i);
    return m_coreSwitchInterfaces6.GetAddress(i, 1);
}

Ptr<Node>
PointToPointFatTreeHelper::GetServerNode(uint32_t i) const
{
    NS_LOG_FUNCTION(this << i);
    return m_servers.Get(i);
}

Ptr<Node>
PointToPointFatTreeHelper::GetEdgeSwitchNode(uint32_t i) const
{
    NS_LOG_FUNCTION(this << i);
    return m_edgeSwitches.Get(i);
}

Ptr<Node>
PointToPointFatTreeHelper::GetAggregateSwitchNode(uint32_t i) const
{
    NS_LOG_FUNCTION(this << i);
    return m_aggregateSwitches.Get(i);
}

Ptr<Node>
PointToPointFatTreeHelper::GetCoreSwitchNode(uint32_t i) const
{
    NS_LOG_FUNCTION(this << i);
    return m_coreSwitches.Get(i);
}

NodeContainer
PointToPointFatTreeHelper::GetMServers() const
{
    return m_servers;
}

uint32_t
PointToPointFatTreeHelper::GetNumServers() const
{
    return numServers;
}

int
main(int argc, char* argv[])
{
    Config::SetDefault("ns3::OnOffApplication::PacketSize", UintegerValue(512));
    Config::SetDefault("ns3::OnOffApplication::DataRate", StringValue("500kb/s"));

    LogComponentEnable("UdpEchoClientApplication", LOG_LEVEL_INFO);
    LogComponentEnable("UdpEchoServerApplication", LOG_LEVEL_INFO);

    uint32_t nPods = 4;
    std::string animFile = "fat-tree-animation.xml"; // Name of file for animation output
    bool nullmsg = false;

    CommandLine cmd;
    cmd.AddValue("nPods", "Number of pods", nPods);
    cmd.AddValue("animFile", "File Name for Animation Output", animFile);
    cmd.AddValue("nullmsg", "Enable the use of null-message synchronization", nullmsg);
    cmd.Parse(argc, argv);

    if (nullmsg)
    {
        GlobalValue::Bind("SimulatorImplementationType",
                          StringValue("ns3::NullMessageSimulatorImpl"));
    }
    else
    {
        GlobalValue::Bind("SimulatorImplementationType",
                          StringValue("ns3::DistributedSimulatorImpl"));
    }

    MpiInterface::Enable(&argc, &argv);

    uint32_t systemId = MpiInterface::GetSystemId();
    uint32_t systemCount = MpiInterface::GetSize();

    InternetStackHelper internet;
    Ipv4NixVectorHelper nixRouting;
    Ipv4StaticRoutingHelper staticRouting;

    Ipv4ListRoutingHelper list;
    list.Add(staticRouting, 0);
    list.Add(nixRouting, 10);
    internet.SetRoutingHelper(list);

    // Create the point-to-point link helpers
    PointToPointHelper pointToPointRouter;
    pointToPointRouter.SetDeviceAttribute("DataRate", StringValue("10Mbps"));
    pointToPointRouter.SetChannelAttribute("Delay", StringValue("1ms"));

    PointToPointFatTreeHelper d(nPods, pointToPointRouter);
    // Install Stack
    d.InstallStack(internet);

    d.AssignIpv4Addresses(Ipv4Address("10.0.0.0"), Ipv4Mask("/16"));

    uint32_t numServers = d.GetNumServers();
    NodeContainer servers = d.GetMServers();

    uint32_t serversMpiStride = numServers / systemCount;

    for (uint32_t potentialSystemId = 0; potentialSystemId < systemCount; potentialSystemId++)
    {
        // Each server sends a packet to all other servers
        if (potentialSystemId == systemId)
        {
            UdpEchoServerHelper echoServer(9);
            ApplicationContainer serverApps;
            uint32_t start = systemId * serversMpiStride;
            uint32_t end = start + serversMpiStride;
            for (uint32_t nodeIdx = start; nodeIdx < end; nodeIdx++)
            {
                serverApps.Add(echoServer.Install(servers.Get(nodeIdx)));
            }
            serverApps.Start(Seconds(1.0));
            serverApps.Stop(Seconds(10.0));
        }
    }

    std::cout << "numServers: " << numServers << std::endl;
    // uint32_t packetNumber = 0;
    for (uint32_t sender = 0; sender < numServers; sender++)
    {
        uint32_t lower = systemId * serversMpiStride;
        uint32_t upper = lower + serversMpiStride;
        if (sender < lower || sender >= upper)
            continue;
        for (uint32_t receiver = 0; receiver < numServers; receiver++)
        {
            if (sender != receiver)
            {
                UdpEchoClientHelper echoClient(d.GetServerIpv4Address(receiver),
                                               9); // Use the appropriate local IP address
                // std::cout << "packet " << packetNumber << " sending from: " <<
                // d.GetServerIpv4Address(sender) << " to " << d.GetServerIpv4Address(receiver) <<
                // std::endl;
                echoClient.SetAttribute("MaxPackets", UintegerValue(1));
                echoClient.SetAttribute("Interval", TimeValue(Seconds(1.0)));
                echoClient.SetAttribute("PacketSize", UintegerValue(1024));

                ApplicationContainer clientApps = echoClient.Install(servers.Get(sender));
                clientApps.Start(Seconds(2.0));
                clientApps.Stop(Seconds(10.0));
                // packetNumber += 1;
            }
        }
    }

    // Set the bounding box for animation
    d.BoundingBox(-1000, -1000, 1000, 1000);

    // Create the animation object and configure for specified output
    // AnimationInterface anim(animFile);
    // anim.EnablePacketMetadata (); // Optional
    // anim.EnableIpv4L3ProtocolCounters (Seconds (0), Seconds (10)); // Optional

    // Set up the actual simulation
    // Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

    Simulator::Run();
    std::cout << "Animation Trace file created:" << animFile.c_str() << std::endl;
    Simulator::Destroy();

    MpiInterface::Disable();
    return 0;
}
