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
#include "ManagePoNcPtnExTrust.h"

//---------------------------------------------------------------------------

CManagePoNcPtnExTrust*	t_ManagePoNcPtnExTrust = NULL;

//---------------------------------------------------------------------------

CManagePoNcPtnExTrust::CManagePoNcPtnExTrust()
{
	t_DBMgrPoNcPtnExTrust	= new CDBMgrPoNcPtnExTrust();
}
//---------------------------------------------------------------------------

CManagePoNcPtnExTrust::~CManagePoNcPtnExTrust()
{
	if(t_DBMgrPoNcPtnExTrust)	{	delete t_DBMgrPoNcPtnExTrust;	t_DBMgrPoNcPtnExTrust = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManagePoNcPtnExTrust::LoadDBMS()
{
	TListDBPoNcPtnExTrust tDBPoNcPtnExTrustList;
    TListDBPoNcPtnExTrustItor begin, end;
	if(SetER(t_DBMgrPoNcPtnExTrust->LoadExecute(&tDBPoNcPtnExTrustList)))
    {
    	return g_nErrRtn;
    }

    begin = tDBPoNcPtnExTrustList.begin();	end = tDBPoNcPtnExTrustList.end();
    for(begin; begin != end; begin++)
    {
    	AddItem(begin->tDPH.nID, *begin);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoNcPtnExTrust::GetHash(UINT32 nID, String& strOrgValue)
{
	PDB_PO_NC_PTN_EX_TRUST pdata 			= NULL;
	{
		if( (pdata = FindItem(nID)) == NULL)
		{
			WriteLogE("not find po_nc_ptn_ex_trust by hash : [%d]", ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			return ERR_INFO_NOT_OP_BECAUSE_NOT_FIND;
		}
	}

	{
		strOrgValue = SPrintf("%s,"
							"%u,%u,%u,"
							"%s,%s,%s,", 
							GetHdrHashInfo(pdata).c_str(),
							pdata->nPolicyID, pdata->nUnitID, pdata->nBlockMode,
							pdata->strSFName.c_str(), pdata->strOFName.c_str(), pdata->strFileKey.c_str());
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoNcPtnExTrust::AddPoNcPtnExTrust(DB_PO_NC_PTN_EX_TRUST&	data)
{
	if(SetER(t_DBMgrPoNcPtnExTrust->InsertExecute(&data)))
    {
    	return g_nErrRtn;
    }

    AddItem(data.tDPH.nID, data);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoNcPtnExTrust::EditPoNcPtnExTrust(DB_PO_NC_PTN_EX_TRUST&	data)
{
	PDB_PO_NC_PTN_EX_TRUST pdata = FindItem(data.tDPH.nID);
	if(!pdata)	return ERR_DBMS_UPDATE_FAIL;

	if(SetER(t_DBMgrPoNcPtnExTrust->UpdateExecute(&data)))
    {
    	return g_nErrRtn;
    }

    EditItem(data.tDPH.nID, data);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoNcPtnExTrust::DelPoNcPtnExTrust(UINT32 nID)
{
	PDB_PO_NC_PTN_EX_TRUST pdata = FindItem(nID);
	if(!pdata)	return ERR_DBMS_DELETE_FAIL;

	if(SetER(t_DBMgrPoNcPtnExTrust->DeleteExecute(pdata->tDPH.nID)))
    {
    	return g_nErrRtn;
    }

    DeleteItem(nID);
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoNcPtnExTrust::ApplyPoNcPtnExTrust(DB_PO_NC_PTN_EX_TRUST&	data)
{
	if(FindItem(data.tDPH.nID))
	{
		return EditPoNcPtnExTrust(data);
	}
	return AddPoNcPtnExTrust(data);
}
//---------------------------------------------------------------------------

INT32					CManagePoNcPtnExTrust::SyncPoNcPtnExTrust(UINT32 nPolicyID)
{
	TListID tDelIDList;

	{
		TMapDBPoNcPtnExTrustItor begin, end;
		begin = m_tMap.begin();	end = m_tMap.end();
		for(begin; begin != end; begin++)
		{
			PDB_PO_NC_PTN_EX_UNIT pdata_unit = t_ManagePoNcPtnExUnit->FindItem(begin->second.nUnitID);
			if(!pdata_unit)
			{
				tDelIDList.push_back(begin->first);
				continue;
			}

			begin->second.tDPH.nUsedMode = pdata_unit->tDPH.nUsedMode;
			begin->second.nBlockMode = pdata_unit->nBlockMode;
			begin->second.nPolicyID = nPolicyID;

			t_DBMgrPoNcPtnExTrust->UpdateExecute(&(begin->second));			
		}
	}

	{
		TListIDItor begin, end;
		begin = tDelIDList.begin();	end = tDelIDList.end();
		for(begin; begin != end; begin++)
		{
			DelPoNcPtnExTrust(*begin);
		}
	}
	return 0;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoNcPtnExTrust::SetPkt(MemToken& SendToken)
{
    SendToken.TokenAdd_32(m_tMap.size());

	TMapDBPoNcPtnExTrustItor begin, end;
    begin = m_tMap.begin();	end = m_tMap.end();
    for(begin; begin != end; begin++)
    {
    	SetPkt(&(begin->second), SendToken);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoNcPtnExTrust::SetPkt(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_NC_PTN_EX_TRUST pdata = FindItem(nID);
	if(!pdata)	return -1;

	return SetPkt(pdata, SendToken);
}
//---------------------------------------------------------------------------

INT32					CManagePoNcPtnExTrust::SetPkt(PDB_PO_NC_PTN_EX_TRUST pdata, MemToken& SendToken)
{
	SendToken.TokenAdd_DPH(pdata->tDPH);

	SendToken.TokenAdd_32(pdata->nPolicyID);
	SendToken.TokenAdd_32(pdata->nUnitID);
	SendToken.TokenAdd_32(pdata->nBlockMode);

	SendToken.TokenAdd_String(pdata->strSFName);
	SendToken.TokenAdd_String(pdata->strOFName);
	SendToken.TokenAdd_String(pdata->strFileKey);

	SendToken.TokenSet_Block();
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoNcPtnExTrust::GetPkt(MemToken& RecvToken, DB_PO_NC_PTN_EX_TRUST& data)
{
	if (!RecvToken.TokenDel_DPH(data.tDPH))						goto INVALID_PKT;

	if (!RecvToken.TokenDel_32(data.nPolicyID))					goto INVALID_PKT;
	if (!RecvToken.TokenDel_32(data.nUnitID))					goto INVALID_PKT;
	if (!RecvToken.TokenDel_32(data.nBlockMode))				goto INVALID_PKT;

	if ( RecvToken.TokenDel_String(data.strSFName) < 0)			goto INVALID_PKT;
	if ( RecvToken.TokenDel_String(data.strOFName) < 0)			goto INVALID_PKT;
	if ( RecvToken.TokenDel_String(data.strFileKey) < 0)		goto INVALID_PKT;

	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------



