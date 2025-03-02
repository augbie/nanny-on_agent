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

#ifndef ManagePoNcPtnSPRuleH
#define ManagePoNcPtnSPRuleH
//---------------------------------------------------------------------------

class CManagePoNcPtnSPRule : public CManagePoBase<DB_PO_NC_PTN_SP_RULE>
{
public:
	INT32					LoadDBMS();

public:
	INT32					InitHash();
	String					GetHash();

public:
	INT32					AddPoNcPtnSPRule(DB_PO_NC_PTN_SP_RULE&	data);
    INT32					EditPoNcPtnSPRule(DB_PO_NC_PTN_SP_RULE&	data);
	INT32					DelPoNcPtnSPRule(UINT32 nID);
	INT32					ApplyPoNcPtnSPRule(DB_PO_NC_PTN_SP_RULE&	data);

public:
	String					GetName(UINT32 nID);    
	UINT32					GetNextLocalID();

public:
	INT32					SetPkt(MemToken& SendToken);
	INT32					SetPkt(UINT32 nID, MemToken& SendToken);
	INT32					SetPkt(PDB_PO_NC_PTN_SP_RULE pdata, MemToken& SendToken);
	INT32					GetPkt(MemToken& RecvToken, DB_PO_NC_PTN_SP_RULE& data);

	INT32					SetPktSync(TListPVOID& tIDList);
public:
	CManagePoNcPtnSPRule();
    ~CManagePoNcPtnSPRule();

};

extern CManagePoNcPtnSPRule*	t_ManagePoNcPtnSPRule;

#endif
