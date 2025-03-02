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

#ifndef ManagePoFaOpSubUnitH
#define ManagePoFaOpSubUnitH
//---------------------------------------------------------------------------

class CManagePoFaOpSubUnit : public CManagePoBase<DB_PO_FA_OP_SUB_UNIT>
{
public:
	INT32					LoadDBMS();

public:
	INT32					GetHash(UINT32 nID, String& strOrgValue);

public:
	INT32					AddPoFaOpSubUnit(DB_PO_FA_OP_SUB_UNIT&	dpfosu);
    INT32					EditPoFaOpSubUnit(DB_PO_FA_OP_SUB_UNIT&	dpfosu);
	INT32					DelPoFaOpSubUnit(UINT32 nID);
	INT32					ClearPoFaOpSubUnit();
	INT32					ApplyPoFaOpSubUnit(DB_PO_FA_OP_SUB_UNIT&	dpfosu);

public:
	String					GetName(UINT32 nID);    

public:
	INT32					SetPkt(MemToken& SendToken);
	INT32					SetPkt(UINT32 nID, MemToken& SendToken);
	INT32					SetPkt(PDB_PO_FA_OP_SUB_UNIT pdpfosu, MemToken& SendToken);
	INT32					GetPkt(MemToken& RecvToken, DB_PO_FA_OP_SUB_UNIT& dpfosu);

public:
	CManagePoFaOpSubUnit();
    ~CManagePoFaOpSubUnit();

};

extern CManagePoFaOpSubUnit*	t_ManagePoFaOpSubUnit;

#endif
