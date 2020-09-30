/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
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
 * Author: Romagnolo Stefano <romagnolostefano93@gmail.com>
 */

#include "lora-tx-current-model.h"
#include "ns3/log.h"
#include "ns3/double.h"
#include "lora-utils.h"

namespace ns3 {
namespace lorawan {

NS_LOG_COMPONENT_DEFINE ("LoraTxCurrentModel");

NS_OBJECT_ENSURE_REGISTERED (LoraTxCurrentModel);

TypeId
LoraTxCurrentModel::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::LoraTxCurrentModel")
    .SetParent<Object> ()
    .SetGroupName ("Lora")
  ;
  return tid;
}

LoraTxCurrentModel::LoraTxCurrentModel ()
{
}

LoraTxCurrentModel::~LoraTxCurrentModel ()
{
}

// Similarly to the wifi case
NS_OBJECT_ENSURE_REGISTERED (LinearLoraTxCurrentModel);

TypeId
LinearLoraTxCurrentModel::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::LinearLoraTxCurrentModel")
    .SetParent<LoraTxCurrentModel> ()
    .SetGroupName ("Lora")
    .AddConstructor<LinearLoraTxCurrentModel> ()
    .AddAttribute ("Eta", "The efficiency of the power amplifier.",
                   DoubleValue (0.10),
                   MakeDoubleAccessor (&LinearLoraTxCurrentModel::SetEta,
                                       &LinearLoraTxCurrentModel::GetEta),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("Voltage", "The supply voltage (in Volts).",
                   DoubleValue (3.3),
                   MakeDoubleAccessor (&LinearLoraTxCurrentModel::SetVoltage,
                                       &LinearLoraTxCurrentModel::GetVoltage),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("StandbyCurrent", "The current in the STANDBY state (in Watts).",
                   DoubleValue (0.0014),      // idle mode = 1.4mA
                   MakeDoubleAccessor (&LinearLoraTxCurrentModel::SetStandbyCurrent,
                                       &LinearLoraTxCurrentModel::GetStandbyCurrent),
                   MakeDoubleChecker<double> ())
  ;
  return tid;
}

LinearLoraTxCurrentModel::LinearLoraTxCurrentModel ()
{
  NS_LOG_FUNCTION (this);
}

LinearLoraTxCurrentModel::~LinearLoraTxCurrentModel ()
{
  NS_LOG_FUNCTION (this);
}

void
LinearLoraTxCurrentModel::SetEta (double eta)
{
  NS_LOG_FUNCTION (this << eta);
  m_eta = eta;
}

void
LinearLoraTxCurrentModel::SetVoltage (double voltage)
{
  NS_LOG_FUNCTION (this << voltage);
  m_voltage = voltage;
}

void
LinearLoraTxCurrentModel::SetStandbyCurrent (double idleCurrent)
{
  NS_LOG_FUNCTION (this << idleCurrent);
  m_idleCurrent = idleCurrent;
}

double
LinearLoraTxCurrentModel::GetEta (void) const
{
  return m_eta;
}

double
LinearLoraTxCurrentModel::GetVoltage (void) const
{
  return m_voltage;
}

double
LinearLoraTxCurrentModel::GetStandbyCurrent (void) const
{
  return m_idleCurrent;
}

double
LinearLoraTxCurrentModel::CalcTxCurrent (double txPowerDbm) const
{
  NS_LOG_FUNCTION (this << txPowerDbm);
  return DbmToW (txPowerDbm) / (m_voltage * m_eta) + m_idleCurrent;
}


NS_OBJECT_ENSURE_REGISTERED (ConstantLoraTxCurrentModel);

TypeId
ConstantLoraTxCurrentModel::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::ConstantLoraTxCurrentModel")
    .SetParent<LoraTxCurrentModel> ()
    .SetGroupName ("Lora")
    .AddConstructor<ConstantLoraTxCurrentModel> ()
    .AddAttribute ("TxCurrent",
                   "The radio Tx current in Ampere.",
                   DoubleValue (0.028),        // transmit at 0dBm = 28mA
                   MakeDoubleAccessor (&ConstantLoraTxCurrentModel::SetTxCurrent,
                                       &ConstantLoraTxCurrentModel::GetTxCurrent),
                   MakeDoubleChecker<double> ())
  ;
  return tid;
}

ConstantLoraTxCurrentModel::ConstantLoraTxCurrentModel ()
{
  NS_LOG_FUNCTION (this);
}

ConstantLoraTxCurrentModel::~ConstantLoraTxCurrentModel ()
{
  NS_LOG_FUNCTION (this);
}

void
ConstantLoraTxCurrentModel::SetTxCurrent (double txCurrent)
{
  NS_LOG_FUNCTION (this << txCurrent);
  m_txCurrent = txCurrent;
}

double
ConstantLoraTxCurrentModel::GetTxCurrent (void) const
{
  return m_txCurrent;
}

double
ConstantLoraTxCurrentModel::CalcTxCurrent (double txPowerDbm) const
{
  NS_LOG_FUNCTION (this << txPowerDbm);
  return m_txCurrent;
}

// Similarly to the wifi case
NS_OBJECT_ENSURE_REGISTERED (LiandoLoraTxCurrentModel);

TypeId
LiandoLoraTxCurrentModel::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::LiandoLoraTxCurrentModel")
    .SetParent<LoraTxCurrentModel> ()
    .SetGroupName ("Lora")
    .AddConstructor<LiandoLoraTxCurrentModel> ()
    .AddAttribute ("Voltage", "The supply voltage (in Volts).",
                   DoubleValue (3.3),
                   MakeDoubleAccessor (&LiandoLoraTxCurrentModel::SetVoltage,
                                       &LiandoLoraTxCurrentModel::GetVoltage),
                   MakeDoubleChecker<double> ())
  ;
  return tid;
}

LiandoLoraTxCurrentModel::LiandoLoraTxCurrentModel ()
{
  NS_LOG_FUNCTION (this);
}

LiandoLoraTxCurrentModel::~LiandoLoraTxCurrentModel ()
{
  NS_LOG_FUNCTION (this);
}

void
LiandoLoraTxCurrentModel::SetVoltage (double voltage)
{
  NS_LOG_FUNCTION (this << voltage);
  m_voltage = voltage;
}

double
LiandoLoraTxCurrentModel::GetVoltage (void) const
{
  return m_voltage;
}

double
LiandoLoraTxCurrentModel::CalcTxCurrent (double txPowerDbm) const
{
  NS_LOG_FUNCTION (this << txPowerDbm);
  std::vector<double> PtxdBm {5.0, 8.0, 1.0, 14.0, 17.0, 20.0};
  std::vector<double> PowerW {0.15, 0.2, 0.25, 0.3, 0.4, 0.4};

  // Less than 5 dBm
  if (txPowerDbm < PtxdBm[0])
  {
    return PowerW[0] / m_voltage;
  }

  // Between 5 dBm and 20 dBm
  for (std::size_t i = 1; i < PtxdBm.size(); ++i)
  {
    if (txPowerDbm < PtxdBm[i])
    {
      return PowerW[i] - (PtxdBm[i] - txPowerDbm) * 
        (PowerW[i] - PowerW[i-1]) / (PtxdBm[i] - PtxdBm[i-1]);
    }
  }

  // Exceed 20 dBm
  return PowerW[PowerW.size()-1] / m_voltage;
}

}
} // namespace ns3
