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

#ifndef ManagePoNcPtnExTrustH
#define ManagePoNcPtnExTrustH
//---------------------------------------------------------------------------

class CManagePoNcPtnExTrust : public CManagePoBase<DB_PO_NC_PTN_EX_TRUST>
{
public:
	INT32					LoadDBMS();

public:
	INT32					GetHash(UINT32 nID, String& strOrgValue);

public:
	INT32					AddPoNcPtnExTrust(DB_PO_NC_PTN_EX_TRUST&	data);
    INT32					EditPoNcPtnExTrust(DB_PO_NC_PTN_EX_TRUST&	data);
	INT32					DelPoNcPtnExTrust(UINT32 nID);
	INT32					ApplyPoNcPtnExTrust(DB_PO_NC_PTN_EX_TRUST&	data);

public:
	INT32					SyncPoNcPtnExTrust(UINT32 nPolicyID);

public:
	INT32					SetPkt(MemToken& SendToken);
	INT32					SetPkt(UINT32 nID, MemToken& SendToken);
	INT32					SetPkt(PDB_PO_NC_PTN_EX_TRUST pdata, MemToken& SendToken);
	INT32					GetPkt(MemToken& RecvToken, DB_PO_NC_PTN_EX_TRUST& data);

public:
	CManagePoNcPtnExTrust();
    ~CManagePoNcPtnExTrust();

};

extern CManagePoNcPtnExTrust*	t_ManagePoNcPtnExTrust;

#endif
