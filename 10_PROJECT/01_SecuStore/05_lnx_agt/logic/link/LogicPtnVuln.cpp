/*
 * Copyright (C) 2021-2026 ASHINi Corp. 
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public 
 * License as published by the Free Software Foundation; either 
 * version 3 of the License, or any later version. 
 * 
 * This program is distributed in the hope that it will be useful, 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 * General Public License for more details. 
 * 
 * You should have received a copy of the GNU General Public 
 * License along with this program. If not, see <https://www.gnu.org/licenses/>.   
 *  
 * 
 */

//---------------------------------------------------------------------------


#include "stdafx.h"
#include "com_struct.h"
#include "LogicPtnVuln.h"

//---------------------------------------------------------------------------

CLogicPtnVuln*		t_LogicPtnVuln = NULL;

//---------------------------------------------------------------------------

CLogicPtnVuln::CLogicPtnVuln()
{	
	m_strLogicName = "logic ptn patch";
}
//---------------------------------------------------------------------------

CLogicPtnVuln::~CLogicPtnVuln()
{
}
//---------------------------------------------------------------------------

void		CLogicPtnVuln::AnalyzePkt_PtnPatch(PPKT_DATA pkt_data)
{
	InitBaseMember(pkt_data);
	switch(m_nPktCode)
    {
		default:
        {
        	WriteLogE("[%s] not define pkt code from console : [%d][%d]", m_strLogicName.c_str(), m_nPktType, m_nPktCode);
			break;
        }
    }
	InitBaseMember();
    return;
}
//---------------------------------------------------------------------------

void		CLogicPtnVuln::SendPkt_Sync()
{
	TMapID tIDMap;
	t_ManageLinkEnv->GetKeyIDMap(G_TYPE_PTN_PATCH, tIDMap);
	m_tMutex.Lock();
	SendData_Link(G_TYPE_PTN_PATCH, G_CODE_COMMON_SYNC, SendToken);
	SendToken.Clear();
	m_tMutex.UnLock();
}
//---------------------------------------------------------------------------



